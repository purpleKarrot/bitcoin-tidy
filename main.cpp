#include <clang-tidy/ClangTidyModule.h>

#include "observers.h"
#include "outpoint-assignment.h"

class BitcoinModule final : public clang::tidy::ClangTidyModule
{
public:
  void addCheckFactories(
    clang::tidy::ClangTidyCheckFactories& Factories) override
  {
    Factories.registerCheck<OutpointAssignment>("bitcoin-outpoint-assignment");
    Factories.registerCheck<OutpointObservers>("bitcoin-outpoint-observers");
    Factories.registerCheck<TxinObservers>("bitcoin-txin-observers");
    Factories.registerCheck<TxoutObservers>("bitcoin-txout-observers");
    Factories.registerCheck<TxObservers>("bitcoin-tx-observers");
    Factories.registerCheck<BlockHdrObservers>("bitcoin-blockhdr-observers");
    Factories.registerCheck<BlockObservers>("bitcoin-block-observers");
  }
};

static clang::tidy::ClangTidyModuleRegistry::Add<BitcoinModule> X(
  "bitcoin-module", "Adds bitcoin checks.");

int volatile BitcoinModuleAnchorSource = 0;
