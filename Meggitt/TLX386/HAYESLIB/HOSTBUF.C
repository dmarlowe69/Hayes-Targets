/* file: hostbuf.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Packet Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *      Packet functions
 *
 *  written by denton marlowe
 *  (c)1989-1993 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date           Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *  This Software was Designed using the following development tools:
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

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include       <graph.h>
 #include       <stdlib.h>
 #include       <stdio.h>
 #include       <string.h>
 #include       <ctype.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include       "remtron.h"
 #include       "protocal.h"
 #include       "8250.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Xmodem stuff
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/

/***************************************************************************
 *
 *      general defintions
 *
 ***************************************************************************/

 #define TRUE           1
 #define FALSE          0
 #define ERR            -1
 #define OK             0
 #define STATUS_ERR     2

/***************************************************************************
 *
 *      defintions
 *
 ***************************************************************************/

 #define CMD_SYNC           'C'
 #define DATA_SYNC                      'D'
 #define STATUS_SYNC            'S'
 #define ECHO_SYNC                      'E'
 #define KERNEL_SYNC            'K'
 #define VERSION_SYNC           'V'
 #define STATUS_SYNC_ERR        'R'

 #define NORMAL     0
 #define INVERT         1

/***************************************************************************
 *
 *      data structures
 *
 ***************************************************************************/

 struct CMD_TM cmd_buffer;
 struct CMD_TM tm_buffer;

 volatile struct CMD_TM cmd;
 volatile struct CMD_TM tm;

 int tm_invert_flag = NORMAL;
 int cmd_invert_flag = NORMAL;

 int comm_status;

/****************************************************************************
 *
 *      packet function prototypes
 *
 ***************************************************************************/

 void dump(char *buff,int len);

/****************************************************************************
 *
 *      buffer function prototypes
 *
 ***************************************************************************/

 void init_tm_buffer(void);
 void init_cmd_buffer(void);

 void invert_tm_buffer(void);
 void invert_cmd_buffer(void);

/****************************************************************************
 *
 *      XMODEM function prototypes
 *
 ***************************************************************************/

 void send_tm_buffer(void);
 void send_tm_packet(char sync, int echo);

 int receive_cmd_packet(void);

 void load_tm_pack_buffer(char sync);
 void load_cmd_pack_buffer(char sync);

/****************************************************************************
 *
 *      packet function prototypes
 *
 ***************************************************************************/

 void load_tm_data(void);
 void buffer_cmd_data(char sync);

 #pragma page(1)

