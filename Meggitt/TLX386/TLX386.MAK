# file: TLX386.mak
#****************************************************************************
#
#       BRAIN Make file for RAM based code
#
#               default build: APP386.exe
#
#       written by denton marlowe
#       (c)1991 Hayes Targets
#
#****************************************************************************
#!INCLUDE hayes.mak
#****************************************************************************
#
#		Base File Name
#
#****************************************************************************
BASE_NAME=TLX386
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
MODEL=L
#MODEL=S
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
CPU=I486
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
#                XX        PC MHz
#
#****************************************************************************
CLOCK=X
#****************************************************************************
#
#       macro to set floating point option
#
#				(BLANK) 	no floating point
#				A			alternate math or emulator
#				E			emulator math
#				7			inline Co-processor
#
#****************************************************************************
MATH=A
#MATH=7
#MATH=E
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
ST=STARTUP
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
#HAYESLIB=$(LIBX)\$(MODEL)$(CPU)_$(CLOCK)$(HMATH).lib
HAYESLIB=hayeslib\$(MODEL)$(CPU)_$(CLOCK)$(HMATH).lib
!IF "$(CPU)"=="I486"
#GNDLIB=$(LIBX)\$(MODEL)486GND$(HMATH).lib
GNDLIB=gnd\$(MODEL)486GND$(HMATH).lib
!ELSE
GNDLIB=$(LIBX)\$(MODEL)$(CPU)GND$(HMATH).lib
!ENDIF
CLIB=$(LIB)\$(MODEL)libc$(CMATH).lib
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
#				Cx				public symbols case sensitive
#				Zm				Masm 5.1 Compatibility
#				Dmem$(MODEL)=1	set memory model
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
#				G1		use 80186 instructions
#				G2		use 80286 instructions
#               G0      use 8086 instructions
#				W2		warning level 2
#				FPa		use alternate math package
#               FPi87   use inline 8087
#				FPi		use inline emulator library
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
LFLAGS=/MAP /CODEVIEW /NOE
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
MEM=RAM
#****************************************************************************
#
#       macro to set Location file configuration
#
#               RAM     standard loadable test configuration
#               ROM     stand alone romable configuration
#               RAMH    loadable into high ram test configuration
#
#****************************************************************************
LOCATE=RAM
#****************************************************************************
#
#       macro to set Location file configuration
#
#               RAM     standard loadable test configuration
#               ROM     stand alone romable configuration
#               RAMH    loadable into high ram test configuration
#
#****************************************************************************
#LOCATE=ROM
LOCATE=RAMH
#****************************************************************************
#
#       locate files for rom
#
#****************************************************************************

TLX386.abm :          TLX386.exe TLX386.map TLX386.loc TLX386.mak
                        loc @TLX386.loc

#****************************************************************************
#
#       link files
#
#****************************************************************************

TLX386.exe :  $(START) \
                TLX386.obj \
        platform.obj pilot.obj \
        dspio.obj icu.obj tcu.obj aio.obj \
        rom.obj speaker.obj terminal.obj \
        ground.obj menu_ana.obj menu_rlc.obj menu_rst.obj menu_utl.obj \
        menu_sbx.obj menu_sta.obj menu_tm.obj menu_gnd.obj \
        menu_pio.obj \
		$(HAYESLIB) $(GNDLIB) $(CLIB) \
                TLX386.mak
                        $(LINK) @<<TLX386.lnk
$(START)+
TLX386+
platform+pilot+
dspio+icu+tcu+aio+
rom+speaker+terminal+
ground+menu_ana+menu_rlc+menu_rst+menu_utl+
menu_sbx+menu_sta+menu_tm+menu_gnd+
menu_pio
TLX386.exe
TLX386.map
$(HAYESLIB)+$(GNDLIB)+$(CLIB)
NUL.DEF
<<KEEP

#****************************************************************************
#
#       compile application specific C source code
#
#****************************************************************************

TLX386.obj :   TLX386.c TLX386.mak

platform.obj :   platform.c TLX386.mak

pilot.obj :   pilot.c TLX386.mak

dspio.obj :   dspio.c TLX386.mak

icu.obj :   icu.c TLX386.mak

tcu.obj :   tcu.c TLX386.mak

aio.obj :   aio.c TLX386.mak

#analog.obj :   analog.c TLX386.mak

rom.obj :   rom.c

speaker.obj :   speaker.c

terminal.obj : terminal.c

ground.obj :   ground.c TLX386.mak

menu_ana.obj :   menu_ana.c TLX386.mak

menu_rlc.obj :   menu_rlc.c TLX386.mak

menu_rst.obj :   menu_rst.c TLX386.mak

menu_utl.obj :   menu_utl.c TLX386.mak

menu_sbx.obj :   menu_sbx.c TLX386.mak

menu_sta.obj :   menu_sta.c TLX386.mak

menu_tm.obj :   menu_tm.c TLX386.mak

menu_gnd.obj :   menu_gnd.c TLX386.mak

menu_pio.obj :  menu_pio.c TLX386.mak

#****************************************************************************
#
#       inference rules
#
#****************************************************************************

.ASM.OBJ:
	$(MASM) $*.ASM;

.C.OBJ:
	$(CL) $*.C

#****************************************************************************
#
#       end of make file
#
#****************************************************************************
