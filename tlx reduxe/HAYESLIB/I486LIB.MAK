# file: i486lib.mak
#****************************************************************************
#
#	HAYES Library MAKE File
#
#	default build: Sv40_10A.lib
#
#	written by denton marlowe
#	(c)1991 Hayes Targets
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
#!IFDEF TOWER
#LIB=c:\msvc\lib
#INC=c:\msvc\include
#LIBX=c:\msvc\libx
#INCX=c:\msvc\includex
#ST=$(LIBX)
#!MESSAGE TOWER configuration
#!ELSE
#!IFDEF I486
#LIB=c:\msvc\lib
#INC=c:\msvc\include
#LIBX=c:\msvc\libx
#INCX=c:\msvc\includex
#ST=$(LIBX)
#!MESSAGE 486 configuration
#!ELSE
#LIB=c:\msvc\lib
#INC=c:\msvc\include
#LIBX=c:\msvc\libx
#INCX=c:\msvc\includex
#ST=$(LIBX)
#!MESSAGE laptop configuration
#!ENDIF
#!ENDIF
#****************************************************************************
#
#       macro to set compiler version
#
#		5	  Version 5.1
#		6	  Version 6.0
#		7	  Version 7.0
#		7	  Version 8.0
#
#****************************************************************************
VER=7
#****************************************************************************
#
#       macro to set memory model
#
#		S	   Small
#		C	   Compact
#		M	   Meduim
#		L	   Large
#
#****************************************************************************
MODEL=S
#****************************************************************************
#
#       macro to set processor type (limited to 3 characters)
#
#		V20	   LPM-SBC8-X
#		V40	   LPM-SBC40-X
#		V50	   LPM-SBC50-X
#		PC	   IBM PC
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
#		10	   10 MHz
#		 8		8 MHz
#		 5		5 MHz
#		XX	   PC MHz
#
#****************************************************************************
CLOCK=X
#****************************************************************************
#
#       macro to set floating point option
#
#		(BLANK)		   no floating point
#		A			   alternate math or emulator
#		E			   emulator math
#		7			   inline Co-processor
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
#               G1      use 80186 instructions
#               G0      use 8086 instructions
#				W2		warning level 2
#				FPa 	use alternate math package
#               FPi87   use inline 8087
#				FPi		use inline emulator library
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
#       macros to set Library output name
#
#****************************************************************************
!IF "$(REMOTE)" == "REMOTE_I_O"
HAYES=$(MODEL)$(CPU)_RM$(HMATH)
!ELSE
!IF "$(CPU)" == "PC"
HAYES=$(MODEL)$(CPU)_XX$(HMATH)
!ELSE
HAYES=$(MODEL)$(CPU)_$(CLOCK)$(HMATH)
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

!INCLUDE hayes_h.inc

#****************************************************************************
#
#       lib files
#
#****************************************************************************

!IF "$(REMOTE)" == "REMOTE_I_O"

$(HAYES).lib :		 led.obj watchdog.obj ppi.obj lbx_pio.obj delay.obj \
			rtc.obj \
			analog.obj \
			rlc.obj status.obj lpm_sbx.obj \
			get_flag.obj \
            protocal.obj checksum.obj \
            hostbuf.obj 8250.obj \
			i486lib.mak
				del $(HAYES).lib
				del $(HAYES).lst
				lib @<<hayes.lnk
$(HAYES).lib
Y
+led+watchdog+ppi+lbx_pio+delay&
+rtc&
+analog&
+rlc+status+lpm_sbx&
+get_flag&
+protocal+checksum&
+hostbuf+8250
$(HAYES).lst
<<KEEP

!ELSE IF "$(CPU)" == "PC"

$(HAYES).lib :		 delay.obj \
			hardware.obj icu.obj \
			remtron.obj cmd_tmio.obj analog.obj \
			rlc.obj status.obj lpm_sbx.obj \
			get_flag.obj \
			protocal.obj checksum.obj \
			i486lib.mak
				del $(HAYES).lib
				del $(HAYES).lst
				lib @<<hayes.lnk
$(HAYES).lib
Y
+delay&
+hardware+icu&
+analog+remtron+cmd_tmio&
+rlc+status+lpm_sbx&
+get_flag&
+protocal+checksum
$(HAYES).lst
<<KEEP

#****************************************************************************
#
#		i486 lib files
#
#****************************************************************************

!ELSE IF "$(CPU)" == "I486"

$(HAYES).lib :		 watchdog.obj delay.obj \
			cmos.obj hardware.obj icu.obj tcu.obj \
			rom.obj lbx_pio.obj remtron.obj cmd_tmio.obj analog.obj \
			rlc.obj status.obj lpm_sbx.obj \
			rdeb.obj \
			get_flag.obj \
			peek.obj poke.obj ramtest.obj \
			protocal.obj checksum.obj mon_i_or.obj mon_puts.obj cmdtmbuf.obj \
			dspio.obj \
			mit_io.obj \
			i486lib.mak
				del $(HAYES).lib
				del $(HAYES).lst
				lib @<<hayes.lnk
$(HAYES).lib
Y
watchdog+delay&
+cmos+hardware+icu+tcu&
+rom+lbx_pio+remtron+cmd_tmio+analog&
+rlc+status+lpm_sbx&
+rdeb&
+get_flag&
+peek+poke+ramtest&
+protocal+checksum+mon_i_or+mon_puts+cmdtmbuf&
+dspio&
+mit_io
$(HAYES).lst
<<KEEP

