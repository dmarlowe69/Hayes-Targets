// file: ground.c
 #define PC_CONSOLE
//////////////////////////////////////////////////////////////////////////////
//
//      Ground test functions
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include Microsoft definitions
//

 #include	<stdlib.h>
 #include	<dos.h>
 #include	<conio.h>
 #include   "dspio.h"
 #include   "mds_ex.h"

//
//      general defintions
//

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

//
//  function defintions
//

 #ifdef PC_CONSOLE

 #pragma message("Using PC STDIN and STDOUT for console I/O")

 #include	<conio.h>
 #include	<stdio.h>

 #define com_check()    _kbhit()
 #define com_putc(x)    _putch(x)
 #define com_puts(x)    _cputs(x)
 #define com_getc() 	getch()
 #define com_gets(x)	gets(x)

 #else

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
//  function pointers
//

 void (*com_open_ptr)(unsigned baud_rate);  /* initialize UART */
 void (*com_close_ptr)(void);           /* close out uart */
 int (*com_check_ptr)(void);			/* check for character */
 int (*com_getc_ptr)(void);			/* get charcter */
 int (*com_poll_ptr)(void);			/* wait for character */
 void (*com_flush_ptr)(void);			/* flush characters from receive buffer */
 void (*com_putc_ptr)(int ch);          /* output character to UART */
 void (*com_gets_ptr)(char *str);       /* input string from receive buffer */
 void (*com_puts_ptr)(char *str);		/* output string to UART */

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

//////////////////////////////////////////////////////////////////////////////
//
//      Ground Comm Port Initialization
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_gnd(int port)
 {
 #pragma message("Using DSPIO comm_ports 0 or 1")

	if(port == 0) {
		com_open_ptr = dspio_1_open;
		com_close_ptr = dspio_1_close;
		com_check_ptr = dspio_1_check;
		com_getc_ptr = dspio_1_getc;
		com_poll_ptr = dspio_1_poll;
		com_flush_ptr = dspio_1_flush;
		com_putc_ptr = dspio_1_putc;
		com_gets_ptr = dspio_1_gets;
		com_puts_ptr = dspio_1_puts;
        dspio_printf_init(dspio_1_puts);
        }
	else
		{
		com_open_ptr = dspio_2_open;
		com_close_ptr = dspio_2_close;
		com_check_ptr = dspio_2_check;
		com_getc_ptr = dspio_2_getc;
		com_poll_ptr = dspio_2_poll;
		com_flush_ptr = dspio_2_flush;
		com_putc_ptr = dspio_2_putc;
		com_gets_ptr = dspio_2_gets;
		com_puts_ptr = dspio_2_puts;
        dspio_printf_init(dspio_2_puts);
        }
 }
//////////////////////////////////////////////////////////////////////////////
//
//  	Ground functions menu
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 int gnd_menu(void)
 {
	int c;
    int exit_flag = FALSE;

	while(TRUE)
	{
        com_puts("\n\nGROUND FUNCTION MENU\r\n");

        com_puts("0. SET PRESET ALTITUDE\r\n");
        com_puts("1. RCVY/FLY Mode Toggle\r\n");
        com_puts("2. \r\n");
        com_puts("3. \r\n");
        com_puts("4. \r\n");
        com_puts("5. \r\n");
        com_puts("5. \r\n");
        com_puts("6. \r\n");
        com_puts("7. \r\n");
        com_puts("8. \r\n");
        com_puts("9. Return\r\n");
        com_puts("X. Return and Exit\r\n");

        com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

		switch(toupper(c))
		{

            case '0':
                    com_puts("\r\nEnter Preset Altitude (FT): ");
                    scanf( "%f", &CMDPRESET );
                    printf("\nPreset = %f FT\n",CMDPRESET);
                    pause();
                    break;
            case '1':
                    com_puts("\nRecovery/Fly Mode\r\n");
                    if(RCVY_FLY == FLY_MODE)
                    {
                    	RCVY_FLY = RCVY_MODE;
                    	printf("RCVY_MODE\n");
                    }
                	else
                    {
                        RCVY_FLY = FLY_MODE;
                    	printf("FLY_MODE\n");
                    }
                    pause();
                    break;
            case '2':
                    break;
			case '3':
                    break;
			case '4':
                    break;
            case '5':
                    break;
            case '6':
                    break;
            case '7':
                    break;
            case '8':
                    break;
            case '9':
                    exit_flag = FALSE;
                    com_puts("\r\n    Exiting Ground Test Menu \r\n");
                    return(exit_flag);
                    break;
            case 'X':
                    exit_flag = TRUE;
                    com_puts("\r\n    Exiting Ground Test Menu and Autopilot\r\n");
                    return(exit_flag);
                    break;
            default:
					break;
		}
	}
 }

//
//      General Serial Utilites
//

//////////////////////////////////////////////////////////////////////////////
//
//  binary display routine
//
//  This function prints an 8 bit ascii binary representation of it's
//  argument to the serial port.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void int_to_binary(int num,char *str)
 {
	int x;
	int mask;

	mask = 0x80;
	for(x = 0; x <=7; x++)
		{
		if(num & mask)
			*str++ = '1';
		else
			*str++ = '0';
		mask = mask >> 1;
		}

	*str = '\0';
 }
//////////////////////////////////////////////////////////////////////////////
//
//  Binary display routine
//
//  This function prints an 12 bit ascii binary representation of it's
//  argument to the serial port.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void int_to_binary12(int num,char *str)
 {
	int x;
	int mask;

	mask = 0x800;
	for(x = 0; x <=11; x++)
		{
		if(num & mask)
			*str++ = '1';
		else
			*str++ = '0';
		mask = mask >> 1;
		}

	*str = '\0';
 }
//////////////////////////////////////////////////////////////////////////////
//
//  Binary display routine
//
//  This function prints an 16 bit ascii binary representation of it's
//  argument to the serial port.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void int_to_binary16(int num,char *str)
 {
	int x;
	int mask;

    mask = 0x8000;
    for(x = 0; x <=15; x++)
		{
		if(num & mask)
			*str++ = '1';
		else
			*str++ = '0';
		mask = mask >> 1;
		}

	*str = '\0';
 }
//////////////////////////////////////////////////////////////////////////////
//
//  New Line routine
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void new_line(void)
 {
    com_puts("\r\n");
 }
//////////////////////////////////////////////////////////////////////////////
//
//  PAUSE routine
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void pause(void)
 {
	char c;

    com_puts("\r\npress any key");

	c = com_getc();	/* get a character from the serial port */

	new_line();
 }
