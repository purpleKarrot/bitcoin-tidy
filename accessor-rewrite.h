#pragma once

#include <clang-tidy/ClangTidyCheck.h>

#include <string>

class AccessorRewriteCheck : public clang::tidy::ClangTidyCheck
{
public:
  using ClangTidyCheck::ClangTidyCheck;

  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
  void check(clang::ast_matchers::MatchFinder::MatchResult const& Res) override;
  void storeOptions(clang::tidy::ClangTidyOptions::OptionMap& Opts) override;

private:
  std::string ClassName;
  llvm::StringMap<std::string> MemberToAccessor;
};
