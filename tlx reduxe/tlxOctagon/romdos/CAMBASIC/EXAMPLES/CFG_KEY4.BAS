1 'Example 4 of CONFIG KEYPAD$, KEYPAD$, ON KEYPAD$ and RETURN KEYPAD$ statements
10 DATA 65,66,67,68,69,70,71,72:'ASCII 'A' to 'H'
20 DATA 73,74,75,76,77,78,79,80:'ASCII 'I' to 'P'
30 CONFIG PIO &100,0,0,0,1,0:'Lower & upper C ports of an 8255 at &100 are input & output ports
40 OFFSET = KEYPAD$(2):SEGMENT = KEYPAD$(3):'Get keypad table address
50 FOR X = 0 TO 15
60      READ Y
70      POKE OFFSET + X, Y, SEGMENT:'Load new translation table
80 NEXT X
90 CONFIG KEYPAD$ &102,0,8:'Keypad is type 0 (16-key), C port address=&102, debounce=80ms
100 ON KEYPAD$ GOSUB ..getkey
110 ..idle
120 GOTO ..idle
130 ..getkey
140 A$ = KEYPAD$(0):'Get key
150 POSITION = KEYPAD$(1):'Get position of key
160 PRINT "Key pressed is ";A$,"key position is ",POSITION
170 RETURN KEYPAD$
