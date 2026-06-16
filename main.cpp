#include <clang-tidy/ClangTidyModule.h>

#include "accessor-rewrite.h"

class BitcoinModule final : public clang::tidy::ClangTidyModule
{
public:
  void addCheckFactories(clang::tidy::ClangTidyCheckFactories& Factories) override
  {
    Factories.registerCheck<AccessorRewriteCheck>("bitcoin-accessor-rewrite");
  }
};

static clang::tidy::ClangTidyModuleRegistry::Add<BitcoinModule> X(
  "bitcoin-module", "Adds bitcoin checks.");

int volatile BitcoinModuleAnchorSource = 0;
