#include "accessor-rewrite.h"

#include <clang/AST/ExprObjC.h>
#include <clang/Basic/SourceLocation.h>

using namespace clang;
using namespace clang::ast_matchers;

void AccessorRewriteCheck::storeOptions(
  clang::tidy::ClangTidyOptions::OptionMap& Opts)
{
  // Options.store(Opts, "Class", ClassName);
  // Options.store(Opts, "Member", MemberName);
  // Options.store(Opts, "Accessor", AccessorName);
  ClassName = "COutPoint";
  MemberToAccessor["n"] = "index";
  MemberToAccessor["hash"] = "txid";
}

void AccessorRewriteCheck::registerMatchers(MatchFinder* Finder)
{
  ClassName = "COutPoint";
  MemberToAccessor["n"] = "index";
  MemberToAccessor["hash"] = "txid";

  if (ClassName.empty() || MemberToAccessor.empty()) {
    return;
  }

  Finder->addMatcher(
    memberExpr(
      unless(hasAncestor(functionDecl(anyOf(isImplicit(), isDefaulted())))),
      member(fieldDecl(hasParent(cxxRecordDecl(hasName(ClassName))))))
      .bind("member"),
    this);
}

void AccessorRewriteCheck::check(MatchFinder::MatchResult const& Result)
{
  auto const* ME = Result.Nodes.getNodeAs<MemberExpr>("member");
  if (!ME || ME->isImplicitAccess() || ME->getMemberLoc().isMacroID()) {
    return;
  }

  auto const* FD = dyn_cast<FieldDecl>(ME->getMemberDecl());
  if (!FD)
    return;

  auto It = MemberToAccessor.find(FD->getNameAsString());
  if (It == MemberToAccessor.end())
    return;

  std::string const& Accessor = It->second;

  // Skip obvious write accesses.
  auto Parents = Result.Context->getParents(*ME);
  if (!Parents.empty()) {
    if (auto const* BO = Parents[0].get<BinaryOperator>()) {
      if (BO->isAssignmentOp() && BO->getLHS() == ME) {
        return;
      }
    }

    if (auto const* UO = Parents[0].get<UnaryOperator>()) {
      if (UO->isIncrementDecrementOp()) {
        return;
      }
    }
  }

  diag(ME->getMemberLoc(), "replace direct member access with accessor")
    << FixItHint::CreateReplacement(ME->getMemberLoc(), Accessor + "()");
}
