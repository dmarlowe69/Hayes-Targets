1 'Example of CONFIG TIMER, CLEAR TIMER, RESUME TIMER, START TIMER, STOP TIMER and TIMER statement
10 CONFIG PIO &100,0,0,0,0,0:'Configure an 8255 at &100 to be output ports
20 CLEAR TIMER 0:'Make sure timer 0 is cleared and deactivated in the beginning
30 CONFIG TIMER 0,&100,1,1.05,2.55,1,AUTO:'Timer 0, I/O address &100, bit 1.
40 START TIMER 0:'Continuous signal at &100, bit 1
50 FOR Z = 1 TO 10
60      DELAY 5
70      PRINT TIMER(0):'Print timer 0 every 5 secs while there is continous output @ &100, bit 1
80 NEXT Z
90 STOP TIMER 0:'Suspends timer 0
100 PRINT "Suspend timer 0 for 10 seconds":DELAY 10:'No output at &100, bit 1
110 RESUME TIMER 0:'Re-enable timer 0.  Continous output at &100, bit 1 again.
120 PRINT "Resume timer 0"
130 GOTO 130;'Continue output at &100, bit 1
