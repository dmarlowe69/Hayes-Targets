// file: rom.h

//////////////////////////////////////////////////////////////////////////////
//
//      ROMable functions header file
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      ROMable function prototypes
//

 extern void (*printfptr)(char *string);				/* function pointer */
 void printf_init(void (*ptr)(char *string));

 int sprintf(char *string,char const *format, ...); /* same as microsoft function */
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

//
//      ROMable function prototypes - Code in file ST.ASM (startup code)
//

 void exit(int exit_status);                        /* same as microsoft function */
