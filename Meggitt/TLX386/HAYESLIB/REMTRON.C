/* file: remtron.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("REMTRON Command/Telemetry Link functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *	REMTRON CMD/TM functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *	MASM Version 6.0
 *	Datalight C_Thru_ROM Version 2.0c
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"			/* configuration information */
 #include	"hardware.h"		/* utility routines */
 #include	"8255.h"			/* interface hardware definitions */

 #ifdef PC

 #include	"lpm_sbx.h"			/* interface hardware definitions */

 #else

 #include	"lbx_pio.h"			/* interface hardware definitions */

 #endif

 #include	"icu.h"				/* interrupt controller definitions */

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Test for 240KHz encoder flag
 *
 ***************************************************************************/

 #if defined(FAST_ENCODER)

 #pragma message("USING 240KHz ENCODER")

 #else

 #pragma message("USING STANDARD ENCODER")

 #endif

/****************************************************************************
 *
 *	definitions for REMTRON data link interface
 *
 ***************************************************************************/

 #ifdef PC

 #define LPM_SBX_PIO

 #pragma message("LPM_SBX_PIO Interface across STD-BUS")

 #else

 #define SBX_PIO

 #pragma message("SBX_PIO Interface across SBX-BUS")

 #endif

/****************************************************************************
 *
 *	definitions for REMTRON data link interface
 *
 ***************************************************************************/

 #ifdef LPM_SBX_PIO

 #define CMD_CNTRL	   mcs0_1_control
 #define CMD_ADDR_PORT mcs0_1_portb
 #define CMD_DATA_PORT mcs0_1_porta
 #define CMD_AUX_PORT  mcs0_1_portc

 #define TM_CNTRL	   mcs1_1_control
 #define TM_ADDR_PORT  mcs1_1_portb
 #define TM_DATA_PORT  mcs1_1_porta
 #define TM_AUX_PORT   mcs1_1_portc

 #elif defined(SBX_PIO)

 #define CMD_CNTRL     mcs0_control
 #define CMD_ADDR_PORT mcs0_portb
 #define CMD_DATA_PORT mcs0_porta
 #define CMD_AUX_PORT  mcs0_portc

 #define TM_CNTRL      mcs1_control
 #define TM_ADDR_PORT  mcs1_portb
 #define TM_DATA_PORT  mcs1_porta
 #define TM_AUX_PORT   mcs1_portc

 #elif defined(PCM_PIO)

 #define CMD_TM_CNTRL	pcm_mcs0_control

 #define CMD_DATA_PORT	pcm_mcs0_porta
 #define TM_DATA_PORT	pcm_mcs1_portb
 #define CMD_ADDR_PORT	pcm_mcs0_portc
 #define TM_ADDR_PORT	pcm_mcs1_portc

 #endif

/****************************************************************************
 *
 *	definitions for REMTRON data link interface
 *
 ***************************************************************************/

 #define CMD_ENABLE    0x01
 #define CMD_DISABLE   0x00
 #define TM_ENABLE	   0x00
 #define TM_DISABLE    0x01

/****************************************************************************
 *
 *	data structures for REMTRON data link
 *
 ***************************************************************************/

 #define NUM_CMD_TM_CHANNELS    16
 #define NUM_TM_CHANNELS        16
 #define NUM_CMD_CHANNELS       9

 #define TM_FRAME_COUNT         15

 struct CMD_TM {
	unsigned char addr;
	unsigned char bit[8];
	unsigned char num_channels;
	unsigned int frame_count;
	unsigned char data[NUM_CMD_TM_CHANNELS];
	};

 volatile struct CMD_TM cmd;
 volatile struct CMD_TM tm;

