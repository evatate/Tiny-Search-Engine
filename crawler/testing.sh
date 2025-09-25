# !/bin/bash

#Error Checking

#No Extra Arguments
./crawler

#One Argument
./crawler 1

#Two Arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ test

#External URL
./crawler https://github.com/CS50DartmouthWI24/home/blob/main/knowledge/units/indexer.md test 1

#Max Depth out of Range
./crawler https://github.com/CS50DartmouthWI24/home/blob/main/knowledge/units/indexer.md test 100


mkdir test
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../indexer/data1 1


mkdir lettersTest1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html lettersTest1 1

mkdir lettersTest3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../indexer/data 2

mkdir lettersTest9
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html lettersTest9 9