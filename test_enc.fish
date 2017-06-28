#!/usr/bin/fish

./ransom test
echo "Testing encrypted files..."
cd test
md5sum -c sums 
prevd

./ransom test keyfile
echo "Testing decrypted files..."
cd test
md5sum -c sums 
