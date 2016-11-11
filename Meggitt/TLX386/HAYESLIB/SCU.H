/* file: scu.h */

/***************************************************************************
 *
 *      SCU header file
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
 *      scu function prototypes
 *
 ***************************************************************************/

 void scu_open(unsigned baud_rate);    /* initialize SCU */

 #if defined(V20)

 unsigned int lookup_baud(unsigned baud);

 #endif

 void scu_close(void);				   /* close out SCU */
 int scu_check(void);                  /* check for character */
 int scu_getc(void);                   /* get charcter */
 int scu_poll(void);                   /* wait for character */
 void scu_flush(void);                 /* flush characters from receive buffer */
 void scu_putc(int ch);                /* output character to SCU */
 void scu_gets(char *str);             /* input string from receive buffer */
 void scu_puts(char *str);             /* output string to SCU */
