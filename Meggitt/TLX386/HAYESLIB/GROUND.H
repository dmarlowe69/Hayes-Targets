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
 *	remove defintions from Config.h header file
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

 #ifdef PC

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

 #define com_open(x) (*com_open_ptr)(x)
 #define com_close() (*com_close_ptr)()
 #define com_check() (*com_check_ptr)()
 #define com_getc() (*com_getc_ptr)()
 #define com_poll() (*com_poll_ptr)()
 #define com_flush() (*com_flush_ptr)()
 #define com_putc(x) (*com_putc_ptr)(x)
 #define com_gets(x) (*com_gets_ptr)(x)
 #define com_puts(x) (*com_puts_ptr)(x)

 #else

 #pragma message("Using Comm_port Functions")

 #define com_puts(x) scu_puts(x)
 #define com_getc()  scu_getc()
 #define com_gets(x) scu_gets(x)
 #define com_putc(x) scu_putc(x)
 #define com_check() scu_check()

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

 #ifndef PC

 extern void (*com_open_ptr)(unsigned baud_rate);	/* initialize UART */
 extern void (*com_close_ptr)(void);				/* close out uart */
 extern int (*com_check_ptr)(void);					/* check for character */
 extern int (*com_getc_ptr)(void);					/* get charcter */
 extern int (*com_poll_ptr)(void);					/* wait for character */
 extern void (*com_flush_ptr)(void);				/* flush characters from receive buffer */
 extern void (*com_putc_ptr)(int ch);				/* output character to UART */
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
