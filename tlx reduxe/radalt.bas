DECLARE FUNCTION rx1! ()
DECLARE FUNCTION rx2! ()
init:
SCREEN 12
WINDOW (0, 0)-(640, 100)
o$ = "com1:9600,n,8,1,cd,cs,rs,ds"
DIM b(2000), f(2000), d(2000)


ON ERROR GOTO 100

fileno = 47
fi$ = CHR$(fileno)






INPUT "Enter filename "; f$
'f1$ = f$ + fi$ + ".dat"





OPEN o$ FOR RANDOM AS #1
'OPEN f1$ FOR OUTPUT AS #2
strt:


CLOSE #2
fileno = fileno + 1
fi$ = CHR$(fileno)
f1$ = f$ + fi$ + ".dat"
OPEN f1$ FOR OUTPUT AS #2

rstrt:
CLS
PRINT "Radalt diagnostic software"


PRINT "1        Normal mode"
PRINT "2        Fixed scan"
PRINT "3        Ram dump"
PRINT "4        Calibrate scan"
PRINT "5        Set quality threshold"
PRINT "6        Set search point"
PRINT ""
INPUT g

IF g = 1 THEN GOTO stn
IF g = 2 THEN GOTO fs
IF g = 3 THEN GOTO rdmp
IF g = 4 THEN GOTO cal
IF g = 5 THEN GOTO smoo
IF g = 6 THEN GOTO setp





stn:


checksum = 35
checksum = checksum XOR 1
checksum = checksum XOR 0
checksum = checksum XOR 0
checksum = checksum XOR 0

PRINT #1, CHR$(1);
PRINT #1, CHR$(35);
PRINT #1, CHR$(1);
PRINT #1, CHR$(0);
PRINT #1, CHR$(0);
PRINT #1, CHR$(0);
PRINT #1, CHR$(checksum)



st: CLS
sttt:

LOCATE 1, 1




a = ASC(INPUT$(1, 1))
IF a <> 1 THEN GOTO sttt
a = ASC(INPUT$(1, 1))
IF a <> 35 THEN GOTO sttt

checksum = 35

a = rx1
checksum = checksum XOR a
b = rx1
checksum = checksum XOR b

range = (256 * a) + b



a = rx1
checksum = checksum XOR a
b = rx1
checksum = checksum XOR b
l = (256 * a) + b

range = range / 10



a = rx1
checksum = checksum XOR a

IF checksum <> 0 THEN GOTO sttt



a = ASC(INPUT$(1, 1))
IF a <> 1 THEN GOTO sttt
a = ASC(INPUT$(1, 1))
IF a <> 36 THEN GOTO sttt

checksum = 36

hr = ASC(INPUT$(1, 1))
checksum = checksum XOR hr



mux = ASC(INPUT$(1, 1))

checksum = checksum XOR mux

deviation = rx1
checksum = checksum XOR deviation
dummy = rx1
checksum = checksum XOR dummy

a = rx1
checksum = checksum XOR a
IF checksum <> 0 THEN GOTO sttt

level = l

PRINT USING "####.# ##### ##### ##### ##### ####"; range; level; hr; deviation; mux; dummy
PRINT #2, USING "####.# ##### #### #### ####"; range; level; hr; deviation; mux






n = n + 1
PSET (n, range)
PSET (n, level / 60), 3

IF (n > 640) THEN
CLS
n = 0
END IF


IF INKEY$ <> "" THEN GOTO strt







GOTO sttt


100 RESUME NEXT

fs:
INPUT "Enter height ", h
INPUT "Enter gain (1,2,3,4 or 5) "; g
gh = 136
IF g = 1 THEN gh = 136
IF g = 2 THEN gh = 137
IF g = 3 THEN gh = 138
IF g = 4 THEN gh = 139

IF h > 255 THEN
hhi = h / 256
hlo = h - 256
ELSE
hhi = 0
hlo = h
END IF

checksum = 35
checksum = checksum XOR 2
checksum = checksum XOR gh
checksum = checksum XOR hhi
checksum = checksum XOR hlo

PRINT #1, CHR$(1);
PRINT #1, CHR$(35);
PRINT #1, CHR$(2);
PRINT #1, CHR$(gh);
PRINT #1, CHR$(hhi);
PRINT #1, CHR$(hlo);
PRINT #1, CHR$(checksum)


GOTO st

cal:
INPUT "Enter height ", h
INPUT "Enter gain (1,2,3,4 or 5) "; g
gh = 136
IF g = 1 THEN gh = 136
IF g = 2 THEN gh = 137
IF g = 3 THEN gh = 138
IF g = 4 THEN gh = 139


