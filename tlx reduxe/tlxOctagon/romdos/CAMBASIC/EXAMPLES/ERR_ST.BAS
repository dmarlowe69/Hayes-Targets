1 'Example of ERR, ERROR statements
10 ON ERROR GOTO 100:'Enable error handler
20 ERR 11:'Simulate "divide by zero" error
25 ERR 0:'Simulate "Unknown" error
30 PRINT "Program ends"
40 END
100 PRINT "Inside error handler"
110 PRINT "Error Code = ";ERR
120 ON ERROR GOTO 100:'Re-enable error trapping
130 RESUME NEXT
