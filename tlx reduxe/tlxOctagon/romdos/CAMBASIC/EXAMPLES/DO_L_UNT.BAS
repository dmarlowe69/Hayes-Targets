1 'Example of DO ..LOOP UNTIL statement
10 X = 0
20 DO
30      PRINT X:INC X
40 LOOP UNTIL X = 100:'Boolean expression is checked after the statements in the loop are executed
50 X = 0
60 DO UNTIL X = 100:'Boolean expression is checked before the statements in the loop are executed
70      PRINT X:INC X
80 LOOP
