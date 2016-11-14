'        This software, including source code, is provided at no cost.  It
'        is intended to be used for tutorial and demo purposes, and therefore,
'        no warranty of any kind, express or implied is provided.  Under no
'        circumstances whatsoever shall Octagon Systems Corporation be held
'        liable for any damages, including any lost profits, lost savings,
'        or other incidental or consequential damages arising from the use
'        of, or inablity to use, this software.

'       Octagon Systems Corporation (TM)
'       6510 W. 91st. Ave.
'       Westminster, CO  80030

'This program shows how to write out COM1 using QuickBasic 4.5

'QuickBasic's PRINT statement, among others, does direct screen writing
'instead of performing INT 10 or DOS INT 21 calls to write to the screen.

'To force QuickBasic to use INT 10/DOS INT 21, use the PRINTS routine found
'in this program. The PRINTS uses QuickBasic's INTERRUPT call. PRINTS will
'work when the video is COM1 or is via a video card. To compile a program
'with the INTERRUPT call you must do the following:

' 1) Include the QB.BI file in the same directory.
' 2a) If compiling with the interactive QB, include the /L option. Ie.
'      QB DEMO.BAS /L
' 2b) If compiling with the batch QB (BC) enter the following:
'      BC DEMO.BAS/O/T/C:512;
'      LINK /EX /NOE /NOD:BRUN45.LIB DEMO,,,BCOM45.LIB+QB.LIB;

'Descriptions for some function names are below:

' The following routine is used when the main console is through COM1.
' (They work when its not COM1 as well.)

DECLARE SUB PRINTS (A$)         'prints to com1/video using int 10's
DECLARE SUB PRINTSL (A$)        'calls PRINTS and adds a cr,lf

'$INCLUDE: 'QB.BI'

'Assumes ANSII.SYS driver with color screen
'Otherwise you may get funny characters at the beginning.
clrscr$ = CHR$(27) + CHR$(59) 'clear serial port screen

'set the color of the screen and characters.  Yellow chars, blue background
colorscrn$ = CHR$(27) + CHR$(70) + CHR$(14) + CHR$(27) + CHR$(65) + CHR$(1)

'clear displays

	CALL PRINTS(clrscr$)
	CALL PRINTSL(colorscrn$)

	CALL PRINTSL("")
	CALL PRINTSL("Octagon Systems presents the Micro PC")
	CALL PRINTSL("")

	CALL PRINTSL("Featuring:")
	CALL PRINTSL("        >>>  Wide operating temperature         <<<")
	CALL PRINTSL("        >>>  High shock and vibration tolerance <<<")
	CALL PRINTSL("        >>>  ROM-DOS with solid state disk      <<<")
	CALL PRINTSL("        >>>  Watch dog timer                    <<<")
	CALL PRINTSL("")

	PRINTS ("Press any key to continue ")

	WHILE INKEY$ = ""
	WEND

	CALL PRINTSL("")
	CALL PRINTSL("")

'give a little bit about the board

	CALL PRINTSL("The 6040 is 100% AT compatible.  It has:")
	CALL PRINTSL("   >>> 25Mhz, 386SX CPU (Phoneix BIOS)                       <<<")
	CALL PRINTSL("   >>> 2MB of DRAM                                           <<<")
	CALL PRINTSL("   >>> Built-in flash file system                            <<<")
	CALL PRINTSL("   >>> Built-in networking software                          <<<")
	CALL PRINTSL("   >>> 1 1MB flash disk                                      <<<")
	CALL PRINTSL("   >>> 1 128K SRAM disk                                      <<<")
	CALL PRINTSL("   >>> COM1 and COM2 ports                                   <<<")
	CALL PRINTSL("   >>> LPT1 printer port (EPP, ECP, Floppy, Bi-directional)  <<<")
	CALL PRINTSL("   >>> 8 12-bit analog-to-digital channels                   <<<")
	CALL PRINTSL("   >>> 2 12-bit digital-to-analog channels                   <<<")
	CALL PRINTSL("   >>> 24 EZI/O lines (individually programmable)            <<<")
	CALL PRINTSL("   >>> 2 opto-isolated interrupt inputs                      <<<")
	CALL PRINTSL("   >>> Built-in real time clock                              <<<")
	CALL PRINTSL("   >>> Keyboard & speaker port                               <<<")
	CALL PRINTSL("   >>> Advanced Power Management                             <<<")
	CALL PRINTSL("   >>> Onboard Floppy interface (via LPT port)               <<<")
	CALL PRINTSL("")

	CALL PRINTS("The 6040 can also drive LCD displays and")
	CALL PRINTSL(" read keypads from the printer port and the EZ I/O port.")

'give chance to break out
	PRINTS ("Press any key to exit ")

	WHILE INKEY$ = ""
	WEND


	CALL PRINTSL("")
	CALL PRINTSL("")

END

SUB PRINTS (A$)
'
' This routine prints a string using the Bios write TTY (INT 10H subfunc 0EH)
' Input to this routine is a string.
'
DIM inregs AS RegType, outregs AS RegType

FOR I = 1 TO LEN(A$)               ' For each character in the string
	ch = ASC(MID$(A$, I, 1))   ' extract the character.
	inregs.ax = &HE00 + ch     ' Set the subfunction code and
	inregs.bx = 1              ' and character to display.
	CALL INTERRUPT(&H10, inregs, outregs) ' Then generate the interrupt.
NEXT I

END SUB

SUB PRINTSL (A$)
'
' This routine prints a string using the PRINTS routine and automatically
' appends the CR, LF like PRINT normally does.
'
	CALL PRINTS(A$ + CHR$(10) + CHR$(13)) ' print string with LF,CR

END SUB

