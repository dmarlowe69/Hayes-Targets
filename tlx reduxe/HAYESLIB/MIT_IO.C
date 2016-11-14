/* file: mit_io.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("MIT/LL Telemetry Link functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *	MIT/LL I/O	functions
 *
 *	written by denton marlowe
 *	(c)1990-1994 Hayes Targets
 *
 ************************************************************************@H*/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/94	D.marlowe	Original release
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

 //#define _DEBUG_MIT
 #ifdef  _DEBUG_MIT
 #pragma message("WARNING : debug Flag set")
 #endif

 #ifdef HAYESLIB

 #pragma message("includes for hayeslib")

 #include	"config.h"			/* configuration information */
 #include	"hardware.h"		/* utility routines */
 #include	"icu.h"				/* interrupt controller definitions */
 //#define ICU_MASTER_BASE		0x20
 #define ICU_MASTER_BASE		ICU_BASE

 #include	"8255.h"			/* interface hardware definitions */
 #include	"ppi.h"				/* interface hardware definitions */
 #include	"lpm_sbx.h"			/* interface hardware definitions */
 #include	"lbx_pio.h"			/* interface hardware definitions */
 #include	"remtron.h"			/* interface hardware definitions */
 #include	"cmd_tmio.h"		/* interface protocal definitions */

 #else

 #pragma message("includes for stand alone")

 #include	<hayes.h>
 //#define ICU_MASTER_BASE		0x20
 #define ICU_MASTER_BASE		ICU_BASE

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

/****************************************************************************
 *
 *		MIT TM Interrupt
 *
 ***************************************************************************/

 #define MIT_IRQ	14				/* MIT tied to ICU interrupt number 14 */
									/* MIT tied to ICU interrupt number 6 of slave ICU */
 #define MIT_EOI (EOI | (MIT_IRQ&0x07))

/****************************************************************************
 *
 *	single pio mode definitions
 *
 ***************************************************************************/

 #define ID_LPM_SBX_PIO_J1_J1	10 // first slot j1
 #define ID_LPM_SBX_PIO_J1_J2	11 // first slot j2
 #define ID_LPM_SBX_PIO_J2_J1	12 // second slot j1
 #define ID_LPM_SBX_PIO_J2_J2	13 // second slot j2

/****************************************************************************
 *
 *	definitions for MIT/LL data link interface
 *
 ***************************************************************************/

 #define MIT_ENABLE		0x00
 #define MIT_DISABLE	0x01

 #define MIT_DATA_READY 	0x09	/* port c, bit 4 on */
 #define MIT_DATA_NOT_READY 0x08	/* port c, bit 4 off */

/****************************************************************************
 *
 *	data structures for MIT/LL data link
 *
 ***************************************************************************/

 #define NUM_MIT_CHANNELS	  16

 #define MIT_FRAME_COUNT	  15

 struct MIT_TM {
	unsigned int frame_count;
	unsigned char addr;
	unsigned int data[NUM_MIT_CHANNELS];
	};

 volatile struct MIT_TM mit_tm;

/****************************************************************************
 *
 *	data structures for MIT/LL data link
 *
 ***************************************************************************/

 struct MIT_PORT {
	unsigned int cntrl;
	unsigned int addr_port;
	unsigned int ldata_port;
	unsigned int hdata_port;
	unsigned int aux_port;
	unsigned int mode;
	unsigned int single_dual;
	};

 volatile struct MIT_PORT mit_port = {
	0,	// cntrl
	0,	// addr_port
	0,	// data_port
	0,	// aux_port
	0,	// mode
	0}; // single_dual

/****************************************************************************
 *
 *	MIT Application function prototypes
 *
 ***************************************************************************/

 #define FUNCTION_OFF		0
 #define FUNCTION_ON		1

 #define FRAME_COUNT_OFF	0
 #define FRAME_COUNT_ON		1

 struct MIT_APP {
	int frame_count_flag;					// frame counting enable switch
	int function_flag;						// function enable switch
	int irq_function_flag;					// function enable switch
	void (far *funptr_application)(void);	// pointer to function
	void (far *funptr_irq)(void);			// pointer to function
	};

 volatile struct MIT_APP mitApp = {
	FRAME_COUNT_OFF,
	FUNCTION_OFF,
	FUNCTION_OFF,
	NULL,
	NULL};

/****************************************************************************
 *
 *	MIT function prototypes
 *
 ***************************************************************************/

 void open_mit_port(int port_id);

 void init_mit(void);

 void init_mit_msc(void);			/* initialize MIT interface hardware */
 void deinit_mit_msc(void);			/* deinitialize MIT interface hardware */

