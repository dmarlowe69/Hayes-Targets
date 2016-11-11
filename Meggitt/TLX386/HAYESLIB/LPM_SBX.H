/* file: lpm_sbx.h */

/***************************************************************************
 *
 *      LBX-PIO PPI header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      define base addresses of LPM-SBX-1 devices
 *
 ***************************************************************************/

 #define SBX_0_MCS0 LPM_SBX_BASE
 #define SBX_0_MCS1 (LPM_SBX_BASE+0x08)

 #define SBX_1_MCS0 (LPM_SBX_BASE+0x10)
 #define SBX_1_MCS1 (LPM_SBX_BASE+0x18)

 #define LBX_ICU_BASE (LPM_SBX_BASE+0x20)

/****************************************************************************
 *
 *      define addresses for iSBX #1 ppi at J2
 *
 ***************************************************************************/

 #define mcs0_0_porta SBX_0_MCS0     /* address of port a */
 #define mcs0_0_portb SBX_0_MCS0+1   /* address of port b */
 #define mcs0_0_portc SBX_0_MCS0+2   /* address of port c */
 #define mcs0_0_control SBX_0_MCS0+3 /* address of control register */

/****************************************************************************
 *
 *      define addresses for iSBX #1 ppi at J1
 *
 ***************************************************************************/

 #define mcs1_0_porta SBX_0_MCS1     /* address of port a */
 #define mcs1_0_portb SBX_0_MCS1+1   /* address of port b */
 #define mcs1_0_portc SBX_0_MCS1+2   /* address of port c */
 #define mcs1_0_control SBX_0_MCS1+3 /* address of control register */

/****************************************************************************
 *
 *      define addresses for iSBX #2 ppi at J2
 *
 ***************************************************************************/

 #define mcs0_1_porta SBX_1_MCS0     /* address of port a */
 #define mcs0_1_portb SBX_1_MCS0+1   /* address of port b */
 #define mcs0_1_portc SBX_1_MCS0+2   /* address of port c */
 #define mcs0_1_control SBX_1_MCS0+3 /* address of control register */

/****************************************************************************
 *
 *      define addresses for iSBX #2 ppi at J1
 *
 ***************************************************************************/

 #define mcs1_1_porta SBX_1_MCS1     /* address of port a */
 #define mcs1_1_portb SBX_1_MCS1+1   /* address of port b */
 #define mcs1_1_portc SBX_1_MCS1+2   /* address of port c */
 #define mcs1_1_control SBX_1_MCS1+3 /* address of control register */

/****************************************************************************
 *
 *      assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI lpm_sbx[4];

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_lpm_sbx(int port_0, int port_1, int port_2, int port_3);
						/* initialize ppi on LBX-PIO CARD */
 int toggle_bit_lpm_sbx(int bit, int port); 	/* toggle PPI port c bits */
 int test_bit_lpm_sbx(int bit, int port);       /* test PPI port c bit */
 int set_bit_lpm_sbx(int bit, int port);        /* set PPI port c bit */
 int reset_bit_lpm_sbx(int bit, int port);      /* reset PPI port c bit */

 int read_porta_lpm_sbx(int port);              /* read byte from port A */
 int read_portb_lpm_sbx(int port);              /* read byte from port A */
 int read_portc_lpm_sbx(int port);              /* read byte from port A */

 int write_porta_lpm_sbx(int byte, int port);   /* write byte from port A */
 int write_portb_lpm_sbx(int byte, int port);   /* write byte from port A */
 int write_portc_lpm_sbx(int byte, int port);   /* write byte from port A */
