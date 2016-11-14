1 'Example of BCD function
10 N = BCD(6789)
20 M = N AND &FF
30 OUT 1,M
40 N = INT(N/&100)
50 OUT 2,N