/****************************************************************************
 *
 *	MIT/LL function prototype for interrupt service routines
 *
 ***************************************************************************/

 void interrupt far mit_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
											/* MIT interrupt service routine */

 void (interrupt far *mit_int_funptr)(void);
											/* pointer to interrupt function */

 void mit_delay(void);						/* delay function */

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_mit_application(void);
 void disable_mit_application(void);

/****************************************************************************
 *
 * interrupt functions
 *
 ***************************************************************************/

 void enable_mit_irq_function(void);
 void disable_mit_irq_function(void);

 void install_mit_irq(void (far *routine)(void));
 void far dummy_mit_irq(void);

 void (far *funptr_mit_irq)(void);	/* pointer to function */

/****************************************************************************
 *
 * frame count functions
 *
 ***************************************************************************/

 void enable_mit_frame_count(void);
 void disable_mit_frame_count(void);

/****************************************************************************
 *
 *	MIT function to telemetry encoder function
 *
 ***************************************************************************/

 void install_mit_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_mit_application(void);				/* dummy main timing loop application */

/****************************************************************************
 *
 *	MIT function pointer to telemetry encoder function
 *
 ***************************************************************************/

 void (far *funptr_mit_application)(void);	/* pointer to function */

 void (__interrupt __far *mit_vector)();		/* pointer to current IRQ handler */

 void (__interrupt __far *mit_old_vector)();	/* pointer to old IRQ handler */

/****************************************************************************
 *
 *	runtime debugger
 *
 ***************************************************************************/

 void echo_to_mit(void);
 void enable_mit(void);			/* enable TM system */
 void disable_mit(void);		/* disable TM system */

 void load_mit_tm_addr(int addr,unsigned int data);
 void load_mit_tm_array(unsigned int data[]);

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 struct PPI mit_ppi[2];

 #pragma page(1)

