1  'Example of DEC, DECF statements
10 A = 4: B = 5: C = 6: D = 7
20 DEC A: DECF B:'Fast way to decrement
30 C = C - 1: D = D -4:'Slow way to decrement
40 PRINT A,B,C,D