/****************************************************************************
 *
 *	CMD function pointer to command decoder function
 *
 ***************************************************************************/

 void (far *funptr_cmd_application)(void);  /* pointer to function */

 void (far *funptr_tm_application)(void);	/* pointer to function */

 void (__interrupt __far *cmd_vector)();		/* pointer to current IRQ handler */

 void (__interrupt __far *tm_vector)();		/* pointer to current IRQ handler */

 void (__interrupt __far *cmd_old_vector)();/* pointer to old IRQ handler */

 void (__interrupt __far *tm_old_vector)();	/* pointer to old IRQ handler */

/****************************************************************************
 *
 *      CMD/TM function prototypes
 *
 ***************************************************************************/

 void init_REMTRON(void);       /* initialize CMD/TM system */
 void init_REMTRON16(void);     /* initialize CMD/TM system */
 void de_init_REMTRON(void);	/* deinitialize CMD/TM system */

 void init_mcs(void);			/* initialize CMD/TM interface hardware */

 void cmd_tm_reset(void);       /* reset CMD/TM system */
 void cmd_tm_enable(void);      /* enable CMD/TM system */
 void cmd_tm_disable(void);     /* disable CMD/TM system */
 void enable_cmd(void);         /* enable CMD system */
 void disable_cmd(void);		/* disable CMD system */
 void enable_tm(void);          /* enable TM system */
 void disable_tm(void);         /* disable TM system */

 void echo_cmd_to_tm(void);     /* echo CMD data to TM output */

 void install_cmd_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_cmd_application(void);              /* dummy main timing loop application */

 void install_tm_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_tm_application(void);				/* dummy main timing loop application */

/****************************************************************************
 *
 *	REMTRON function prototype for interrupt service routines
 *
 ***************************************************************************/

 void interrupt far cmd_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* CMD interrupt service routine */

 void interrupt far tm_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* TM interrupt service routine */

/****************************************************************************
 *
 *	REMTRON function prototype for interrupt service routines
 *
 ***************************************************************************/

 void interrupt far cmd16_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
					    /* CMD interrupt service routine */

 void interrupt far tm16_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
					    /* TM interrupt service routine */

/****************************************************************************
 *
 *	end of CMD/TM definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      CMD/TM initialization routine
 *
 *      DELCARATION:    void init_REMTRON(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    CMD/TM system enabled
 *
 *      LOCALS:         int i
 *
 *      EXTERNALS
 *       Referenced:    void int_mcs(void)
 *                      void _disable(void)
 *                      void install_vector(void far *routine,int irq)
 *                      void interrupt far cmd_int_handler(void)
 *                      void interrupt far tm_int_handler(void)
 *                      void int_enable(int irq)
 *                      void _enable(void)
 *                      struct CMD_TM cmd
 *                      struct CMD_TM tm
 *
 *      INCLUDES:       <dos.h>
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function installs the CMD/TM interrupt service routines, and
 *      enable interrupts from the CMD/TM system.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void init_REMTRON(void)
 {
 int i;
/****************************************************************************
 *
 *      initialize command and telemetry interface hardware
 *
 ***************************************************************************/

	init_mcs();

/****************************************************************************
 *
 *	initialize command and telemetry data
 *
 ***************************************************************************/

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		cmd.data[i] = i;
		tm.data[i] = i;
		}

	cmd.frame_count = 0;
	tm.frame_count = 0;
	tm.data[TM_FRAME_COUNT] = 0;

/****************************************************************************
 *
 *	disable interrupts
 *
 ***************************************************************************/

	_disable();

/****************************************************************************
 *
 *	Install interrupt service routines for CMD/TM
 *
 ***************************************************************************/

 #ifdef PC

	cmd_old_vector = _dos_getvect(CMD_VEC);
	tm_old_vector = _dos_getvect(TM_VEC);

	_dos_setvect(CMD_VEC,cmd_int_handler);
	_dos_setvect(TM_VEC,tm_int_handler);

#else

	install_vector(cmd_int_handler,CMD_IRQ);
	install_vector(tm_int_handler,TM_IRQ);

 #endif

/****************************************************************************
 *
 *	Install command decoder function pointer
 *
 ***************************************************************************/

	install_cmd_application(dummy_cmd_application);