/****************************************************************************
 *
 *
 * ********** I N I T I A L I Z A T I O N **********
 *
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *		MIT initialization routine
 *
 *		written by denton marlowe
 *		(c)1990-1994 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void open_mit_port(int port_id)
 {

//
// setup port
//
	switch(port_id) {

		case ID_LPM_SBX_PIO_J1_J1:
			mit_port.cntrl	   = mcs1_0_control;
			mit_port.addr_port	= mcs1_0_portc;
			mit_port.ldata_port	= mcs1_0_porta;
			mit_port.hdata_port	= mcs1_0_portb;
			mit_port.aux_port	= mcs1_0_portc;

			init_mit_msc();

			init_mit();
			break;

		case ID_LPM_SBX_PIO_J1_J2:
			mit_port.cntrl	   = mcs0_0_control;
			mit_port.addr_port	= mcs0_0_portc;
			mit_port.ldata_port	= mcs0_0_porta;
			mit_port.hdata_port	= mcs0_0_portb;
			mit_port.aux_port	= mcs0_0_portc;

			init_mit_msc();

			init_mit();
			break;

		case ID_LPM_SBX_PIO_J2_J1:
			mit_port.cntrl	   = mcs1_1_control;
			mit_port.addr_port	= mcs1_1_portc;
			mit_port.ldata_port	= mcs1_1_porta;
			mit_port.hdata_port	= mcs1_1_portb;
			mit_port.aux_port	= mcs1_1_portc;

			init_mit_msc();

			init_mit();
			break;

		case ID_LPM_SBX_PIO_J2_J2:
			mit_port.cntrl	   = mcs0_1_control;
			mit_port.addr_port	= mcs0_1_portc;
			mit_port.ldata_port	= mcs0_1_porta;
			mit_port.hdata_port	= mcs0_1_portb;
			mit_port.aux_port	= mcs0_1_portc;

			init_mit_msc();

			init_mit();
			break;

		default:								// ID_LPM_SBX_PIO_J1_J1
			mit_port.cntrl	   = mcs1_0_control;
			mit_port.addr_port	= mcs1_0_portc;
			mit_port.ldata_port	= mcs1_0_porta;
			mit_port.hdata_port	= mcs1_0_portb;
			mit_port.aux_port	= mcs1_0_portc;

			init_mit_msc();

			init_mit();
			break;

		}

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		MIT/LL interface Hardware initialization
 *
 *		written by denton marlowe
 *		(c)1990-1994 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_mit_msc(void)
 {
/****************************************************************************
 *
 *		program 8255 ppi number 2 on iSBX board for MIT interface
 *
 *		port a output for low byte of data word
 *		port b output for high byte of data word
 *		port c split
 *		lower  input mit address input
 *		higher output handshake controls
 *
 ***************************************************************************/

	mit_port.mode = ppi_mode_0_1;
	outp( mit_port.cntrl, ppi_mode_0_1 );	/* mode 0 control word */

	outp( mit_port.ldata_port, 0x00 );
	outp( mit_port.hdata_port, 0x00 );

	outp( mit_port.cntrl, MIT_DATA_NOT_READY );
 }

 #pragma page(1)

 void deinit_mit_msc(void)
 {
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		MIT initialization routine
 *
 *		written by denton marlowe
 *		(c)1990-1994 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_mit(void)
 {
 int i;
/****************************************************************************
 *
 *	initialize command and telemetry data
 *
 ***************************************************************************/

	for(i=0;i<NUM_MIT_CHANNELS;i++) {
		mit_tm.data[i] = i;
		}

	mit_tm.frame_count = 0;
	mit_tm.data[MIT_FRAME_COUNT] = 0;

/****************************************************************************
 *
 *	disable interrupts
 *
 ***************************************************************************/

	_disable();

/****************************************************************************
 *
 *	Install interrupt service routines for MIT
 *
 ***************************************************************************/

 #ifdef PC

	mit_old_vector = _dos_getvect(MIT_VEC);

	_dos_setvect(MIT_VEC,mit_int_handler);

 #else

	install_vector(mit_int_handler,MIT_IRQ);

 #endif

/****************************************************************************
 *
 *	Install telemetry encoder function pointer
 *
 ***************************************************************************/

	install_mit_application(dummy_mit_application);

/****************************************************************************
 *
 *	Install telemetry encoder irq function pointer
 *
 ***************************************************************************/

	install_mit_irq(dummy_mit_irq);

/****************************************************************************
 *
 *	get irq function pointer for chain interrupt
 *
 ***************************************************************************/

 //#ifdef _DEBUG_MIT
 //
	//mit_int_funptr = get_vector(TM_IRQ);	/* set pointer to function */
 //
 //#endif

/****************************************************************************
 *
 *	unmask interrupt at interrupt controller
 *
 ***************************************************************************/

	int_enable(ICU_IRQ);
	int_enable(MIT_IRQ);

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
 *      telemetry link interrupt service routine
 *
 *		written by denton marlowe
 *		(c)1990-1994 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void interrupt far mit_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {

 #ifdef _DEBUG_MIT
	// use ret2 tm interrupt
	mit_tm.addr = ((inp( tm_port.addr_port) >> 4) & 0x0f);

 #else

	mit_tm.addr = (inp( mit_port.addr_port) & 0x0f);	/* input address and mask high nibble */

 #endif

	if( mit_tm.addr == 0 ) {
		mit_tm.frame_count++;
		if(mitApp.frame_count_flag == FRAME_COUNT_ON) {
			mit_tm.data[MIT_FRAME_COUNT]++;		/* increment frame counter */
			}
		if(mitApp.function_flag == FUNCTION_ON) {
			(*funptr_mit_application)();		/* call mit encoder code */
			}
	}

	outp( mit_port.ldata_port, (mit_tm.data[mit_tm.addr]&0xff) );
										/* output data from output array */

	outp( mit_port.hdata_port, ((mit_tm.data[mit_tm.addr]>>8)&0xf) );
										/* output data from output array */

	mit_delay();

	outp( mit_port.cntrl, MIT_DATA_READY );

	if(mitApp.irq_function_flag == FUNCTION_ON) {
		(*funptr_mit_irq)();			/* call mit encoder code */
		}

	outp( mit_port.cntrl, MIT_DATA_NOT_READY );

	outp(ICU_SLAVE_BASE,MIT_EOI);			/* specific EOI command */

	outp(ICU_MASTER_BASE,ICU_EOI);			/* specific EOI command */

 }											/* return from interrupt */

 void mit_delay(void)
 {
 int i;

	i=12;

	while(i--);
 }

 #pragma page(1)

/****************************************************************************
 *
 * interrupt functions
 *
 ***************************************************************************/

 void enable_mit_irq_function(void)
 {
	mitApp.irq_function_flag = FUNCTION_ON;
 }

 void disable_mit_irq_function(void)
 {
	mitApp.irq_function_flag = FUNCTION_OFF;
 }

 #pragma page(1)

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_mit_application(void)
 {
	mitApp.function_flag = FUNCTION_ON;
 }

 void disable_mit_application(void)
 {
	mitApp.function_flag = FUNCTION_OFF;
 }

 #pragma page(1)

/****************************************************************************
 *
 * frame count functions
 *
 ***************************************************************************/

 void enable_mit_frame_count(void)
 {
	mitApp.frame_count_flag = FRAME_COUNT_ON;
 }

 void disable_mit_frame_count(void)
 {
	mitApp.frame_count_flag = FRAME_COUNT_OFF;
 }

 #pragma page(1)

/****************************************************************************
 *
 * install irq functions
 *
 ***************************************************************************/

 void install_mit_irq(void (far *routine)(void))
 {
	int_disable(MIT_IRQ);			/* disable timer/counter interrupts */

	//mitApp.funptr_mit_irq = routine;	/* install pointer to application */
	funptr_mit_irq = routine;		/* install pointer to application */

	int_enable(MIT_IRQ);				/* enable timer/counter interrutps */
 }

 void far dummy_mit_irq(void)
 {

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		MIT/LL install data encoder function
 *
 *		written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void install_mit_application(void (far *routine)(void))
 {
	int_disable(MIT_IRQ);					 /* disable timer/counter interrupts */

	funptr_mit_application = routine;		 /* install pointer to application */

	int_enable(MIT_IRQ);					 /* enable timer/counter interrutps */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      Dummy Application function
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void far dummy_mit_application(void)
 {

 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		MIT/LL echo command to telemetry function
 *
 *		written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void echo_to_mit(void)
 {
	disable_cmd();
	disable_mit();

	mit_tm.data[0] = cmd.data[1];		/* address	1 out of telemetry decoder */
	mit_tm.data[1] = cmd.data[2];		 /* address  2 out of telemetry decoder */
	mit_tm.data[2] = cmd.data[3];		 /* address  3 out of telemetry decoder */
	mit_tm.data[3] = cmd.data[4];		 /* address  4 out of telemetry decoder */
	mit_tm.data[4] = cmd.data[5];		 /* address  5 out of telemetry decoder */
	mit_tm.data[5] = cmd.data[6];		 /* address  6 out of telemetry decoder */
	mit_tm.data[6] = cmd.data[7];		 /* address  7 out of telemetry decoder */
	mit_tm.data[7] = cmd.data[8];		 /* address  8 out of telemetry decoder */

	mit_tm.data[8] = cmd.data[9];		/* address	9 out of telemetry decoder */

	mit_tm.data[9] = cmd.data[10];		/* address 10 out of telemetry decoder */
	mit_tm.data[10] = cmd.data[11]; 	/* address 11 out of telemetry decoder */
	mit_tm.data[11] = cmd.data[12]; 	/* address 12 out of telemetry decoder */
	mit_tm.data[12] = cmd.data[13]; 	/* address 13 out of telemetry decoder */
	mit_tm.data[13] = cmd.data[14]; 	/* address 14 out of telemetry decoder */
	mit_tm.data[14] = cmd.data[15]; 	/* address 15 out of telemetry decoder */

 #if !defined(MIT_FRAME_COUNT)

	mit_tm.data[15] = cmd.data[0];		/* address	0 out of telemetry decoder */

 #endif

	enable_mit();
	enable_cmd();
 }

 #pragma page(1)

 void enable_mit(void)
 {
	int_enable(MIT_IRQ);
	int_enable(ICU_IRQ);
 }

 #pragma page(1)

 void disable_mit(void)
 {
	int_disable(MIT_IRQ);
 }

 void i_o_delay(void);
 #define ISR_SELECT 0x02
 #define IRR_SELECT 0x03
 void mit_icu(void)
 {
 #ifdef _IRR
 int irq_mask;

	//
	// master
	//
	outp(ICU_BASE,IRR_SELECT);	/* set read IRR Register */

	i_o_delay();

	irq_mask = inp(ICU_BASE);

	printf(" Master_irq_mask %x\r\n",irq_mask);

	//
	// slave
	//
	outp(ICU_SLAVE_BASE,IRR_SELECT);	/* set read IRR Register */

	i_o_delay();

	irq_mask = inp(ICU_SLAVE_BASE);

	printf(" Slave_irq_mask %x\r\n",irq_mask);
 #else
 int irq_mask;
	irq_mask = inp(0x21);
	printf(" Master_irq_mask %x\r\n",irq_mask);
	irq_mask = inp(0xA1);
	printf(" Slave_irq_mask %x\r\n",irq_mask);
 #endif
 }

 void i_o_delay(void)
 {
	int i;

	i=0;

	i++;
 }

/****************************************************************************
 *
 * application interface routines
 *
 ***************************************************************************/

 //
 // MIT/LL Telemetry data loader
 //
 void load_mit_tm_addr(int addr,unsigned int data)
 {
	mit_tm.data[addr] = data;
 }

 //
 // MIT/LL Telemetry data loader
 //
 void load_mit_tm_array(unsigned int data[])
 {
 int i;

	for(i=0;i<NUM_MIT_CHANNELS;i++) {
		mit_tm.data[i] = data[i];
		}
 }
