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
