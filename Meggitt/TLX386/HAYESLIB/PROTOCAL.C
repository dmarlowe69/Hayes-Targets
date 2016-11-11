/* file: protocal.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Protocal Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *	Protocal functions
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date	       Engineer        Description
 *
 *	02/06/90	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 3.3
 *	Microsoft C Version 6.00a
 *	MASM Version 5.1
 *	Datalight C_Thru_ROM Version 1.61
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

 #include	<hayes.h>
 #include	<gnd.h>

 #include   "checksum.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	remove defintions from Hayes.h
 *
 ***************************************************************************/

 #ifdef REMOTE_I_O

 #undef com_puts
 #undef com_getc
 #undef com_gets
 #undef com_putc
 #undef com_check

 #pragma message("Using PC 8250 Serial I/O")

 #define com_puts   comm_puts
 #define com_getc   comm_getc
 #define com_gets   comm_gets
 #define com_putc   comm_putc
 #define com_check  comm_check

 #endif

/***************************************************************************
 *
 *	general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

/***************************************************************************
 *
 *	defintions
 *
 ***************************************************************************/

 #define BuffCmd	pack_buffer[0]
 #define BuffLen	*(int *)(pack_buffer+1)
 #define BuffData	(pack_buffer+3)

 #define XBuffCmd	Xpack_buffer[0]
 #define XBuffLen	*(int *)(Xpack_buffer+1)
 #define XBuffData	(Xpack_buffer+3)

 #define DATASIZE		128
 #define OVERHEAD		4
 #define PACKETSIZE 	(DATASIZE+OVERHEAD)

 unsigned char pack_buffer[PACKETSIZE];
 unsigned char Xpack_buffer[PACKETSIZE];

 signed int packet_error;

 #define ERR_OK 		0
 #define ERR_COMMAND	1
 #define ERR_LENGTH 	2
 #define ERR_CHECKSUM	3
 #define ERR_EXIT		4

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void send(void);
 int receive(void);

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem stuff
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem Send
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void send(void)
 {
 int i,len;
 unsigned char *p;

	Xmakechecksum();

	len= *(int *)(&Xpack_buffer[1])+OVERHEAD;

	p=Xpack_buffer;

	for(i=0;i<len;++i,++p)
		com_putc(*p);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem Receive
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 int receive(void)
 {
 int c,i,len;

/** get command and length ***/

	BuffCmd=com_getc();

	if(BuffCmd<'A' || BuffCmd>'Z') {		/* check for bad command */
		packet_error = ERR_COMMAND;
		return ERR;
		}

	for(i=1;i<3;++i)
		pack_buffer[i]=com_getc();

/*** get the data part ***/

	len= *(unsigned *)(pack_buffer+1);

	if(len<0 || len>DATASIZE) {				/* check for bad length */
		packet_error = ERR_LENGTH;
		return ERR;
		}

	for(i=0;i<len;++i)
		pack_buffer[3+i]=com_getc();

/*** get checksum and verify it ***/

	c=com_getc();

	if(c!=checksum(&pack_buffer[3],len)) {	/* check for bad checksum */
		packet_error = ERR_CHECKSUM;
		return ERR;
		}

	pack_buffer[3+len]=c;

	packet_error = ERR_OK;

	return OK;
 }
