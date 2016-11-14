/* file: monloop.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Packet Functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/****************************************************************************
 *
 *	Packet functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/90	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 7.00
 *	MASM Version 6.0
 *	Datalight C_Thru_ROM Version 2.0c
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include	<stdio.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	<hayes.h>
 #include	<gnd.h>

 #include	"cmd_tm.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem stuff
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

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

 #define CMD_SYNC			'C'
 #define DATA_SYNC			'D'
 #define STATUS_SYNC		'S'
 #define ECHO_SYNC			'E'
 #define KERNEL_SYNC		'K'
 #define EXIT_SYNC			'X'
 #define STATUS_SYNC_ERR	'R'

/****************************************************************************
 *
 *	Packet processing function prototypes
 *
 ***************************************************************************/

 int monloop(void);
 void mon_bad(void);

 #pragma page(1)

/****************************************************************************
 *
 *  monloop() is entered every time the target program
 *  passes control back to the Kernel.  This routine
 *  is the main "idle" loop where the Kernel sits waiting
 *  for commands from RDEB.
 *
 ***************************************************************************/
 int monloop(void)
 {
/****************************************************************************
 *
 *	wait for a packet from RDEB
 *
 ***************************************************************************/

	if(receive()!=OK) {			/* receive command, or flush if error */
		mon_bad();				/* tell the RDEB we're alive */
		if(BuffCmd == 'X') {
			return(ERR_EXIT);
		}
		return(ERR);			/* exit */
		}

/****************************************************************************
 *
 *	process the packet/command from RDEB
 *
 ***************************************************************************/

	switch(BuffCmd) {

		case 'C':							/* command sync */
			load_cmd_data();
			//(*funptr_cmd_application)();	/* call command decoder code */
			decode_cmd_data();
			cmd.frame_count += 1;			/* increment frame counter */
			return(OK);						/* exit */
			break;

		case 'D':					/* data sync */
			return(OK);				/* exit */
			break;

		case 'S':					/* status sync */
			buffer_tm_data('S');	/* get telemtry data sent from PC */
			send();					/* send to PC */
			return(OK);				/* exit */
			break;

		case 'X':					/* exit	sync */
			return(ERR_EXIT);		/* exit */
			break;

		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *  This routine is executed if a "bad packet" comes across
 *  the serial line.
 *
 ***************************************************************************/
 void mon_bad(void)
 {
	uart_putc('?');

	uart_flush();

 }

/****************************************************************************
 *
 *	functions from cmdtmbuf in hayeslib
 *
 ***************************************************************************/

 //void load_cmd_data(void);			/* load cmd.data from pack_buffer */
 //void buffer_tm_data(char sync);	/* load Xpack_buffer from tm.data */

/****************************************************************************
 *
 *	Remote Packet Routines
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Load cmd data from receive buffer
 *
 *	This function takes the received data packet and loads the RTD1 command
 *	data structure.
 *
 *	Warning:	RTD1 interrupt must be disabled
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/
 //void load_cmd_data(void)
 //{
 //int i;
 //
 //	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
 //		cmd.data[i] = pack_buffer[3+i];
 //		}
 //}

 #pragma page(1)

/****************************************************************************
 *
 *	Load transmit buffer with tm data
 *
 *	This function takes the TM data loaded for the RTE2 and places it in the
 *	transmit buffer.
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/
 //void buffer_tm_data(char sync)
 //{
 //int i;
 //
 //	XBuffCmd=sync;
 //	XBuffLen= NUM_CMD_TM_CHANNELS;
 //	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
 //		*(unsigned *)(&Xpack_buffer[3+i])=tm.data[i];
 //		}
 //}