!ELSE

$(HAYES).lib :		 v40init.obj led.obj watchdog.obj ppi.obj lbx_pio.obj delay.obj \
			cmos.obj hardware.obj icu.obj tcu.obj rtc.obj scu.obj \
			uart.obj rom.obj remtron.obj analog.obj \
			rlc.obj status.obj lpm_sbx.obj \
			rdeb.obj \
			get_flag.obj \
			peek.obj poke.obj ramtest.obj \
			protocal.obj checksum.obj mon_i_or.obj mon_puts.obj cmdtmbuf.obj \
			dspio.obj \
			i486lib.mak
				del $(HAYES).lib
				del $(HAYES).lst
				lib @<<hayes.lnk
$(HAYES).lib
Y
+v40init+led+watchdog+ppi+lbx_pio+delay&
+cmos+hardware+icu+tcu+scu&
+rtc+uart+rom+remtron+cmd_tmio+analog&
+rlc+status+lpm_sbx&
+rdeb&
+get_flag&
+peek+poke+ramtest&
+protocal+checksum+mon_i_or+mon_puts+cmdtmbuf&
+dspio
$(HAYES).lst
<<KEEP

!ENDIF
#****************************************************************************
#
#       compile C source code
#
#****************************************************************************

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
cmos.obj :		cmos.c cmos.h config.h i486lib.mak
!ENDIF
!ENDIF

hardware.obj :	hardware.c hardware.h config.h i486lib.mak

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
v40init.obj :	v40init.c v40init.h config.h clock.h i486lib.mak
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
led.obj :		led.c led.h config.h i486lib.mak
!ENDIF

!IF "$(CPU)" != "PC"
watchdog.obj :	watchdog.c watchdog.h config.h i486lib.mak
!ENDIF

delay.obj : 	delay.c delay.h config.h i486lib.mak

!IF "$(CPU)" != "PC"
ppi.obj :		ppi.c ppi.h 8255.h config.h i486lib.mak
!ENDIF

!IF "$(CPU)" != "PC"
lbx_pio.obj :	lbx_pio.c lbx_pio.h 8255.h config.h i486lib.mak
!ENDIF

!IF "$REMOTE" != "REMOTE_I_O"
icu.obj :		icu.c icu.h config.h i486lib.mak
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
tcu.obj :		tcu.c tcu.h config.h clock.h i486lib.mak
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
scu.obj :		scu.c scu.h config.h i486lib.mak
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
rtc.obj :		rtc.c rtc.h config.h i486lib.mak
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
uart.obj :		uart.c uart.h config.h i486lib.mak
!ENDIF
!ENDIF

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"
rom.obj :		rom.c rom.h config.h i486lib.mak
!ENDIF
!ENDIF

analog.obj :	analog.c analog.h config.h i486lib.mak

!IF "$REMOTE" != "REMOTE_I_O"
remtron.obj :	remtron.c remtron.h lbx_pio.h 8255.h config.h i486lib.mak

cmd_tmio.obj :	cmd_tmio.c cmd_tmio.h config.h i486lib.mak
!ENDIF

rlc.obj :	rlc.c rlc.h config.h i486lib.mak

status.obj :   status.c status.h config.h i486lib.mak

lpm_sbx.obj :	lpm_sbx.c lpm_sbx.h 8255.h config.h i486lib.mak

!IF "$(CPU)" != "PC"
dspio.obj : 	dspio.c dspio.h config.h i486lib.mak
!ENDIF

#****************************************************************************
#
#		compile customer payload interface code
#
#****************************************************************************

mit_io.obj :	mit_io.c mit_io.h config.h i486lib.mak

#****************************************************************************
#
#       compile remote debugger interface code
#
#****************************************************************************

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"

rdeb.obj :		rdeb.c rdeb.h

!ENDIF
!ENDIF

#****************************************************************************
#
#	compile C - assembly source code
#
#****************************************************************************

get_flag.obj :	get_flag.c

#****************************************************************************
#
#       compile assembly source code
#
#****************************************************************************

!IF "$(CPU)" != "PC"
!IF "$REMOTE" != "REMOTE_I_O"

cfltcvt.obj :	cfltcvt.asm mmacros.inc

install.obj :   install.asm mmacros.inc 

peek.obj :      peek.asm mmacros.inc

poke.obj :      poke.asm mmacros.inc

ramtest.obj :   ramtest.asm

!ENDIF
!ENDIF

#****************************************************************************
#
#       communications
#
#****************************************************************************

!IF "$(CPU)" != "PC"

protocal.obj:	protocal.c protocal.h

checksum.obj:   checksum.c checksum.h

mon_i_or.obj:	 mon_i_or.c mon_i_or.h

mon_puts.obj:	 mon_puts.c mon_puts.h

cmdtmbuf.obj:	 cmdtmbuf.c cmdtmbuf.h

host_buf.obj:	  hostbuf.c hostbuf.h

8250.obj:         8250.c 8250.h

!ENDIF

#****************************************************************************
#
#       inference rules
#
#****************************************************************************

.ASM.OBJ:
	$(MASM) /Fo$*.obj /Ta $*.ASM

.C.OBJ:
	$(CL) $*.C

#****************************************************************************
#
#       end of Hayes Library Make file
#
#****************************************************************************
