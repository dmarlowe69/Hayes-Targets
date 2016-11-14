ORIGIN = PWB
ORIGIN_VER = 2.1.49
PROJ = SWAP
PROJFILE = SWAP.MAK
DEBUG = 0

CC	= cl
CFLAGS_G  = /W2 /BATCH
CFLAGS_D  = /f /Zi /Od
CFLAGS_R  = /f- /Ot /Oi /Ol /Oe /Og /Gs
CXX  = cl
CXXFLAGS_G	= /W2 /BATCH
CXXFLAGS_D	= /f /Zi /Od
CXXFLAGS_R	= /f- /Ot /Oi /Ol /Oe /Og /Gs
FOR  = fl
FORFLAGS_R	= /Ot /Ol /Gs
FORFLAGS_D	= /Zi /Od /4Yb
ASM  = ml
H2INC  = h2inc
AFLAGS_G  = /W2 /Zm /FPi /D_Large /I.
AFLAGS_D  = /Zi
AFLAGS_R  = /nologo
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LFLAGS_G  = /NOI /BATCH /ONERROR:NOEXE
LFLAGS_D  = /CO /FAR /PACKC
LFLAGS_R  = /EXE /FAR /PACKC
LINKER	= link
ILINK  = ilink
LRF  = echo > NUL
ILFLAGS  = /a /e

FILES  = SWAP.ASM SWAPTEST.C
OBJS  = SWAP.obj SWAPTEST.obj

all: $(PROJ).exe

.SUFFIXES:
.SUFFIXES:
.SUFFIXES: .obj .c .asm

SWAP.obj : SWAP.ASM
!IF $(DEBUG)
		$(ASM) /c $(AFLAGS_G) $(AFLAGS_D) /FoSWAP.obj SWAP.ASM
!ELSE
		$(ASM) /c $(AFLAGS_G) $(AFLAGS_R) /FoSWAP.obj SWAP.ASM
!ENDIF

SWAPTEST.obj : SWAPTEST.C swap.h
!IF $(DEBUG)
		@$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoSWAPTEST.obj SWAPTEST.C
<<
!ELSE
		@$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoSWAPTEST.obj SWAPTEST.C
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

.asm.obj :
!IF $(DEBUG)
		$(ASM) /c $(AFLAGS_G) $(AFLAGS_D) /Fo$@ $<
!ELSE
		$(ASM) /c $(AFLAGS_G) $(AFLAGS_R) /Fo$@ $<
!ENDIF


run: $(PROJ).exe
		$(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
		CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
