/* file: mon_i_oh.c */

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

 void mon_outpw(unsigned port,unsigned value);
 void mon_outp(unsigned port,unsigned value);
 unsigned mon_inpw(unsigned port);
 unsigned mon_inp(unsigned port);

 #pragma page(1)

/***************************************************************************
 *
 *	REMOTE I/O HOST	FUNCTIONS
 *
 ***************************************************************************/

 #pragma page(1)

/***************************************************************************
 *
 *  Name: mon_out()
 *	Description: write to a port (WORD)
 *
 ***************************************************************************/
 void mon_outpw(unsigned port,unsigned value)
 {

	XBuffCmd='O';
    XBuffLen= 5;
	*(unsigned *)(&Xpack_buffer[3]) = port;
	*(unsigned char *)(&Xpack_buffer[5]) = 1;
    *(unsigned *)(&Xpack_buffer[6]) = value;

	send(0);

	receive();
 }

 #pragma page(1)

/***************************************************************************
 *
 *  Name: mon_out()
 *	Description: write to a port (BYTE)
 *
 ***************************************************************************/
 void mon_outp(unsigned port,unsigned value)
 {

	XBuffCmd='O';
    XBuffLen= 5;
	*(unsigned *)(&Xpack_buffer[3]) = port;
    *(unsigned char *)(&Xpack_buffer[5]) = 0;
    *(unsigned *)(&Xpack_buffer[6]) = value;

	send(0);

	receive();
 }

 #pragma page(1)

/***************************************************************************
 *
 *  Name: mon_in()
 *	Description: read form a port (WORD)
 *
 ***************************************************************************/
 unsigned mon_inpw(unsigned port)
 {
 unsigned value;

	XBuffCmd='I';
    XBuffLen= 3;
	*(unsigned *)(&Xpack_buffer[3]) = port;

	Xpack_buffer[5] = (unsigned char)(1);

	send(0);

	receive();

    value = *(unsigned *)(&pack_buffer[3]);

	return(value);
 }

 #pragma page(1)

/***************************************************************************
 *
 *  Name: mon_in()
 *	Description: read form a port (BYTE)
 *
 ***************************************************************************/
 unsigned mon_inp(unsigned port)
 {
 unsigned value;

	XBuffCmd='I';
    XBuffLen= 3;
	*(unsigned *)(&Xpack_buffer[3]) = port;

	Xpack_buffer[5] = (unsigned char)(0);

	send(0);

	receive();

    value = *(unsigned *)(&pack_buffer[3]);

	return(value);
 }
