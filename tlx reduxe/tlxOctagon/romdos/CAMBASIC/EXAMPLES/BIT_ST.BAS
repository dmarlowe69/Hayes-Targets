1 'Example of BIT statement
10 CONFIG PIO &100,0,0,0,0,0
20 BIT &100,2,1:'Set bit 2 of I/O address &100
30 FOR A = 0 TO 1
40      BIT &100,5,A:'Write value of A to bit 5 of I/O address &100
50      DELAY 1
60 NEXT
70 BIT &100,1,OFF:'Reset bit 1 of I/O address &100
80 BIT &100,0,NOT:'Reset bit 1 of I/O address &100
90 DELAY .5
100 GOTO 80
