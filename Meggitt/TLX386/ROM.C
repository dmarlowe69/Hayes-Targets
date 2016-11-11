// file: rom.c

//////////////////////////////////////////////////////////////////////////////
//
//      ROMable Highlevel Serial Output functions
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include Microsoft definitions
//

 #include       <stdio.h>
 #include       <stdarg.h>

//
//      ROMable function prototypes
//

 void (*printfptr)(char *string);					/* function pointer */
 void printf_init(void (*ptr)(char *string));

 int sprintf(char *string,char const *format, ...); /* same as microsoft function */
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

 //int _flsbuf(int i_dummy, FILE * f_dummy);		 /* same as microsoft function */

//
//      ROMable function prototypes - Code in file ST.ASM (startup code)
//

 //void exit(int exit_status);						 /* same as microsoft function */

//
//      ROMable Printf definitions
//

 #define MAXSTR 256               /* maximum string length used by printf */

 extern int _cfltcvt_tab[];       /* Jmp table required by Printf */

//////////////////////////////////////////////////////////////////////////////
//
//      ROMable sprintf - string format routine
//
//      DELCARATION:    int sprintf(char *string,char const *format, ...)
//
//      PARAMETERS:     pointer to character buffer
//                      character string and/or formatting specifications
//
//      RETURNS:        Length of output string
//                      formated string stored in character buffer
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    Formated string stored in buffer
//
//      LOCALS:         Char string[MAXSTR]
//                      va_list arglist
//                      int retval
//                      FILE str
//
//      EXTERNALS
//       Referenced:    int outfile(...)
//
//      INCLUDES:       <stdio.h>
//                      <stdarg.h>
//
//      WARNINGS:       None
//
//      SOURCE:         rom.c
//
//      DESCRIPTION:
//
//      Use sprintf to format character strings and values of C variables,
//      in a specified manner, and store in a buffer. See the description
//      of the microsoft library function sprintf for further details.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
//
//      ROMable printf - formated string output routine
//
//      DELCARATION:    int com_printf(char const *format, ...)
//
//      PARAMETERS:     character string and/or formatting specifications
//
//      RETURNS:        Length of output string
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    Formated string output to UART
//
//      LOCALS:         Char string[MAXSTR]
//                      va_list arglist
//                      int retval
//                      FILE str
//
//      EXTERNALS
//       Referenced:    int outfile(...)
//                      uart_puts(char *string) or scu_puts(char *string)
//
//      INCLUDES:       <stdio.h>
//                      <stdarg.h>
//
//      WARNINGS:       None
//
//      SOURCE:         rom.c
//
//      DESCRIPTION:
//
//      Use printf to write character strings and values of C variables,
//      formated in a specified manner, to the UART. See the description
//      of the microsoft library function printf for further details.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
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

	dspio_1_puts(string);

	return(retval);
 }

 void printf_init(void (*ptr)(char *string))
 {
	printfptr = ptr;
 }
//////////////////////////////////////////////////////////////////////////////
//
//      ROMable printf - formated string output routine
//
//      DELCARATION:    int printf(char const//format, ...)
//
//      PARAMETERS:     character string and/or formatting specifications
//
//      RETURNS:        Length of output string
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    Formated string output to UART
//
//      LOCALS:         Char string[MAXSTR]
//                      va_list arglist
//                      int retval
//                      FILE str
//
//      EXTERNALS
//       Referenced:    int outfile(...)
//                      uart_puts(char *string) or scu_puts(char *string)
//
//      INCLUDES:       <stdio.h>
//                      <stdarg.h>
//
//      WARNINGS:       None
//
//      SOURCE:         rom.c
//
//      DESCRIPTION:
//
//      Use printf to write character strings and values of C variables,
//      formated in a specified manner, to the UART. See the description
//      of the microsoft library function printf for further details.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
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

		dspio_1_puts(string);

	return(retval);
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Error Trap - Flush Output Buffers and EXIT
//
//      DELCARATION:    int _flsbuf(int i_dumm, FILE// f_dummy)
//
//      PARAMETERS:     None
//
//      RETURNS:        Nothing
//
//      ENTRY
//       Conditions:    error during call to _output(...)
//
//      EXIT
//       Conditions:    Error Trap - exits to the EXIT(...) function
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    void exit(int exit_status)
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         rom.c
//
//      DESCRIPTION:
//
//      This fufills a the _flsbuf() call from _output(...) and is used
//      as a 'placeholder' for the standard library routine. This technique
//      is neccessary to prevent the inclusion of the default startup code
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 //int _flsbuf(int i_dummy, FILE * f_dummy)
 //{
 //	exit(10);						/* call exit routine */
 //}
