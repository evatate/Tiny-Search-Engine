#erroneous commands
./querier
./querier a
./querier a a.txt
./querier ~/cs50-dev/shared/tse/crawldata/toscrape-1 a
./querier a ~/cs50-dev/shared/tse/output/toscrape-1.index
./querier a ~/cs50-dev/shared/tse/output/toscrape-1.index b


# toScrape1 command

echo "funny or books" | ./querier ~/cs50-dev/shared/tse/crawldata/toscrape-1 ~/cs50-dev/shared/tse/output/toscrape-1.index

# Valgrind Check
echo "funny and books" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./querier ~/cs50-dev/shared/tse/crawldata/toscrape-1 ~/cs50-dev/shared/tse/output/toscrape-1.index


# Fuzz Testing

./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 0 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index
