ORIGIN = PWB
ORIGIN_VER = 2.1.49
PROJ = CABLE
PROJFILE = CABLE.MAK
DEBUG = 1

PWBRMAKE  = pwbrmake
NMAKEBSC1  = set
NMAKEBSC2  = nmake
CC  = cl
CFLAGS_G  = /AL /W2 /G1 /BATCH
CFLAGS_D  = /Od /Zi
CFLAGS_R  = /Od /Gs
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LFLAGS_G  = /NOI /BATCH
LFLAGS_D  = /CO /NOFA /NOPACKC /NOPACKF
LFLAGS_R  = /NOF /NOPACKC
LINKER  = link
ILINK  = ilink
LRF  = echo > NUL
BRFLAGS  =  /o $(PROJ).bsc
BROWSE  = 0
LLIBS_G  = graphics.lib pgchart.lib
CVFLAGS  =  /25 /F

FILES  = CABLE.C CABINPUT.C CABCALC.C CABWORK.C CABPRINT.C CABEXCEL.C\
        CABCHART.C MENU.C MSC.C MOUSE.C
OBJS  = CABLE.obj CABINPUT.obj CABCALC.obj CABWORK.obj CABPRINT.obj\
        CABEXCEL.obj CABCHART.obj MENU.obj MSC.obj MOUSE.obj
SBRS  = CABLE.sbr CABINPUT.sbr CABCALC.sbr CABWORK.sbr CABPRINT.sbr\
        CABEXCEL.sbr CABCHART.sbr MENU.sbr MSC.sbr MOUSE.sbr

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

CABCHART.obj : CABCHART.C cable.h

CABCHART.sbr : CABCHART.C cable.h

MENU.obj : MENU.C menu.h mouse.h

MENU.sbr : MENU.C menu.h mouse.h

MSC.obj : MSC.C menu.h mouse.h cable.h

MSC.sbr : MSC.C menu.h mouse.h cable.h

MOUSE.obj : MOUSE.C

MOUSE.sbr : MOUSE.C


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
