1 'Example program using NET$(0) function and NET$ statement
2 'Use Octagon Assign ID command to assign the ID 05 to the
3 'node that has an ID FF.
4 '
5 CLEAR NET$
10 NET$ = ">FFC0533." : 'Send Octagon Assign ID 05 command to node with ID FF
20 Y = 2000
30 DEC Y : IF Y = 0 THEN GOTO 70 : 'Give up if timeout
40 A$ = NET$(0) : 'Try to get reply from node with ID FF
50 IF A$="" THEN GOTO 30
60 PRINT "Received: ";A$
70 END
