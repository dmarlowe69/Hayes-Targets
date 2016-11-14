1 'Example 1 of BLOAD statement
10 DEF SEG &6000:'Set segment to &6000
20 BLOAD "PROG1", &F000:'Load at address 6000:F000
30 DIM X(100):'Declare an array space of 400 bytes
40 BLOAD "PROG2", VARPTR(X(0)), VARSEG(X(0)):'Load PROG at array space
