1 'Example of BIT function
5 CONFIG PIO &100,0,0,0,0,0:'Configure 8255 at &100 to be all output ports
10 OUT &100, @11010001
20 FOR X = 7 TO 0 STEP -1:'Display bit 7 first
30 PRINT BIT(&100,X);"";
40 NEXT X
50 PRINT
