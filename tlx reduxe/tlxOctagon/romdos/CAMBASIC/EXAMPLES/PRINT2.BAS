1 'Example 2 of PR, PRINT statement
10 OPEN "O",11,"TEST1.BAS":'Open the TEST1.BAS file for output; assign file id 11 to it
20 OPEN "I",12,"TEST2.BAS":'Open the TEST2.BAS file for input; assign file id 12 to it
30 PRINT #11,"This line is sent to TEST1.BAS"
40 CLOSE 11,12:'Close the TEST1.BAS and TEST2.BAS files
