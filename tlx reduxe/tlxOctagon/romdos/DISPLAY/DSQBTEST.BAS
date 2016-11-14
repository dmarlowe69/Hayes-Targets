DECLARE SUB PRINTDISPLAY (DSPTYP!)
'
DECLARE SUB ANYKEY ()
DECLARE SUB MOVECURSOR (B)
DECLARE SUB LPRINTS (A$)
' The following three routines are used when the main console is through
' COM1. (They work when its not COM1 as well...)
DECLARE FUNCTION KEYHIT$ ()
DECLARE FUNCTION INKEY2$ ()
DECLARE SUB prints (A$)
DECLARE SUB printsl (A$)
'
DECLARE SUB CENTERS (A$)
DECLARE FUNCTION DSPINST ()
DECLARE FUNCTION GETTYPE ()
DECLARE FUNCTION GETLPTN ()
COMMON SHARED LPT, DSPTYPE, MAXCOL, CRLF$
'$INCLUDE: 'QB.BI'
'
'  Test program for the VF and LCD displays using DISPLAY.EXE.
'  To compile this use the following line with QuickBasic 4.5
'     QB DSQBTEST /L
'
'  Before executing this basic program, do this:
'   1) Turn off power to the system.
'   2) Connect the display to the DP-IFB or LCD-IFB card.
'   3) Connect power to the IFB card ( if needed ).
'   4) Connect the IFB card to the CPU/5540/5600... card.
'   5) Apply power to the system.
'   6) Execute DISPLAY.EXE with the correct display option.
'
CRLF$ = CHR$(10) + CHR$(13)' set up line feed and carriage return string

DSPTYPE = DSPINST
IF (DSPTYPE = 0) THEN END  ' display not installed/no displays options used

'
'  NUMROWS=NUMBER OF ROWS, MAXCOL=MAX COLUMN, DSPTYPE=DISPLAY TYPE
'
IF (DSPTYPE = 1) THEN NUMROWS = 2: MAXCOL = 20
IF (DSPTYPE = 2) THEN NUMROWS = 4: MAXCOL = 20
IF (DSPTYPE = 3) THEN NUMROWS = 4: MAXCOL = 40
IF (DSPTYPE = 4) THEN NUMROWS = 4: MAXCOL = 20
IF (DSPTYPE = 5) THEN NUMROWS = 2: MAXCOL = 40
IF (DSPTYPE = 6) THEN NUMROWS = 2: MAXCOL = 20
'
'  clear the display first and display header
'
CALL LPRINTS(CHR$(&H15))
IF NUMROWS > 2 THEN CALL LPRINTS(CRLF$)
CALL CENTERS("DISPLAY test routine")
printsl "DISPLAY test routine"
CALL ANYKEY
'
'  Print something on each line.
'  End the last line to be displayed with ";" so it doesn't scroll up
'
CALL LPRINTS(CRLF$)
FOR n = 1 TO NUMROWS
CALL LPRINTS("This is line ")
CALL LPRINTS(STR$(n - 1))
IF n <> NUMROWS THEN CALL LPRINTS(CRLF$)
NEXT n
CALL ANYKEY
'
'  Clear the display and do cursor stuff
'
CALL LPRINTS(CHR$(&H15))
printsl "try some cursor control stuff"
FOR n = 0 TO NUMROWS - 1
prints "Move cursor to position " + STR$(n * 5)
printsl " of line " + STR$(n) + " and print <<<"
CALL LPRINTS(CHR$(&H1B))
CALL LPRINTS(CHR$((n * &H40) + (n * 5)))
CALL LPRINTS("<<<")
NEXT n
CALL ANYKEY
'
' Now clear the display
'
printsl "Now clear the display"
CALL LPRINTS(CHR$(&H15))
CALL ANYKEY
'
' Now try the applicable cursor changes
'
' The DP 2x20 doesn't have a cursor skip
IF DSPTYPE = 1 THEN GOTO LEFTRIGHT
'
printsl "Now to change the cursor"
CALL LPRINTS("Turn the cursor off")
CALL LPRINTS(CHR$(&HE))
CALL ANYKEY
CALL LPRINTS(CRLF$)
CALL LPRINTS("Now back on again")
CALL LPRINTS(CHR$(&HF))
CALL ANYKEY
CALL LPRINTS(CRLF$)
'  The DP 4x20 doesn't have the blinking cursor option
IF DSPTYPE = 2 THEN GOTO LEFTRIGHT
CALL LPRINTS("Now make it blink")
CALL LPRINTS(CHR$(&H17))
CALL LPRINTS(CHR$(&H1))
CALL ANYKEY
CALL LPRINTS(CRLF$)
CALL LPRINTS("Now back to normal")
CALL LPRINTS(CHR$(&H17))
CALL LPRINTS(CHR$(0))
CALL ANYKEY
CALL LPRINTS(CRLF$)
'
'  Do the cursor movement left/right test
'
LEFTRIGHT:
CALL LPRINTS(CRLF$)
printsl "Move the cursor by 1 character left/right"
IF DSPTYPE = 1 THEN printsl "and display a * at the new location"

