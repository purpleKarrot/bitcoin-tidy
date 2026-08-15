#include "combine-assignments.h"

#include <clang/AST/ExprObjC.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Lex/Lexer.h>

namespace {

using namespace clang;
using namespace clang::ast_matchers;

struct AnnotationInfo {
  unsigned Index;
  unsigned Total;
};

std::optional<AnnotationInfo> GetAnnotationInfo(FieldDecl const *FD) {
  auto const *Attr = FD->getAttr<AnnotateAttr>();
  if (!Attr)
    return std::nullopt;

  StringRef Annotation = Attr->getAnnotation();
  if (!Annotation.consume_front("constructor-argument:"))
    return std::nullopt;

  auto [IdxStr, TotalStr] = Annotation.split('/');

  unsigned Idx, Total;
  if (IdxStr.getAsInteger(10, Idx) || TotalStr.getAsInteger(10, Total))
    return std::nullopt;

  if (Idx == 0 || Idx > Total)
    return std::nullopt;

  return AnnotationInfo{Idx - 1, Total};
}

struct AssignmentInfo {
  MemberExpr const *ME = nullptr;
  Expr const *Base = nullptr;
  Expr const *RHS = nullptr;
  unsigned Index = 0;
  unsigned Total = 0;
};

// Given an assignment's LHS and RHS expressions, returns the annotated
// field assignment info if LHS is base.member with an annotated field.
std::optional<AssignmentInfo> ExtractAssignment(Expr const *LHS,
                                                Expr const *RHS) {
  auto *ME = dyn_cast<MemberExpr>(LHS->IgnoreParenImpCasts());
  if (!ME)
    return std::nullopt;

  auto *FD = dyn_cast<FieldDecl>(ME->getMemberDecl());
  if (!FD)
    return std::nullopt;

  auto Info = GetAnnotationInfo(FD);
  if (!Info)
    return std::nullopt;

  return AssignmentInfo{ME, ME->getBase()->IgnoreParenImpCasts(), RHS,
                        Info->Index, Info->Total};
}

std::optional<AssignmentInfo> GetAssignment(Stmt const *S) {
  // Built-in assignment
  if (auto *BO = dyn_cast<BinaryOperator>(S))
    if (BO->isAssignmentOp())
      return ExtractAssignment(BO->getLHS(), BO->getRHS());

  // Strip a surrounding expression statement (overloaded operator=
  // may return a value wrapped in ExprWithCleanups).
  if (auto *E = dyn_cast<ExprWithCleanups>(S))
    S = E->getSubExpr();

  // Overloaded operator=
  if (auto *Op = dyn_cast<CXXOperatorCallExpr>(S))
    if (Op->getOperator() == OO_Equal)
      return ExtractAssignment(Op->getArg(0), Op->getArg(1));

  return std::nullopt;
}

/// Convenience: get the source text of an expression.
std::string GetText(Expr const *E, SourceManager const &SM,
                    LangOptions const &LO) {
  return Lexer::getSourceText(
             CharSourceRange::getTokenRange(E->getSourceRange()), SM, LO)
      .str();
}

/// Infer the constructor name (class name) from the base expression's type.
/// Returns an empty string if the type is not a CXXRecordDecl.
std::string GetClassName(Expr const *Base) {
  auto const *Record = Base->getType().getCanonicalType()->getAsCXXRecordDecl();
  return Record ? Record->getName().str() : std::string{};
}

} // namespace

void CombineAssignments::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(compoundStmt().bind("body"), this);
}

void CombineAssignments::check(MatchFinder::MatchResult const &Result) {
  auto const *Body = Result.Nodes.getNodeAs<CompoundStmt>("body");
  if (!Body)
    return;

  auto &SM = *Result.SourceManager;
  auto const &LO = Result.Context->getLangOpts();

  llvm::SmallVector<Stmt const *, 16> Stmts(Body->body());

  for (size_t i = 0; i < Stmts.size(); ++i) {
    auto First = GetAssignment(Stmts[i]);
    if (!First)
      continue;

    std::string BaseText = GetText(First->Base, SM, LO);

    // Collect the run of consecutive annotated assignments to the same base.
    // Members[k] = AssignmentInfo for constructor-argument index k (0-based).
    std::vector<std::optional<AssignmentInfo>> Members;
    auto EnsureSize = [&](unsigned Idx) {
      if (Members.size() <= Idx)
        Members.resize(Idx + 1, std::nullopt);
    };

    EnsureSize(First->Index);
    Members[First->Index] = First;

    size_t j = i + 1;
    size_t LastStmt = i;

    while (j < Stmts.size()) {
      auto A = GetAssignment(Stmts[j]);
      if (!A)
        break;

      if (GetText(A->Base, SM, LO) != BaseText)
        break;

      if (A->Index < Members.size() && Members[A->Index] != std::nullopt)
        break; // duplicate index

      EnsureSize(A->Index);
      Members[A->Index] = A;
      LastStmt = j;
      ++j;
    }

    // Count how many annotated assignments were collected.
    size_t Count = 0;
    for (auto M : Members)
      if (M)
        ++Count;
    if (Count < 2)
      continue;

    // ExpectedTotal comes from the annotation: aggregate_init:<idx>:<total>
    unsigned ExpectedTotal = First->Total;

    // Completeness check: all constructor arguments 0..ExpectedTotal-1
    // must be present, and all annotations must agree on the total.
    auto IsComplete = [&]() {
      if (Members.size() < ExpectedTotal)
        return false;
      for (unsigned k = 0; k < ExpectedTotal; ++k) {
        if (!Members[k])
          return false; // gap
        if (Members[k]->Total != ExpectedTotal)
          return false; // inconsistent annotation
      }
      return true;
    };

    if (!IsComplete())
      continue;

    // Build the constructor argument list in index order (0, 1, ..., N-1).
    std::string Args;
    for (unsigned k = 0; k < ExpectedTotal; ++k) {
      if (k > 0)
        Args += ", ";
      Args += GetText(Members[k]->RHS, SM, LO);
    }

    std::string ClassName = GetClassName(First->Base);
    if (ClassName.empty())
      continue;

    std::string Replacement = BaseText + " = " + ClassName + "(" + Args + ")";

    CharSourceRange ReplaceRange = CharSourceRange::getTokenRange(
        SourceRange(Stmts[i]->getBeginLoc(), Stmts[LastStmt]->getEndLoc()));

    diag(Stmts[i]->getBeginLoc(),
         "replace separate member assignments to %0 with a constructor call")
        << ClassName << FixItHint::CreateReplacement(ReplaceRange, Replacement);

    i = LastStmt;
  }
}