/****************************************************************************
 *
 *	unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);
	int_enable(TM_IRQ);

	_enable();
 }
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      CMD initialization routine
 *
 *      DELCARATION:    void init_RTD_16(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    CMD/TM system enabled
 *
 *      LOCALS:         int i
 *
 *      EXTERNALS
 *       Referenced:    void int_mcs(void)
 *                      void _disable(void)
 *                      void install_vector(void far *routine,int irq)
 *                      void interrupt far cmd16_int_handler(void)
 *                      void int_enable(int irq)
 *                      void _enable(void)
 *                      struct CMD_TM cmd
 *
 *      INCLUDES:       <dos.h>
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function installs the CMD/TM interrupt service routines, and
 *      enable interrupts from the CMD/TM system.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void init_REMTRON16(void)
 {
 int i;
/****************************************************************************
 *
 *      initialize command and telemetry interface hardware
 *
 ***************************************************************************/

	init_mcs();

/****************************************************************************
 *
 *      initialize command and telemetry data
 *
 ***************************************************************************/

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		cmd.data[i] = i;
		tm.data[i] = i;
		}

	cmd.frame_count = 0;
	tm.frame_count = 0;
	tm.data[TM_FRAME_COUNT] = 0;

/****************************************************************************
 *
 *      disable interrupts
 *
 ***************************************************************************/

	_disable();

/****************************************************************************
 *
 *      Install interrupt service routines for CMD/TM
 *
 ***************************************************************************/

	install_vector(cmd16_int_handler,CMD_IRQ);
	install_vector(tm16_int_handler,TM_IRQ);

/****************************************************************************
 *
 *      Install command decoder function pointer
 *
 ***************************************************************************/

	install_cmd_application(dummy_cmd_application);

/****************************************************************************
 *
 *		Install telemetry encoder function pointer
 *
 ***************************************************************************/

	install_tm_application(dummy_tm_application);

/****************************************************************************
 *
 *      unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);
	int_enable(TM_IRQ);

	_enable();
 }

 #pragma page(1)

/****************************************************************************
 *
 *	de-initialize CMD/TM system
 *
 ***************************************************************************/
 void de_init_REMTRON(void)
 {

	_disable(); 					/* disable interrupts till installation complete */

	int_disable(CMD_IRQ);			/* disable interrupts from CMD */
	int_disable(TM_IRQ);			/* disable interrupts from TM */

 #ifdef PC

	_dos_setvect(CMD_VEC,cmd_old_vector);
	_dos_setvect(TM_VEC,tm_old_vector);

 #endif

	_enable();						/* enable interrupts to processor */

 }
 #endif

 #pragma page(1)

