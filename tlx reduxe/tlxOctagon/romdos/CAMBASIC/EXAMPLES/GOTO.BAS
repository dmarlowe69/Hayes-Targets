1 'Example of GOTO statement
10 INPUT "Enter Y or N";REPLY$
20 IF REPLY$ = "Y" THEN GOTO ..good:'If Y, good answer, else fall through to check for N
30 IF REPLY$ <> "N" THEN GOTO 10:'Not Y, not N, start again
40 ..good
50 PRINT "You have entered ";REPLY$
