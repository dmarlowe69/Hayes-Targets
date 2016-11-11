/* file: led.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Status LED Functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      Status LED functions
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
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

 #include       <conio.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include       "config.h"

 #if defined(V50)

 #include		"ppi.h"

 #endif

 #ifdef REMOTE_I_O

 #include		"remoteio.h"

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Status Led function prototypes
 *
 ***************************************************************************/

 void led_init(void);           /* initialize status LED on SBC40 */
 int led_toggle(void);          /* toggle status LED on SBC40 */

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define LED_ON  1
 #define LED_OFF 0

/****************************************************************************
 *
 *      end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      LED initialization function
 *
 *      DELCARATION:    void led_init(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
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
 *       Referenced:    None
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         led.c
 *
 *      DESCRIPTION:
 *
 *      This function is a dummy initialization routine
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void led_init(void)
 {

 #if defined(V50)

	init_ppi();			/* initialize ppi on LPM-SBC40/50 card */
	reset_bit(0);		/* reset PPI port c bit */

 #endif
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      LED toggle function
 *
 *      DELCARATION:    int led_toggle(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        state of LED after toggle
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    LED toggled on or off
 *
 *      LOCALS:         static int led_status
 *
 *      EXTERNALS
 *       Referenced:    int outp(unsigned portid, int value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         led.c
 *
 *      DESCRIPTION:
 *
 *      This function toggles the state of the on board status LED
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int led_toggle(void)
 {
 #if defined(V40)

 static char led_status = LED_OFF;				/* local variable */

	led_status = ~led_status & LED_ON;      /* invert and mask the on/off status */
	outp(LED_PORT,led_status);              /* output status to led */
	return(led_status); 					/* return state of LED */

 #endif


 #if defined(V50)

 static char led_status = LED_OFF;				/* local variable */

	led_status = ~led_status & LED_ON;      /* invert and mask the on/off status */
	toggle_bit(0x00);						/* toggle PPI port c bits */
	return(led_status); 					/* return state of LED */

 #endif

 }