/****************************************************************************
 *
 *
 * ********** I N T E R R U P T   S E R V I C E  R O U T I N E S **********
 *
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      command link interrupt service routine
 *
 *      DELCARATION:    void interrupt far cmd_int_handler(unsigned int es, unsigned int ds,
 *                                       unsigned int di, unsigned int si,
 *                                       unsigned int bp, unsigned int sp,
 *                                       unsigned int bx, unsigned int dx,
 *                                       unsigned int cx, unsigned int ax,
 *                                       unsigned int ip, unsigned int cs,
 *                                       unsigned int flags)
 *
 *      PARAMETERS:     Processor's registers
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    DATA VALID strobe from REMTRON RTD1
 *
 *      EXIT
 *       Conditions:    input data loaded into data structure
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void _enable(void)
 *                      int outp(unsigned portid,int value)
 *                      int inp(unsigned portid)
 *                      struct CMD_TM cmd
 *
 *      INCLUDES:       <dos.h>
 *                      <conio.h>
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This routine is accessed by an interrupt caused by the STROBE output
 *      from the RTD1 command decoder. The routine reads the address output
 *      from the decoder, and then inputs the command data from the decoder
 *      and stores it in the command data array.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void interrupt far cmd_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	_enable();									/* enable interrupts */

	outp( CMD_CNTRL, CMD_ENABLE );				/* enable decoder output */

	cmd.addr = inp( CMD_ADDR_PORT ) & 0x0f; 	/* input address and mask high nibble */

	cmd.data[cmd.addr] = inp( CMD_DATA_PORT );	/* input data into input array */

	if(cmd.addr == NUM_CMD_CHANNELS) {
		(*funptr_cmd_application)();			/* call command decoder code */
		cmd.frame_count += 1;					/* increment frame counter */
		}

	outp( CMD_CNTRL, CMD_DISABLE ); 			/* disable decoder output */

	outp(ICU_BASE,CMD_EOI); 					/* specific EOI command */

 }                                              /* return from interrupt */
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      telemetry link interrupt service routine
 *
 *      DELCARATION:    void interrupt far tm_int_handler(unsigned int es, unsigned int ds,
 *                                       unsigned int di, unsigned int si,
 *                                       unsigned int bp, unsigned int sp,
 *                                       unsigned int bx, unsigned int dx,
 *                                       unsigned int cx, unsigned int ax,
 *                                       unsigned int ip, unsigned int cs,
 *                                       unsigned int flags)
 *
 *      PARAMETERS:     Processor's registers
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    STROBE from REMTRON RTE2
 *
 *      EXIT
 *       Conditions:    tm data output to encoder
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid,int value)
 *                      int inp(unsigned portid)
 *                      struct CMD_TM tm
 *
 *      INCLUDES:       <conio.h>
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This routine is accessed by an interrupt caused by the STROBE output
 *      from the RTE2 telemetry encoder. The routine reads the address output
 *      from the encoder, and then outputs the required data from the
 *      telemetry data array to the encoder. Note that the data output to the
 *      encoder is not the current address, but the next address. This is
 *      required because the STROBE signal is short compaired with the
 *      response time of the ICU. The data output from this routine is latched
 *      at the falling edge of the STROBE. It is not possible to output the
 *      data for the current address in time, so the next address is output,
 *      to be latched on the next interrupt. This is only required for the
 *      higher speed encoders (240KHz clock or greater, with divide by 16)
 *
 *      note: no other interrupts are allowed during this service routine and
 *              due to timing considerations, this routine should have the
 *              highest priorty as far as the ICU is concerned.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void interrupt far tm_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */

	tm.addr = (inp( TM_ADDR_PORT) & 0x0f) + 1;      /* input address and mask high nibble */

	if( tm.addr == 16)
		{
		tm.addr = 0;                    /* wrap address to 0x00 */

 #if defined(TM_FRAME_COUNT)

 #pragma message("frame count in rte2 ch15/rtd1 ch0")

		tm.data[TM_FRAME_COUNT]++;		/* increment frame counter */
 #else
		tm.frame_count++;
 #endif
		}

 #if defined(V50)
	#ifdef FAST_ENCODER

 #pragma message("timming trick for fast encoder and v50")

	outp( TM_CNTRL, TM_ENABLE );					/* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */
	outp( TM_CNTRL, TM_ENABLE );                    /* enable encoder output */

	#endif
 #endif

	outp( TM_DATA_PORT, tm.data[tm.addr] );         /* output data from output array */

	outp( TM_CNTRL, TM_DISABLE );                   /* disable encoder output */

	outp(ICU_BASE,TM_EOI);                          /* specific EOI command */

 }                                                      /* return from interrupt */
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      command link interrupt service routine
 *
 *      DELCARATION:    void interrupt far cmd16_int_handler(unsigned int es, unsigned int ds,
 *                                       unsigned int di, unsigned int si,
 *                                       unsigned int bp, unsigned int sp,
 *                                       unsigned int bx, unsigned int dx,
 *                                       unsigned int cx, unsigned int ax,
 *                                       unsigned int ip, unsigned int cs,
 *                                       unsigned int flags)
 *
 *      PARAMETERS:     Processor's registers
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    DATA VALID strobe from REMTRON RTD1
 *
 *      EXIT
 *       Conditions:    input data loaded into data structure
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void _enable(void)
 *                      int outp(unsigned portid,int value)
 *                      int inp(unsigned portid)
 *                      struct CMD_TM cmd
 *
 *      INCLUDES:       <dos.h>
 *                      <conio.h>
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This routine is accessed by an interrupt caused by the STROBE output
 *      from the RTD1 command decoder. The routine reads the address output
 *      from the decoder, and then inputs the command data from the decoder
 *      and stores it in the command data array.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void interrupt far cmd16_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	_enable();									/* enable interrupts */

	outp( CMD_CNTRL, CMD_ENABLE );				/* enable decoder output */

	cmd.addr = inp( CMD_ADDR_PORT ) & 0x0f;		/* input address and mask high nibble */

	cmd.data[cmd.addr] = inp( CMD_DATA_PORT );	/* input data into input array */

