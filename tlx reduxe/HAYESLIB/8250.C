/* file: 8250.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Comm Port Interface")
 #pragma subtitle("(c)1989-1992 Hayes Targets")

/****************************************************************************
 *
 *      Commport Interface
 *
 *  written by denton marlowe
 *      (c)1989-1993 HAYES TARGETS
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
 *      This Software was Designed using the following development tools:
 *
 *      MS-DOS Verison 5.0
 *      Microsoft C Version 7.00
 *      MASM Version 6.1
 *      Datalight C_Thru_ROM Version 2.00c
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include       <dos.h>
 #include       <conio.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include       "8250.h"

/* #define UNDER_DOS */
/*******************************************************************

	8250.C  created 12/18/87  by Drew Gislason
	Copyright (C) 1987,1989,1989 by Datalight
	All Rights Reserved

	Modified  05/24/88  DG  Made "Generic"
	Modified  07/08/89  DG  Packets and Turbo C Compatible

	Low level interrupt driven serial communications for the
	8250 UART.  Uses no DOS calls nor BIOS calls.  Assumes a
	completely "dumb" machine.

	UART Specific Initialization Routines
		comm_open(baud,parms,port)      - initialize comm port
		comm_poll()                                             - check for and return a received character
		comm_putc(c)                                    - put a character
		comm_close()                                    - disable interrupt i/o

	UART Specific Character I/O Routines
		interrupt comm_rec_char_int()           - character i/o isr

	General Character Routines
		comm_getc()             - wait indefinitely for a character
		comm_flush()            - flush the input buffer

********************************************************************/

#ifdef __TURBOC__
#include <dos.h>
#else
#define MK_FP(seg,ofs)  ((void far *) \
			   (((unsigned long)(seg) << 16) | (unsigned)(ofs)))
#define enable()                _enable()
#define disable()               _disable()
#endif

/* Globals */
comport cp;
void (__interrupt __far *oldserial)();
unsigned long int time_out_value;

/* Prototypes */
void comm_to_char(void);
static void setportvect(void (interrupt far *routine)());


/*
  []--------------------------------------------------[]
	|                                                                                                                                       |
	|                       CHARACTER DRIVEN I/O ROUTINES                           |
	|                                                                                                                                       |
  []--------------------------------------------------[]
*/

/****************************************************************************
 *
 *      ISR (Interrupt Service Routine)
 *
 *       Interrupt driven CHARACTER receive.  This routine stuffs
 *  characters received into a ring buffer.
 *
 *  NOTE: Do NOT have any LOCALS in this ISR or ctrl-break will
 *                not work.
 *
 ***************************************************************************/
 #define CAN 0x18

 void far interrupt comm_rec_char_int(void)
 {
	static unsigned char c;

	/* get the data */
	c = inportb(DATAREG);

	/* put data in buffer and check for wrap around */
	cp.buffer[cp.head++]= c;
	if(cp.head >= cp.bufsize)
		cp.head= 0;

	/* send an end-of-interrupt to the 8259 */
	outportb(EOIPORT, 0x20);
 }

