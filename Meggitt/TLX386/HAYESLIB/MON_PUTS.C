/* file: mon_puts.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("CMD/TM Test function")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *      BIRD BRAIN SERIAL COMMUNICATIONS
 *
 *      written by Denton Marlowe
 *      (c)1991 HAYES TARGETS
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

 #pragma page(1)

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<string.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include	"protocal.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void mon_puts(char *str);

 #pragma page(1)

/***************************************************************************
 *
 *	This special packet is sent to RDEB with a string to be
 *  displayed on the "target" screen. If string is longer than packet
 *  buffer, it will be broken into as many packets as required.
 *
 ***************************************************************************/
 void mon_puts(char *str)
 {
	int i,m,len;

	len = strlen(str)+1;	 /* include terminating Null in count */

	while(len>0) {
		XBuffCmd='P';
		m= min(128,len);
		XBuffLen= m;
            for(i=0;i<m;++i) {
				Xpack_buffer[3+i]=*str++;
				}
		send();
		len-=m;
		}
 }
