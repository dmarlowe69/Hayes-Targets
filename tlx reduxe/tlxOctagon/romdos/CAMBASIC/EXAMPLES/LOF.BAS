1 'Example of LOF function
10 OPEN "R",12,"TEST.BIN":'Open TEST.BIN for random access; assign file id 12 to file
20 PRINT "File size = ";LOF(12)
30 CLOSE 12
