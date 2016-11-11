/* file: mon_i_or.c */

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

 void mon_out(void);
 void mon_in(void);

 #pragma page(1)

/***************************************************************************
 *
 *	REMOTE I/O EMBEDDED FUNCTIONS
 *
 ***************************************************************************/

 #pragma page(1)

/***************************************************************************
 *
 *  Name: mon_out()
 *  Description: write to a port (BYTE or WORD)
 *
 ***************************************************************************/
 void mon_out(void)
 {
 unsigned port;
 unsigned value;

    port= *(unsigned *)(&pack_buffer[3]);
	value= *(unsigned *)(&pack_buffer[6]);

	if(pack_buffer[5])		/* word */
		outpw(port,value);
	else
		outp(port,value);

	XBuffCmd='O';
	*(int *)(&Xpack_buffer[1])=0;

    send();
 }

 #pragma page(1)

/***************************************************************************
 *
 *  Name: mon_in()
 *  Description: read form a port (BYTE or WORD)
 *
 ***************************************************************************/
 void mon_in(void)
 {
 unsigned port;
 unsigned value=0;

	port=*(unsigned *)(&pack_buffer[3]);

	if(pack_buffer[5])		/* word */
		value=inpw(port);
	else
		value=inp(port);

	XBuffCmd='I';
	*(int *)(&Xpack_buffer[1])=2;
	*(unsigned *)(&Xpack_buffer[3])=value;

	send();
 }
