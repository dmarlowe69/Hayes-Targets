# file: tlxr.mak
#****************************************************************************
#
#       BRAIN Make file for ROM based code
#
#       default build: TLXR.exe
#
#       written by denton marlowe
#       (c)1991 Hayes Targets
#
#****************************************************************************
#!INCLUDE hayes.mak
#****************************************************************************
#
#       macro to set path names
#
#       LIB     Libraries
#       INC     Include files
#       ST      Startup code
#
#****************************************************************************
!INCLUDE c:\c\mit_ll\path.inc
#****************************************************************************
#
#       macro to set compiler version
#
#               5         Version 5.1
#               6         Version 6.0
#               7         Version 7.0
#               7         Version 8.0
#
#****************************************************************************
VER=7
#****************************************************************************
#
#       macro to set memory model
#
#               S          Small
#               C          Compact
#               M          Meduim
#               L          Large
#
#****************************************************************************
MODEL=S
#****************************************************************************
#
#       macro to set processor type (limited to 3 characters)
#
#               V20        LPM-SBC8-X
#               V40        LPM-SBC40-X
#               V50        LPM-SBC50-X
#               I486       LPM-486SLC-X
#               PC         IBM PC
#
#****************************************************************************
CPU=V40
#****************************************************************************
#
#       macro to set processor type (limited to 3 characters)
#
#****************************************************************************
TARGET_CPU=NEC$(CPU)
#****************************************************************************
#
#       macro to set processor clock speed in MHz
#
#               10         10 MHz
#                8          8 MHz
#                5          5 MHz
#                X       I486 MHz
#               XX         PC MHz
#
#****************************************************************************
CLOCK=10
#****************************************************************************
#
#       macro to set floating point option
#
#       (BLANK)         no floating point
#       A               alternate math or emulator
#       E               emulator math
#       7               inline Co-processor
#
#****************************************************************************
MATH=A
#****************************************************************************
#
#       macro to set floating point compiler option
#
#****************************************************************************
!IF "$(MATH)"==""
HMATH=A
CMATH=A
FPMATH=FPa
!ENDIF

!IF "$(MATH)"=="A"
HMATH=A
CMATH=A
FPMATH=FPa
!ENDIF

!IF "$(MATH)"=="E"
HMATH=E
CMATH=E
FPMATH=FPi
!ENDIF

!IF "$(MATH)"=="7"
HMATH=7
CMATH=7
FPMATH=FPi87
!ENDIF
#****************************************************************************
#
#       macros to set object file names of startup code
#
#****************************************************************************
STRAM=$(ST)\$(MODEL)RAM.obj
STRAMFP=$(ST)\$(MODEL)RAMA.obj
STRAMFPI=$(ST)\$(MODEL)RAM7.obj
STROM=$(ST)\$(MODEL)$(CPU)_$(CLOCK).obj
STROMFP=$(ST)\$(MODEL)$(CPU)_$(CLOCK)A.obj
STROMFPI=$(ST)\$(MODEL)$(CPU)_$(CLOCK)7.obj
#****************************************************************************
#
#       macros to set library file names
#
#****************************************************************************
HAYESLIB=$(LIBX)\$(MODEL)$(CPU)_$(CLOCK)$(HMATH).lib
!IF "$(CPU)"=="I486"
GNDLIB=$(LIBX)\$(MODEL)486GND$(HMATH).lib
!ELSE
GNDLIB=$(LIBX)\$(MODEL)$(CPU)GND$(HMATH).lib
!ENDIF
CLIB=$(LIB)\$(MODEL)libc$(CMATH).lib
FLIB=$(LIBX)\$(MODEL)libfor$(CMATH).lib
#****************************************************************************
#
#       macro to set startup code option
#
#       STRAM           ram startup code
#       STRAMFP         ram startup code with floating point calls
#       STRAMFPI        ram startup code with in-line 8087
#       STROM           rom startup code
#       STROMFP         rom startup code with floating point calls
#       STROMFPI        rom startup code with in-line 8087
#
#****************************************************************************
START=$(STRAMFP)
#****************************************************************************
#
#       macros to compile assembly language source
#
#       options:
#               Cx              public symbols case sensitive
#               Zm              Masm 5.1 Compatibility
#               Dmem$(MODEL)=1  set memory model
#               D$(CPU)         set host processor board
#               DVER=$(VER)     set C compiler version
#
#****************************************************************************
MFLAGS=/c /Cx /Zm /Dmem$(MODEL)=1 /D$(CPU)=1 /DCLOCK=$(CLOCK) /DVER=$(VER)
MASM=ml $(MFLAGS)
#****************************************************************************
#
#       macros to compile "C" language source
#
#       options:
#               AS      small memory model
#               Zi      generates symbolic debugging information
#               Oi      enables use of intrinsic functions
#               Gs      stack checking calls are not generated
#               c       compile without linking
#               G1      use 80186 instructions
#               G2      use 80286 instructions
#               G0      use 8086 instructions
#               W2      warning level 2
#               FPa     use alternate math package
#               FPi87   use inline 8087
#               FPi     use inline emulator library
#               Fs      source listing
#               Fc      source/assembly/object code lisiting
#
#****************************************************************************
CFLAGS=/A$(MODEL) /Zi /Oi /Gs /c /G2 /W2 /$(FPMATH) /D$(CPU) /DCLOCK=$(CLOCK)
CL=cl $(CFLAGS)
#****************************************************************************
#
#       macros to link object models into EXE
#
#       options:
#               MAP               generate MAP listing of global symbols
#               CODEVIEW          generate codeview debugger information
#               NOExtended search accept user defined substitute for function
#
#****************************************************************************
LFLAGS=/NOI /MAP /CODEVIEW /NOE
LINK=link $(LFLAGS)
#****************************************************************************
#
#       macros to set output filename
#
#****************************************************************************

