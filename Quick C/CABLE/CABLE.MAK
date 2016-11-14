ORIGIN = PWB
ORIGIN_VER = 2.1.49
PROJ = cable
PROJFILE = cable.mak
DEBUG = 1

PWBRMAKE  = pwbrmake
NMAKEBSC1  = set
NMAKEBSC2  = nmake
CC	= cl
CFLAGS_G  = /AL /W2 /G1 /BATCH
CFLAGS_D  = /Od /Zi
CFLAGS_R  = /Od /Gs
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LFLAGS_G  = /NOI /BATCH
LFLAGS_D  = /CO /NOFA /NOPACKC /NOPACKF
LFLAGS_R  = /NOF /NOPACKC
LINKER	= link
ILINK  = ilink
LRF  = echo > NUL
BRFLAGS  =	/o $(PROJ).bsc
BROWSE	= 0
LLIBS_G  = graphics.lib pgchart.lib
CVFLAGS  =	/25 /F

FILES  = SWAPL.OBJ CABLE.C CABINPUT.C CABCALC.C CABWORK.C CABPRINT.C\
		CABEXCEL.C CABCHART.C MENU.C MSC.C MOUSE.C SCRDUMP.C EDITOR.C\
		CABCALC2.C SHELL.C
OBJS_EXT  = SWAPL.OBJ
OBJS  = CABLE.obj CABINPUT.obj CABCALC.obj CABWORK.obj CABPRINT.obj\
		CABEXCEL.obj CABCHART.obj MENU.obj MSC.obj MOUSE.obj SCRDUMP.obj\
		EDITOR.obj CABCALC2.obj SHELL.obj $(OBJS_EXT)
SBRS  = CABLE.sbr CABINPUT.sbr CABCALC.sbr CABWORK.sbr CABPRINT.sbr\
		CABEXCEL.sbr CABCHART.sbr MENU.sbr MSC.sbr MOUSE.sbr SCRDUMP.sbr\
		EDITOR.sbr CABCALC2.sbr SHELL.sbr

all: $(PROJ).exe

.SUFFIXES:
.SUFFIXES:
.SUFFIXES: .obj .sbr .c

CABLE.obj : CABLE.C c:\c\mit_ll\INCLUDE\float.h c:\c\mit_ll\INCLUDE\math.h\
		c:\c\mit_ll\INCLUDE\stdio.h c:\c\mit_ll\INCLUDE\stdlib.h

CABLE.sbr : CABLE.C c:\c\mit_ll\INCLUDE\float.h c:\c\mit_ll\INCLUDE\math.h\
		c:\c\mit_ll\INCLUDE\stdio.h c:\c\mit_ll\INCLUDE\stdlib.h

CABINPUT.obj : CABINPUT.C cable.h msc.h mouse.h

CABINPUT.sbr : CABINPUT.C cable.h msc.h mouse.h

CABCALC.obj : CABCALC.C c:\c\mit_ll\INCLUDE\float.h c:\c\mit_ll\INCLUDE\math.h\
		c:\c\mit_ll\INCLUDE\stdio.h cable.h

CABCALC.sbr : CABCALC.C c:\c\mit_ll\INCLUDE\float.h c:\c\mit_ll\INCLUDE\math.h\
		c:\c\mit_ll\INCLUDE\stdio.h cable.h

CABWORK.obj : CABWORK.C c:\c\mit_ll\INCLUDE\float.h c:\c\mit_ll\INCLUDE\math.h\
		c:\c\mit_ll\INCLUDE\stdio.h cable.h

CABWORK.sbr : CABWORK.C c:\c\mit_ll\INCLUDE\float.h c:\c\mit_ll\INCLUDE\math.h\
		c:\c\mit_ll\INCLUDE\stdio.h cable.h

CABPRINT.obj : CABPRINT.C cable.h msc.h

CABPRINT.sbr : CABPRINT.C cable.h msc.h

CABEXCEL.obj : CABEXCEL.C c:\c\mit_ll\INCLUDE\float.h\
		c:\c\mit_ll\INCLUDE\math.h c:\c\mit_ll\INCLUDE\stdio.h cable.h

CABEXCEL.sbr : CABEXCEL.C c:\c\mit_ll\INCLUDE\float.h\
		c:\c\mit_ll\INCLUDE\math.h c:\c\mit_ll\INCLUDE\stdio.h cable.h

CABCHART.obj : CABCHART.C cable.h print.h