IF h > 255 THEN
hhi = h / 256
hlo = h - 256
ELSE
hhi = 0
hlo = h
END IF



PRINT #1, CHR$(36);
PRINT #1, CHR$(4);
PRINT #1, CHR$(gh);
PRINT #1, CHR$(hhi);
PRINT #1, CHR$(hlo);

GOTO strt





rdmp:

nptso = 0

CLS
INPUT "Enter height ", h
INPUT "Enter gain (1,2,3,4 or 5) "; g

IF h > 255 THEN
hhi = h / 256
hlo = h - 256
ELSE
hhi = 0
hlo = h
END IF



gh = 136
IF g = 1 THEN gh = 136
IF g = 2 THEN gh = 137
IF g = 3 THEN gh = 138
IF g = 4 THEN gh = 139


a = LOC(1)
FOR n = 1 TO a
a$ = INPUT$(1, 1)
NEXT



checksum = 35
checksum = checksum XOR 3
checksum = checksum XOR gh
checksum = checksum XOR hhi
checksum = checksum XOR hlo

PRINT #1, CHR$(1);
PRINT #1, CHR$(35);
PRINT #1, CHR$(3);
PRINT #1, CHR$(gh);
PRINT #1, CHR$(hhi);
PRINT #1, CHR$(hlo);
PRINT #1, CHR$(checksum)





CLS





ofs = 2048
fsd = 2048


rd2:

IF ASC(INPUT$(1, 1)) <> 35 THEN GOTO rd2
IF ASC(INPUT$(1, 1)) <> 77 THEN GOTO rd2


CLS
npts = rx2

LOCATE 2, 1
PRINT USING "##### #####"; min; max


max = 0
min = 4096
LOCATE 0, 0
PRINT npts



IF npts > 1000 THEN GOTO rd2
IF npts = 0 THEN GOTO rd2


IF npts <> nptso THEN

nptso = npts
WINDOW (0, -1)-(npts, 1)
LOCATE 1, 1
PRINT npts
END IF

PRINT #2, ""
PRINT #2, npts
PRINT #2, "start"
FOR n = 0 TO npts - 2

v = rx2
PRINT #2, v

'save max and min values
IF v > max THEN max = v
IF v < min THEN min = v
IF v >= 4095 THEN v = 4000

PSET (n, (v - ofs) / fsd)


'save unfiltered data
b(n) = v




NEXT

GOTO smug
FOR n = 0 TO npts - 7
f(n) = 0
FOR g = 0 TO 4
f(n) = f(n) + b(n + g)
NEXT g
f(n) = f(n) / 5
NEXT n

FOR n = 0 TO npts - 7
PSET (n, (f(n) - ofs) / fsd), 3
NEXT

FOR n = 0 TO npts - 8
d(n) = f(n + 1) - f(n)
IF d(n) > 0 THEN d(n) = .5
IF d(n) < 0 THEN d(n) = -.5

PSET (n, d(n)), 7
NEXT

'INPUT k$
'CLS



smug:



k$ = INKEY$



IF k$ = "c" THEN
CLS
GOTO rd2

END IF

IF k$ = "s" THEN
INPUT " enter new offset and max scale value ", ofs, fsd
CLS
GOTO rd2
END IF


IF k$ = "" THEN GOTO rd2


GOTO init



smoo:

CLS
INPUT "Enter new threshold ", thresh

checksum = 35
checksum = checksum XOR 5
checksum = checksum XOR thresh
checksum = checksum XOR 0
checksum = checksum XOR 0

PRINT #1, CHR$(1);
PRINT #1, CHR$(35);
PRINT #1, CHR$(5);
PRINT #1, CHR$(thresh);
PRINT #1, CHR$(0);
PRINT #1, CHR$(0);
PRINT #1, CHR$(checksum)


GOTO rstrt

setp:

CLS
INPUT "Enter search height ", h

IF h > 255 THEN
hhi = h / 256
hlo = h - 256
ELSE
hhi = 0
hlo = h
END IF


checksum = 35
checksum = checksum XOR 8
checksum = checksum XOR 137
checksum = checksum XOR hhi
checksum = checksum XOR hlo

PRINT #1, CHR$(1);
PRINT #1, CHR$(35);
PRINT #1, CHR$(8);
PRINT #1, CHR$(137);
PRINT #1, CHR$(hhi);
PRINT #1, CHR$(hlo);
PRINT #1, CHR$(checksum)


GOTO st

FUNCTION rx1

rx1 = ASC(INPUT$(1, 1))




END FUNCTION

FUNCTION rx2

b = ASC(INPUT$(1, 1))
a = ASC(INPUT$(1, 1))

rx2 = a + 256 * b


END FUNCTION

