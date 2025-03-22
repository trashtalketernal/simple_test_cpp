This is my simple test case that i made out of curiosity(Two number multiplication).

The results were expected and really indicative

1. Case of reading from file directly and calculating after: ~65 000 +- 4000 microseconds
2. Case of loading all the data to RAM and then calculating: 1500 +-200 microseconds
3. Case of maping the data to memory. I used it because it was stated that OS will handle the caching of data and grant some performance but: 6000 +-400 microseconds
4. Case of loading data to ram and using AVX2 to efficiently chunk the data: 700 +-50 microseconds

Greate litle example of performance enhancement.
