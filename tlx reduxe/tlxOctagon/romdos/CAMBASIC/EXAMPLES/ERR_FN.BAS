1 'Example of ERR, ERROR functions
10 ON ERROR GOTO 1000:'Error handler
15 X = 0
20 A = 5 / X:'Generate divide by zero error
30 PRINT "X = ";X,"5 / X = ";A
40 PRINT "Program ends"
50 END
1000 PRINT "Divide by zero error at line ";ERL
1005 PRINT "Error code is ";ERR
1010 X = X+ .1:'Adjust X a little bit to correct problem
1020 ON ERR GOTO 1000:'Re-enable error trapping
1030 RESUME