/****************************************************************************
	Enable the 8250 UART for interrupt driven receive and
	polling transmit.  Initialized a structure defining the
	port,   and set up the interrupt vector for receive.

	baud            (300,1200,2400..38400,57600,115)
	parms           (3=N81, 26=7E1, see COMM.H)
	port            (0 or 1)
 ***************************************************************************/
 void comm_open(unsigned baud,unsigned parms,unsigned port)
 {
	unsigned char mask;
	unsigned char val;

	set_time_out(100000U);					/* set default time out */

	/* Check for defaults */
	if(port > 1) port = DEFPORT;            /* if port is invalid, use default */
	if(!parms)  parms = DEFPARMS;
	if(!baud)   baud  = DEFBAUD;

	/* load port parameters in struture	*/
	cp.serial_port = port;					/* set serial port to comm 1*/
	cp.comm_port = cp.serial_port + 1;		/* set serial port to comm 1*/
	cp.baud_rate = baud;					/* set baud rate */

#if PACKET
	/* Special case for 115K baud (because processor may be too slow) */
	if(baud == 115)
		PacDelay = 100;
	else
		PacDelay = 0;
#endif

	cp.base= (port ? BASE2 : BASE1);
	mask= (cp.base==BASE1 ? MASK1 : MASK2);
	cp.vector = (port ? VECT1 : VECT2);
	baud= makebaud(baud);
	oldserial = _dos_getvect( cp.vector );

	/* disable interupts for this port (8259 controller) */
	val =inportb(MASKPORT);
	val|= mask;                                             /* set the bit to 1 to disable */
	outportb(MASKPORT,val);

	/* initialize the structure */
	cp.user_break = 0;
	cp.head= cp.tail= 0;
	cp.bufsize= BUFSIZE;

 #ifdef XMIT_IRQ
	cp.xhead= cp.xtail= 0;
	cp.xbufsize= BUFSIZE;
	cp.xmitIRQ = 0;
 #endif

	/* initialize the serial port */
	outportb(LINECONTROL,0x80);                     /* set high bit, so we can set baud */
	outportb(BAUDLOW,baud & 0xFF);          /* baud rate */
	outportb(BAUDHIGH,baud >> 8);
	outportb(LINECONTROL,parms);                    /* stop/data/parity bits */

#if !POLLING
	/* enable interrupts */
	comm_to_char();                                 /* point vector to character ISR */
	val=inportb(MASKPORT);
	val&= ~mask;                                            /* set bit to 0 to enable */
	outportb(MASKPORT,val);                 /* enable interrupt mask */
	outportb(MODEMCONTROL,0x0B);    /* enable interrupts (bit3), and pins high */
	outportb(INTENABLE,0x01);               /* enable receive interrupt */
	val=inportb(INTID);
#endif

	if(val & (LS_DRDY | LS_OERR))
		inportb(DATAREG);                               /* throw away pending data on open */
}

/*
 *  Disable interrupts for this serial port.
 */
void comm_close(void)
{
	unsigned char val;
	unsigned char mask = (cp.base==BASE1 ? MASK1 : MASK2);

	/* disable interrupt mask for this channel (8259) */
	val=inportb(MASKPORT);
	val|= mask;
	outportb(MASKPORT,val);

	_dos_setvect(cp.vector,oldserial);
}

#if POLLING

/*
 *  Check UART for a received character.  Return it.
 */
int comm_poll(void)
{
	unsigned char val;

	val=inp(LINESTATUS);
	if(val&0x01)
		return (unsigned char)inportb(DATAREG);
	return ERR;
}

#else

/*
 *  Grab a character out of the ring buffer.
 */
int comm_poll(void)
{
	unsigned c;
	if(cp.head!=cp.tail) {
		c=cp.buffer[cp.tail];
		if(++cp.tail >= cp.bufsize)
			cp.tail=0;
		return c&0xff;
		}
	return ERR;
}

#endif


/*
 *  Send a character out the UART.  This is a "polled"
 *  transmit, not interrupt driven.
 */
 void comm_putc(char c)
 {
 #ifdef XMIT_IRQ

	putc_xmit(c);

 #else

	unsigned char val;
	int delay;

	while(TRUE) {
		val=inportb(LINESTATUS);
		if(val & 0x20) {                                /* trans buffer is empty */
 #if PACKET
			if(PacDelay)
				for(delay=0; delay<PacDelay; ++delay)
					/* waste time at 115K baud */;
 #endif
			outportb(DATAREG,c);
			return;
			}
		}
 #endif
 }

/*
  []--------------------------------------------------------[]
	|                                                                                                                                                       |
	|                       GENERIC ROUTINES TO ALL UART/CONTROLLERS                        |
	|                                                                                                                                                       |
  []--------------------------------------------------------[]
*/


 #pragma page(1)

