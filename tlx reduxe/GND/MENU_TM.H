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
