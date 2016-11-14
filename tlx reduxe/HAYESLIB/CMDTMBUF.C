/* file: cmdtmbuf.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("cmdtmbuf Functions")
 #pragma subtitle("(c)1991 Hayes Targets")

/****************************************************************************
 *
 *	cmdtmbuf functions
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

 #include	"remtron.h"
 #include	"protocal.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/***************************************************************************
 *
 *	defintions
 *
 ***************************************************************************/

 #define NORMAL 	0
 #define INVERT 	1

/***************************************************************************
 *
 *	data structures
 *
 ***************************************************************************/

 struct CMD_TM cmd_buffer;
 struct CMD_TM tm_buffer;

 int tm_invert_flag = NORMAL;
 int cmd_invert_flag = NORMAL;

/****************************************************************************
 *
 *	buffer function prototypes
 *
 ***************************************************************************/

 void init_tm_buffer(void);
 void init_cmd_buffer(void);

 void invert_tm_buffer(void);
 void invert_cmd_buffer(void);

/****************************************************************************
 *
 *	Remote buffer Routines
 *
 ***************************************************************************/

 void load_cmd_pack_buffer(char sync);
 void load_tm_buffer(void);			/* load telemetry transmit buffer */

 void echo_packet_to_xpacket(void);

/****************************************************************************
 *
 *	serial command/telemetry data transfer functions
 *
 ***************************************************************************/

 void load_cmd_data(void);			/* load cmd.data from pack_buffer */
 void buffer_tm_data(char sync);	/* load Xpack_buffer from tm.data */

 #pragma page(1)

/****************************************************************************
 *
 *	buffer Routines
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	initialize telemetry buffer
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
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
 *	initialize command buffer
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
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
 *	Invert Tm buffer
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
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
 *	Invert Cmd buffer
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
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
 *	Remote buffer Routines
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem Load
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
		*(unsigned *)(&Xpack_buffer[3+i])=cmd.data[i];
		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	Load telemetry transmit buffer from user buffer
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void load_tm_buffer(void)
 {
 unsigned char i;

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {

		tm.data[i] = pack_buffer[3+i];
		}

 //#ifdef LED_DEBUG
 //
 //	 write_LED(0, (char)(pack_buffer[3+8])); /* update bank of LEDs */
 //
 //#endif

 }

 #pragma page(1)

/****************************************************************************
 *
 *	Xmodem Load
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void echo_packet_to_xpacket(void)
 {
 int i;

	XBuffCmd=BuffCmd;

	XBuffLen= BuffLen;

	for(i=0;i<BuffLen;i++) {
		Xpack_buffer[3+i] = pack_buffer[3+i];
		}
 }

 #pragma page(1)

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
 void load_cmd_data(void)
 {
 int i;

	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		cmd.data[i] = pack_buffer[3+i];
		}
 }

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
 void buffer_tm_data(char sync)
 {
 int i;

	XBuffCmd=sync;
	XBuffLen= NUM_CMD_TM_CHANNELS;
	for(i=0;i<NUM_CMD_TM_CHANNELS;i++) {
		*(unsigned *)(&Xpack_buffer[3+i])=tm.data[i];
		}
 }
