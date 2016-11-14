1 'Example of LIST statement
10 'This is a test program
20 X = INP(&100)
30 IF X = 1 THEN GOSUB ..valve_off ELSE GOSUB ..valve_on
40 DELAY 1
50 GOTO 20
60 END
70 ..valve_off
80 OUT &110,0
90 RETURN
100 ..valve_on
110 OUT &110,1
120 RETURN
