1 'Example of FIELD statement
10 OPEN "R",11,"TEST.TXT":'Open TEST.TXT for random access
20 FIELD 11,20 AS FNAME$, 20 AS LNAME$, 40 AS SAYING$, 4 AS LUCKY$:'Define the fields
30 LINE INPUT "Enter your first name";A$
40 LSET FNAME$ = A$:'Left-justify the first name
50 LINE INPUT "Enter your last name";A$
60 LSET LNAME$ = A$:'Left-justify the last name
70 LINE INPUT "Enter your cool saying";A$
80 LSET SAYING$ = A$:'Left-justify the saying
90 INPUT "Enter your lucky number";N
100 RSET LUCKY$ = MKS$(N):'Convert numeric input to string, then right-justify it
110 PUT 11,1:'Write to record 1
120 CLOSE 11
