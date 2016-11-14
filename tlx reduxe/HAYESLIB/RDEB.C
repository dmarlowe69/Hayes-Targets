/* file: rdeb.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Remote Debugger interface functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      Serial Output routines for use with Remote Debugger
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

 #include       <dos.h>

/****************************************************************************
 *
 *      Include BIRD definitions
 *
 ***************************************************************************/

 #include		"hardware.h"

 #include		"rdeb.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Check Memory Model
 *
 ***************************************************************************/

 #if defined(M_I86SM)

 #pragma message("Small Memory Model")

 #elif defined(M_I86CM)

 #pragma message("Compact Memory Model")

 #elif defined(M_I86MM)

 #pragma message("Medium Memory Model")

 #elif defined(M_I86LM)

 #pragma message("Large Memory Model")

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      RDEB Output string
 *
 *      DELCARATION:    int rdeb_putstr(char *str)
 *
 *      PARAMETERS:     pointer to character string (NULL terminated)
 *
 *      RETURNS:        Lenght of given string
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         union REGS regs
 *                      struct SREGS sregs
 *
 *      EXTERNALS
 *       Referenced:    void segread(&sregs)
 *                      int int86(KERNEL_PUTS, &regs, &regs)
 *
 *      INCLUDES:       dos.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rdeb.c
 *
 *      DESCRIPTION:
 *
 *      This routine will output a Null terminated string to the RDEB
 *      console using a software interrrupt hook at the KERNEL_PUTS
 *      vector, which is set up by the kernel.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int rdeb_putstr(char *str)
 {
	union REGS regs;        /* processor general purpose registers */
	struct SREGS sregs;     /* processor segment registers */

	segread(&sregs);        /* return contents of processor's location registers */

	regs.x.cx=strlen(str);  /* stash lenght of string in CX register */
	if(regs.x.cx) {         /* if length is not zero */

		regs.x.si= FP_OFF(str); /* get offset address to string */
		regs.x.dx= sregs.ds;    /* default data segment */

 #ifndef M_I86SM                        /* if not small memory model */
 #ifndef M_I86MM                        /* if not medium memory model */

		regs.x.dx= FP_SEG(str); /* pointer has a segment part */

 #endif
 #endif

		int86(KERNEL_PUTS, &regs, &regs);       /* invoke software interrupt */
		}
	return regs.x.cx;               /* return length of string */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      RDEB Output string
 *
 *      DELCARATION:    void rdeb_puts(char *string)
 *
 *      PARAMETERS:     pointer to character string (NULL terminated)
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
 *       Referenced:    int rdeb_putstr(char *str)
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rdeb.c
 *
 *      DESCRIPTION:
 *
 *      This routine will output a Null terminated string to the RDEB
 *      console, and then output a newline character.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void rdeb_puts(char *string)
 {
	rdeb_putstr(string);    /* send string to rdeb */
	rdeb_putstr("\n");      /* send newline character */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      RDEB Output character
 *
 *      DELCARATION:    void rdeb_putch(char)
 *
 *      PARAMETERS:     single character
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         static charaacter array
 *
 *      EXTERNALS
 *       Referenced:    int rdeb_putstr(char *str)
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rdeb.c
 *
 *      DESCRIPTION:
 *
 *      This function outputs a single character the RDEB console
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void rdeb_putch(char c)
 {
	static char str[2];     /* static character array */

	str[0] = c;             /* put character in array */
	str[1] = '\0';          /* make sure null terminated */
	rdeb_putstr(str);       /* invoke KERNEL putstr */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      C_THRU_ROM Entry Procedure
 *
 *      DELCARATION:    void C_THRU_ROM(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        does not return
 *
 *      ENTRY
 *       Conditions:    Kernel Communications Port must be initialized
 *                      to desired Baud rate and handshake parameters
 *
 *      EXIT
 *       Conditions:    N/A
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    r_main
 *
 *      INCLUDES:       rdeb.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rdeb.c
 *
 *      DESCRIPTION:
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void C_THRU_ROM(void)
 {
	_disable(); 					/* disable interrutps at CPU

	int_disable_all();              /* disable interrutps at ICU */

	funptr_reset = (void (interrupt far *)(void)) 0xfff80000;
					/* install pointer to application */

	(*funptr_reset)();              /* call application code */
 }

 #pragma page(1)

 void CALIBRATE(void)
 {
	_disable(); 					/* disable interrutps at CPU

	int_disable_all();              /* disable interrutps at ICU */

	funptr_reset = (void (interrupt far *)(void)) 0xfffd0000;
					/* install pointer to application */

	(*funptr_reset)();              /* call application code */
 }
