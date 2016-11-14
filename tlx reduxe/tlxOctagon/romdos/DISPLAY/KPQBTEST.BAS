DECLARE SUB PRINTKPT (kptype!)
DECLARE FUNCTION KEYPINST! ()
'
' This routine uses the keypad functions in the DISPLAY TSR.
'
' The following three routines are used when the main console is through
' COM1. (They work when its not COM1 as well...)
DECLARE FUNCTION KEYHIT$ ()
DECLARE FUNCTION INKEY2$ ()
DECLARE SUB PRINTS (A$)
'
' The following 6 routines are for the keypad support.
DECLARE FUNCTION KEYPBCNT ()
DECLARE FUNCTION GETKEYP ()
DECLARE FUNCTION CKEYPRSD ()
DECLARE SUB CLRKEYPB ()
DECLARE SUB KEYPBNCE (ticks)
DECLARE SUB KEYPRPT (ron)
DECLARE FUNCTION KEYPINST ()

COMMON SHARED lptnum, crlf$     ' globals

'$INCLUDE: 'QB.BI'

KEYP16$ = "123A456B789C*0#D"
KEYP32$ = "123456789ABCDEFGHIJKLMNOPQRSTUVW"
KEYP64$ = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>."

crlf$ = CHR$(10) + CHR$(13)   ' Define end of line string

' First display an opening string using PRINTS

CALL PRINTS(crlf$ + "KEYPAD TEST ROUTINE. USING QUICKBASIC 4.5 AND DISPLAY TSR." + crlf$)

' Check if DISPLAY running or if there wasn't a keypad defined.

KEYPTYPE = KEYPINST
IF (KEYPTYPE = 0) THEN END

CALL PRINTS("PRESS A SERIES OF KEYS ON THE KEYPAD." + crlf$)
CALL PRINTS("WHEN DONE, PRESS A KEY ON THE KEYBOARD." + crlf$)

' Loop until a key on the keyboard is pressed.

WHILE (INKEY2$ = "")          ' while no keyboard key is pressed.
  IF (KEYPBCNT > 0) THEN      ' if a keypad key is in the buffer then
    A = GETKEYP               ' go get the key from the buffer and
    IF KEYPTYPE = 1 THEN      ' depending on the keypad type
      CALL PRINTS(MID$(KEYP16$, A, 1) + "  ")  ' print out character
    ELSEIF KEYPTYPE = 2 THEN
      CALL PRINTS(MID$(KEYP32$, A, 1) + "  ")  ' print out character
    ELSE
      CALL PRINTS(MID$(KEYP64$, A, 1) + "  ")  ' print out character
    END IF
  END IF
WEND

CALL PRINTS(crlf$ + crlf$)

CALL PRINTS("TURN ON REPEAT FLAG AND SET DEBOUNCE TO 9 (~1/2 SEC)" + crlf$)
CALL PRINTS(crlf$)
CALL PRINTS("PRESS A SERIES OF KEYS ON THE KEYPAD." + crlf$)
CALL PRINTS("WHEN DONE, PRESS A KEY ON THE KEYBOARD." + crlf$)

CALL KEYPRPT(1)               ' repeat on
CALL KEYPBNCE(9)              ' set debounce value to 9

' Loop until a key on the keyboard is pressed.

WHILE (INKEY2$ = "")          ' while no keyboard key is pressed.
  IF (KEYPBCNT > 0) THEN      ' if a keypad key is in the buffer then
    A = GETKEYP               ' go get the key from the buffer and
    IF KEYPTYPE = 1 THEN      ' depending on the keypad type
      CALL PRINTS(MID$(KEYP16$, A, 1) + "  ")  ' print out character
    ELSEIF KEYPTYPE = 2 THEN
      CALL PRINTS(MID$(KEYP32$, A, 1) + "  ")  ' print out character
    ELSE
      CALL PRINTS(MID$(KEYP64$, A, 1) + "  ")  ' print out character
    END IF
  END IF
WEND

CALL KEYPRPT(0)               ' repeat off
CALL KEYPBNCE(1)              ' set debounce value to 1

CALL PRINTS(crlf$ + crlf$)

CALL PRINTS("PRESS 4 KEYS ON THE KEYPAD, THESE WILL BE CLEARED." + crlf$)
CALL PRINTS("THEN PRESS 4 MORE KEYS ON THE KEYPAD." + crlf$)
CALL PRINTS("THE LAST 4 KEYS WILL BE DISPLAYED WHEN ALL ENTERED." + crlf$)

