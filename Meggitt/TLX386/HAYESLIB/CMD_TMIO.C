/* file: cmd_tmio.c */

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

 #include	<stdlib.h>

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"			/* configuration information */
 #include	"hardware.h"		/* utility routines */
 #include	"icu.h"				/* interrupt controller definitions */

 #include	"8255.h"			/* interface hardware definitions */
 #include	"ppi.h"			/* interface hardware definitions */
 #include	"lpm_sbx.h"			/* interface hardware definitions */
 #include	"lbx_pio.h"			/* interface hardware definitions */

 #include	"remtron.h"			/* interface hardware definitions */

 #ifdef I486
 #pragma message("PCM-I/O48 ppi on I/O list")
 #define _PCM_CARD
 #endif

 #ifdef _PCM_CARD

 #define pcm_mcs0_porta		PCM_BASE	/* address of port a */
 #define pcm_mcs0_portb 	PCM_BASE+1	/* address of port b */
 #define pcm_mcs0_portc 	PCM_BASE+2	/* address of port c */
 #define pcm_mcs0_control	PCM_BASE+3	/* address of control register */

 #define pcm_mcs1_porta		PCM_BASE+4	/* address of port a */
 #define pcm_mcs1_portb		PCM_BASE+5	/* address of port b */
 #define pcm_mcs1_portc		PCM_BASE+6	/* address of port c */
 #define pcm_mcs1_control	PCM_BASE+7	/* address of control register */

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	CMD/TM definitions
 *
 ***************************************************************************/

 #define ID_MODE_SINGLE 	0
 #define ID_MODE_DOUBLE		0

 #define ID_NO_PIO			-1

 #define ID_PIO				0

 #define ID_SBX_PIO 		1

 #define ID_LPM_SBX_PIO 	2
 #define ID_LPM_SBX_PIO_J1	3
 #define ID_LPM_SBX_PIO_J2	4

/****************************************************************************
 *
 *	single pio mode definitions
 *
 ***************************************************************************/

 #ifdef _PCM_CARD

 #define ID_PCM_PIO 		5
 #define ID_PCM_PIO_J1		6
 #define ID_PCM_PIO_J2		7

 #endif

 #define ID_SBX_PIO_J1			8
 #define ID_SBX_PIO_J2			9

 #define ID_LPM_SBX_PIO_J1_J1	10 // first slot j1
 #define ID_LPM_SBX_PIO_J1_J2	11 // first slot j2
 #define ID_LPM_SBX_PIO_J2_J1	12 // second slot j1
 #define ID_LPM_SBX_PIO_J2_J2	13 // second slot j2

 #define ID_ONBOARD_PIO			14 // OnBoard PIO (V40/V50)

/****************************************************************************
 *
 *	definitions for REMTRON data link interface
 *
 ***************************************************************************/

 #define CMD_ENABLE 	0x01
 #define CMD_DISABLE	0x00

 #define TM_ENABLE		0x00
 #define TM_DISABLE 	0x01

 #define SINGLE_PORT	0x00
 #define DUAL_PORT		0x01

/****************************************************************************
 *
 *	data structures for REMTRON data link
 *
 ***************************************************************************/

 struct CMD_TM_PORT {
	unsigned int cntrl;
	unsigned int addr_port;
	unsigned int data_port;
	unsigned int aux_port;
	unsigned int mode;
	unsigned int single_dual;
	};

 volatile struct CMD_TM_PORT cmd_port = {
	0,	// cntrl
	0,	// addr_port
	0,	// data_port
	0,	// aux_port
	0,	// mode
	0}; // single_dual

 volatile struct CMD_TM_PORT tm_port = {
	0,	// cntrl
	0,	// addr_port
	0,	// data_port
	0,	// aux_port
	0,	// mode
	0}; // single_dual

