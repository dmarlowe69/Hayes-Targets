1 'Example of DELAY statement
10 PRINT TIME$(0)
20 DELAY 5.5:'Delay that suspends interrupts
30 PRINT TIME$(0)
40 ..idle
50 FOR X = 1 TO 2000:'Delay that allows interrupts to occur
60 ..DO NOTHING
70 NEXT
80 PRINT TIME$(0)
