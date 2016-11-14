1 'Example of CLEAR TICKA statement
10 CONFIG PIO &100,0,1,1,1,1: 'Configure pio chip at &100 to be all inputs
20 CLEAR TICKA
30 IF BIT(&100,0) = 1 THEN 30:'Exit loop when bit 0 goes low
40 PRINT TICK(0)
