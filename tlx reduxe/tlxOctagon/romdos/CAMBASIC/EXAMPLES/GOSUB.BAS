1 'Example of GOSUB statement
10 FOR X = 1 TO 3
20 GOSUB ..print_square
30 NEXT X
40 END
50 ..print_square
60 PRINT "Square of ";X;" = ";X * X
70 RETURN
