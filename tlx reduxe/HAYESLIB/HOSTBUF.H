/* file: hostbuf.h */

/****************************************************************************
 *
 *	packet function header file
 *
 *	written by denton marlowe
 *	(c)1989-1992 Hayes Targets
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

/***************************************************************************
 *
 *	general defintions
 *
 ***************************************************************************/

 #define TRUE		1
 #define FALSE		0
 #define ERR		-1
 #define OK 		0
 #define STATUS_ERR	2

/***************************************************************************
 *
 *	defintions
 *
 ***************************************************************************/

 #define CMD_SYNC           'C'
 #define DATA_SYNC			'D'
 #define STATUS_SYNC		'S'

 #define ECHO_SYNC			'E'
 #define KERNEL_SYNC		'K'
 #define VERSION_SYNC		'V'
 #define EXIT_SYNC			'X'
 #define STATUS_SYNC_ERR	'R'

 #define INPUT_SYNC			'I'
 #define OUTPUT_SYNC		'O'

 #define GO_SYNC			'G'
 #define HALT_SYNC			'H'
 #define CALIB_SYNC 		'B'

 #define NORMAL 	0
 #define INVERT 	1

/***************************************************************************
 *
 *	data structures
 *
 ***************************************************************************/

 extern struct CMD_TM cmd_buffer;
 extern struct CMD_TM tm_buffer;

 extern volatile struct CMD_TM cmd;
 extern volatile struct CMD_TM tm;

 extern int tm_invert_flag;
 extern int cmd_invert_flag;

 extern int comm_status;

/****************************************************************************
 *
 *	packet function prototypes
 *
 ***************************************************************************/

 void dump(char *buff,int len);

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
 *	function prototypes
 *
 ***************************************************************************/

 void send_tm_buffer(void);
 void send_tm_packet(char sync, int echo);

 int receive_cmd_packet(void);

 void load_tm_pack_buffer(char sync);
 void load_cmd_pack_buffer(char sync);

/****************************************************************************
 *
 *	packet function prototypes
 *
 ***************************************************************************/

 void load_tm_data(void);
 void buffer_cmd_data(char sync);
