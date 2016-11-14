1 'Example of Fast Branching on Keys
10 'Branch on the Key Position
20 CONFIG PIO &100,0,1,1,1,0
30 CONFIG KEYPAD$ &102,0,8
40 ON KEYPAD$ GOSUB ..Key_interrupt
50 GOTO 50
60 ..Key_interrupt
70 K=KEYPAD$(1)
80 ON K GOSUB 100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600
90 RETURN KEYPAD$
100 ..service key position 1
110 PRINT "This is service for key position 1"
150 RETURN
200 ..service key position 2
210 PRINT "This is service for key position 2"
250 RETURN
300 ..service key position 3
310 PRINT "This is service for key position 3"
350 RETURN
400 ..service key position 4
410 PRINT "This is service for key position 4"
450 RETURN
500 ..service key position 5
510 PRINT "This is service for key position 5"
550 RETURN
600 ..service key position 6
610 PRINT "This is service for key position 6"
650 RETURN
700 ..service key position 7
710 PRINT "This is service for key position 7"
750 RETURN
800 ..service key position 8
810 PRINT "This is service for key position 8"
850 RETURN
900 ..service key position 9
910 PRINT "This is service for key position 9"
950 RETURN
1000 ..service key position 10
1010 PRINT "This is service for key position 10"
1050 RETURN
1100 ..service key position 11
1110 PRINT "This is service for key position 11"
1150 RETURN
1200 ..service key position 12
1210 PRINT "This is service for key position 12"
1250 RETURN
1300 ..service key position 13
1310 PRINT "This is service for key position 13"
1350 RETURN
1400 ..service key position 14
1410 PRINT "This is service for key position 14"
1450 RETURN
1500 ..service key position 15
1510 PRINT "This is service for key position 15"
1550 RETURN
1600 ..service key position 16
1610 PRINT "This is service for key position 16"
1650 RETURN
