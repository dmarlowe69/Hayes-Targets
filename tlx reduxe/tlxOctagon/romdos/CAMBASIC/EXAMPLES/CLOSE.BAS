1 'Example of CLOSE statement
10 OPEN "O",11,"TEST1.BAS":'Open the TEST1.BAS file for output; assign file id 11 to it
20 OPEN "A",12,"TEST2.BAS":'Open the TEST2.BAS file for appending; assign file id 12 to it
30 PRINT #11,"This line is sent to TEST1.BAS"
40 PRINT #12,"This line is sent to TEST2.BAS"
50 CLOSE 11,12:'Close the TEST1.BAS and TEST2.BAS files
60 SHELL "TYPE TEST1.BAS
70 'List TEST1.BAS to the screen
80 PRINT:'Print a line to seperate the TEST2.BAS listing
90 SHELL "TYPE TEST2.BAS
100 'List TEST2.BAS to the screen
