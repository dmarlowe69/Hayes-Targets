1 'Example of PRESET, PSET statements
10 SCREEN 8:COLOR 1,1:CLS
20 FOR M = 0 TO 299 STEP 3
30      FOR N = M TO 20 + M
40              PSET (N,50),2:'Draw the line in new location
50      NEXT N
60      FOR N = M TO 20 + M
70              PRESET (N,50):'Erase the line
80      NEXT N
90 NEXT M
