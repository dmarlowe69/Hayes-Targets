1 'Example of CVS function
10 OPEN "R",11,"TEST.TXT":'Open TEST.TXT for random access
20 FIELD 11,20 AS FNAME$,4 AS LUCKY$:'Define the fields
30 LINE INPUT "ENTER YOUR NAME";A$
35 LSET FNAME$ = A$:'Left-justify the name
40 INPUT "ENTER YOUR AGE";N
50 RSET LUCKY$ = MKS$(N):'Convert numeric input to 4 -byte string representation, right-justify it
60 PUT 11,1:'Write to record 1
70 CLOSE 11
80 ..
90 OPEN "R",11,"TEST.TXT":'Open TEST.TXT for random access
100 FIELD 11,20 AS FN$, 4 AS L$:'Define the fields
110 GET 11,1:'READ IN RECORD 1
120 PRINT "YOUR NAME IS ";FN$
130 PRINT "NEXT YEAR YOU WILL BE ",CVS(L$) + 1:'Convert 4-byte representation to numeric
140 CLOSE 11
