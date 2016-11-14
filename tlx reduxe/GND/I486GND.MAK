# file: i486gnd.mak
#****************************************************************************
#
#       Make file for ground test routines
#
#       default build: Sv40gndA.lib
#
#       written by denton marlowe
#       (c)1991 Hayes Targets
#
#****************************************************************************
#!INCLUDE make.inc
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
#               PC         IBM PC
#
#****************************************************************************
CPU=I486
#REMOTE=REMOTE_I_O
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
#                8              8 MHz
#                5              5 MHz
#               XX         PC MHz
#
#**************************************************************************
CLOCK=X
#****************************************************************************
#
#       macro to set floating point option
#
#               (BLANK)            no floating point
#               A                          alternate math or emulator
#               E                          emulator math
#               7                          inline Co-processor
#
#****************************************************************************
MATH=7
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
GNDLIB=$(LIBX)\$(MODEL)$(CPU)GND$(HMATH).lib
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
#                               Cx                              public symbols case sensitive
#                               Zm                              Masm 5.1 Compatibility
#                               Dmem$(MODEL)=1  set memory model
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
#               G0      use 8086 instructions
#                               W2              warning level 2
#                               FPa     use alternate math package
#               FPi87   use inline 8087
#                               FPi             use inline emulator library
#
#****************************************************************************
CFLAGS=/A$(MODEL) /Zi /Oi /Gs /c /G1 /W2 /$(FPMATH) /D$(CPU) /DCLOCK=$(CLOCK)
!IF "$(REMOTE)" == "REMOTE_I_O"
CL=cl $(CFLAGS) /DHAYESLIB /D$(REMOTE)
!ELSE
CL=cl $(CFLAGS) /DHAYESLIB
!ENDIF
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
#       macros to back up files
#
#       options:
#               u               update backup file
#               x               exclude files lisited
#
#****************************************************************************
PFLAGS=-u -x*.ZAK -x*.BAK -x*.ZIP
PKZIP=pkzip $(PFLAGS)
#****************************************************************************
#
#               macros to set Library output name
#
#****************************************************************************
!IF "$(REMOTE)" == "REMOTE_I_O"
HAYES=$(MODEL)RMGND$(HMATH)
!ELSE
!IF "$(CPU)" == "PC"
HAYES=$(MODEL)$(CPU)GND$(HMATH)
!ELSE IF "$(CPU)" == "I486"
HAYES=$(MODEL)486GND$(HMATH)
!ELSE
HAYES=$(MODEL)$(CPU)GND$(HMATH)
!ENDIF
!ENDIF
#****************************************************************************
#
#       targets
#
#****************************************************************************
all:    hayes.h $(HAYES).lib
#****************************************************************************
#
#       create header file
#
#****************************************************************************

hayes.h :       ground.h menu_gnd.h \
		menu_ana.h menu_rtc.h menu_rlc.h menu_sta.h \
		menu_tm.h \
		terminal.h menu_utl.h menu_sbx.h menu_rst.h rom.h \
        menu_pio.h \
        speaker.h \
                i486gnd.mak
			del hayes.h
			del gnd.h
			copy *.h hayes.hhh
			ren hayes.hhh hayes.h
			copy hayes.h gnd.h
			del hayes.h
#****************************************************************************
#
#       lib files
#
#****************************************************************************

!IF "$(REMOTE)" == "REMOTE_I_O"

$(HAYES).lib :  menu_gnd.obj \
			menu_ana.obj menu_rtc.obj menu_rlc.obj menu_sta.obj \
			menu_utl.obj menu_sbx.obj \
                        i486gnd.mak
				del $(HAYES).lib
				del $(HAYES).lst
                                lib @<<i486gnd.lnk
$(HAYES).lib
Y
+menu_gnd&
+menu_ana+menu_rtc+menu_rlc+menu_sta&
+menu_utl+menu_sbx
$(HAYES).lst
<<KEEP

!ELSE IF "$(CPU)" == "PC"

$(HAYES).lib :  menu_gnd.obj \
			menu_ana.obj menu_rtc.obj menu_rlc.obj menu_sta.obj \
			menu_tm.obj \
			menu_utl.obj menu_sbx.obj menu_rst.obj \
                        i486gnd.mak
				del $(HAYES).lib
				del $(HAYES).lst
                                lib @<<i486gnd.lnk
$(HAYES).lib
Y
+menu_gnd&
+menu_ana+menu_rtc+menu_rlc+menu_sta&
+menu_tm&
+menu_utl+menu_sbx+menu_rst
$(HAYES).lst
<<KEEP

!ELSE IF "$(CPU)" == "I486"

$(HAYES).lib :	ground.obj menu_gnd.obj \
			menu_ana.obj menu_rtc.obj menu_rlc.obj menu_sta.obj \
			menu_tm.obj \
			terminal.obj menu_utl.obj menu_sbx.obj menu_rst.obj rom.obj \
            menu_pio.obj speaker.obj \
                        i486gnd.mak
				del $(HAYES).lib
				del $(HAYES).lst
                                lib @<<i486gnd.lnk
$(HAYES).lib
Y
+ground+menu_gnd&
+menu_ana+menu_rtc+menu_rlc+menu_sta&
+menu_tm&
+terminal+menu_utl+menu_sbx+menu_rst+rom&
+menu_pio&
+speaker
$(HAYES).lst
<<KEEP

!ELSE

$(HAYES).lib :  ground.obj menu_gnd.obj \
			menu_ana.obj menu_rtc.obj menu_rlc.obj menu_sta.obj \
			menu_tm.obj \
			terminal.obj menu_utl.obj menu_sbx.obj menu_rst.obj rom.obj \
                        i486gnd.mak
				del $(HAYES).lib
				del $(HAYES).lst
                                lib @<<i486gnd.lnk
$(HAYES).lib
Y
+ground+menu_gnd&
+menu_ana+menu_rtc+menu_rlc+menu_sta&
+menu_tm&
+terminal+menu_utl+menu_sbx+menu_rst+rom
$(HAYES).lst
<<KEEP
!ENDIF
#****************************************************************************
#
#       compile C source code
#
#****************************************************************************

menu_gnd.obj :  menu_gnd.c menu_gnd.h ground.h

menu_ana.obj :  menu_ana.c menu_ana.h ground.h

menu_rlc.obj :  menu_rlc.c menu_rlc.h ground.h

menu_sta.obj :  menu_sta.c menu_sta.h ground.h

!IF "$REMOTE" != "REMOTE_I_O"
menu_tm.obj :   menu_tm.c menu_tm.h ground.h
!ENDIF

menu_utl.obj :  menu_utl.c menu_utl.h ground.h

menu_sbx.obj :  menu_sbx.c menu_utl.h ground.h

!IF "$(CPU)" != "PC"
menu_rtc.obj :  menu_rtc.c menu_rtc.h ground.h
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
menu_rst.obj :  menu_rst.c menu_rst.h ground.h
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
ground.obj :    ground.c ground.h
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
terminal.obj :  terminal.c terminal.h ground.h
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
rom.obj :       rom.c rom.h
!ENDIF
!ENDIF

!IF "$(CPU)" == "I486"
menu_pio.obj:	menu_pio.c menu_pio.h
!ENDIF

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
