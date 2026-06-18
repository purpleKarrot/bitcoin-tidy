#include <clang-tidy/ClangTidyModule.h>

#include "observers.h"
#include "outpoint-assignment.h"
#include "tx-deref.h"

class BitcoinModule final : public clang::tidy::ClangTidyModule
{
public:
  void addCheckFactories(
    clang::tidy::ClangTidyCheckFactories& Factories) override
  {
    Factories.registerCheck<BlockObservers>("bitcoin-block-observers");
    Factories.registerCheck<CoinObservers>("bitcoin-coin-observers");
    Factories.registerCheck<OutpointAssignment>("bitcoin-outpoint-assignment");
    Factories.registerCheck<OutpointObservers>("bitcoin-outpoint-observers");
    Factories.registerCheck<TxDeref>("bitcoin-tx-deref");
    Factories.registerCheck<TxinObservers>("bitcoin-txin-observers");
    Factories.registerCheck<TxObservers>("bitcoin-tx-observers");
    Factories.registerCheck<TxoutObservers>("bitcoin-txout-observers");
  }
};

static clang::tidy::ClangTidyModuleRegistry::Add<BitcoinModule> X(
  "bitcoin-module", "Adds bitcoin checks.");

int volatile BitcoinModuleAnchorSource = 0;
