/* file: menu_utl.h */

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
 *      General Serial utilites function prototypes
 *
 ***************************************************************************/

 void int_to_binary(int num,char *str); 	/* 8 bit integer to binary */
 void int_to_binary12(int num,char *str);	/* 12 bit integer to binary */
 void new_line(void);				/* CRLF */
 void pause(void);				/* press any key */
