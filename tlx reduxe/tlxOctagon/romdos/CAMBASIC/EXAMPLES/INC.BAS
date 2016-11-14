1 'Example of INC, INCF statements
10 A = 4: B = 5: C = 6: D = 7
20 INC A: INCF B:'Fast way to increment
30 C = C + 1: D = D + 4:'Slow way to increment
40 PRINT A,B,C,D