printsl "Forward"
CALL LPRINTS("Cursor - Forward" + CRLF$)
CALL MOVECURSOR(9)
CALL LPRINTS(CRLF$)

printsl "Backward"
CALL LPRINTS("Cursor - Backward" + CRLF$)
FOR n = 1 TO MAXCOL - 2
  CALL LPRINTS(" ")
NEXT n
CALL MOVECURSOR(8)

CALL ANYKEY
'
'  print end of test message
'
CALL LPRINTS(CHR$(&H15))
IF NUMROWS > 2 THEN CALL LPRINTS(CRLF$)
CALL CENTERS("End of DISPLAY test")
END

'
'  Now for the subroutines
'
'  The ANYKEY subroutine
'
SUB ANYKEY
prints "Press any key to continue"
DO WHILE (INKEY2$ = "")
LOOP
printsl ""
END SUB

'
' This centers the string in the display
'
SUB CENTERS (A$)
CALL LPRINTS(SPACE$((MAXCOL - LEN(A$)) / 2) + A$)
END SUB

'
'   DSPINST ()
'   This routine uses INT 17, function 3, subfunction 6 and
'   INT 17, function 7 to determine the number of displays and
'   type of displays installed. When more than one display is
'   installed it prompts the user to which display to test
'   Returns the display type to use and sets global LPT
'   If display is not installed or no displays are installed
'   the display type returned is 0.
'
FUNCTION DSPINST

DIM inregs AS RegType, outregs AS RegType
DIM dsptypes(3)

numdps = 0                              ' initialize number of keypads
FOR j = 0 TO 2
  dsptypes(j + 1) = 0
NEXT j
FOR I = 0 TO 2                          ' check each LPT
inregs.ax = &H306                       ' check for display installation
inregs.dx = I                           ' dp lpt number
CALL INTERRUPT(&H17, inregs, outregs)   ' do int to check if tsr running
IF ((outregs.ax = 1) AND (outregs.bx = 2) AND (outregs.cx = 3)) THEN
  inregs.ax = &H307                     ' find out which keypad type in use
  inregs.dx = I                         ' dp lpt number
  CALL INTERRUPT(&H17, inregs, outregs) ' do interrupt
  j = (outregs.ax AND &HFF00) / &H100   ' get display type
  IF (j <> 0) THEN
    numdps = numdps + 1
    LPT = I
    dsptypes(LPT + 1) = j             ' save display type
  END IF
END IF
NEXT I

IF numdps = 0 THEN
  CALL prints("DISPLAY NOT EXECUTED WITH DISPLAY OPTION ")
  CALL prints("OR DISPLAY NOT EXECUTED." + CRLF$)
  j = 0
ELSEIF numdps = 1 THEN
  j = dsptypes(LPT + 1)
ELSE
  j = 0
  WHILE (j = 0)
  FOR I = 0 TO 2
    IF (dsptypes(I + 1) <> 0) THEN
      CALL PRINTDISPLAY(dsptypes(I + 1))
      CALL prints(" connected to LPT" + RIGHT$(STR$(I + 1), 1) + "." + CRLF$)
    END IF
  NEXT I

  CALL prints("Which LPT number is the test display on?")

  A$ = ""
  WHILE (A$ = "")          ' while no keyboard key is pressed.
    A$ = INKEY2$
  WEND
  IF A$ >= "1" AND A$ <= "3" THEN
    j = dsptypes(VAL(A$))
    IF (j <> 0) THEN LPT = VAL(A$) - 1
  END IF
  CALL prints(CRLF$)

  WEND

