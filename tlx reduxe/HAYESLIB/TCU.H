/* tcu.h */

/****************************************************************************
 *
 *      BIRD Timer/Counter Unit (TCU) header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      TCU function prototypes
 *
 ***************************************************************************/

 void init_timer(unsigned int baud_rate);       /* setup main timing loop interrupt */
 void uart_baud(unsigned int baud_rate);        /* setup baud rate for UART */
 void scu_baud(unsigned int baud_rate);         /* setup baud rate for serial control unit (SCU) */
 void install_application(void (far *routine)());
						/* install pointer to main timing loop application */
 void far dummy_application(void);              /* dummy main timing loop application */

/****************************************************************************
 *
 *      TCU data structure
 *
 ***************************************************************************/

 struct TCU_TABLE {
	unsigned int ticks;             /* timing loop counter */
	};

 extern struct TCU_TABLE tcu_table; /* define data structre */

