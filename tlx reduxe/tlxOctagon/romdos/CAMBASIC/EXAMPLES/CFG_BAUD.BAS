1 'Example program using CONFIG BAUD, CONFIG COM$, ON COM$, COM$, CLEAR COM$
2 'RETURN COM$ and COMSTAT
3 '
4 '
5 '
10 CLEAR COM$ 1:CLEAR COM$ 3:'Make sure COM ports are initialized in beginning 
20 '
30 'Configure communication parameters
40 CONFIG BAUD 1,6,4,0:'COM1 = 9600, 8 bits, no parity, 1 stop bit
50 CONFIG BAUD 3,4,2,1,11:'COM3 = 2400, 7 bits, odd parity, 1 stop bit, IRQ11
60 '
70 'Define serial string termination
80 CONFIG COM$ 1,13,0,1,1:'<CR> terminates string at COM1, XON/XOFF, echo on
90 CONFIG COM$ 3,4,5,0,0:'Ctl-D or length = 5 terminates string at COM3
100 '
110 'Define and initialize valid data flags
120 COM1FLAG = 0
130 COM3FLAG = 0
140 '
150 'Declare locations of communication interrupt handlers
160 ON COM$ 1 GOSUB ..in_com1
170 ON COM$ 3 GOSUB ..in_com3
180 '
190 'Main body of program
200 ..idle_loop
210 IF COM1FLAG = 1 THEN PRINT INCOM1$:COM1FLAG = 0
220 'If valid COM1 data, print data to screen, reset valid data flag
230 IF COM3FLAG = 1 THEN PRINT INCOM3$:COM3FLAG = 0
240 'If valid COM3 data, print data to screen, reset valid data flag
250 FOR X = 1 TO 5000: NEXT X:'Simulate program doing other tasks
260 GOTO ..idle_loop
270 '
280 '
290 ..in_com1
300 'COM1 interrupt handler
310 'Check for communication errors with COMSTAT
320 IF COMSTAT(1) <> 0 THEN GOTO ..reset_com_1 ELSE GOTO ..good_com_1
330 '
340 ..reset_com_1
350 'If framing, over run or parity errors - reset COM1
360 CLEAR COM$ 1
370 CONFIG BAUD 1,6,4,0
380 CONFIG COM$ 1,13,0,1,1
390 GOTO ..return_com_1
400 '
410 ..good_com_1
420 INCOM1$ = COM$(1):'Assign COM1 data to INCOM1$
430 COM1FLAG = 1:'Set COM1 valid data flag
440 '
450 ..return_com_1
460 RETURN COM$
470 '
480 ..in_com3
490 'COM3 interrupt handler
500 'Check for communication errors with COMSTAT
510 IF COMSTAT(3) <> 0 THEN GOTO ..reset_com_3 ELSE GOTO ..good_com_3
520 '
530 ..reset_com_3
540 'If framing, over run or parity errors - reset COM3 
550 CLEAR COM$ 3
560 CONFIG BAUD 3,4,2,1,11
570 CONFIG COM$ 3,13,0,0,0
580 GOTO ..return_com_3
590 '
600 ..good_com_3
610 INCOM3$ = COM$(3)
620 COM3FLAG = 1
630 '
640 ..return_com_3
650 RETURN COM$
