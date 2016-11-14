1 'Example of inputting a multi-digit number
10 'Input a multi-digit number
20 CONFIG PIO &100,0,1,1,1,0
30 CONFIG KEYPAD$ &102,0,8
40 ON KEYPAD$ GOSUB ..Key_interrupt
50 PRINT R
60 DELAY .25
70 GOTO 50
80 ..
90 ..Key_interrupt
100 B$=KEYPAD$(0)
110 IF B$=CHR$(13) THEN GOSUB ..Get_value
120 A$ = A$ + B$
130 RETURN KEYPAD$
140 ..
150 ..Get_value
160 R = VAL(A$)
170 A$ = "":B$ = ""
180 RETURN
