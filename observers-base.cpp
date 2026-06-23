#include "observers-base.h"

#include <clang/Analysis/Analyses/ExprMutationAnalyzer.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace clang::ast_matchers;

static bool isRangeForMutated(
  clang::MemberExpr const* ME, clang::ASTContext& Ctx)
{
  DynTypedNode Current = DynTypedNode::create(*ME);
  while (true) {
    auto Parents = Ctx.getParents(Current);
    if (Parents.empty())
      break;
    Current = Parents[0];

    if (auto const* ForRange = Current.get<CXXForRangeStmt>()) {
      if (auto const* LoopVar = ForRange->getLoopVariable()) {
        QualType LoopVarType = LoopVar->getType();
        if (auto const* RefType = LoopVarType->getAs<ReferenceType>()) {
          if (!RefType->getPointeeType().isConstQualified())
            return true;
        }
        if (auto const* PtrType = LoopVarType->getAs<PointerType>()) {
          if (!PtrType->getPointeeType().isConstQualified())
            return true;
        }
      }
      return false;
    }

    // Stop at statement boundaries that can't be part of a
    // range-for init chain.
    if (
      Current.get<CompoundStmt>() || Current.get<ForStmt>() ||
      Current.get<WhileStmt>() || Current.get<DoStmt>() ||
      Current.get<IfStmt>() || Current.get<SwitchStmt>())
      break;
  }
  return false;
}

void ObserversBase::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(
    memberExpr(
      unless(hasAncestor(functionDecl(anyOf(isImplicit(), isDefaulted())))),
      member(fieldDecl(hasParent(cxxRecordDecl(hasName(ClassName))))))
      .bind("member"),
    this);
}

void ObserversBase::check(MatchFinder::MatchResult const& Result)
{
  auto const* ME = Result.Nodes.getNodeAs<MemberExpr>("member");
  if (!ME || ME->isImplicitAccess()) {
    return;
  }

  SourceLocation Loc = ME->getMemberLoc();
  if (Loc.isMacroID()) {
    SourceManager const& SM = *Result.SourceManager;
    LangOptions const& LO = Result.Context->getLangOpts();

    StringRef MacroName = clang::Lexer::getImmediateMacroName(Loc, SM, LO);
    if (MacroName == "READWRITE" || MacroName == "VARINT") {
      return;
    }
  }

  auto const* FD = dyn_cast<FieldDecl>(ME->getMemberDecl());
  if (!FD) {
    return;
  }

  auto It = MemberToAccessor.find(FD->getNameAsString());
  if (It == MemberToAccessor.end()) {
    return;
  }

  Stmt const* EnclosingStmt = nullptr;
  DynTypedNode Current = DynTypedNode::create(*ME);
  while (true) {
    auto Parents = Result.Context->getParents(Current);
    if (Parents.empty()) {
      break;
    }
    Current = Parents[0];
    if (auto const* FuncDecl = Current.get<FunctionDecl>()) {
      EnclosingStmt = FuncDecl->getBody();
      break;
    }
  }

  if (EnclosingStmt) {
    ExprMutationAnalyzer Analyzer(*EnclosingStmt, *Result.Context);
    if (Analyzer.isMutated(ME)) {
      return;
    }
  }

  if (isRangeForMutated(ME, *Result.Context)) {
    return;
  }

  std::string const& Accessor = It->second;
  diag(ME->getMemberLoc(), "replace direct member access with accessor")
    << FixItHint::CreateReplacement(ME->getMemberLoc(), Accessor);
}