/***************************************************************************
 *      Throw out characters until there are no more (either
 *  in the ring buffer or waiting at the port).
 *
 **************************************************************************/
 void comm_flush(void)
 {
	while(comm_poll()!=ERR)
		/* toss the character */;
 }

 #pragma page(1)

/***************************************************************************
 *
 *      Wait indefinitely for a character from the port.
 *
 **************************************************************************/
 int comm_getc(void)
 {
 //#define _TIME_OUT 100000U
 //#define _TIME_OUT 4096U
 int c;
 unsigned long int time_out;

	time_out = 0;
	while(TRUE) {
		if((c=comm_poll())!=ERR)
			break;
		time_out++;
		if(time_out > time_out_value) {
			c = ERR;
			break;
			}
//#ifdef UNDER_DOS
//              if(kbhit()) {
//                      c = getch();
//                      if(c == 27)
//                              comm_quit();
//                      }
//#endif
		}
	return c;
 }

 #pragma page(1)

/***************************************************************************
 *
 *      Set the interrupt driven receive VECTOR to point
 *  to the character driven ISR.
 *
 **************************************************************************/
 void comm_to_char(void)
 {
 #ifdef XMIT_IRQ

	cp.head= cp.tail= 0;
	cp.xhead= cp.xtail= 0;
	cp.xmitIRQ = 0;
	setportvect(comm_rec_xmt_char_int);

 #else

	cp.head= cp.tail= 0;
	setportvect(comm_rec_char_int);

 #endif
 }


/*
 *  Set a vector to point to a routine.
 */
static void setportvect(void (interrupt far *routine)())
{
	disable();
	*(long far *)MK_FP(0,(cp.base==BASE1 ? VECT1 : VECT2)*4) =
		(long)routine;
	enable();
}


/*
 *  From an unsigned value, make a baud rate divisor
 *  for the 8250 on a PC.
 */