' Loop until a key on the keyboard is pressed.

WHILE ((KEYPBCNT < 4) AND (INKEY2$ = ""))  ' while 4 keys not pressed yet.
WEND

CALL CLRKEYPB                 ' now clear the keypad buffer

WHILE ((KEYPBCNT < 4) AND (INKEY2$ = ""))  ' while 4 keys not pressed yet.
WEND

FOR i = 1 TO KEYPBCNT
    A = GETKEYP               ' go get the key from the buffer and
    IF KEYPTYPE = 1 THEN      ' depending on the keypad type
      CALL PRINTS(MID$(KEYP16$, A, 1) + "  ")  ' print out character
    ELSEIF KEYPTYPE = 2 THEN
      CALL PRINTS(MID$(KEYP32$, A, 1) + "  ")  ' print out character
    ELSE
      CALL PRINTS(MID$(KEYP64$, A, 1) + "  ")  ' print out character
    END IF
NEXT i

CALL PRINTS(crlf$ + crlf$)

CALL PRINTS("CHECK WHICH KEYPAD KEY IS PRESSED" + crlf$)
CALL PRINTS("PRESS A SERIES OF KEYS ON THE KEYPAD." + crlf$)
CALL PRINTS("WHEN DONE, PRESS A KEY ON THE KEYBOARD." + crlf$)

' Loop until a key on the keyboard is pressed.

LASTKEYP = 0
CR$ = CHR$(13)
PRINTS (CR$)
WHILE (INKEY2$ = "")          ' while no keyboard key is pressed.
    A = CKEYPRSD              ' go get the current key pressed.
    IF (A <> LASTKEYP) THEN
      IF (A = 0) THEN
	CALL PRINTS("             " + CR$)
      ELSE
	IF KEYPTYPE = 1 THEN    ' depending on the keypad type
	  CALL PRINTS("KEY " + MID$(KEYP16$, A, 1) + " PRESSED" + CR$)
	ELSEIF KEYPTYPE = 2 THEN
	  CALL PRINTS("KEY " + MID$(KEYP32$, A, 1) + " PRESSED" + CR$)
	ELSE
	  CALL PRINTS("KEY " + MID$(KEYP64$, A, 1) + " PRESSED" + CR$)
	END IF
      END IF
    END IF
    LASTKEYP = A
WEND

CALL PRINTS(crlf$)
CALL CLRKEYPB                 ' clear the keypad buffer

END

FUNCTION CKEYPRSD
'
' Go get the current keypad key pressed.
' Returns 0 if no keypad key is being pressed.
' else returns 1-n depending on 16 or 32 character keypad.
' NOTE: Does not remove the key from the keypad buffer. (if it went into it)
'
DIM inregs AS RegType, outregs AS RegType

inregs.ax = &H302                       ' check key pressed.
inregs.dx = lptnum                      ' set LPT number
CALL INTERRUPT(&H17, inregs, outregs)   ' do interrupt
CKEYPRSD = (outregs.ax AND &HFF00) / &H100 ' return key pressed.

END FUNCTION

SUB CLRKEYPB
'
' clears the keypad buffer.
'
DIM inregs AS RegType, outregs AS RegType

inregs.ax = &H303                       ' clear keypad buffer.
inregs.dx = lptnum                      ' lptnum
CALL INTERRUPT(&H17, inregs, outregs)   ' do interrupt

END SUB

FUNCTION GETKEYP
'
' Go get a key from the keypad buffer using the Display TSR keypad interrupt.
' Returns 0 if no keypad key is present
' else returns 1-n depending on 16 or 32 character keypad.
'
DIM inregs AS RegType, outregs AS RegType

inregs.ax = &H301                       ' get keypad key from buffer.
inregs.dx = lptnum                      ' set lpt number
CALL INTERRUPT(&H17, inregs, outregs)   ' Get the first keypad key in buffer.
GETKEYP = (outregs.ax AND &HFF00) / &H100  ' return key from buffer. (0-n)

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

FUNCTION KEYPBCNT
'
' Get number of keys in the keypad buffer.
'
DIM inregs AS RegType, outregs AS RegType

inregs.ax = &H300                       ' get count of keys in keypad buffer
inregs.dx = lptnum                      ' kp lpt number
CALL INTERRUPT(&H17, inregs, outregs)   ' do interrupt
KEYPBCNT = (outregs.ax AND &HFF00) / &H100

END FUNCTION

