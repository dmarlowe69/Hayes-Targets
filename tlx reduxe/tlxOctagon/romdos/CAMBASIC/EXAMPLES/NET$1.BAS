1 'Example program using NET$(0) function and NET$ statement
2 'Use Octagon Roll Call command to talk to the node FF.
3 'If node FF exists in the network, it will respond with a reply
4 'which conforms to the Octagon message format.
5 CLEAR NET$
10 NET$ = ">FFACC." : 'Send Octagon Roll-call command to node with ID FF.
20 Y = 2000
30 DEC Y : IF Y = 0 THEN GOTO 70 : 'Give up if timeout
40 A$ = NET$(0) : 'Try to get reply from node with ID FF
50 IF A$="" THEN GOTO 30
60 PRINT "Received: ";A$
70 END
