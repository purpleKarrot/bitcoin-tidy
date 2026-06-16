cmake_minimum_required(VERSION 4.0)
project(BitcoinTidy)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(AddLLVM)
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(SYSTEM ${CLANG_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

add_library(bitcoin-tidy MODULE
  accessor-rewrite.cpp
  main.cpp
  )

target_compile_features(bitcoin-tidy PRIVATE cxx_std_20)
