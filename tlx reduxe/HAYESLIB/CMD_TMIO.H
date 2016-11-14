/* file: cmd_tmio.h */

/****************************************************************************
 *
 *	REMTRON CMD/TM functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_CMD_TMIO

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

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

 #define ID_PCM_PIO 		5
 #define ID_PCM_PIO_J1		6
 #define ID_PCM_PIO_J2		7

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

 extern volatile struct CMD_TM_PORT cmd_port;
 extern volatile struct CMD_TM_PORT tm_port;

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


 extern volatile struct CMD_TM_APP cmdApp;
 extern volatile struct CMD_TM_APP tmApp;

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

 extern void (far *funptr_cmd_irq)(void);	/* pointer to function */

 extern void (far *funptr_tm_irq)(void);	/* pointer to function */

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

 extern struct PPI cmd_tm_ppi[2];

 #define _INC_CMD_TMIO
 #endif /* ifndef _INC_CMD_TMIO */
