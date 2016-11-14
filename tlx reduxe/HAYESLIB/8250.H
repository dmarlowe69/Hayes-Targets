/* file: 8250.h */

/****************************************************************************
 *
 *      8250 function header file
 *
 *      written by denton marlowe
 *      (c)1989-1992 Hayes Targets
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

/*******************************************************************

	8250.H   8250 UART Defines and Structures

	Copyright (C) 1987,1988,1989 by Datalight
	All Rights Reserved

	Tabsize = 3

********************************************************************/


/*
	[]-------------------------------------[]
	 |                                                                                                      |
	 |                      USER SELECTABLE DEFINES                 |
	 |                                                                                                      |
	[]-------------------------------------[]
*/

/* The Defaults -- used only if invalid values are passed to comm_open() */
#define DEFBAUD         B9600   /* B300 - B57600 */
#define DEFPORT         0               /* 0 or 1 */

#define BUFSIZE         2048    /* Do NOT change these  */
#define DEFPARMS        N81             /*              ""                              */


/*
	[]-------------------------------------[]
	 |                                                                                                      |
	 |                              SYSTEM DEFINES                                  |
	 |                                                                                                      |
	[]-------------------------------------[]
*/
/* Common Defines */
#define TRUE  1
#define FALSE 0
#define ERR  -1
#define OK    0

/* Microsoft Compiler Dependant */
#ifndef __TURBOC__

#define inportb(port)           inp(port)
#define outportb(port,val)      outp(port,val)

#endif

/* do this in-line for speed (at 115K speed is important) */
#define comm_data_rdy() while(!(inportb(LINESTATUS) & LS_DRDY)) /* wait */

/* Communications Masks, Ports and Vectors */
#define MASK1   0x10                    /* 8259 interrupt masks */
#define MASK2   0x08
#define BASE1   0x3F8                   /* Base port address for 8250 UART */
#define BASE2   0x2F8
#define VECT1   0x0C                    /* Interrupts that the 8250 UART generates */
#define VECT2   0x0B

/* 8259 Interrupt Controller Ports */
#define EOIPORT 0x20
#define EOIVAL          0x20
#define MASKPORT        0x21            /* each bit: 0=interrupt enabled, 1=masked off */

/* 8250 UART Port Addresses */
#define DATAREG      (cp.base)
#define BAUDLOW      (cp.base)
#define BAUDHIGH     (cp.base+1)
#define INTENABLE    (cp.base+1)
#define INTID        (cp.base+2)
#define LINECONTROL  (cp.base+3)
#define MODEMCONTROL (cp.base+4)
#define LINESTATUS   (cp.base+5)
#define MODEMSTATUS  (cp.base+6)

/*
	Stop/Data/Parity Bits for 8250 UART

	Bits 43210
	     ---xx              Data Bits (00=5, 01=6, 10=7, 11=8)
	     --x--              Stop Bits (0=1, 1=2)
	     xx---              Parity (00=None, 01=Odd, 11=Even)
*/
#define N81     3
#define E71    26

/* 8250 (PC) UART Baud Rate Divisors */
#define B300  384
#define B1200  96
#define B2400  48
#define B4800  24
#define B9600  12
#define B19200  6
#define B38400  3
#define B57600  2
#define B115200 1                       /* highest PC clock rate */


/*
	(Line Status Register)          (Modem Status Register)
	1....... Timeout Error           1....... Carrier Detect
	.1...... Trans Sh-Reg Empty      .1...... Ring Indicator
	..1..... Trans Hold-Reg Empty    ..1..... Data-Set-Ready
	...1.... Break-Detect Error      ...1.... Clear-To-Send
	....1... Framing Error           ....1... (change in above)
	.....1.. Parity Error            .....1..        ""
	......1. Overrun Error           ......1.        ""
	.......1 Data Ready              .......1        ""
*/

 #define LS_OVERRUN_ERROR	0x02
 #define LS_PARITY_ERROR	0x04
 #define LS_FRAMING_ERROR	0x08
 #define LS_BREAK_ERROR		0x10
 #define LS_ERROR (LS_OVERRUN_ERROR | LS_PARITY_ERROR	| LS_FRAMING_ERROR | LS_BREAK_ERROR)


#define LS_DRDY 		0x01
#define LS_OERR         0x02
#define LS_TRDY         0x20
#define MS_CARRIER      0x80
#define MS_RING         0x40
#define MS_DSR          0x20
#define MS_CTS          0x10


/* Structures */
typedef struct {
	int  base;
    int  port;
    int  comm_port;
    int  serial_port;
	unsigned int baud_rate;
	int  user_break;
	int  head,tail;
	int  bufsize;

 #ifdef XMIT_IRQ
	int  xhead,xtail;
	int  xbufsize;
	int  xmitIRQ;
 #endif
	unsigned char vector;
	unsigned char buffer[BUFSIZE];

 #ifdef XMIT_IRQ
	unsigned char xbuffer[BUFSIZE];
 #endif

	} comport;

/* Externals */
extern comport cp;


/*
	[]-------------------------------------------[]
	 |                                                                                                                      |
	 |                                      PACKET DRIVEN I/O                                       |
	 |                                                                                                                      |
	 |      NOTE:   Not used in the C_thru_ROM      Kernel  |
	 |                                                                                                                      |
	[]-------------------------------------------[]
*/

#define PAC_BEG         pacbuf
#define PAC_COUNT               (*(int *)pacbuf)
#define PAC_CMD         pacbuf[2]
#define PAC_SECTOR      (*(unsigned *)(pacbuf+3))
#define PAC_RDBUFF      (pacbuf+3)
#define PAC_WRBUFF   (pacbuf+5)
#define PAC_CHGCODE     pacbuf[3]
#define PAC_ERRCODE     pacbuf[3]
#define PAC_BPB         (*(struct bpb *)(pacbuf+3))

struct bpb {
	unsigned                sec_size;
	unsigned char   sec_per_clus;
	unsigned                        res_sec;
	unsigned char   num_fats;
	unsigned                        dir_ent;
	unsigned                        num_sec;
	unsigned char   media;
	unsigned                        fat_size;
	unsigned                        sec_per_trak;
	unsigned                        sides;
	unsigned                        hidden;
	};

extern unsigned char pacbuf[];

 void comm_open(unsigned baud,unsigned parms,unsigned port);
 void comm_close(void);
 int comm_poll(void);
 int comm_check(void);
 void comm_putc(char c);
 void comm_flush(void);
 int comm_getc(void);
 void comm_to_char(void);
 void comm_quit(void);
 void comm_gets(char *str);
 void comm_puts(char *str);
 int makebaud(unsigned baud);

 #ifdef XMIT_IRQ

 void far interrupt comm_rec_xmt_char_int(void);
 void work_xmit_buffer(void);
 int xmit_poll(void);
 void putc_xmit(char c);

 #endif

 void change_baud_rate(void);
 void change_serial_port(void);
 void set_time_out(unsigned long int time_out);
