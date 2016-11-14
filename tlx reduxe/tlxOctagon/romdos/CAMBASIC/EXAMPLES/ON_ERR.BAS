1 'Example of ON ERR, ON ERROR statement
10 ON ERROR GOTO 1000:'Error handler
20 X = 0
30 A = 5 / X:'Generate divide by zero error
40 PRINT "X = ";X,"5 / x = ";A
50 PRINT "Program ends"
60 END
1000 PRINT "Divide by zero error at line ";ERL
1010 PRINT "Error code is ";ERR
1020 X = X + .1:'Adjust x a little bit to correct problem
1030 RESUME
