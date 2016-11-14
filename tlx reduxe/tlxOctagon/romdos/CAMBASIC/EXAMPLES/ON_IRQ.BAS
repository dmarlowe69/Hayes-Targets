10 'This example uses the hardware interrupt IRQ5 and a software induced 
20 'interrupt IRQ11
30 '
40 ON IRQ 5 GOSUB ..irq5
50 ON IRQ 11 GOSUB ..irq11
60 '
70 ..start
80 PRINT "This is a test of the ON_IRQ command."
90 FOR X = 0 TO 1000:FOR Y = 0 TO 3:NEXT Y:NEXT X:'This is just a delay loop
100 IRQ 11:'IRQ11 is software induced
110 GOTO ..start
120 '
130 ..irq5
140 PRINT "IRQ5 has been detected."
150 RETURN IRQ
160 '
170 ..irq11
180 PRINT "IRQ11 has been detected."
190 RETURN IRQ
