1 'Example of LOC function
10 OPEN "R",11,"TEST.TXT":'Open TEST.TXT for random access
20 FIELD 11,20 AS FNAME$:'Define the fields
30 LINE INPUT "Enter your name";A$
35 LSET FNAME$ = A$:'Left-justify the name
60 PUT 11,1:'Write to record 1
70 DO:PUT 11:LOOP UNTIL LOC(11) = 10
80 CLOSE 11