/****************************************************************************
 *
 *      Xmodem Debugging
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void dump(char *buff,int len)
 {
 int n,i;
 unsigned char c;
 unsigned char * s = buff;
 char buffer[81];

	n=0;
	while(n<len) {
		for(i=0;i<16;++i) {
			if(i==8) {
				if(n+i<len)
					_outtext("| ");
				else
					_outtext("  ");
			}
			if(n+i>=len)
				_outtext("   ");
			else {
				sprintf(buffer,"%02X ",*(s+i));
				_outtext(buffer);
			}
		}
		_outtext(" ");
		buffer[1]= '\0';
		for(i=0;i<16;++i) {
			c=*(s+i);
			if(n+i>=len)
				_outtext(" ");
			else if(c>=' ' && c<='~')
				{
				buffer[0] = c;
				_outtext(buffer);
				}
			else
				{
				buffer[0]= '.';
				_outtext(buffer);
				}
			}
		_outtext("\n");
		s+= 16;
		n+= 16;
		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *      buffer Routines
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      initialize telemetry buffer
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void init_tm_buffer(void)
 {
	tm_buffer.data[0] = 0;
	tm_buffer.data[1] = 1;
	tm_buffer.data[2] = 2;
	tm_buffer.data[3] = 3;
	tm_buffer.data[4] = 4;
	tm_buffer.data[5] = 5;
	tm_buffer.data[6] = 6;
	tm_buffer.data[7] = 7;
	tm_buffer.data[8] = 8;
	tm_buffer.data[9] = 9;
	tm_buffer.data[10] = 10;
	tm_buffer.data[11] = 11;
	tm_buffer.data[12] = 12;
	tm_buffer.data[13] = 13;
	tm_buffer.data[14] = 14;
	tm_buffer.data[15] = 15;
 }

 #pragma page(1)

/****************************************************************************
 *
 *      initialize command buffer
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void init_cmd_buffer(void)
 {
	cmd_buffer.data[0] = 0;
	cmd_buffer.data[1] = 1;
	cmd_buffer.data[2] = 2;
	cmd_buffer.data[3] = 3;
	cmd_buffer.data[4] = 4;
	cmd_buffer.data[5] = 5;
	cmd_buffer.data[6] = 6;
	cmd_buffer.data[7] = 7;
	cmd_buffer.data[8] = 8;
	cmd_buffer.data[9] = 9;
	cmd_buffer.data[10] = 10;
	cmd_buffer.data[11] = 11;
	cmd_buffer.data[12] = 12;
	cmd_buffer.data[13] = 13;
	cmd_buffer.data[14] = 14;
	cmd_buffer.data[15] = 15;
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Invert Tm buffer
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void invert_tm_buffer(void)
 {
 int i;

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++)
		tm_buffer.data[i] ^= 0xff;
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Invert Cmd buffer
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void invert_cmd_buffer(void)
 {
 int i;

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++)
		cmd_buffer.data[i] ^= 0xff;
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Host Packet Routines
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Send command packet to STD-BUS
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void send_tm_packet(char sync, int echo)
 {
 char buffer[81];
 int len;

	load_tm_pack_buffer(sync);
	send();

    len= *(int *)(&Xpack_buffer[1])+OVERHEAD;

    if(echo != 0) {
		sprintf(buffer,"(%c) sending %d bytes...chk (%02X)\n",
			XBuffCmd,len-OVERHEAD,Xpack_buffer[len-1]);
		_outtext(buffer);
		dump(&Xpack_buffer[3],len-OVERHEAD);
	}

 }

 #pragma page(1)

/****************************************************************************
 *
 *      Receive packet from STD-BUS
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 int receive_cmd_packet(void)
 {
 unsigned int num_bytes;
 unsigned char i;
 int status;

	status = receive();

	if(status == ERR) {
		return(status);
		}

	if(BuffLen != NUM_CMD_TM_CHANNELS)
		return(status);

	num_bytes = BuffLen;
	for(i=0;i<num_bytes;i++){               /* get data bytes */

		if(cmd_invert_flag == INVERT)
			cmd_buffer.data[i] = pack_buffer[i+3]^0xff;
		else
			cmd_buffer.data[i] = pack_buffer[i+3];
	}

	return(status);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Xmodem Load
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void load_tm_pack_buffer(char sync)
 {
 int i;

	XBuffCmd=sync;
	XBuffLen= NUM_CMD_TM_CHANNELS;
	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		if(tm_invert_flag == INVERT)
			*(unsigned *)(&Xpack_buffer[3+i])=(unsigned char)(tm_buffer.data[i]^0xff);
		else
			*(unsigned *)(&Xpack_buffer[3+i])=(unsigned char)tm_buffer.data[i];
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Xmodem Load
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void load_cmd_pack_buffer(char sync)
 {
 int i;

	XBuffCmd=sync;
	XBuffLen= NUM_CMD_TM_CHANNELS;
	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		if(tm_invert_flag == INVERT)
			*(unsigned *)(&Xpack_buffer[3+i])=(unsigned char)(cmd_buffer.data[i]^0xff);
		else
			*(unsigned *)(&Xpack_buffer[3+i])=(unsigned char)cmd_buffer.data[i];
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *  Xmodem example
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 //
 //   init_cmd_buffer();                  /* clear cmd_buffer for input */
 //   send_tm_packet(CMD_SYNC,0);         /* send me RTU RTD1 input data */
 //   comm_status = receive_cmd_packet(); /* receive RTD1 input data */
 //

 #pragma page(1)

/****************************************************************************
 *
 *	Load tm data from receive buffer
 *
 *	This function takes the received data packet and loads the RTE2 command
 *	data structure.
 *
 *	Warning:	RTE2 interrupt must be disabled
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/
 void load_tm_data(void)
 {
 int i;

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		tm.data[i] = pack_buffer[3+i];
		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Load transmit buffer with cmd data
 *
 *	This function takes the TM data loaded for the RTE2 and places it in the
 *	transmit buffer.
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/
 void buffer_cmd_data(char sync)
 {
 int i;

	XBuffCmd=sync;
	XBuffLen= NUM_CMD_TM_CHANNELS;
	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		*(unsigned *)(&Xpack_buffer[3+i])=cmd.data[i];
		}
 }
