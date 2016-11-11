/* file: ppi.h */

/***************************************************************************
 *
 *      PPI header file
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
 *	define addresses for LPM-SBC40-8 ppi
 *
 ***************************************************************************/

 #if !defined(V50)

 #define ppi_porta PPI_BASE      /* address of port a */
 #define ppi_portb PPI_BASE+1    /* address of port b */
 #define ppi_portc PPI_BASE+2    /* address of port c */
 #define ppi_control PPI_BASE+3  /* address of control register */

 #else

 #define ppi_porta PPI_BASE      /* address of port a */
 #define ppi_portb PPI_BASE+2    /* address of port b */
 #define ppi_portc PPI_BASE+4    /* address of port c */
 #define ppi_control PPI_BASE+6  /* address of control register */

 #endif

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI ppi;

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_ppi(void);           /* initialize ppi on LPM-SBC40 card */
 int toggle_bit(int bit);       /* toggle PPI port c bits */
 int test_bit(int bit);         /* test PPI port c bit */
 int set_bit(int bit);          /* set PPI port c bit */
 int reset_bit(int bit);        /* reset PPI port c bit */

 int read_porta(void);          /* read byte from port A */
 int read_portb(void);          /* read byte from port A */
 int read_portc(void);          /* read byte from port A */

 int write_porta(int byte);     /* write byte from port A */
 int write_portb(int byte);     /* write byte from port A */
 int write_portc(int byte);     /* write byte from port A */

 int toggle_bit_port(int bit, int port);/* toggle PPI port bit */
 int test_bit_port(int bit, int port);  /* test PPI port bit */
 int set_bit_port(int bit, int port);   /* set PPI port bit */
 int reset_bit_port(int bit, int port); /* reset PPI port bit */
