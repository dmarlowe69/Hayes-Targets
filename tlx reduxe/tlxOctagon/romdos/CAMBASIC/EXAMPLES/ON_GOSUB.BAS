1 'Example of ON ..GOSUB, ON ..GOTO statements
10 FOR X = 1 TO 3
20      ON X GOSUB 100, 200, 300
30 NEXT 
40 END
50 ..
100 'Initialize serial port here, for example
105 PRINT "Initialized serial port."
110 RETURN
130 ..
200 'Initialize display port here, for example
205 PRINT "Initialized display port."
210 RETURN
230 ..
300 'Initialize keypad port here, for example
305 PRINT "Initialized keypad port."
310 RETURN
