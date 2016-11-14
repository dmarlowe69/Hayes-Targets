1 'Example 1 of CONFIG KEYPAD$, KEYPAD$, ON KEYPAD$ and RETURN KEYPAD$ statements
10 CONFIG PIO &100,0,0,0,1,0:'Lower and upper C ports of an 8255 at &100 are input and output ports
20 CONFIG KEYPAD$ &102,0,8:'Keypad type = 0 (16-key), port C address = &102, debounce = 80ms
30 ON KEYPAD$ GOSUB ..getkey
40 ..idle
50 GOTO ..idle
100 ..getkey
110 A$ = KEYPAD$(0):'Get key
120 POSITION = KEYPAD$(1):'Get position of key
130 PRINT "Key pressed is ";A$,"key position is ",POSITION
140 IF A$ = "0" THEN ON KEYPAD$ GOSUB:'Disable keypad if the key pressed is "0"
150 RETURN KEYPAD$

