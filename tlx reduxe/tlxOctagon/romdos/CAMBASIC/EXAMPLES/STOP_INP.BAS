1 'Example of ON INP, START INP and STOP INP statements
5 CONFIG PIO &100,0,1,0,0,0:'Configure an 8255 at &100 to be an input port
10 ON INP 1,&100,@00000011,@00000001 GOSUB 100
20 START INP 1
30 ..idle
40 GOTO ..idle
60 ..
100 STOP INP 1
110 PRINT "I/O condition satisfied."
120 START INP 1
130 RETURN INP
