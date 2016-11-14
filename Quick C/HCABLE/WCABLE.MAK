ORIGIN = PWB
ORIGIN_VER = 2.1.49
PROJ = WCABLE
PROJFILE = WCABLE.MAK
DEBUG = 0

CC  = cl
CFLAGS_G  = /W2 /GA /GEf /Zp /BATCH /DWINVER=0x0300
CFLAGS_D  = /f /Zi /Od /Gs
CFLAGS_R  = /f- /Oe /Og /Os /Gs
CXX  = cl
CXXFLAGS_G  = /W2 /GA /GEf /Zp /BATCH /DWINVER=0x0300
CXXFLAGS_D  = /f /Zi /Od /Gs
CXXFLAGS_R  = /f- /Oe /Og /Os /Gs
FOR  = fl
FORFLAGS_R  = /Ot /Ol /Gs
FORFLAGS_D  = /Zi /Od /4Yb
ASM  = ml
H2INC  = h2inc
AFLAGS_G  = /Cx /W2 /WX
AFLAGS_D  = /Zi
AFLAGS_R  = /nologo
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LFLAGS_G  =  /BATCH /ONERROR:NOEXE
LFLAGS_D  = /CO /NOF
LFLAGS_R  = /NOFA
LLIBS_G  = c:\msvc\lib\LIBW.LIB
LINKER  = link
ILINK  = ilink
LRF  = echo > NUL
ILFLAGS  = /a /e
RC  = rc
RCFLAGS2  = /30
CL_DEFS  = c:\msvc\bin\cl.def
LLIBS_R  = /NOD:SLIBCE c:\msvc\lib\SLIBCEWQ
LLIBS_D  = /NOD:SLIBCE SLIBCEWQ

FILES  = hcable.C DATAFILE.C AIRFRAME.C ANALYSIS.C DESIGN.C DESIGN6.C PRINT6.C\
        REVS.C DESIGNST.C TESTDATA.C
OBJS  = hcable.obj DATAFILE.obj AIRFRAME.obj ANALYSIS.obj DESIGN.obj\
        DESIGN6.obj PRINT6.obj REVS.obj DESIGNST.obj TESTDATA.obj

all: $(PROJ).exe

.SUFFIXES:
.SUFFIXES:
.SUFFIXES: .obj .c

hcable.obj : hcable.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /Fohcable.obj hcable.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /Fohcable.obj hcable.C
<<
!ENDIF

DATAFILE.obj : DATAFILE.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoDATAFILE.obj DATAFILE.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoDATAFILE.obj DATAFILE.C
<<
!ENDIF

AIRFRAME.obj : AIRFRAME.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoAIRFRAME.obj AIRFRAME.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoAIRFRAME.obj AIRFRAME.C
<<
!ENDIF

ANALYSIS.obj : ANALYSIS.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoANALYSIS.obj ANALYSIS.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoANALYSIS.obj ANALYSIS.C
<<
!ENDIF

DESIGN.obj : DESIGN.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoDESIGN.obj DESIGN.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoDESIGN.obj DESIGN.C
<<
!ENDIF

DESIGN6.obj : DESIGN6.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoDESIGN6.obj DESIGN6.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoDESIGN6.obj DESIGN6.C
<<
!ENDIF

PRINT6.obj : PRINT6.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoPRINT6.obj PRINT6.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoPRINT6.obj PRINT6.C
<<
!ENDIF

REVS.obj : REVS.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoREVS.obj REVS.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoREVS.obj REVS.C
<<
!ENDIF

DESIGNST.obj : DESIGNST.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoDESIGNST.obj DESIGNST.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoDESIGNST.obj DESIGNST.C
<<
!ENDIF

TESTDATA.obj : TESTDATA.C
!IF $(DEBUG)
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_D) /FoTESTDATA.obj TESTDATA.C
<<
!ELSE
        @$(CC) @<<$(PROJ).rsp
/c $(CFLAGS_G)
$(CFLAGS_R) /FoTESTDATA.obj TESTDATA.C
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
! IFDEF DEF_FILE
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
! ELSE
$(CL_DEFS) $(LFLAGS_G) $(LFLAGS_D);
! ENDIF
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
! IFDEF DEF_FILE
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
! ELSE
$(CL_DEFS) $(LFLAGS_G) $(LFLAGS_R);
! ENDIF
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
        WX $(WXFLAGS) $(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
        WX /p $(WXFLAGS) CVW $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
