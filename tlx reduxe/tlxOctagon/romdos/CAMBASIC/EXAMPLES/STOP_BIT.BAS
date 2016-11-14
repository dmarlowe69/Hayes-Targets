1 'Example of ON BIT, START BIT and STOP BIT statements
5 CONFIG PIO &100,0,1,0,0,0:'Configure an 8255 at &100 to be an input port
10 ON BIT 1,&100,5 GOSUB 100
20 START BIT 1
30 ..idle
40 GOTO ..idle
60 ..
100 STOP BIT 1
110 PRINT "Bit 5 of I/O address &100 has toggled."
120 START BIT 1
130 RETURN BIT
