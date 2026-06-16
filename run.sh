run-clang-tidy \
  -p /Users/dpfeifer/Projects/bitcoin/build \
  -config-file="/Users/dpfeifer/Projects/bitcoin-tidy/.clang-tidy" \
  -load="/Users/dpfeifer/Projects/bitcoin-tidy/build/libbitcoin-tidy.so" \
  -fix
