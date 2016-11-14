1 'Example of PUT statement in graphics routine
5 DIM CARRAY(2000):'Declare an array to hold image
10 SCREEN 8
20 CIRCLE (100,100),49:'Draw a circle
30 CIRCLE (100,100),30:'Draw another concentric circle
40 PAINT (100,65),14:'Paint it yellow
50 GET (50,50)-(150,150),CARRAY(0):'Save image
60 PUT (250,50)-(350,150),CARRAY(0):'Put image elsewhere
