#include <clang-tidy/ClangTidyModule.h>

#include "combine-assignments.h"
#include "tx-deref.h"
#include "use-observers.h"

class BitcoinModule final : public clang::tidy::ClangTidyModule {
public:
  void
  addCheckFactories(clang::tidy::ClangTidyCheckFactories &Factories) override {
    Factories.registerCheck<CombineAssignments>("bitcoin-combine-assignments");
    Factories.registerCheck<TxDeref>("bitcoin-tx-deref");
    Factories.registerCheck<UseObservers>("bitcoin-use-observers");
  }
};

static clang::tidy::ClangTidyModuleRegistry::Add<BitcoinModule>
    X("bitcoin-module", "Adds bitcoin checks.");

int volatile BitcoinModuleAnchorSource = 0;
