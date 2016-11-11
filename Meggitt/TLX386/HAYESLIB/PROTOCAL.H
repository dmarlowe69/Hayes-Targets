/* file: protocal.h */

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

 extern unsigned char pack_buffer[PACKETSIZE];
 extern unsigned char Xpack_buffer[PACKETSIZE];

 extern signed int packet_error;

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
