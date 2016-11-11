/* file: lbx_pio.h */

/***************************************************************************
 *
 *	LBX-PIO PPI header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	define addresses for iSBX ppi number 1
 *
 ***************************************************************************/

 #if !defined(V50)

 #define mcs0_porta MCS_0_BASE     /* address of port a */
 #define mcs0_portb MCS_0_BASE+1   /* address of port b */
 #define mcs0_portc MCS_0_BASE+2   /* address of port c */
 #define mcs0_control MCS_0_BASE+3 /* address of control register */

 #else

 #define mcs0_porta MCS_0_BASE     /* address of port a */
 #define mcs0_portb MCS_0_BASE+2   /* address of port b */
 #define mcs0_portc MCS_0_BASE+4   /* address of port c */
 #define mcs0_control MCS_0_BASE+6 /* address of control register */

 #endif

/****************************************************************************
 *
 *	define addresses for iSBX ppi number 0
 *
 ***************************************************************************/

 #if !defined(V50)

 #define mcs1_porta MCS_1_BASE     /* address of port a */
 #define mcs1_portb MCS_1_BASE+1   /* address of port b */
 #define mcs1_portc MCS_1_BASE+2   /* address of port c */
 #define mcs1_control MCS_1_BASE+3 /* address of control register */

 #else

 #define mcs1_porta MCS_1_BASE     /* address of port a */
 #define mcs1_portb MCS_1_BASE+2   /* address of port b */
 #define mcs1_portc MCS_1_BASE+4   /* address of port c */
 #define mcs1_control MCS_1_BASE+6 /* address of control register */

 #endif

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI sbx[2];

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_ppi_sbx(int port_0, int port_1);
					/* initialize ppi on LBX-PIO CARD */
 int toggle_bit_sbx(int bit, int port);	/* toggle PPI port c bits */
 int test_bit_sbx(int bit, int port);	/* test PPI port c bit */
 int set_bit_sbx(int bit, int port);	/* set PPI port c bit */
 int reset_bit_sbx(int bit, int port);	/* reset PPI port c bit */

 int read_porta_sbx(int port);			/* read byte from port A */
 int read_portb_sbx(int port);			/* read byte from port A */
 int read_portc_sbx(int port);			/* read byte from port A */

 int write_porta_sbx(int byte, int port);	/* write byte from port A */
 int write_portb_sbx(int byte, int port);	/* write byte from port A */
 int write_portc_sbx(int byte, int port);	/* write byte from port A */