static int makebaud(unsigned baud)
{
	switch(baud) {
		case   300: baud = B300;   break;
		case  1200: baud = B1200;  break;
		case  2400: baud = B2400;  break;
		case  4800: baud = B4800;  break;
		case  9600: baud = B9600;  break;
		case 19200:     baud = B19200; break;
		case 38400: baud = B38400; break;
		case 57600: baud = B57600; break;
		case 115:   baud = B115200; break;
		default:                baud = B9600;  break;
		}
	return baud;
}
/***************************************************************************
 *
 * This routine simply returns TRUE (1) or FALSE (0) if there are any
 * characters available in the receive buffer
 *
 *      written by Denton Marlowe
 *      (c)1990 HAYES TARGETS
 *
 ***************************************************************************/
 int comm_check(void)
 {
	if(cp.head == cp.tail)
		return(ERR);      /* no character available */
	return(TRUE);             /* yes, we have some */
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 * ****** G E N E R A L   U A R T   S T R I N G   F U N C T I O N S ******
 *
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8250 comm string input routine
 *
 *      DELCARATION:    void comm_gets(char *str)
 *
 *      PARAMETERS:     pointer to character buffer for input string
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Character string loaded into buffer
 *
 *      LOCALS:         int c
 *
 *      EXTERNALS
 *       Referenced:    int comm_getc(void)
 *                      void comm_putc(int c)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         comm.c
 *
 *      DESCRIPTION:
 *
 *      This function retrieves a string from the serial port. It terminates
 *      on the carriage return character (0x0d). Does NOT recognize a
 *      backspace character.
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void comm_gets(char *str)
 {
	int c;

	while((c = comm_getc()) != '\r') {      /* Until a carriage return */
		*str++ = c;                     /* store it in the string */
		}

	*str = '\0';                            /* Terminate the string */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8250 comm string output routine
 *
 *      DELCARATION:    void comm_puts(char *str)
 *
 *      PARAMETERS:     pointer to Null terminated character string
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Character string output to comm
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void comm_putc(int c)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         comm.c
 *
 *      DESCRIPTION:
 *
 *      Write a string to the serial port, calls comm_putc() repeatedly
 *      untill string is exhausted (NULL terminated).
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void comm_puts(char *str)
 {
	while(*str)                 /* till NULL character */
	comm_putc(*str++);          /* output character */
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 * ****** INTERRUPT DRIVEN UART TRANSMIT FUNCTIONS ******
 *
 *
 ***************************************************************************/

 #pragma page(1)

 #ifdef XMIT_IRQ

/****************************************************************************
 *      ISR (Interrupt Service Routine)
 *
 *       Interrupt driven CHARACTER receive.  This routine stuffs
 *  characters received into a ring buffer.
 *
 *  NOTE: Do NOT have any LOCALS in this ISR or ctrl-break will
 *                not work.
 *
 ***************************************************************************/

 void far interrupt comm_rec_xmt_char_int(void)
 {
	static unsigned char c;
	static unsigned char val;
	static unsigned char line_status;

	_enable();

	while(TRUE) {
		val = inp(INTID);
		if((val&0x01)==1)
			break;

		switch(val) {
			case 0x04:							/* REC IRQ */
				c = inp(DATAREG);               /* get the data */

				line_status = inp(LINESTATUS);	/* check line status */
				if((line_status & LS_ERROR)) == 0) {
					cp.buffer[cp.head++]= c;	/* put data in buffer */
					if(cp.head >= cp.bufsize)	/* check for wrap around */
						cp.head= 0;
				}
				break;

			case 0x02:							/* xmitter IRQ */
				work_xmit_buffer();
				break;

		} /* switch */

	} /* while */

	/* send an end-of-interrupt to the 8259 */
	outp(EOIPORT, 0x20);
 }

/****************************************************************************
 *
 *      Transmitter holding register Empty Interrupt Service routine
 *
 ***************************************************************************/
 void work_xmit_buffer(void)
 {
	static unsigned char c;

	if(cp.xhead==cp.xtail) {
		outp(INTENABLE,(inp(INTENABLE)&~0x02));
		return;                                         /* return */
		}

	c=cp.xbuffer[cp.xtail];
	if(++cp.xtail >= cp.xbufsize)
		cp.xtail=0;

	outp(DATAREG,c);                                /* output character */
 }

/****************************************************************************
 *
 *      Grab a character out of the ring buffer.
 *
 ***************************************************************************/
 int xmit_poll(void)
 {
	unsigned c;

	if(cp.xhead!=cp.xtail) {
		c=cp.xbuffer[cp.xtail];
		if(++cp.xtail >= cp.xbufsize)
			cp.xtail=0;
		return c&0xff;
		}
	return ERR;
 }

/****************************************************************************
 *
 *      Put a character in the ring buffer.
 *
 ***************************************************************************/
 void putc_xmit(char c)
 {
	cp.xbuffer[cp.xhead++]= c;
	if(cp.xhead >= cp.xbufsize)
		cp.xhead= 0;

	outp(INTENABLE,(inp(INTENABLE)|0x02));
 }

 #endif

 #pragma page(1)

/*****************************************************************************
 *
 *      change baud rate
 *
 ****************************************************************************/
 void change_baud_rate(void)
 {
	comm_flush();

	comm_close();

    if(cp.baud_rate == 9600U)
	cp.baud_rate = 19200U;         /* set baud rate */
	else
	cp.baud_rate = 9600U;          /* set baud rate */

    comm_open(cp.baud_rate,0,cp.serial_port); /* open the serial port */

	comm_flush();
 }

 #pragma page(1)

/*****************************************************************************
 *
 *      change baud rate
 *
 ****************************************************************************/
 void change_serial_port(void)
 {
	comm_flush();

	comm_close();

    if(cp.serial_port == 1)
	cp.serial_port = 0;            /* set port */
	else
	cp.serial_port = 1;            /* set port */

    cp.comm_port = cp.serial_port + 1;    /* set serial port to comm 1*/

    comm_open(cp.baud_rate,0,cp.serial_port); /* open the serial port */

	comm_flush();
 }
 #pragma page(1)

 void set_time_out(unsigned long int time_out)
 {
	time_out_value = time_out;
 }
