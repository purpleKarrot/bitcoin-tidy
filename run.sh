run-clang-tidy \
  -p /home/dp/Projects/bitcoin/build \
  -config-file="/home/dp/Projects/bitcoin-tidy/.clang-tidy" \
  -load="/home/dp/Projects/bitcoin-tidy/build/libbitcoin-tidy.so" \
  -j 1 \
  -fix
