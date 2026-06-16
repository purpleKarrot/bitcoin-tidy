#pragma once

#include <clang-tidy/ClangTidyCheck.h>

class OutpointObservers : public clang::tidy::ClangTidyCheck
{
public:
  using ClangTidyCheck::ClangTidyCheck;

  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
  void check(clang::ast_matchers::MatchFinder::MatchResult const& Res) override;
};
