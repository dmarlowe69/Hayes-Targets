1 'Example 2 of CALL ABSOLUTE statement
10 IREGS.AX = &4800:'Prepare to call DOS int &21, func &48
20 IREGS.BX = &30:'Request &300 bytes
30 DOSINT &21
40 IF OREGS.C <> 0 THEN PRINT "Memory request failed":END
50 ASMSEG = OREGS.AX:'Save returned pointer to memory space
60 BLOAD "test.bin",0,ASMSEG:'Load users's assembly routine
70 CALL ABSOLUTE 0,ASMSEG,3,4:'Call assembly routine at ASMSEG:0000 to get 3 to the power of 4
75 PRINT "3 to the power 4 = ";DPEEK(4,ASMSEG):'Result is at location 4 in this example.
80 IREGS.AX = &4900:'Prepare to call DOS int &21, function &49 to release memory
90 IREGS.ES = ASMSEG:'Segment of memory to release
100 DOSINT &21
110 IF OREGS.C <> 0 THEN PRINT "Memory release failed":END
