#include "tx-deref.h"

using namespace clang;
using namespace clang::ast_matchers;

void TxDeref::registerMatchers(MatchFinder* Finder)
{
  auto IsTx = hasType(qualType(hasCanonicalType(
    recordType(hasDeclaration(cxxRecordDecl(hasName("CTransaction")))))));

  Finder->addMatcher(
    cxxOperatorCallExpr(
      hasOverloadedOperatorName("->"), hasArgument(0, expr(IsTx).bind("base")))
      .bind("arrow"),
    this);

  Finder->addMatcher(
    cxxOperatorCallExpr(
      hasOverloadedOperatorName("*"), argumentCountIs(1),
      hasArgument(0, expr(IsTx).bind("base")))
      .bind("deref"),
    this);
}

void TxDeref::check(MatchFinder::MatchResult const& Result)
{
  if (
    auto const* Arrow = Result.Nodes.getNodeAs<CXXOperatorCallExpr>("arrow")) {
    SourceLocation ArrowLoc = Arrow->getOperatorLoc();
    diag(ArrowLoc, "replace CTransaction operator-> with '.'")
      << FixItHint::CreateReplacement(ArrowLoc, ".");
  }

  if (
    auto const* Deref = Result.Nodes.getNodeAs<CXXOperatorCallExpr>("deref")) {
    SourceLocation StarLoc = Deref->getOperatorLoc();
    diag(StarLoc, "remove dereference of CTransaction")
      << FixItHint::CreateRemoval(StarLoc);
  }
}
