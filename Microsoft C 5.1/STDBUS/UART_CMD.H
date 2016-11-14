/* file: uart_cmd.h */

/***************************************************************************
 *
 *	Uart CMD header file
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		   Engineer 	   Description
 *
 *	02/06/90	   D.marlowe	   Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	defintions
 *
 ***************************************************************************/

 #define ENABLE 	0
 #define DISABLE	1
 #define USE		2
 #define DISUSE 	3
 #define NODATA 	4

/****************************************************************************
 *
 *	Ground function prototypes
 *
 ***************************************************************************/

 void uart_cmd_initialize(void);
 void uart_cmd_de_initialize(void);
 int uart_cmd(void);
 int uart_cmd_port(void);

 void uart_port(void);
 void uart_menu(void);
