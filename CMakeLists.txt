cmake_minimum_required(VERSION 4.0)
project(BitcoinTidy)

find_package(Clang REQUIRED)

add_library(bitcoin-tidy MODULE
  outpoint-assignment.cpp
  observers-base.cpp
  observers.cpp
  tx-deref.cpp
  main.cpp
  )

target_compile_features(bitcoin-tidy PRIVATE cxx_std_17)
target_include_directories(bitcoin-tidy PRIVATE ${CLANG_INCLUDE_DIRS})
target_link_libraries(bitcoin-tidy PRIVATE clangTidy)