/****************************************************************************
 *
 *	CMD/TM Application function prototypes
 *
 ***************************************************************************/

 #define FUNCTION_OFF		0
 #define FUNCTION_ON		1

 #define FRAME_COUNT_OFF	0
 #define FRAME_COUNT_ON		1

 struct CMD_TM_APP {
	int frame_count_flag;					// frame counting enable switch
	int function_flag;						// function enable switch
	int irq_function_flag;					// function enable switch
	void (far *funptr_application)(void);	// pointer to function
	void (far *funptr_irq)(void);			// pointer to function
	};


 volatile struct CMD_TM_APP cmdApp = {
	FRAME_COUNT_OFF,
	FUNCTION_OFF,
	FUNCTION_OFF,
	NULL,
	NULL};

 volatile struct CMD_TM_APP tmApp = {
	FRAME_COUNT_OFF,
	FUNCTION_OFF,
	FUNCTION_OFF,
	NULL,
	NULL};

/****************************************************************************
 *
 *	CMD/TM function prototypes
 *
 ***************************************************************************/

 void set_cmdtm_port_mode(int mode);
 //void open_cmdtm_port(int port_id, int mode);
 void open_cmdtm_port(int port_id);
 void init_cmdtm(void);

 void init_single_cmd_tm(void);

 void init_dual_msc(void);			/* initialize CMD/TM interface hardware */
 void deinit_dual_msc(void);			/* deinitialize CMD/TM interface hardware */

 void init_single_msc(void);			/* initialize CMD/TM interface hardware */
 void deinit_single_msc(void);			/* deinitialize CMD/TM interface hardware */

/****************************************************************************
 *
 *	CMD/TM function prototypes
 *
 ***************************************************************************/

 int cmd_addr_data_dual(struct CMD_TM *cmd);
 int cmd_address_dual(void);
 int cmd_data_dual(void);

 int tm_addr_data_dual(struct CMD_TM *tm);
 int tm_address_dual(void);
 int tm_data_dual(int data);

/****************************************************************************
 *
 *	REMTRON function prototype for interrupt service routines
 *
 ***************************************************************************/

 void interrupt far rtd1_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* CMD interrupt service routine */

 void interrupt far rte2_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* TM interrupt service routine */

 void interrupt far rtd1s_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* CMD interrupt service routine */

 void interrupt far rte2s_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* TM interrupt service routine */

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_cmd_application(void);
 void disable_cmd_application(void);
 void enable_tm_application(void);
 void disable_tm_application(void);

/****************************************************************************
 *
 * interrupt functions
 *
 ***************************************************************************/

 void enable_cmd_irq_function(void);
 void disable_cmd_irq_function(void);
 void enable_tm_irq_function(void);
 void disable_tm_irq_function(void);

 void install_cmd_irq(void (far *routine)(void));
 void far dummy_cmd_irq(void);
 void install_tm_irq(void (far *routine)(void));
 void far dummy_tm_irq(void);

 void (far *funptr_cmd_irq)(void);	/* pointer to function */

 void (far *funptr_tm_irq)(void);	/* pointer to function */

/****************************************************************************
 *
 * frame count functions
 *
 ***************************************************************************/

 void enable_cmd_frame_count(void);
 void disable_cmd_frame_count(void);
 void enable_tm_frame_count(void);
 void disable_tm_frame_count(void);

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 struct PPI cmd_tm_ppi[2];

 #pragma page(1)

