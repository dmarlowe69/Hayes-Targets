1 'Example of OUT statement
10 CONFIG PIO &100,0,0,0,0,0: 'Configure 5255 at &100 to be all output ports
20 OUT &100,12
30 PRINT INP(&100)
