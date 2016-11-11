/* file: status.h */

/***************************************************************************
 *
 *      STATUS header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *  STATUS data structure
 *
 ***************************************************************************/

 #ifndef STATUS_STRUCT

 struct STATUS_LED_SW {
	unsigned int status_addr;	/* base I/O address of PPI */
	unsigned int led_addr[4];	/* array of LED bank addresses */
	unsigned int sw_addr[4];	/* array of switch bank addresses */
	unsigned int led[4];		/* status led banks */
	unsigned int sw[4];		/* status switch bank */
	unsigned int byte;		/* I/O byte */
    };

 #define STATUS_STRUCT

 #endif

 extern struct STATUS_LED_SW status_led_sw;
					/* define structure storage */

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define LED_SET  1
 #define LED_RESET    0
 #define SWITCH_SET  1
 #define SWITCH_RESET    0

 #define LED_ERROR    -1
 #define SWITCH_ERROR    -1

 #define NUM_BANKS 4

/****************************************************************************
 *
 *  STATUS initialization function prototypes
 *
 ***************************************************************************/

 void init_status(void);                /* initialize STATUS card */
 int toggle_LED(int bank, int LED);     /* toggle STATUS LED */
 int test_LED(int bank, int LED);       /* return state of LED  */
 int set_LED(int bank, int LED);        /* set STATUS LED */
 int reset_LED(int bank, int LED);      /* reset STATUS LED */

 int read_switch(int bank, int SW);     /* read status of switch */
 int set_LED_switch(int bank, int SW);  /* set led to state of switch */

 int write_LED(int bank, int LED);	/* update bank of LEDs */
 int read_LED(int bank);		/* read bank of LEDs */

 int read_SW(int bank); 		/* read bank of Switches */
