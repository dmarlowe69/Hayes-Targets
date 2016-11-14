1 'Example of BSAVE statement
10 DIM X(100), Y(100):'Declare 2 array of 400 bytes each (each array element is 4 bytes).
20 X(0) = 12:X(1) = 34:'Save values to array elements
30 BSAVE "PROG1.BIN", VARPTR(X(0)), 400,VARSEG(X(0))
40 PRINT Y(0),Y(1)
50 BLOAD "PROG1.BIN", VARPTR(Y(0)), VARSEG(Y(0)):'Load PROG1.BIN to another array
60 PRINT Y(0),Y(1)
