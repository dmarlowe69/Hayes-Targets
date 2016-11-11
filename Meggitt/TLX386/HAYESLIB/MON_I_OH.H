/* file: mon_i_oh.h */

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

 void mon_outpw(unsigned port,unsigned value);
 void mon_outp(unsigned port,unsigned value);
 unsigned mon_inpw(unsigned port);
 unsigned mon_inp(unsigned port);
