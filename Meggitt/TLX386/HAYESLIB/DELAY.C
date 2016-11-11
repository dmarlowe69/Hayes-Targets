/* file: delay.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("utility Functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *	utility functions
 *
 *	written by denton marlowe
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
 *	Include Microsoft definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	utility function prototypes
 *
 ***************************************************************************/

 void delay(unsigned count);    /* delay for count , 32000 = 0.25 seconds */

 void multi_delay(unsigned count);

/****************************************************************************
 *
 *	end of hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      delay function
 *
 *      DELCARATION:    void delay(unsigned count)
 *
 *      PARAMETERS:     count down value
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
 *      SOURCE:         delay.c
 *
 *      DESCRIPTION:
 *
 *      This routine is a general purpose delay loop
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void delay(unsigned count)
 {
	while(--count)
		;               /* delay loop 0.025 milli-seconds/count */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	multi-delay function
 *
 ***************************************************************************/
 void multi_delay(unsigned count)
 {
	while(--count) {
						   /* delay loop 0.025 milli-seconds/count */
		delay(32000);
		delay(32000);
		delay(32000);
		delay(32000);

		}

 }
