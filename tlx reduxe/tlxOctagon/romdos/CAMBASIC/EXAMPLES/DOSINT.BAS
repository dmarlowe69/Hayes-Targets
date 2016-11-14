1 'Example of DOSINT statement
10 IREGS.AX = &4800: 'Prepare to call DOS int &21, function &48
20 IREGS.BX = &30: 'Request &300 bytes
30 DOSINT &21
40 IF OREGS.C <> 0 THEN PRINT "Memory request failed": END
50 ASMSEG = OREGS.AX: 'Save returned pointer to memory space
60 BLOAD "test.bin",0,ASMSEG:'Load user's assembly routine
70 CALL ABSOLUTE 0,ASMSEG: 'Call assembly routine at ASMSEG:0000
80 IREGS.AX = &4900: 'Prepare to call DOS int &21, function &49 to release memory
90 IREGS.ES = ASMSEG: 'Segment of memory to release
100 DOSINT &21
110 IF OREGS.C <> 0 THEN PRINT "Memory release failed":END