SUB KEYPBNCE (ticks)
'
' Set keypad debounce value in number of ticks.
'
DIM inregs AS RegType, outregs AS RegType

inregs.ax = &H304                       ' Debounce value set.
inregs.bx = ticks                       ' Number of 18.2 times per sec. ticks
inregs.dx = lptnum                      ' kp lpt number
CALL INTERRUPT(&H17, inregs, outregs)   ' do interrupt

END SUB

FUNCTION KEYPINST
'
' Test if keypad is installed.
' returns 0 if no keypad is installed
'         1 if 4x4 keypad is installed
'         2 if 4x8 keypad is installed
'
DIM inregs AS RegType, outregs AS RegType
DIM kptypes(3)

numkps = 0                              ' initialize number of keypads
FOR j = 0 TO 2
  kptypes(j + 1) = 0
NEXT j
FOR i = 0 TO 2                          ' check each LPT
inregs.ax = &H306                       ' check for display installation
inregs.dx = i                           ' kp lpt number
CALL INTERRUPT(&H17, inregs, outregs)   ' do int to check if tsr running
IF ((outregs.ax = 1) AND (outregs.bx = 2) AND (outregs.cx = 3)) THEN
  inregs.ax = &H307                     ' find out which keypad type in use
  inregs.dx = i                         ' kp lpt number
  CALL INTERRUPT(&H17, inregs, outregs) ' do interrupt
  j = outregs.ax AND &HFF
  IF (j <> 0) THEN
    numkps = numkps + 1
    lptnum = i
    kptypes(lptnum + 1) = j             ' save keypad type
  END IF
END IF
NEXT i

IF numkps = 0 THEN
  CALL PRINTS("DISPLAY NOT EXECUTED WITH KEYPAD OPTION ")
  CALL PRINTS("OR DISPLAY NOT EXECUTED." + crlf$)
  j = 0
ELSEIF numkps = 1 THEN
  j = kptypes(lptnum + 1)
ELSE
  j = 0
  WHILE (j = 0)
  FOR i = 0 TO 2
    IF (kptypes(i + 1) <> 0) THEN
      CALL PRINTKPT(kptypes(i + 1))
      CALL PRINTS(" connected to LPT" + RIGHT$(STR$(i + 1), 1) + "." + crlf$)
    END IF
  NEXT i

  CALL PRINTS("Which LPT number is the test keypad on?")

  A$ = ""
  WHILE (A$ = "")          ' while no keyboard key is pressed.
    A$ = INKEY2$
  WEND
  IF A$ >= "1" AND A$ <= "3" THEN
    j = kptypes(VAL(A$))
    IF (j <> 0) THEN lptnum = VAL(A$) - 1
  END IF
  CALL PRINTS(crlf$)

  WEND

END IF
IF (j <> 0) THEN
  KEYPINST = j
  CALL PRINTS("Testing")
  CALL PRINTKPT(j)
  CALL PRINTS("connected to LPT" + RIGHT$(STR$(lptnum + 1), 1) + "." + crlf$)
END IF

END FUNCTION

SUB KEYPRPT (ron)
'
' Set keypad repeat flag. (allows a key to be repeated if held down)
'
DIM inregs AS RegType, outregs AS RegType

inregs.ax = &H305                       ' Repeat flag set.
inregs.bx = ron                         ' ron = 0 repeat off, = 1 repeat on
inregs.dx = lptnum                      ' kp lpt number
CALL INTERRUPT(&H17, inregs, outregs)   ' do interrupt

END SUB

' print the type of key pad
SUB PRINTKPT (kptype)
  IF (kptype = 1) THEN
    CALL PRINTS(" 4x4 keypad ")
  ELSEIF (kptype = 2) THEN
    CALL PRINTS(" 4x8 keypad ")
  ELSEIF (kptype = 3) THEN
    CALL PRINTS(" 8x8 touch screen ")
  ELSE
    CALL PRINTS(" unknown ")
  END IF
END SUB

SUB PRINTS (A$)
'
' This routine prints a string using the Bios write TTY (INT 10H subfunc 0EH)
' Input to this routine is a string.
'
DIM inregs AS RegType, outregs AS RegType

FOR i = 1 TO LEN(A$)                    ' For each character in the string
  ch = ASC(MID$(A$, i, 1))              ' extract the character.
  inregs.ax = &HE00 + ch                ' Set the subfunction code and
  inregs.bx = 1                         ' and character to display.
  CALL INTERRUPT(&H10, inregs, outregs) ' Then generate the interrupt.
NEXT i

END SUB

