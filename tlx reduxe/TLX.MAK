ORIGIN = PWB
ORIGIN_VER = 2.1.49
PROJ = TLX
PROJFILE = TLX.MAK
DEBUG = 1

ASM  = ml
H2INC  = h2inc
AFLAGS_G  = /Cx /W2 /WX
AFLAGS_D  = /Zi
AFLAGS_R  = /nologo
CC  = cl
CFLAGS_G  = /AL /W2 /G2 /BATCH
CFLAGS_D  = /f- /Od /Oi /FPa /Zi /Gs
CFLAGS_R  = /f- /Od /Ol /Og /Oe /Oi /FPa /Gs
CXX  = cl
CXXFLAGS_G  = /W2 /BATCH
CXXFLAGS_D  = /f /Zi /Od
CXXFLAGS_R  = /f- /Ot /Oi /Ol /Oe /Og /Gs
FOR  = fl
FORFLAGS_R  = /Ot /Ol /Gs
FORFLAGS_D  = /Zi /Od /4Yb
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LFLAGS_G  = /NOI /BATCH /ONERROR:NOEXE
LFLAGS_D  = /CO /FAR /PACKC
LFLAGS_R  = /EXE /FAR /PACKC
LINKER  = link
ILINK  = ilink
LRF  = echo > NUL
ILFLAGS  = /a /e

FILES  = tlx.C TCU.C AIOTCU.C ICU.C DSPIO.C GROUND.C TLXPILOT.C AIODRV.C
OBJS  = tlx.obj TCU.obj AIOTCU.obj ICU.obj DSPIO.obj GROUND.obj TLXPILOT.obj\
        AIODRV.obj

all: $(PROJ).exe

.SUFFIXES:
.SUFFIXES:
.SUFFIXES: .obj .c

tlx.obj : tlx.C config.h icu.h tcu.h dspio.h ground.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /Fotlx.obj tlx.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /Fotlx.obj tlx.C
<<
!ENDIF

TCU.obj : TCU.C config.h icu.h borland.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoTCU.obj TCU.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoTCU.obj TCU.C
<<
!ENDIF

AIOTCU.obj : AIOTCU.C config.h borland.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoAIOTCU.obj AIOTCU.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoAIOTCU.obj AIOTCU.C
<<
!ENDIF

ICU.obj : ICU.C config.h borland.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoICU.obj ICU.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoICU.obj ICU.C
<<
!ENDIF

DSPIO.obj : DSPIO.C config.h icu.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoDSPIO.obj DSPIO.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoDSPIO.obj DSPIO.C
<<
!ENDIF

GROUND.obj : GROUND.C dspio.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoGROUND.obj GROUND.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoGROUND.obj GROUND.C
<<
!ENDIF

TLXPILOT.obj : TLXPILOT.C ground.h cmd_tm.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoTLXPILOT.obj TLXPILOT.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoTLXPILOT.obj TLXPILOT.C
<<
!ENDIF

AIODRV.obj : AIODRV.C config.h
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoAIODRV.obj AIODRV.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoAIODRV.obj AIODRV.C
<<
!ENDIF


$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
        $(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_D)
$(LIBS: = +^
) +
$(LLIBS_G: = +^
) +
$(LLIBS_D: = +^
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
$(LIBS: = +^
) +
$(LLIBS_G: = +^
) +
$(LLIBS_R: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
        $(LINKER) @$(PROJ).lrf


.c.obj :
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /Fo$@ $<
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /Fo$@ $<
<<
!ENDIF


run: $(PROJ).exe
        $(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
        CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
