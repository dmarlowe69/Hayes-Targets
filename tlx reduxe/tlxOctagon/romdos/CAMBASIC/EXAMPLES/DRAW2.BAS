1 'Example 2 of DRAW statement
10 SCREEN 8
20 DRAW "M 50,80":'Absolute movement
30 DRAW "M 80,50"
40 DRAW "M+40,-20":'Relative movement
50 DRAW "M-40,-20"
60 DRAW "M-40,+20"
70 DRAW "M+40,+20"
80 'Using a string variable
90 Y = 190
100 X$ = "400" : Y$ = STR$(Y) : INC Y
110 S$ = "M" + X$ + "," + Y$
120 DRAW S$
