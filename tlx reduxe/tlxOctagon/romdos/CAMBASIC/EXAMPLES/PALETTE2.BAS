1 'Example 2 of PALETTE statement
5 DIM CARRY(3):'4 elements are enough for 16 byte entries
10 SCREEN 8
20 DRAW "C5":'Set color to magenta
30 DRAW "F60 L120 E60":'Draw a triangle in magenta
40 DRAW "BD30":'Move down to inside of triangle
50 DRAW "P14,5":'Paint the interior of triangle to yellow
60 PTR = VARPTR(CARRAY(0))
70 DEF SEG VARSEG(CARRAY(0))
80 FOR X = 0 TO 15
90 POKE PTR + X, 15 - X
100 NEXT X
110 PALETTE USING CARRAY(0):'Map color using new palette (yellow becomes blue)
