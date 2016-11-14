/* file: rom.h */

/***************************************************************************
 *
 *      ROMable functions header file
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
 *      ROMable function prototypes
 *
 ***************************************************************************/


 extern void (*printfptr)(char *string);				/* function pointer */
 void printf_init(void (*ptr)(char *string));

 int sprintf(char *string,char const *format, ...); /* same as microsoft function */
 int uart_printf(char const *,...);                 /* same as microsoft function */
 int scu_printf(char const *,...);                  /* same as microsoft function */
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

/****************************************************************************
 *
 *      ROMable function prototypes - Code in file ST.ASM (startup code)
 *
 ***************************************************************************/

 void exit(int exit_status);                        /* same as microsoft function */
