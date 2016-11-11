/* file: remtron.h */

/****************************************************************************
 *
 *	REMTRON CMD/TM functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_REMTRON

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/***************************************************************************
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

/***************************************************************************
 *
 *	define variables for command link interrupt service routine
 *
 ***************************************************************************/

 extern volatile struct CMD_TM cmd;

/***************************************************************************
 *
 *	define variables for telemetry link interrupt service routine
 *
 ***************************************************************************/

 extern volatile struct CMD_TM tm;

/****************************************************************************
 *
 *	CMD/TM function prototypes
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

 void install_cmd_application(void (far *routine)());
						    /* install pointer to main timing loop application */
 void far dummy_cmd_application(void);              /* dummy main timing loop application */

 void install_tm_application(void (far *routine)());
						    /* install pointer to main timing loop application */
 void far dummy_tm_application(void);			 /* dummy main timing loop application */

/****************************************************************************
 *
 *	TCU function prototype for interrupt service routine
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
 *	CMD function pointer to command decoder function
 *
 ***************************************************************************/

 extern void (far *funptr_cmd_application)(void);	/* pointer to function */

 extern void (far *funptr_tm_application)(void);	/* pointer to function */

 extern void (__interrupt __far *cmd_vector)();		/* pointer to current IRQ handler */

 extern void (__interrupt __far *tm_vector)();		/* pointer to current IRQ handler */

 extern void (__interrupt __far *cmd_old_vector)();	/* pointer to old IRQ handler */

 extern void (__interrupt __far *tm_old_vector)();	/* pointer to old IRQ handler */

 #define _INC_REMTRON
 #endif /* ifndef _INC_REMTRON */
