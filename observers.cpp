#include "observers.h"

OutpointObservers::OutpointObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "COutPoint";
  MemberToAccessor["n"] = "GetIndex";
  MemberToAccessor["hash"] = "GetTxid";
}

TxinObservers::TxinObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CTxIn";
  MemberToAccessor["prevout"] = "GetPrevout";
  MemberToAccessor["scriptSig"] = "GetScript";
  MemberToAccessor["nSequence"] = "GetSequence";
  MemberToAccessor["scriptWitness"] = "GetWitness";
}

TxoutObservers::TxoutObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CTxOut";
  MemberToAccessor["nValue"] = "GetAmount";
  MemberToAccessor["scriptPubKey"] = "GetScript";
}

TxObservers::TxObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CTransaction";
  MemberToAccessor["vin"] = "GetInputs";
  MemberToAccessor["vout"] = "GetOutputs";
  MemberToAccessor["version"] = "GetVersion";
  MemberToAccessor["nLockTime"] = "GetLocktime";
}

BlockHdrObservers::BlockHdrObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CBlockHeader";
  MemberToAccessor["nVersion"] = "GetVersion";
  MemberToAccessor["hashPrevBlock"] = "GetPrevBlock";
  MemberToAccessor["hashMerkleRoot"] = "GetMerkeRoot";
  MemberToAccessor["nTime"] = "GetTime";
  MemberToAccessor["nBits"] = "GetBits";
  MemberToAccessor["nNonce"] = "GetNonce";
}

BlockObservers::BlockObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CBlock";
  MemberToAccessor["nVersion"] = "GetHeader().GetVersion";
  MemberToAccessor["hashPrevBlock"] = "GetHeader().GetPrevBlock";
  MemberToAccessor["hashMerkleRoot"] = "GetHeader().GetMerkeRoot";
  MemberToAccessor["nTime"] = "GetHeader().GetTime";
  MemberToAccessor["nBits"] = "GetHeader().GetBits";
  MemberToAccessor["nNonce"] = "GetHeader().GetNonce";
  MemberToAccessor["vtx"] = "GetTransactions";
}
