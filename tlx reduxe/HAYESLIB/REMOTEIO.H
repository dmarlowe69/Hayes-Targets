/* file: remoteio.h */

/***************************************************************************
 *
 *	Remote I/O header file
 *
 *	written by denton marlowe
 *	(c)1989,1993 Hayes Targets
 *
 **************************************************************************/

/***************************************************************************
 *
 *	Remote input and output routines
 *
 ***************************************************************************/

 #ifdef REMOTE_I_O

 void mon_outpw(unsigned port,unsigned value);
 void mon_outp(unsigned port,unsigned value);

 unsigned mon_inpw(unsigned port);
 unsigned mon_inp(unsigned port);

 #undef outp
 #undef outpw

 #undef inp
 #undef inpw

 #define outp(x,y)	mon_outp(x,y)
 #define outpw(x,y)	mon_outpw(x,y)

 #define inp(x) 	mon_inp(x)
 #define inpw(x)	mon_inpw(x)

 #pragma message("Using Remote inp and outp Routines")

 #endif
