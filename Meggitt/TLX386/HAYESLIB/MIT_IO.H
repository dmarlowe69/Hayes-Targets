/* file: mit_io.h */

/****************************************************************************
 *
 *	MIT/LL I/O	functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_MIT_IO

 #define _INC_MIT_IO
 #pragma message("_INC_MIT_IO")

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

 extern volatile struct MIT_TM mit_tm;

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

 extern volatile struct MIT_PORT mit_port;

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

 extern volatile struct MIT_APP mitApp;

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

 extern void (interrupt far *mit_int_funptr)(void);
											/* pointer to interrupt function */

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_mit_application(void);
 void disable_mit_application(void);

 void install_mit_application(void (far *routine)(void));
 void far dummy_mit_application(void);
 void echo_to_mit(void);
 void enable_mit(void);			/* enable TM system */
 void disable_mit(void);		/* disable TM system */

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

 extern struct PPI mit_ppi[2];

 void mit_icu(void);

 #pragma message("_INC_MIT_IO")
 #endif /* ifndef _INC_MIT_IO */