/****************************************************************************
 *
 *
 * ********** I N I T I A L I Z A T I O N **********
 *
 *
 ***************************************************************************/

 void set_cmdtm_port_mode(int mode)
 {
	switch(mode) {
		case DUAL_PORT:
			cmd_port.single_dual = DUAL_PORT;
			tm_port.single_dual = DUAL_PORT;
			break;

		case SINGLE_PORT:
			cmd_port.single_dual = SINGLE_PORT;
			tm_port.single_dual = SINGLE_PORT;
			break;

		default:
			cmd_port.single_dual = DUAL_PORT;
			tm_port.single_dual = DUAL_PORT;
			break;
	}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      CMD/TM initialization routine
 *
 *		DELCARATION:	void open_cmdtm_port(void)
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

 //void open_cmdtm_port(int port_id, int mode)
 void open_cmdtm_port(int port_id)
 {
//
// setup mode
//
 //	switch(mode) {
 //		case DUAL_PORT:
 //			cmd_port.single_dual = DUAL_PORT;
 //			tm_port.single_dual = DUAL_PORT;
 //			break;
 //
 //		case SINGLE_PORT:
 //			cmd_port.single_dual = SINGLE_PORT;
 //			tm_port.single_dual = SINGLE_PORT;
 //			break;
 //
 //		default:
 //			cmd_port.single_dual = DUAL_PORT;
 //			tm_port.single_dual = DUAL_PORT;
 //			break;
 //	}

//
// setup port
//
	switch(port_id) {

		case ID_SBX_PIO:
			cmd_port.cntrl	   = mcs0_control;
			cmd_port.addr_port = mcs0_portb  ;
			cmd_port.data_port = mcs0_porta  ;
			cmd_port.aux_port  = mcs0_portc  ;

			tm_port.cntrl	   = mcs1_control;
			tm_port.addr_port  = mcs1_portb  ;
			tm_port.data_port  = mcs1_porta  ;
			tm_port.aux_port   = mcs1_portc  ;

			init_dual_msc();

			init_cmdtm();
			break;

		case ID_LPM_SBX_PIO:
			cmd_port.cntrl	   = mcs0_1_control;
			cmd_port.addr_port = mcs0_1_portb  ;
			cmd_port.data_port = mcs0_1_porta  ;
			cmd_port.aux_port  = mcs0_1_portc  ;

			tm_port.cntrl	   = mcs1_1_control;
			tm_port.addr_port  = mcs1_1_portb  ;
			tm_port.data_port  = mcs1_1_porta  ;
			tm_port.aux_port   = mcs1_1_portc  ;

			init_dual_msc();

			init_cmdtm();
			break;

		case ID_LPM_SBX_PIO_J1:
			cmd_port.cntrl	   = mcs0_0_control;
			cmd_port.addr_port = mcs0_0_portb  ;
			cmd_port.data_port = mcs0_0_porta  ;
			cmd_port.aux_port  = mcs0_0_portc  ;

			tm_port.cntrl	   = mcs1_0_control;
			tm_port.addr_port  = mcs1_0_portb  ;
			tm_port.data_port  = mcs1_0_porta  ;
			tm_port.aux_port   = mcs1_0_portc  ;

			init_dual_msc();

			init_cmdtm();
			break;

		case ID_LPM_SBX_PIO_J2:
			cmd_port.cntrl	   = mcs0_1_control;
			cmd_port.addr_port = mcs0_1_portb  ;
			cmd_port.data_port = mcs0_1_porta  ;
			cmd_port.aux_port  = mcs0_1_portc  ;

			tm_port.cntrl	   = mcs1_1_control;
			tm_port.addr_port  = mcs1_1_portb  ;
			tm_port.data_port  = mcs1_1_porta  ;
			tm_port.aux_port   = mcs1_1_portc  ;

			init_dual_msc();

			init_cmdtm();
			break;

 #ifdef _PCM_CARD

		case ID_PCM_PIO:
			cmd_port.cntrl	   = pcm_mcs0_control;
			cmd_port.addr_port = pcm_mcs0_portc  ;
			cmd_port.data_port = pcm_mcs0_porta  ;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = pcm_mcs0_control;
			tm_port.addr_port  = pcm_mcs0_portc  ;
			tm_port.data_port  = pcm_mcs0_portb	 ;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_PCM_PIO_J1:
			cmd_port.cntrl	   = pcm_mcs1_control;
			cmd_port.addr_port = pcm_mcs1_portc	 ;
			cmd_port.data_port = pcm_mcs1_porta	 ;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = pcm_mcs1_control;
			tm_port.addr_port  = pcm_mcs1_portc	 ;
			tm_port.data_port  = pcm_mcs1_portb	 ;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_PCM_PIO_J2:
			cmd_port.cntrl	   = pcm_mcs0_control;
			cmd_port.addr_port = pcm_mcs0_portc  ;
			cmd_port.data_port = pcm_mcs0_porta  ;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = pcm_mcs0_control;
			tm_port.addr_port  = pcm_mcs0_portc  ;
			tm_port.data_port  = pcm_mcs0_portb	 ;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

 #endif

		case ID_SBX_PIO_J1:
			cmd_port.cntrl	   = mcs1_control;
			cmd_port.addr_port = mcs1_portc;
			cmd_port.data_port = mcs1_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = mcs1_control;
			tm_port.addr_port  = mcs1_portc;
			tm_port.data_port  = mcs1_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_SBX_PIO_J2:
			cmd_port.cntrl	   = mcs0_control;
			cmd_port.addr_port = mcs0_portc;
			cmd_port.data_port = mcs0_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = mcs0_control;
			tm_port.addr_port  = mcs0_portc;
			tm_port.data_port  = mcs0_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_LPM_SBX_PIO_J1_J1:
			cmd_port.cntrl	   = mcs1_0_control;
			cmd_port.addr_port = mcs1_0_portc;
			cmd_port.data_port = mcs1_0_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = mcs1_0_control;
			tm_port.addr_port  = mcs1_0_portc;
			tm_port.data_port  = mcs1_0_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_LPM_SBX_PIO_J1_J2:
			cmd_port.cntrl	   = mcs0_0_control;
			cmd_port.addr_port = mcs0_0_portc;
			cmd_port.data_port = mcs0_0_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = mcs0_0_control;
			tm_port.addr_port  = mcs0_0_portc;
			tm_port.data_port  = mcs0_0_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_LPM_SBX_PIO_J2_J1:
			cmd_port.cntrl	   = mcs1_1_control;
			cmd_port.addr_port = mcs1_1_portc;
			cmd_port.data_port = mcs1_1_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = mcs1_1_control;
			tm_port.addr_port  = mcs1_1_portc;
			tm_port.data_port  = mcs1_1_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

		case ID_LPM_SBX_PIO_J2_J2:
			cmd_port.cntrl	   = mcs0_1_control;
			cmd_port.addr_port = mcs0_1_portc;
			cmd_port.data_port = mcs0_1_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = mcs0_1_control;
			tm_port.addr_port  = mcs0_1_portc;
			tm_port.data_port  = mcs0_1_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;

 #if defined(V45)

		case ID_ONBOARD_PIO:					// OnBoard PIO (V40/V50)

			cmd_port.cntrl	   = ppi_control;
			cmd_port.addr_port = ppi_portc;
			cmd_port.data_port = ppi_porta;
			cmd_port.aux_port  = 0;

			tm_port.cntrl	   = ppi_control;
			tm_port.addr_port  = ppi_portc;
			tm_port.data_port  = ppi_portb;
			tm_port.aux_port   = 0;

			init_single_msc();

			init_single_cmd_tm();
			break;
 #endif

		default:								// SBX_PIO
			cmd_port.cntrl	   = mcs0_control;
			cmd_port.addr_port = mcs0_portb  ;
			cmd_port.data_port = mcs0_porta  ;
			cmd_port.aux_port  = mcs0_portc  ;

			tm_port.cntrl	   = mcs1_control;
			tm_port.addr_port  = mcs1_portb  ;
			tm_port.data_port  = mcs1_porta  ;
			tm_port.aux_port   = mcs1_portc  ;

			init_dual_msc();

			init_cmdtm();
			break;

		}

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON interface Hardware initialization
 *
 *		DELCARATION:	void init_dual_msc(void)
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

 void init_dual_msc(void)
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

	cmd_port.mode = ppi_mode_0_14;
	outp( cmd_port.cntrl, ppi_mode_0_14 );	/* mode 0 control word */

/****************************************************************************
 *
 *      set bit 0 in port c high to enable command decoder output
 *
 ***************************************************************************/

	outp( cmd_port.cntrl, 0x01 );			/* set bit 0 in port c */

/****************************************************************************
 *
 *      program 8255 ppi number 0 on iSBX board for TM interface
 *
 *      port a output for TM data word
 *      port b input for TM address nibble
 *      port c split - lower output - higher input
 *
 ***************************************************************************/

	tm_port.mode = ppi_mode_0_6;
	outp( tm_port.cntrl, ppi_mode_0_6 );	/* mode 0 control word */

/****************************************************************************
 *
 *      Reset bit 0 in port c low to enable telemetry encoder output
 *
 ***************************************************************************/

	outp( tm_port.cntrl, 0x00 );			/* set bit 0 in port c */
 }

 #pragma page(1)

 void deinit_dual_msc(void)
 {
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      REMTRON interface Hardware initialization
 *
 *		DELCARATION:	void init_single_msc(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *		 Conditions:	PCM-PIO card setup for REMTRON interface
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
 *		program 82c55A ppi's on PCM-PIO board for CMD/TM interface
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_single_msc(void)
 {
/****************************************************************************
 *
 *      program 8255 ppi number 1 on iSBX board for CMD interface
 *
 *      port a input for CMD data word
 *		port b output for TM data word
 *		port c split
 *			lower CMD address input
 *			higher TM address input
 *
 ***************************************************************************/

	cmd_port.mode = ppi_mode_0_13;
	outp( cmd_port.cntrl, ppi_mode_0_13 );	/* mode 0 control word */
 }

 #pragma page(1)

 void deinit_single_msc(void)
 {
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      CMD/TM initialization routine
 *
 *		DELCARATION:	void init_cmdtm(void)
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

 void init_cmdtm(void)
 {
 int i;
/****************************************************************************
 *
 *      initialize command and telemetry interface hardware
 *
 ***************************************************************************/

	//init_mcs();

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

	_dos_setvect(CMD_VEC,rtd1_int_handler);
	_dos_setvect(TM_VEC,rte2_int_handler);

 #else

	install_vector(rtd1_int_handler,CMD_IRQ);
	install_vector(rte2_int_handler,TM_IRQ);

 #endif

/****************************************************************************
 *
 *	Install command decoder function pointer
 *
 ***************************************************************************/

	install_cmd_application(dummy_cmd_application);

/****************************************************************************
 *
 *	Install telemetry encoder function pointer
 *
 ***************************************************************************/

	install_tm_application(dummy_tm_application);

/****************************************************************************
 *
 *	Install command decoder irq function pointer
 *
 ***************************************************************************/

	install_cmd_irq(dummy_cmd_irq);

/****************************************************************************
 *
 *	Install telemetry encoder irq function pointer
 *
 ***************************************************************************/

	install_tm_irq(dummy_tm_irq);

/****************************************************************************
 *
 *	unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);
	int_enable(TM_IRQ);

	_enable();
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      CMD/TM initialization routine
 *
 *		DELCARATION:	void init_single_cmd_tm(void)
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

 void init_single_cmd_tm(void)
 {
 int i;
/****************************************************************************
 *
 *      initialize command and telemetry interface hardware
 *
 ***************************************************************************/

	//init_mcs();

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

	_dos_setvect(CMD_VEC,rtd1_int_handler);
	_dos_setvect(TM_VEC,rte2_int_handler);

#else

	install_vector(rtd1s_int_handler,CMD_IRQ);
	install_vector(rte2s_int_handler,TM_IRQ);

 #endif

/****************************************************************************
 *
 *	Install command decoder function pointer
 *
 ***************************************************************************/

	install_cmd_application(dummy_cmd_application);

/****************************************************************************
 *
 *	Install telemetry encoder function pointer
 *
 ***************************************************************************/

	install_tm_application(dummy_tm_application);

/****************************************************************************
 *
 *	Install command decoder irq function pointer
 *
 ***************************************************************************/

	install_cmd_irq(dummy_cmd_irq);

/****************************************************************************
 *
 *	Install telemetry encoder irq function pointer
 *
 ***************************************************************************/

	install_tm_irq(dummy_tm_irq);

/****************************************************************************
 *
 *	unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(CMD_IRQ);
	int_enable(TM_IRQ);

	_enable();
 }

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
 *		DELCARATION:	void interrupt far rtd1_int_handler(unsigned int es, unsigned int ds,
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

 void interrupt far rtd1_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	//_enable();									/* enable interrupts */

	outp( cmd_port.cntrl, CMD_ENABLE );				/* enable decoder output */

	cmd.addr = inp( cmd_port.addr_port ) & 0x0f;		/* input address and mask high nibble */

	cmd.data[cmd.addr] = inp( cmd_port.data_port );	/* input data into input array */

	if(cmd.addr == NUM_CMD_CHANNELS) {		/* channel 9 last out of RTE1 */
		cmd.frame_count += 1;				/* increment frame counter */
		if(cmdApp.function_flag == FUNCTION_ON) {
			(*funptr_cmd_application)();		/* call command decoder code */
			}
	}

	outp( cmd_port.cntrl, CMD_DISABLE );			/* disable decoder output */

	if(cmdApp.irq_function_flag == FUNCTION_ON) {
		(*funptr_cmd_irq)();				/* call command decoder code */
		}

	outp(ICU_BASE,CMD_EOI);					/* specific EOI command */
 }												/* return from interrupt */

 #pragma page(1)

/*@H*************************************************************************
 *
 *      telemetry link interrupt service routine
 *
 *		DELCARATION:	void interrupt far rte2_int_handler(unsigned int es, unsigned int ds,
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

 void interrupt far rte2_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	//_enable();								/* enable interrupts */

	outp( tm_port.cntrl, TM_ENABLE );			/* enable encoder output */

	tm.addr = (inp( tm_port.addr_port) & 0x0f);	/* input address and mask high nibble */

	if( tm.addr == 0 ) {
		tm.frame_count++;
		if(tmApp.frame_count_flag == FRAME_COUNT_ON) {
			tm.data[TM_FRAME_COUNT]++;		/* increment frame counter */
			}
		if(tmApp.function_flag == FUNCTION_ON) {
			(*funptr_tm_application)();		/* call telemetry encoder code */
			}
	}

	outp( tm_port.data_port, tm.data[tm.addr] ); /* output data from output array */

	outp( tm_port.cntrl, TM_DISABLE );			/* disable encoder output */

	if(tmApp.irq_function_flag == FUNCTION_ON) {
		(*funptr_tm_irq)();					/* call telemetry encoder code */
		}

	outp(ICU_BASE,TM_EOI);					/* specific EOI command */
 }											/* return from interrupt */

 #pragma page(1)

/*@H*************************************************************************
 *
 *      command link interrupt service routine
 *
 *		DELCARATION:	void interrupt far rtd1s_int_handler(unsigned int es, unsigned int ds,
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

 void interrupt far rtd1s_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
 #ifndef I486
	//_enable();									/* enable interrupts */
 #endif

	cmd.addr = inp( cmd_port.addr_port ) & 0x0f;	/* input address and mask high nibble */

	cmd.data[cmd.addr] = inp( cmd_port.data_port );	/* input data into input array */

	if(cmd.addr == NUM_CMD_CHANNELS) {		/* channel 9 last out of RTE1 */
		cmd.frame_count += 1;				/* increment frame counter */
		if(cmdApp.function_flag == FUNCTION_ON) {
			(*funptr_cmd_application)();	/* call command decoder code */
			}
	}

	if(cmdApp.irq_function_flag == FUNCTION_ON) {
		(*funptr_cmd_irq)();			/* call command decoder code */
		}

	outp(ICU_BASE,CMD_EOI);					/* specific EOI command */
 }											/* return from interrupt */

 #pragma page(1)

/*@H*************************************************************************
 *
 *      telemetry link interrupt service routine
 *
 *		DELCARATION:	void interrupt far rte2s_int_handler(unsigned int es, unsigned int ds,
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

 void interrupt far rte2s_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
 #ifndef I486
	//_enable();									/* enable interrupts */
 #endif

	tm.addr = ((inp( tm_port.addr_port) >> 4) & 0x0f);	/* input address and mask high nibble */
//
//	tm.addr= 0;
//	tm.data[0] = tm.data[TM_FRAME_COUNT];
//

	if( tm.addr == 0 ) {
		tm.frame_count++;
		if(tmApp.frame_count_flag == FRAME_COUNT_ON) {
			tm.data[TM_FRAME_COUNT]++;		/* increment frame counter */
			}
		if(tmApp.function_flag == FUNCTION_ON) {
			(*funptr_tm_application)();		/* call telemetry encoder code */
			}
	}

	outp( tm_port.data_port, tm.data[tm.addr] ); /* output data from output array */

	if(tmApp.irq_function_flag == FUNCTION_ON) {
		(*funptr_tm_irq)();			/* call telemetry encoder code */
		}

	outp(ICU_BASE,TM_EOI);					/* specific EOI command */
 }											/* return from interrupt */

 #pragma page(1)

/*@H*************************************************************************
 *
 *	REMTRON interface function
 *
 *	get command address
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 int cmd_address_dual(void)
 {
 int nReturn;

 #ifndef I486

	outp( cmd_port.cntrl, CMD_ENABLE );			/* enable decoder output */

 #endif

	nReturn = inp( cmd_port.addr_port ) & 0x0f;	/* input address and mask high nibble */

	return(nReturn);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	REMTRON interface function
 *
 *	get command data
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 int cmd_data_dual(void)
 {
 int nReturn;

	nReturn = inp( cmd_port.data_port );	/* input data into input array */

	return(nReturn);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	REMTRON interface function
 *
 *	get command address and data
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 int cmd_addr_data_dual(struct CMD_TM *cmd)
 {
 int nReturn;

	cmd->addr = inp( cmd_port.addr_port ) & 0x0f;
	nReturn = cmd->data[cmd->addr] = inp( cmd_port.data_port );

	return(nReturn);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	REMTRON interface function
 *
 *	get telemetry address
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 int tm_address_dual(void)
 {
 int nReturn;

	nReturn = (inp( tm_port.addr_port) & 0x0f);	/* input address and mask high nibble */

	return(nReturn);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	REMTRON interface function
 *
 *	put telemetry data
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 int tm_data_dual(int data)
 {
 int nReturn;

	nReturn = outp( tm_port.data_port, data ); /* output data from output array */

	return(nReturn);
 }

/*@H*************************************************************************
 *
 *	REMTRON interface function
 *
 *	put telemetry data with address
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/
 int tm_addr_data_dual(struct CMD_TM *tm)
 {
 int nReturn;

	tm->addr = (inp( tm_port.addr_port) & 0x0f);	/* input address and mask high nibble */

	if( tm->addr == 0)
		{
 #if defined(TM_FRAME_COUNT)

		tm->data[TM_FRAME_COUNT]++;			/* increment frame counter */
 #else
		tm->frame_count++;
 #endif
		}
	outp( tm_port.data_port, tm->data[tm->addr] ); /* output data from output array */

	return(nReturn);
}

/****************************************************************************
 *
 * interrupt functions
 *
 ***************************************************************************/
 void enable_cmd_irq_function(void)
 {
	cmdApp.irq_function_flag = FUNCTION_ON;
 }

 void disable_cmd_irq_function(void)
 {
	cmdApp.irq_function_flag = FUNCTION_OFF;
 }

 void enable_tm_irq_function(void)
 {
	tmApp.irq_function_flag = FUNCTION_ON;
 }

 void disable_tm_irq_function(void)
 {
	tmApp.irq_function_flag = FUNCTION_OFF;
 }

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_cmd_application(void)
 {
	cmdApp.function_flag = FUNCTION_ON;
 }

 void disable_cmd_application(void)
 {
	cmdApp.function_flag = FUNCTION_OFF;
 }

 void enable_tm_application(void)
 {
	tmApp.function_flag = FUNCTION_ON;
 }

 void disable_tm_application(void)
 {
	tmApp.function_flag = FUNCTION_OFF;
 }

/****************************************************************************
 *
 * frame count functions
 *
 ***************************************************************************/

 void enable_cmd_frame_count(void)
 {
	cmdApp.frame_count_flag = FRAME_COUNT_ON;
 }

 void disable_cmd_frame_count(void)
 {
	cmdApp.frame_count_flag = FRAME_COUNT_OFF;
 }

 void enable_tm_frame_count(void)
 {
	tmApp.frame_count_flag = FRAME_COUNT_ON;
 }

 void disable_tm_frame_count(void)
 {
	tmApp.frame_count_flag = FRAME_COUNT_OFF;
 }

/****************************************************************************
 *
 * install irq functions
 *
 ***************************************************************************/

 void install_cmd_irq(void (far *routine)(void))
 {
	int_disable(CMD_IRQ);				/* disable timer/counter interrupts */

	//cmdApp.funptr_cmd_irq = routine;	/* install pointer to application */
	funptr_cmd_irq = routine;			/* install pointer to application */

	int_enable(CMD_IRQ);				/* enable timer/counter interrutps */
 }

 void far dummy_cmd_irq(void)
 {

 }

 void install_tm_irq(void (far *routine)(void))
 {
	int_disable(TM_IRQ);			/* disable timer/counter interrupts */

	//tmApp.funptr_tm_irq = routine;	/* install pointer to application */
	funptr_tm_irq = routine;		/* install pointer to application */

	int_enable(TM_IRQ);				/* enable timer/counter interrutps */
 }

 void far dummy_tm_irq(void)
 {

 }
