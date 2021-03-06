/* file: checksum.h */

/****************************************************************************
 *
 *	BIRD Communications functions header file
 *
 *	written by denton marlowe
 *	(c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		   Engineer 	   Description
 *
 *	02/06/90	   D.marlowe	   Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void makechecksum(void);
 void Xmakechecksum(void);
 unsigned char checksum(unsigned char *p,int len);
