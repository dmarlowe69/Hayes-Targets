/* file: asm.h */

/****************************************************************************
 *
 *      Assembly Language Function header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	Processor Register Functions
 *
 ***************************************************************************/

 int check_IF(void);
 int _get_flags(void);

 unsigned int _getDS(void);

 unsigned int _getCS(void);

/****************************************************************************
 *
 *      Memory function prototypes
 *
 ***************************************************************************/

 void _install(int vect,void (interrupt far *handler)());

 unsigned int _peek(unsigned int segment, int unsigned offset);

 void _poke(unsigned char value, unsigned int segment, unsigned int offset);

 int _ramtest(unsigned int segment, unsigned int offset_start, unsigned int offset_stop);
