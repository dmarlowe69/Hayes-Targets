/* file: checksum.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("checksum Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *  checksum functions
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *  This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *	MASM Version 6.1
 *	Datalight C_Thru_ROM Version 2.00c
 *
 *		This Software is dedicated to the memory of
 *
 *			NYSSA ANN MARLOWE
 *		    17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include   "protocal.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void makechecksum(void);
 void Xmakechecksum(void);
 unsigned char checksum(unsigned char *p,int len);

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem checksum
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void makechecksum(void)
 {
 int len;

	len= *(int *)(&pack_buffer[1]);

	pack_buffer[3+len]=checksum(&pack_buffer[3],len);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem checksum
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void Xmakechecksum(void)
 {
 int len;

	len= *(int *)(&Xpack_buffer[1]);

	Xpack_buffer[3+len]=checksum(&Xpack_buffer[3],len);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem checksum
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 unsigned char checksum(unsigned char *p,int len)
 {
 unsigned char sum=0;

	while(len--)
		sum+= *p++;

	return sum;
 }
