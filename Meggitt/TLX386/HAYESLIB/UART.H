/* file: uart.h */

/***************************************************************************
 *
 *      UART header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

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
 *      UART function prototypes
 *
 ***************************************************************************/

 void uart_open(unsigned baud_rate);    /* initialize UART */
 void uart_close(void);                 /* close out uart */
 int uart_check(void);                  /* check for character */
 int uart_getc(void);                   /* get charcter */
 int uart_poll(void);                   /* wait for character */
 void uart_flush(void);                 /* flush characters from receive buffer */
 void uart_putc(int ch);                /* output character to UART */
 void uart_gets(char *str);             /* input string from receive buffer */
 void uart_puts(char *str);             /* output string to UART */

