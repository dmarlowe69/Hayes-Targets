1 'Example 2 of CONFIG KEYPAD$, KEYPAD$, ON KEYPAD$ and RETURN KEYPAD$ statements
20 CONFIG KEYPAD$ &378,0,8:'Keypad type=0 (16-key),, port address=&378, debounce=80ms
30 ON KEYPAD$ GOSUB ..getkey
40 ..idle
50 GOTO ..idle
100 ..getkey
110 A$ = KEYPAD$(0):'Get key
120 POSITION = KEYPAD$(1):'Get position of key
130 PRINT "Key pressed is ";A$,"key position is ",POSITION
140 IF A$ = "0" THEN ON KEYPAD$ GOSUB:'Disable keypad if the key pressed is "0"
150 RETURN KEYPAD$
