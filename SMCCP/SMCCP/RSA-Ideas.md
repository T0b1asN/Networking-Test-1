
## RSA

### Idea 1

#### Encoding

1. cut Text into chucks (i.e. 4 chars)
   + ABCDEFGHIJKL -> ABCD EFGH IJKL
2. handle each chunk on its own
   + ABCD
3. Turn each character in chunk to ASCII
   + 065 066 067 068
   + __numbers should include a leading zero if they're under 100__
     + because you can then simply cut in three digit parts when decoding
4. concatinate the numbers in a string
   + "1065066067068"
   + should have non-zero number at start whjich is dismissed, because when character under 100 is at start, cutting doesn't work correctly
5. encode this number with RSA

#### Decoding

1. decode with RSA
2. Turn number back to string
   + "1065066067068"
3. cut characters out (ignore first char)
   + '065' '066' '067' '068'
4. get numbers from characters
   + 65 66 67 68
5. turn numbers to string
   + ABCD

__Problem:__ bitcount needs to be larger for larger excerpts of text

__Possible solution:__ split text inside the function in f.e. 4 char parts, encode them and add all parts together in one string, separated for example by a space (need to look into which chars aren't used by encoding).
