/* file: monloop.h */

/***************************************************************************
 *
 *	Packet header file
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

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
