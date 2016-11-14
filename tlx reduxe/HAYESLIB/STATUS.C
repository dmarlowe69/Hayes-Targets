/* file: status.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("STATUS Initialization functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *	STATUS Initialization functions
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *	MASM Version 6.0
 *	Datalight C_Thru_ROM Version 2.0c
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<conio.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"

 #ifdef REMOTE_I_O

 #include	"remoteio.h"

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

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

 struct STATUS_LED_SW status_led_sw;
					/* define structure storage */

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define LED_SET	1
 #define LED_RESET	0
 #define SWITCH_SET	1
 #define SWITCH_RESET	0

 #define LED_ERROR	-1
 #define SWITCH_ERROR	-1

 #define NUM_BANKS	4

/****************************************************************************
 *
 *	STATUS initialization function prototypes
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

/****************************************************************************
 *
 *      end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *	STATUS initialization
 *
 *	DELCARATION:	void init_status(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *	 Conditions:	STATUS hardware initialized
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid,int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *	Initialize STATUS card
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void init_status(void)
 {
	int i;

	status_led_sw.status_addr = STATUS_BASE_ADDRESS;

	status_led_sw.led_addr[0] = status_led_sw.status_addr;
	status_led_sw.led_addr[1] = status_led_sw.status_addr + 1;
	status_led_sw.led_addr[2] = status_led_sw.status_addr + 2;
	status_led_sw.led_addr[3] = status_led_sw.status_addr + 3;

	status_led_sw.sw_addr[0] = status_led_sw.status_addr + 4;
	status_led_sw.sw_addr[1] = status_led_sw.status_addr + 5;
	status_led_sw.sw_addr[2] = status_led_sw.status_addr + 6;
	status_led_sw.sw_addr[3] = status_led_sw.status_addr + 7;

	for(i=0;i<NUM_BANKS;i++) {
		outp(status_led_sw.led_addr[i],0);	/* turn off all LEDs */
		status_led_sw.led[i] = 0;		/* set status of LEDs */
		status_led_sw.sw[i] = inp(status_led_sw.sw_addr[i]);
							/* read status of switches */
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	toggle status LEDs
 *
 *	DELCARATION:	int toggle_LED(int bank, int LED)
 *
 *	PARAMETERS:	status LED bank number (0..3)
 *			status LED number (0..7)
 *
 *	RETURNS:    state of LED after toggle
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *	 Conditions:	status LED toggled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *                      int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE:     status.c
 *
 *      DESCRIPTION:
 *
 *	This function toggles the given LED of status
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int toggle_LED(int bank, int LED)
 {

	if((LED < 0) | (LED > 7))
		return(LED_ERROR);

	if((bank < 0) | (bank > 3))
		return(LED_ERROR);

	status_led_sw.byte = 0x01 << LED;

	if( (status_led_sw.led[bank] & status_led_sw.byte) ==0)
		{
		status_led_sw.led[bank] = status_led_sw.led[bank] | status_led_sw.byte;
		outp(status_led_sw.led_addr[bank],status_led_sw.led[bank]);
		return(LED_SET);
		}
	else
		{
		status_led_sw.led[bank] = status_led_sw.led[bank] & ~status_led_sw.byte;
		outp(status_led_sw.led_addr[bank],status_led_sw.led[bank]);
		return(LED_RESET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	test status LED
 *
 *	DELCARATION:	int test_LED(int bank, int LED)
 *
 *	PARAMETERS:	status LED bank (0..3)
 *			status LED number (0..7)
 *
 *	RETURNS:	state of LED
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int inp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *	This function returns the state of the given LED of status.
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int test_LED(int bank, int LED)
 {

	if((LED < 0) | (LED > 7))
		return(LED_ERROR);

	if((bank < 0) | (bank > 3))
		return(LED_ERROR);

	status_led_sw.byte = 0x01 << LED;

	if( (status_led_sw.led[bank] & status_led_sw.byte) ==0)
		{
		return(LED_RESET);
		}
	else
		{
		return(LED_SET);
		}
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	set status LED
 *
 *	DELCARATION:	int set_LED(int bank, int LED))
 *
 *	PARAMETERS:	status LED bank (0..3)
 *			status LED number (0..7)
 *
 *	RETURNS:	state of LED changed, otherwise LED_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *	This function sets the given LED of status
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int set_LED(int bank, int LED)
 {
	if((LED < 0) | (LED > 7))
		return(LED_ERROR);

	if((bank < 0) | (bank > 3))
		return(LED_ERROR);

	status_led_sw.byte = 0x01 << LED;

	status_led_sw.led[bank] = status_led_sw.led[bank] | status_led_sw.byte;
	outp(status_led_sw.led_addr[bank],status_led_sw.led[bank]);
	return(LED_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      reset status LED
 *
 *	DELCARATION:	int reset_LED(int bank, int LED)
 *
 *	PARAMETERS:	status LED bank (0..3)
 *			status LED number (0..7)
 *
 *	RETURNS:	state of LED changed, otherwise LED_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the given LED of status
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int reset_LED(int bank, int LED)
 {
	if((LED < 0) | (LED > 7))
		return(LED_ERROR);

	if((bank < 0) | (bank > 3))
		return(LED_ERROR);

	status_led_sw.byte = 0x01 << LED;

	status_led_sw.led[bank] = status_led_sw.led[bank] & ~status_led_sw.byte;
	outp(status_led_sw.led_addr[bank],status_led_sw.led[bank]);
	return(LED_RESET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	write status LED
 *
 *	DELCARATION:	int write_LED(int bank, int LED))
 *
 *	PARAMETERS:	status LED bank (0..3)
 *			status LED byte
 *
 *	RETURNS:	state of LEDs updated, otherwise LED_ERROR (-1)
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *	This function writes a byte to the given LED bank of status
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int write_LED(int bank, int LED)
 {
	if((LED < 0) | (LED > 255))
		return(LED_ERROR);

	if((bank < 0) | (bank > 3))
		return(LED_ERROR);

	status_led_sw.led[bank] = LED;
	outp(status_led_sw.led_addr[bank],LED);
	return(LED_SET);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read status LED
 *
 *	DELCARATION:	int read_LED(int bank)
 *
 *	PARAMETERS:	status LED bank (0..3)
 *
 *	RETURNS:	state of LED
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *	 Referenced:	None
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte from the given LED bank of status
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_LED(int bank)
 {
 int LED;

	if((bank < 0) | (bank > 3))
		return(LED_ERROR);

	LED = status_led_sw.led[bank];
	return(LED);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *	read status Switch
 *
 *	DELCARATION:	int read_SW(int bank)
 *
 *	PARAMETERS:	status Switch bank (0..3)
 *
 *	RETURNS:	state of Switches
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *	 Referenced:	int inp(unsigned portid)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *	SOURCE: 	status.c
 *
 *      DESCRIPTION:
 *
 *	This function reads a byte from the given Switch bank of status
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int read_SW(int bank)
 {
 int sw;

	if((bank < 0) | (bank > 3))
		return(SWITCH_ERROR);

	sw = inp(status_led_sw.sw_addr[bank]);
	status_led_sw.sw[bank] = sw;
	return(sw);
 }
