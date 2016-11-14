1 'Example of INKEY$ function
10 PRINT "Enter Y or N"
20 A$ = INKEY$
30 IF A$ = "" GOTO 20:'Loop until there is a key pressed
40 IF A$  <> "Y" AND A$ <> "N" THEN 20
50 PRINT "You have entered ";A$
