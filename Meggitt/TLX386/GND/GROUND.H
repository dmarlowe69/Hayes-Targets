/* file: ground.h */

/****************************************************************************
 *
 *      BIRD Ground test functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_GROUND

/****************************************************************************
 *
 *	remove defintions from Hayes.h
 *
 ***************************************************************************/

 #undef com_puts
 #undef com_getc
 #undef com_gets
 #undef com_putc
 #undef com_check

/****************************************************************************
 *
 *	check build configuration
 *
 ***************************************************************************/

 //#ifdef PC
 #ifdef PC_CONSOLE

 #pragma message("Using PC STDIN and STDOUT for console I/O")

 #include	<conio.h>
 #include	<stdio.h>

 #define com_puts(x) _cputs(x)
 #define com_getc() getch()
 #define com_gets(x) gets(x)

 #else

 #define UART_PTR

 #if defined(UART_PTR)

 #pragma message("Using Comm_port Function Pointers")

 #ifdef I486_8250
 #define com_open(x,y,z) (*com_open_ptr)(x,y,z)
 #else
 #define com_open(x) (*com_open_ptr)(x)
 #endif
 #define com_close() (*com_close_ptr)()
 #define com_check() (*com_check_ptr)()
 #define com_getc() (*com_getc_ptr)()
 #define com_poll() (*com_poll_ptr)()
 #define com_flush() (*com_flush_ptr)()
 #define com_putc(x) (*com_putc_ptr)(x)
 #define com_gets(x) (*com_gets_ptr)(x)
 #define com_puts(x) (*com_puts_ptr)(x)

 #endif

 #endif // ifdef PC

/***************************************************************************
 *
 *      general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

/****************************************************************************
 *
 *	function pointers
 *
 ***************************************************************************/

 //#ifndef PC
 #ifndef PC_CONSOLE

 #ifdef I486_8250
 extern void (*com_open_ptr)(unsigned baud_rate, unsigned parms,unsigned port);
 #else
 extern void (*com_open_ptr)(unsigned baud_rate);	/* initialize UART */
 #endif
 extern void (*com_close_ptr)(void);				/* close out uart */
 extern int (*com_check_ptr)(void);					/* check for character */
 extern int (*com_getc_ptr)(void);					/* get charcter */
 extern int (*com_poll_ptr)(void);					/* wait for character */
 extern void (*com_flush_ptr)(void);				/* flush characters from receive buffer */
 #ifdef I486_8250
 extern void (*com_putc_ptr)(char ch);				/* output character to UART */
 #else
 extern void (*com_putc_ptr)(int ch);				/* output character to UART */
 #endif
 extern void (*com_gets_ptr)(char *str);			/* input string from receive buffer */
 extern void (*com_puts_ptr)(char *str);			/* output string to UART */

 #endif

/****************************************************************************
 *
 *      Ground function prototypes
 *
 ***************************************************************************/

 #ifndef PC

 void init_gnd(int port);	/* setup required comm port */

 #endif

 #define _INC_GROUND
 #endif	/* _INC_GROUND */
