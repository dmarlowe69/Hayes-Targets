1 'Example of EOF function
10 OPEN "O",11,"TEST1.TXT":'Open the TEST1.TXT file for output; assign file id 11 to it
20 PRINT #11,"This is the 1st line."
30 PRINT #11,"This is the 2nd line."
40 CLOSE 11:'Close the TEST1.TXT file
50 ..
60 OPEN "I",11,"TEST1.TXT":'Open the TEST1.TXT file for input
70 DO
80      INPUT #11,A$:PRINT A$
90 LOOP UNTIL EOF(11) = -1:'Loop until the end-of-file mark is reached
100 CLOSE 11
