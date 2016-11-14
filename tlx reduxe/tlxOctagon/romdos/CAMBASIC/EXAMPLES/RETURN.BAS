1 'Example of RETURN statement
10 ON TICK 1 GOSUB ..dotick
20 PRINT "*"
30 FOR X = 0 TO 600:NEXT X: 'Idle here
35 GOSUB ..monitor : 'Go do something
40 GOTO 20
45 ..dotick
50 PRINT "TICK":RETURN TICK
70 RETURN TICK : 'Return from ON TICK interrupt
100 ..monitor
110 SW = INP(&100) : 'Read sitch ot &100 for example
120 RETURN : 'Return from GOSUB
