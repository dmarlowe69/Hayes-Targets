/* file: rom.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("ROMable Highlevel Serial Output functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      ROMable Highlevel Serial Output functions
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

 #include       <stdio.h>
 #include       <stdarg.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include       "config.h"
 #include       "scu.h"
 #include       "uart.h"
 #include		"dspio.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      ROMable function prototypes
 *
 ***************************************************************************/

 void (*printfptr)(char *string);					/* function pointer */
 void printf_init(void (*ptr)(char *string));

 int sprintf(char *string,char const *format, ...); /* same as microsoft function */
 int uart_printf(char const *,...);                 /* same as microsoft function */
 int scu_printf(char const *,...);                  /* same as microsoft function */
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

 // moved to startup code
 //int _flsbuf(int i_dummy, FILE * f_dummy);		 /* same as microsoft function */

/****************************************************************************
 *
 *      ROMable function prototypes - Code in file ST.ASM (startup code)
 *
 ***************************************************************************/

 void exit(int exit_status);                        /* same as microsoft function */

/****************************************************************************
 *
 *      ROMable Printf definitions
 *
 ***************************************************************************/

 #define MAXSTR 256               /* maximum string length used by printf */

 extern int _cfltcvt_tab[];       /* Jmp table required by Printf */

/****************************************************************************
 *
 *      end of ROMable Printf definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      ROMable sprintf - string format routine
 *
 *      DELCARATION:    int sprintf(char *string,char const *format, ...)
 *
 *      PARAMETERS:     pointer to character buffer
 *                      character string and/or formatting specifications
 *
 *      RETURNS:        Length of output string
 *                      formated string stored in character buffer
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Formated string stored in buffer
 *
 *      LOCALS:         Char string[MAXSTR]
 *                      va_list arglist
 *                      int retval
 *                      FILE str
 *
 *      EXTERNALS
 *       Referenced:    int outfile(...)
 *
 *      INCLUDES:       <stdio.h>
 *                      <stdarg.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *      Use sprintf to format character strings and values of C variables,
 *      in a specified manner, and store in a buffer. See the description
 *      of the microsoft library function sprintf for further details.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int sprintf(char *string,char const *format, ...)
 {
	va_list arglist;
	int retval;
	FILE str;

	FILE *outfile = &str;

	va_start(arglist, format);

	outfile->_flag = _IOWRT|_IOSTRG;
	outfile->_ptr = outfile->_base = string;
	outfile->_cnt = MAXSTR;

	retval = _output(outfile,format,arglist);
	string[retval] = '\0';

	return(retval);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      ROMable printf - formated string output routine
 *
 *      DELCARATION:    int uart_printf(char const *format, ...)
 *
 *      PARAMETERS:     character string and/or formatting specifications
 *
 *      RETURNS:        Length of output string
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Formated string output to UART
 *
 *      LOCALS:         Char string[MAXSTR]
 *                      va_list arglist
 *                      int retval
 *                      FILE str
 *
 *      EXTERNALS
 *       Referenced:    int outfile(...)
 *                      uart_puts(char *string)
 *
 *      INCLUDES:       <stdio.h>
 *                      <stdarg.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *      Use printf to write character strings and values of C variables,
 *      formated in a specified manner, to the UART. See the description
 *      of the microsoft library function printf for further details.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if defined(V40)

 int uart_printf(char const *format, ...)
 {
	char string[MAXSTR];
	va_list arglist;
	int retval;
	FILE str;

	FILE *outfile = &str;

	va_start(arglist, format);

	outfile->_flag = _IOWRT|_IOSTRG;
	outfile->_ptr = outfile->_base = string;
	outfile->_cnt = MAXSTR;

	retval = _output(outfile,format,arglist);

	string[retval] = '\0';

	uart_puts(string);

	return(retval);
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      ROMable printf - formated string output routine
 *
 *      DELCARATION:    int scu_printf(char const *format, ...)
 *
 *      PARAMETERS:     character string and/or formatting specifications
 *
 *      RETURNS:        Length of output string
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Formated string output to SCU
 *
 *      LOCALS:         Char string[MAXSTR]
 *                      va_list arglist
 *                      int retval
 *                      FILE str
 *
 *      EXTERNALS
 *       Referenced:    int outfile(...)
 *                      scu_puts(char *string)
 *
 *      INCLUDES:       <stdio.h>
 *                      <stdarg.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *      Use printf to write character strings and values of C variables,
 *      formated in a specified manner, to the SCU. See the description
 *      of the microsoft library function printf for further details.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #ifndef I486

 int scu_printf(char const *format, ...)
 {
	char string[MAXSTR];
	va_list arglist;
	int retval;
	FILE str;

	FILE *outfile = &str;

	va_start(arglist, format);

	outfile->_flag = _IOWRT|_IOSTRG;
	outfile->_ptr = outfile->_base = string;
	outfile->_cnt = MAXSTR;

	retval = _output(outfile,format,arglist);

	string[retval] = '\0';

	scu_puts(string);

	return(retval);
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      ROMable printf - formated string output routine
 *
 *      DELCARATION:    int com_printf(char const *format, ...)
 *
 *      PARAMETERS:     character string and/or formatting specifications
 *
 *      RETURNS:        Length of output string
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Formated string output to UART
 *
 *      LOCALS:         Char string[MAXSTR]
 *                      va_list arglist
 *                      int retval
 *                      FILE str
 *
 *      EXTERNALS
 *       Referenced:    int outfile(...)
 *                      uart_puts(char *string) or scu_puts(char *string)
 *
 *      INCLUDES:       <stdio.h>
 *                      <stdarg.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *      Use printf to write character strings and values of C variables,
 *      formated in a specified manner, to the UART. See the description
 *      of the microsoft library function printf for further details.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int com_printf(char const *format, ...)
 {
	char string[MAXSTR];
	va_list arglist;
	int retval;
	FILE str;

	FILE *outfile = &str;

	va_start(arglist, format);

	outfile->_flag = _IOWRT|_IOSTRG;
	outfile->_ptr = outfile->_base = string;
	outfile->_cnt = MAXSTR;

	retval = _output(outfile,format,arglist);

	string[retval] = '\0';

 #if defined(V40)

	uart_puts(string);

 #elif defined(V50)

	scu_puts(string);

 #elif defined(V20)

	scu_puts(string);

 #elif defined(I486)

	dspio_1_puts(string);

 #endif

	return(retval);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *		ROMable printf - initialization routine
 *
 *		DELCARATION:	void printf_init(void (*ptr)(char *string))
 *
 *		PARAMETERS: 	pointer to string output function
 *
 *		RETURNS:		None
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *		 Conditions:	printf output function pointer set
 *
 *		LOCALS: 		None
 *
 *      EXTERNALS
 *		 Referenced:	void (*printfptr)(char *string)
 *
 *		INCLUDES:		None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *		In order to use printf the output function must be defined. This
 *		function sets the output function pointer.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void printf_init(void (*ptr)(char *string))
 {
	printfptr = ptr;
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      ROMable printf - formated string output routine
 *
 *		DELCARATION:	int printf(char const *format, ...)
 *
 *      PARAMETERS:     character string and/or formatting specifications
 *
 *      RETURNS:        Length of output string
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Formated string output to UART
 *
 *      LOCALS:         Char string[MAXSTR]
 *                      va_list arglist
 *                      int retval
 *                      FILE str
 *
 *      EXTERNALS
 *       Referenced:    int outfile(...)
 *                      uart_puts(char *string) or scu_puts(char *string)
 *
 *      INCLUDES:       <stdio.h>
 *                      <stdarg.h>
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *      Use printf to write character strings and values of C variables,
 *      formated in a specified manner, to the UART. See the description
 *      of the microsoft library function printf for further details.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int printf(char const *format, ...)
 {
	char string[MAXSTR];
	va_list arglist;
	int retval;
	FILE str;

	FILE *outfile = &str;

	va_start(arglist, format);

	outfile->_flag = _IOWRT|_IOSTRG;
	outfile->_ptr = outfile->_base = string;
	outfile->_cnt = MAXSTR;

	retval = _output(outfile,format,arglist);

	string[retval] = '\0';

	if(printfptr != NULL)
		(printfptr)(string);
	else

 #if defined(V40)

		uart_puts(string);

 #elif defined(V50)

		scu_puts(string);

 #elif defined(V20)

		scu_puts(string);

 #elif defined(I486)

		dspio_1_puts(string);

 #endif

	return(retval);
 }
 #pragma page(1)

/*@H*************************************************************************
 *
 *      Error Trap - Flush Output Buffers and EXIT
 *
 *      DELCARATION:    int _flsbuf(int i_dumm, FILE * f_dummy)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    error during call to _output(...)
 *
 *      EXIT
 *       Conditions:    Error Trap - exits to the EXIT(...) function
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void exit(int exit_status)
 *
 *      INCLUDES:       None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         rom.c
 *
 *      DESCRIPTION:
 *
 *      This fufills a the _flsbuf() call from _output(...) and is used
 *      as a 'placeholder' for the standard library routine. This technique
 *      is neccessary to prevent the inclusion of the default startup code
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 // moved to startup code
 //int _flsbuf(int i_dummy, FILE * f_dummy)
 //{
 //	exit(10);						/* call exit routine */
 //}
