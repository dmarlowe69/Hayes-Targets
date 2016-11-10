|9|		; F(9) key - Insure at top level menu
M|		; Invoke (M)emory menu
FCA		; (F)ill, with (CA)
0080		;	  from (0080)
00BF		;	  thru (00BF)
|4|		; F(4) key - Select word mode
FFECE		; (F)ill, with (FECE)
00C0		;	  from (00C0)
00FF		;	  thru (00FF)
|4|		; F(4) key - Select byte mode
F00		; (F)ill, with (00)
0100		;	  from (0100)
012F		;	  thru (012F)
V0080		; (V)iew, from (0080)
012F		;	  thru (012F)
|E|W|		; (E)scape, Autotest (W)ait
SBACKUP 	; (S)ave, file (BACKUP).H
0080		;	  from (0080)
012F		;	  thru (012F)
GASCII		; (G)et,  file (ASCII).H
V0080		; (V)iew, from (0080)
012F		;	  thru (012F)
|E|W|		; (E)scape, Autotest (W)ait
GBACKUP 	; (G)et,  file (BACKUP).H
V0080		; (V)iew, from (0080)
012F		;	  thru (012F)
|E|W|		; (E)scape, Autotest (W)ait
GDEMO		; (G)et,  file (DEMO).H
|9|		; F(9) key - Return to top level Menu
