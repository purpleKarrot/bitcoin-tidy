#pragma once

#include "observers-base.h"

class OutpointObservers : public ObserversBase
{
public:
  OutpointObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};

class TxinObservers : public ObserversBase
{
public:
  TxinObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};

class TxoutObservers : public ObserversBase
{
public:
  TxoutObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};

class TxObservers : public ObserversBase
{
public:
  TxObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};

class TxMutObservers : public ObserversBase
{
public:
  TxMutObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};

class BlockObservers : public ObserversBase
{
public:
  BlockObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};

class CoinObservers : public ObserversBase
{
public:
  CoinObservers(
    clang::StringRef CheckName, clang::tidy::ClangTidyContext* Context);
};