#****************************************************************************
#
#       macro to set memory configurations for application (RAM OR ROM)
#
#               RAM     ram configuration
#               ROM     rom configuration
#
#****************************************************************************
MEM=ROM
#****************************************************************************
#
#       macro to set Location file configuration
#
#               RAM     standard loadable test configuration
#               ROM     stand alone romable configuration
#               RAMH    loadable into high ram test configuration
#
#****************************************************************************
LOCATE=ROM
#****************************************************************************
#
#       locate files for rom
#
#****************************************************************************

all: tlxr.abm tlxrl.abm

tlxr.abm :    tlxr.exe tlxr.map tlxr.loc tlxr.mak
	    loc @tlxr.loc

tlxrl.abm:    tlxr.exe tlxr.cfg
	    locate tlxr
#****************************************************************************
#
#       link files
#
#****************************************************************************

tlxr.exe :   $(START) \
	tlx.obj \
	platform.obj \
	pilot.obj utility.obj cmd_tm.obj ground.obj \
    uart_cmd.obj monloop.obj \
    $(HAYESLIB) $(GNDLIB) $(CLIB) \
	tlxr.mak
	    $(LINK) @<<tlxr.lnk
$(START)+
tlx+
platform+
pilot+utility+cmd_tm+ground+
uart_cmd+monloop
tlxr.exe
tlxr.map
$(HAYESLIB)+$(GNDLIB)+$(CLIB)
NUL.DEF
<<KEEP
#****************************************************************************
#
#       compile application specific C source code
#
#****************************************************************************

tlx.obj :          tlx.c v_str.h
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

platform.obj :  platform.c
    $(CL) /D$(CPU) /DSTD_ENCODER  /D_EPROM $*.C

pilot.obj :  pilot.c v_str.h
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

utility.obj :  utility.c
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

cmd_tm.obj :  cmd_tm.c
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

ground.obj :  ground.c
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

#****************************************************************************
#
#
#
#****************************************************************************

uart_cmd.obj :  uart_cmd.c
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

monloop.obj :  monloop.c
    $(CL) /D$(CPU) /DSTD_ENCODER /D_EPROM $*.C

#****************************************************************************
#
#       inference rules
#
#****************************************************************************

.ASM.OBJ:
	$(MASM) $*.ASM;

.C.OBJ:
	$(CL) $*.C

.FOR.OBJ:
	$(FL) $*.FOR

#****************************************************************************
#
#       end of make file
#
#****************************************************************************
