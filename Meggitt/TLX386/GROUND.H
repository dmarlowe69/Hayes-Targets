// file: ground.h

//////////////////////////////////////////////////////////////////////////////
//
//      BIRD Ground test functions header file
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

 #ifndef _INC_GROUND

//
//  check build configuration
//

 #ifdef PC_CONSOLE

 #pragma message("Using PC STDIN and STDOUT for console I/O")

 #include	<conio.h>
 #include	<stdio.h>

 #define com_puts(x) _cputs(x)
 #define com_getc() getch()
 #define com_gets(x) gets(x)

 #else

 #define UART_PTR

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

 #endif

//
//      general defintions
//

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

//
//  function pointers
//

 extern void (*com_open_ptr)(unsigned baud_rate);   /* initialize UART */
 extern void (*com_close_ptr)(void);                /* close out uart */
 extern int (*com_check_ptr)(void);					/* check for character */
 extern int (*com_getc_ptr)(void);					/* get charcter */
 extern int (*com_poll_ptr)(void);					/* wait for character */
 extern void (*com_flush_ptr)(void);				/* flush characters from receive buffer */
 extern void (*com_putc_ptr)(int ch);               /* output character to UART */
 extern void (*com_gets_ptr)(char *str);            /* input string from receive buffer */
 extern void (*com_puts_ptr)(char *str);			/* output string to UART */

//
//      Ground function prototypes
//

 void init_gnd(int port);	/* setup required comm port */
 int gnd_menu(void);        /* UART command processor */

//
//  	General Serial utilites function prototypes
//

 void int_to_binary(int num,char *str); 	/* 8 bit integer to binary */
 void int_to_binary12(int num,char *str);	/* 12 bit integer to binary */
 void int_to_binary16(int num,char *str);   /* 16 bit integer to binary */
 void new_line(void);                       /* CRLF */
 void pause(void);                          /* press any key */

 #define _INC_GROUND
 #endif	/* _INC_GROUND */
