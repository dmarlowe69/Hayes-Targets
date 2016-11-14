1 'Example of RESUME statement
10 ON ERROR GOTO 1000:'Error handler
15 X = 0
20 A = 5 / X:'Generate divide by zero error
30 PRINT "X = ";X,"5 / x = ";A
40 PRINT "Program ends"
50 END
1000 PRINT "Divide by zero error at line ";ERL
1010 PRINT "Error code is ";ERR
1020 X = X + .1:'Adjust x a little bit to correct problem
1030 RESUME