END IF
IF (j <> 0) THEN
  DSPINST = j
  CALL prints("Testing")
  CALL PRINTDISPLAY(j)
  CALL prints("connected to LPT" + RIGHT$(STR$(LPT + 1), 1) + "." + CRLF$)
END IF

END FUNCTION

FUNCTION INKEY2$
'
' Go get a key from the Bios keyboard buffer using the Bios keyboard
' interrupt.
' Returns "" if no key is present
' else returns ascii character pressed.
'
DIM inregs AS RegType, outregs AS RegType

IF (ASC(KEYHIT$) <> 0) THEN
  inregs.ax = 0
  CALL INTERRUPT(&H16, inregs, outregs)           ' Bios key return int
  INKEY2$ = CHR$(outregs.ax AND &HFF)             ' return ascii character
ELSE
  INKEY2$ = ""
END IF

END FUNCTION

FUNCTION KEYHIT$
'
' Check if keyboard has been pressed using Bios interrupt.
' Returns 0 if no key has been pressed.
' or the ascii key pressed.
' NOTE: This does not remove the key from the buffer. Use GETKEY
'       for doing that.
'
DIM inregs AS RegType, outregs AS RegType
CONST zflag = &H40

  inregs.ax = &H100
  CALL INTERRUPT(&H16, inregs, outregs)         ' Bios key status int.

  IF ((outregs.flags AND zflag) <> 0) THEN      ' If zero flag set
    KEYHIT$ = CHR$(0)                           ' then no key present
  ELSE
    KEYHIT = CHR$(outregs.ax AND &HFF)          ' Key is present, return
  END IF                                        ' value but dont remove
						' from keyboard buffer.
END FUNCTION

SUB LPRINTS (A$)
'
' This routine prints a string using the Bios LPT print (INT 17H subfunc 0H)
' Input to this routine is a string.
'
DIM inregs AS RegType, outregs AS RegType

FOR I = 1 TO LEN(A$)                    ' For each character in the string
  ch = ASC(MID$(A$, I, 1))              ' extract the character.
  inregs.ax = ch                        ' Set the subfunction code and char
  inregs.dx = LPT                       ' set lpt number (0 = LPT1)
  CALL INTERRUPT(&H17, inregs, outregs) ' Then generate the interrupt.
NEXT I

END SUB

'
'  The MOVECURSOR (movement left/right routine)
'  Since the DP2x20 doesn't have a cursor display a "*"
'
SUB MOVECURSOR (B)
' B=8 backward, B=9 forward
FOR j = 0 TO MAXCOL - 3
IF DSPTYPE <> 1 THEN GOTO HASCURSOR
CALL LPRINTS("*")
CALL LPRINTS(CHR$(8))
HASCURSOR:
CALL LPRINTS(CHR$(B))
FOR n = 1 TO 75
NEXT n
NEXT j
END SUB

' print the type of key pad
SUB PRINTDISPLAY (DSPTYP)
  IF DSPTYP = 1 THEN prints " VF 2x20 "
  IF DSPTYP = 2 THEN prints " VF 4x20 "
  IF DSPTYP = 3 THEN prints " LCD 4x40 "
  IF DSPTYP = 4 THEN prints " LCD 4x20 "
  IF DSPTYP = 5 THEN prints " LCD 2x40 "
  IF DSPTYP = 6 THEN prints " LCD 2x20 "
END SUB

SUB prints (A$)
'
' This routine prints a string using the Bios write TTY (INT 10H subfunc 0EH)
' Input to this routine is a string.
'
DIM inregs AS RegType, outregs AS RegType

FOR I = 1 TO LEN(A$)                    ' For each character in the string
  ch = ASC(MID$(A$, I, 1))              ' extract the character.
  inregs.ax = &HE00 + ch                ' Set the subfunction code and
  inregs.bx = 1                         ' and character to display.
  CALL INTERRUPT(&H10, inregs, outregs) ' Then generate the interrupt.
NEXT I

END SUB

SUB printsl (A$)
'
' This routine prints a string followed by a crlf
'
prints (A$ + CRLF$)

END SUB

