/* file: cmdtmbuf.h */

/****************************************************************************
 *
 *	BIRD Communications functions header file
 *
 *	written by denton marlowe
 *	(c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		   Engineer 	   Description
 *
 *	02/06/90	   D.marlowe	   Original release
 *
 ************************************************************************@R*/

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

 extern struct CMD_TM cmd_buffer;
 extern struct CMD_TM tm_buffer;

 extern int tm_invert_flag;
 extern int cmd_invert_flag;

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