//	if(cmd.addr == 0x00) {					/* channel zero last out */
//		(*funptr_cmd_application)();		/* call command decoder code */
//		cmd.frame_count += 1;				/* increment frame counter */
//		}

	if(cmd.addr == NUM_CMD_CHANNELS) {		/* channel 9 last out of RTE1 */
		(*funptr_cmd_application)();		/* call command decoder code */
		cmd.frame_count += 1;				/* increment frame counter */
		}

	outp( CMD_CNTRL, CMD_DISABLE );			/* disable decoder output */

	outp(ICU_BASE,CMD_EOI);					/* specific EOI command */

 }                                          /* return from interrupt */
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      telemetry link interrupt service routine
 *
 *      DELCARATION:    void interrupt far tm16_int_handler(unsigned int es, unsigned int ds,
 *                                       unsigned int di, unsigned int si,
 *                                       unsigned int bp, unsigned int sp,
 *                                       unsigned int bx, unsigned int dx,
 *                                       unsigned int cx, unsigned int ax,
 *                                       unsigned int ip, unsigned int cs,
 *                                       unsigned int flags)
 *
 *      PARAMETERS:     Processor's registers
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    STROBE from REMTRON RTE2
 *
 *      EXIT
 *       Conditions:    tm data output to encoder
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid,int value)
 *                      int inp(unsigned portid)
 *                      struct CMD_TM tm
 *
 *      INCLUDES:       <conio.h>
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This routine is accessed by an interrupt caused by the STROBE output
 *      from the RTE2 telemetry encoder. The routine reads the address output
 *      from the encoder, and then outputs the required data from the
 *      telemetry data array to the encoder.
 *
 *      note: no other interrupts are allowed during this service routine and
 *              due to timing considerations, this routine should have the
 *              highest priorty as far as the ICU is concerned.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void interrupt far tm16_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	outp( TM_CNTRL, TM_ENABLE );			/* enable encoder output */

	tm.addr = (inp( TM_ADDR_PORT) & 0x0f);	/* input address and mask high nibble */

	if( tm.addr == 0)
		{
 #if defined(TM_FRAME_COUNT)

		tm.data[TM_FRAME_COUNT]++;			/* increment frame counter */
 #else
		tm.frame_count++;
 #endif
		}
	outp( TM_DATA_PORT, tm.data[tm.addr] ); /* output data from output array */

	outp( TM_CNTRL, TM_DISABLE );			/* disable encoder output */

	outp(ICU_BASE,TM_EOI);					/* specific EOI command */

 }                                          /* return from interrupt */
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON interface Hardware initialization
 *
 *      DELCARATION:    void init_mcs(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    LBX-PIO iSBX card setup for REMTRON interface
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid,int value)
 *                      int inp(unsigned portid)
 *
 *      INCLUDES:       <conio.h>
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      program 82c55A ppi's on iSBX board for CMD/TM interface
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486
 void init_mcs(void)
 {
/****************************************************************************
 *
 *      program 8255 ppi number 1 on iSBX board for CMD interface
 *
 *      port a input for CMD data word
 *      port b input for CMD address nibble
 *      port c split - lower output - higher input
 *
 ***************************************************************************/

	outp( CMD_CNTRL, ppi_mode_0_14 );	/* mode 0 control word */

/****************************************************************************
 *
 *      set bit 0 in port c high to enable command decoder output
 *
 ***************************************************************************/

	outp( CMD_CNTRL, 0x01 );			/* set bit 0 in port c */

/****************************************************************************
 *
 *      program 8255 ppi number 0 on iSBX board for TM interface
 *
 *      port a output for TM data word
 *      port b input for TM address nibble
 *      port c split - lower output - higher input
 *
 ***************************************************************************/

	outp( TM_CNTRL, ppi_mode_0_6 );		/* mode 0 control word */

/****************************************************************************
 *
 *      Reset bit 0 in port c low to enable telemetry encoder output
 *
 ***************************************************************************/

    outp( TM_CNTRL, 0x00 );             /* set bit 0 in port c */

 }
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON Encoder/Decoder reset
 *
 *      DELCARATION:    void cmd_tm_reset(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON interface reset
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void cmd_tm_disable(void)
 *                      void cmd_tm_enable(void)
 *                      void delay(unsigned count)
 *
 *      INCLUDES:       hardware.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *      This function resets the REMTRON CMD/TM interface
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void cmd_tm_reset(void)
 {
	cmd_tm_disable();       /* power off system and reset interface */

	delay(32000);           /* allow time to settle */

	cmd_tm_enable();        /* power on system and initialize interface */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON Encoder/Decoder Power on
 *
 *      DELCARATION:    void cmd_tm_enable(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON interface enabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void init_mcs(void)
 *                      void enable_cmd(void)
 *                      void enable_tm(void)
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function enables the REMTRON CMD/TM interface
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void cmd_tm_enable(void)
 {
 #ifndef I486

	init_mcs();             /* initialize cmd/tm hardware */

 #endif

    enable_cmd();           /* enable command link interrupts */
    enable_tm();            /* enable tm link interrupts */
 }

 #pragma page(1)

/*@H************************************************************************
 *
 *      REMTRON CMD/TM Disable
 *
 *      DELCARATION:    void cmd_tm_disable(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON interface disabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void disable_cmd(void)
 *                      void disable_tm(void)
 *                      int outp(unsigned portid, int value)
 *
 *      INCLUDES:       <conio.h>
 *                      ppi.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function disables the REMTRON CMD/TM interface
 *
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ***********************************************************************@H*/

 #pragma page(1)

 void cmd_tm_disable(void)
 {
	disable_cmd();          /* disable command link interrupts */
	disable_tm();           /* disable tm link interrupts */

 #ifndef I486

    outp( CMD_CNTRL, ppi_mode_0_15 );    /* mode 0 control word */

    outp( TM_CNTRL, ppi_mode_0_15 );    /* mode 0 control word */

 #endif
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON enable CMD link interrupt
 *
 *      DELCARATION:    void enable_cmd(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON CMD interrupts enabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int_enable(int irq)
 *
 *      INCLUDES:       icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function enables the interrupt from the REMTRON decoder
 *
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void enable_cmd(void)
 {
	int_enable(CMD_IRQ);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON disable CMD link interrupt
 *
 *      DELCARATION:    void disable_cmd(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON CMD interrupts disabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int_disable(int irq)
 *
 *      INCLUDES:       icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function disables the interrupt from the REMTRON decoder
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void disable_cmd(void)
 {
	int_disable(CMD_IRQ);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON enable TM link interrupt
 *
 *      DELCARATION:    void enable_tm(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON TM interrupts enabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int_enable(int irq)
 *
 *      INCLUDES:       icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function enables the interrupt from the REMTRON decoder
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void enable_tm(void)
 {
	int_enable(TM_IRQ);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON disable TM link interrupt
 *
 *      DELCARATION:    void disable_tm(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    REMTRON TM interrupts disabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int_disable(int irq)
 *
 *      INCLUDES:       icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function disables the interrupt from the REMTRON encoder
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void disable_tm(void)
 {
	int_disable(TM_IRQ);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON echo command to telemetry function
 *
 *      DELCARATION:     void echo_cmd_to_tm(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    CMD data structure copied to TM data structure
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    struct CMD_TM cmd
 *                      struct CMD_TM tm
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function copies the in-comming data from the decoder into the
 *      out-going data for the encoder
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void echo_cmd_to_tm(void)
 {
	disable_cmd();

	tm.data[0] = cmd.data[1];		/* address	1 out of telemetry decoder */
	tm.data[1] = cmd.data[2];       /* address  2 out of telemetry decoder */
	tm.data[2] = cmd.data[3];       /* address  3 out of telemetry decoder */
	tm.data[3] = cmd.data[4];       /* address  4 out of telemetry decoder */
	tm.data[4] = cmd.data[5];       /* address  5 out of telemetry decoder */
	tm.data[5] = cmd.data[6];       /* address  6 out of telemetry decoder */
	tm.data[6] = cmd.data[7];       /* address  7 out of telemetry decoder */
	tm.data[7] = cmd.data[8];       /* address  8 out of telemetry decoder */

	tm.data[8] = cmd.data[9];       /* address  9 out of telemetry decoder */

	tm.data[9] = cmd.data[10];      /* address 10 out of telemetry decoder */
	tm.data[10] = cmd.data[11];     /* address 11 out of telemetry decoder */
	tm.data[11] = cmd.data[12];     /* address 12 out of telemetry decoder */
	tm.data[12] = cmd.data[13];     /* address 13 out of telemetry decoder */
	tm.data[13] = cmd.data[14];     /* address 14 out of telemetry decoder */
	tm.data[14] = cmd.data[15];     /* address 15 out of telemetry decoder */

 #if !defined(TM_FRAME_COUNT)

	tm.data[15] = cmd.data[0];      /* address  0 out of telemetry decoder */

 #endif

	enable_cmd();
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON install data decoder function
 *
 *      DELCARATION:    void install_cmd_application(void far *routine)
 *
 *      PARAMETERS:     far pointer to function
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    user function installed in decoder interrupt
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int_disable(int irq)
 *                      funptr_cmd_application
 *                      int_enable(int irq)
 *
 *      INCLUDES:       icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function load a function pointer with a user supplied function
 *      for execution after frame on command data.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)


 void install_cmd_application(void (far *routine)(void))
 {
	int_disable(CMD_IRQ);                   /* disable timer/counter interrupts */

	funptr_cmd_application = routine;       /* install pointer to application */

	int_enable(CMD_IRQ);                    /* enable timer/counter interrutps */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Dummy Application function
 *
 *      DELCARATION:    void far dummy_cmd_application(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    None
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This empty function is used as the default value for the application
 *      function pointer
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void far dummy_cmd_application(void)
 {

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		REMTRON install data encoder function
 *
 *		DELCARATION:	void install_tm_application(void far *routine)
 *
 *      PARAMETERS:     far pointer to function
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *		 Conditions:	user function installed in encoder interrupt
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int_disable(int irq)
 *                      funptr_cmd_application
 *                      int_enable(int irq)
 *
 *      INCLUDES:       icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This function load a function pointer with a user supplied function
 *		for execution after frame on telemetry data.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)


 void install_tm_application(void (far *routine)(void))
 {
	int_disable(TM_IRQ);					 /* disable timer/counter interrupts */

	funptr_tm_application = routine;		 /* install pointer to application */

	int_enable(TM_IRQ);					 /* enable timer/counter interrutps */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Dummy Application function
 *
 *		DELCARATION:	void far dummy_tm_application(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    None
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         remtron.c
 *
 *      DESCRIPTION:
 *
 *      This empty function is used as the default value for the application
 *      function pointer
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void far dummy_tm_application(void)
 {

 }