CABCHART.sbr : CABCHART.C cable.h print.h

MENU.obj : MENU.C menu.h mouse.h

MENU.sbr : MENU.C menu.h mouse.h

MSC.obj : MSC.C menu.h mouse.h cable.h editor.h swap.h

MSC.sbr : MSC.C menu.h mouse.h cable.h editor.h swap.h

MOUSE.obj : MOUSE.C

MOUSE.sbr : MOUSE.C

SCRDUMP.obj : SCRDUMP.C c:\c\mit_ll\INCLUDE\stdio.h\
		c:\c\mit_ll\INCLUDE\stdlib.h c:\c\mit_ll\INCLUDE\bios.h\
		c:\c\mit_ll\INCLUDE\dos.h
!IF $(DEBUG)
		$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /FoSCRDUMP.obj SCRDUMP.C
!ELSE
		$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /FoSCRDUMP.obj SCRDUMP.C
!ENDIF

SCRDUMP.sbr : SCRDUMP.C c:\c\mit_ll\INCLUDE\stdio.h\
		c:\c\mit_ll\INCLUDE\stdlib.h c:\c\mit_ll\INCLUDE\bios.h\
		c:\c\mit_ll\INCLUDE\dos.h
!IF $(DEBUG)
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FRSCRDUMP.sbr SCRDUMP.C
!ELSE
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FRSCRDUMP.sbr SCRDUMP.C
!ENDIF

EDITOR.obj : EDITOR.C c:\c\mit_ll\INCLUDE\stdio.h c:\c\mit_ll\INCLUDE\dos.h\
		c:\c\mit_ll\INCLUDE\string.h c:\c\mit_ll\INCLUDE\bios.h\
		c:\c\mit_ll\INCLUDE\conio.h
!IF $(DEBUG)
		$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /FoEDITOR.obj EDITOR.C
!ELSE
		$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /FoEDITOR.obj EDITOR.C
!ENDIF

EDITOR.sbr : EDITOR.C c:\c\mit_ll\INCLUDE\stdio.h c:\c\mit_ll\INCLUDE\dos.h\
		c:\c\mit_ll\INCLUDE\string.h c:\c\mit_ll\INCLUDE\bios.h\
		c:\c\mit_ll\INCLUDE\conio.h
!IF $(DEBUG)
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FREDITOR.sbr EDITOR.C
!ELSE
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FREDITOR.sbr EDITOR.C
!ENDIF

CABCALC2.obj : CABCALC2.C c:\c\mit_ll\INCLUDE\float.h\
		c:\c\mit_ll\INCLUDE\math.h c:\c\mit_ll\INCLUDE\stdio.h cable.h
!IF $(DEBUG)
		$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /FoCABCALC2.obj CABCALC2.C
!ELSE
		$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /FoCABCALC2.obj CABCALC2.C
!ENDIF

CABCALC2.sbr : CABCALC2.C c:\c\mit_ll\INCLUDE\float.h\
		c:\c\mit_ll\INCLUDE\math.h c:\c\mit_ll\INCLUDE\stdio.h cable.h
!IF $(DEBUG)
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FRCABCALC2.sbr CABCALC2.C
!ELSE
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FRCABCALC2.sbr CABCALC2.C
!ENDIF

SHELL.obj : SHELL.C swap.h
!IF $(DEBUG)
		$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /FoSHELL.obj SHELL.C
!ELSE
		$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /FoSHELL.obj SHELL.C
!ENDIF

SHELL.sbr : SHELL.C swap.h
!IF $(DEBUG)
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FRSHELL.sbr SHELL.C
!ELSE
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FRSHELL.sbr SHELL.C
!ENDIF


$(PROJ).bsc : $(SBRS)
		$(PWBRMAKE) @<<
$(BRFLAGS) $(SBRS)
<<

$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
		$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_D)
$(LLIBS_G: = +^
) +
$(LLIBS_D: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
		$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_R)
$(LLIBS_G: = +^
) +
$(LLIBS_R: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
		$(LINKER) @$(PROJ).lrf


.c.obj :
!IF $(DEBUG)
		$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /Fo$@ $<
!ELSE
		$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /Fo$@ $<
!ENDIF

.c.sbr :
!IF $(DEBUG)
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FR$@ $<
!ELSE
		$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FR$@ $<
!ENDIF


run: $(PROJ).exe
		$(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
		CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
