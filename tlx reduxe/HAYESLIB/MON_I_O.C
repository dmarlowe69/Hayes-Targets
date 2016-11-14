 #include   "protocal.h"

 void mon_outpw(unsigned port,unsigned value);
 void mon_outp(unsigned port,unsigned value);
 unsigned mon_inpw(unsigned port);
 unsigned mon_inp(unsigned port);

 void mon_out(void);
 void mon_in(void);

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
