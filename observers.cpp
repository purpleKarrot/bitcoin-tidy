#include "observers.h"

OutpointObservers::OutpointObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "COutPoint";
  MemberToAccessor["n"] = "GetIndex()";
  MemberToAccessor["hash"] = "GetTxid()";
}

TxinObservers::TxinObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CTxIn";
  MemberToAccessor["prevout"] = "GetPrevout()";
  MemberToAccessor["scriptSig"] = "GetScript()";
  MemberToAccessor["nSequence"] = "GetSequence()";
  MemberToAccessor["scriptWitness"] = "GetWitness()";
}

TxoutObservers::TxoutObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CTxOut";
  MemberToAccessor["nValue"] = "GetAmount()";
  MemberToAccessor["scriptPubKey"] = "GetScript()";
}

TxObservers::TxObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CTransaction";
  MemberToAccessor["vin"] = "GetInputs()";
  MemberToAccessor["vout"] = "GetOutputs()";
  MemberToAccessor["version"] = "GetVersion()";
  MemberToAccessor["nLockTime"] = "GetLockTime()";
}

BlockObservers::BlockObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "CBlock";
  MemberToAccessor["nVersion"] = "GetHeader().nVersion";
  MemberToAccessor["hashPrevBlock"] = "GetHeader().hashPrevBlock";
  MemberToAccessor["hashMerkleRoot"] = "GetHeader().hashMerkleRoot";
  MemberToAccessor["nTime"] = "GetHeader().nTime";
  MemberToAccessor["nBits"] = "GetHeader().nBits";
  MemberToAccessor["nNonce"] = "GetHeader().nNonce";
  MemberToAccessor["vtx"] = "GetTransactions()";
}

CoinObservers::CoinObservers(
  clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context)
  : ObserversBase(CheckName, Context)
{
  ClassName = "Coin";
  MemberToAccessor["out"] = "GetOutput()";
  MemberToAccessor["fCoinBase"] = "IsCoinBase()";
  MemberToAccessor["nHeight"] = "GetHeight()";
}
