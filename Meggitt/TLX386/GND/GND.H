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
/* file: menu_ana.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      ANALOG serial interface function prototypes
 *
 ***************************************************************************/

 void analog_menu(void);        /* analog functions menu */
 void display_AD(void);         /* display A/D input data */
 void verify_AD(void);          /* verify A/D operation */
 void calibrate_AD(void);       /* calibrate A/D */
 void display_DA(void);         /* display D/A output data */
 void calibrate_DA(void);	/* calibrate D/A */

 void a_d_config_display(void); /* display A/D data structure */
 void d_a_config_display(void); /* display D/A data structure */
/* file: menu_gnd.h */

/****************************************************************************
 *
 *      BIRD Ground test functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/


/****************************************************************************
 *
 *      Ground function prototypes
 *
 ***************************************************************************/

 void gnd_menu(void);		/* UART command processor */
/* file: menu_pio.h */

 void pio_menu(void);
/* file: menu_rlc.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	RLC serial interface function prototypes
 *
 ***************************************************************************/

 void rlc_menu(void);
 void rlc_status(void);
 void rlc_test(void);
 void rlc_report(int status);
 void rlc_config_display(void);
 void rlc_sequence_test(void);
/* file: menu_rst.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      hardware reset functions
 *
 ***************************************************************************/

 void reset_menu(void);                 /* reset functions */
/* file: menu_rtc.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      RTC serial interface function prototypes
 *
 ***************************************************************************/

 void rtc_menu(void);           /* RTC functions menu */
 void set_time(void);           /* set time */
 void set_date(void);           /* set data */
 void display_clock(void);      /* display clock */
/* file: menu_sbx.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	LPM-SBX function prototypes
 *
 ***************************************************************************/

 void lpm_sbx_menu(void);
 void loop_LPM_SBX(void);
 void lpm_sbx_config_display(void);
/* file: menu_sta.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	STATUS serial interface function prototypes
 *
 ***************************************************************************/

 void status_menu(void);
 void status_status(void);
 void status_test(void);
 void led_report(int status);
 void status_config_display(void);
 void status_sequence_test(void);
/* file: menu_tm.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      CMD function pointer to command decoder function
 *
 ***************************************************************************/

 extern void (far *funptr_decoder_application)(void);	/* pointer to function */

/****************************************************************************
 *
 *      1553 serial interface function prototypes
 *
 ***************************************************************************/

 void cmd_tm_menu(void);		/* CMD/TM functions menu */
 void display_CMD(void);        /* display CMD data */
 void calibrate_CMD(void);      /* calibrate CMD data */
 void display_decode(void);     /* display and decode CMD data */
 void display_TM(void);         /* display TM data */
 void calibrate_TM(void);       /* calibrate TM data */

 void install_decoder_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_decoder_application(void);
							/* dummy main timing loop application */
/* file: menu_utl.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      General Serial utilites function prototypes
 *
 ***************************************************************************/

 void int_to_binary(int num,char *str); 	/* 8 bit integer to binary */
 void int_to_binary12(int num,char *str);	/* 12 bit integer to binary */
 void new_line(void);				/* CRLF */
 void pause(void);				/* press any key */
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
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

/****************************************************************************
 *
 *      ROMable function prototypes - Code in file ST.ASM (startup code)
 *
 ***************************************************************************/

 void exit(int exit_status);                        /* same as microsoft function */
/* file: speaker.c */

 void sound_main(void);
 void sound_beeps(int beeps);
 void sound_beep(void);
 void init_sound(unsigned frequency);
 void adjust_sound(unsigned frequency);
 void adjust_sound_union(unsigned frequency);
 void sound_on(void);
 void sound_off(void);
 void sound_delay(unsigned ms_count);
 //void timed_delay(unsigned count);
 int timed_delay(unsigned count);
 unsigned get_count(void);
 void counter_init(void);

/****************************************************************************
 *
 *	sound globals
 *
 ***************************************************************************/

 extern char song[];
 extern char *songpointer;
 extern int octave;
 extern int duration;
 extern int pitch;
 extern int songindex;

 extern void (interrupt far *funptr_sound)(void);	/* pointer to function */

 void main_playsong(void);
 void playtune(char *song);
 void kill_playtune(void);
 void interrupt far beNoisy(void);
 void DoSomething(void);

 void beNoisy_fun(void);

/****************************************************************************
 *
 *	sound functions
 *
 ***************************************************************************/

 void main_laser(void);
 void laser(void);

 void main_siren(void);
 void siren(void);

 void main_music(void);
 void tone(int frequency);

 void main_hearing(void);
 void hear_tone(int frequency);
/* file: terminal.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *       COMM Port test function prototypes
 *
 ***************************************************************************/

 void term(void);			/* terminal loop-back test */
 int com_break(void);			/* test comm ports for access */
