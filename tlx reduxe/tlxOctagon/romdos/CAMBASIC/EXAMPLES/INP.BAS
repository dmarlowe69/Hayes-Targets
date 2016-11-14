1 'Example of INP statement
5 CONFIG PIO &100,0,0,0,0,0: 'Configure 5255 at &100 to be all output ports
10 OUT &100,12
20 PRINT INP(&100)
