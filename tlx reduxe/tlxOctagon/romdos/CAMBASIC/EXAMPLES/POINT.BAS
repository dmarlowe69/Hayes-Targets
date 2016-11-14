1 'Example of POINT function
10 SCREEN 8
20 VIEW (50,50)-(250,250):'Define graphics area
30 CIRCLE (150,100),50:'Draw a circle (at logical x,y of 150,100,physical x,y at 150+50,100+50)
40 PAINT (150,100),14:'Paint it yellow
50 X = POINT(0):Y = POINT(1):LX = POINT(2): LY = POINT(3)
60 PRINT "Physical Coordinates x,y = "X, Y
70 PRINT "Logical Coordiantes x,y = ";LX, LY
80 PRINT "Color number = ";POINT(150,100)
