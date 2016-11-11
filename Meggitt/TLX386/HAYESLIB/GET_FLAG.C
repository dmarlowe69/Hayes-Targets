/* file: get_flag.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Processor Register functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/****************************************************************************
 *
 *	Processor Register functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

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

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	"config.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	define Processor Register Flags
 *
 ***************************************************************************/

 #define CF	0x0001		/* carry flag */
 #define PF	0x0004		/* parity flag */
 #define AF	0x0010		/* aux carry */
 #define ZF	0x0040		/* zero flag */
 #define SF	0x0080		/* sign flag */
 #define TF	0x0100		/* trap flag */

 #define IF	0x0200		/* Interrupt enable flag */

 #define DF	0x0400		/* direction flag */
 #define OF	0x0800		/* overflow flag */

/****************************************************************************
 *
 *	Processor Function Prototypes
 *
 ***************************************************************************/

 int check_IF(void);
 int _get_flags(void);

 #pragma page(1)

/****************************************************************************
 *
 *	check interrupt enable flag in processor status register
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 int check_IF(void)
 {
	if((_get_flags() & IF) == 0)
		return(0);		/* interrutps disabled */
	else
		return(1);		/* interrutps enabled */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	get processor status flags
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 int _get_flags(void)
 {
	_asm
	{
		PUSHF		; Push flags onto stack
		POP	AX	; Pop flags into AX
	}
				/* return flags in AX */
 }
