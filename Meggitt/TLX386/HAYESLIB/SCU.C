/* file: scu.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("SCU functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      8251 SCU functions
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

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

 #include       "config.h"
 #include       "tcu.h"
 #include       "icu.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      I/O addresses of SCU control registers
 *
 *      The SCU_BASE is the address of the 8251 SCU on the SBC40. It is
 *      fixed at address 48H on standard boards.
 *
 ***************************************************************************/

 #if !defined(SCU_BASE)

 #error Base address of SCU not defined

 #endif

/****************************************************************************
 *
 *      SCU definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      I/O addresses of SCU control registers
 *
 ***************************************************************************/

 #if defined(V40)

 #define SRB       SCU_BASE             /* RXD buffer - read */
 #define STB       SCU_BASE             /* TXD buffer - write */
 #define SST       SCU_BASE+1           /* status - read */
 #define SCM       SCU_BASE+1           /* COMMAND - write */
 #define SMD       SCU_BASE+2           /* MODE - write */
 #define SIMK      SCU_BASE+3           /* Intterupt mask - read/write */

 #define SCU_DATA  SCU_BASE             /* R/W character */
 #define SCU_CNTRL SCU_BASE+1           /* scu control */
 #define SCU_STAT  SCU_BASE+1           /* scu status */

 #elif defined(V50)

 #define SRB       SCU_BASE             /* RXD buffer - read */
 #define STB       SCU_BASE             /* TXD buffer - write */
 #define SST       SCU_BASE+2           /* status - read */
 #define SCM       SCU_BASE+2           /* COMMAND - write */
 #define SMD       SCU_BASE+4           /* MODE - write */
 #define SIMK      SCU_BASE+6           /* Intterupt mask - read/write */

 #define SCU_DATA  SCU_BASE             /* R/W character */
 #define SCU_CNTRL SCU_BASE+2           /* scu control */
 #define SCU_STAT  SCU_BASE+2           /* scu status */

 #elif defined(V20)

 #define UBASE 0x90
 #define UDATA UBASE
 #define UCR   UBASE + 1
 #define USR   UBASE + 1
 #define MCR   UBASE + 2
 #define BRSR  UBASE + 3
 #define MSR   UBASE + 3

 #endif

/****************************************************************************
 *
 *      SCU general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define ERR  -1

/****************************************************************************
 *
 *      general bit position definitions
 *
 ***************************************************************************/

 #define B0 0x01
 #define B1 0x02
 #define B2 0x04
 #define B3 0x08
 #define B4 0x10
 #define B5 0x20
 #define B6 0x40
 #define B7 0x80

/****************************************************************************
 *
 *      SST defintions
 *
 ***************************************************************************/

 #define STB_FULL  0x00
 #define STB_EMTPY B0

 #define SRB_EMPTY 0x00
 #define SRB_FULL  B1

 #define PE   B3
 #define OVE  B4
 #define FE   B5
 #define SBRK B6

/****************************************************************************
 *
 *      SCM defintions
 *
 ***************************************************************************/

 #define TXD_DISABLE 0x00
 #define TXD_ENABLE  B0

 #define RXD_DISABLE 0x00
 #define RXD_ENABLE  B2

 #define SBRK_INACTIVE 0x00
 #define SBRK_ACTIVE   B3

 #define CLEAR_ERROR B4

 #define MRDY_MASK    0x00
 #define MRDY_NORMAL  B5

/****************************************************************************
 *
 *      SMD defintions
 *
 ***************************************************************************/

 #define BF_16 B1
 #define BF_64 (B1 | B0)

 #define CL_7  B3
 #define CL_8  (B3 | B2)

 #define PS_NONE 0x00
 #define PS_ODD  B4
 #define PS_EVEN (B5 | B4)

 #define STL_1   B6
 #define STL_2   (B7 | B6)

/****************************************************************************
 *
 *      SIMK defintions
 *
 ***************************************************************************/

 #define RM_ENABLE  0x00
 #define RM_DISABLE B0
 #define TM_ENABLE  0x00
 #define TM_DISABLE B1

/***************************************************************************
 *
 *      SCU Baud Rates definitions for 16X clock
 *
 *      for reference only, actual value computed realtime
 *
 ***************************************************************************/

 #if defined(CLOCK5)

 #define BAUD300    0x209       /* Divisor for 300    baud 5 MHz system */
 #define BAUD1200   0x82        /* Divisor for 1200   baud 5 MHz system */
 #define BAUD2400   0x41        /* Divisor for 2400   baud 5 MHz system */
 #define BAUD4800   0x21        /* Divisor for 4800   baud 5 MHz system */
 #define BAUD9600   0x10        /* Divisor for 9600   baud 5 MHz system */
 #define BAUD19200  0x08        /* Divisor for 19200  baud 5 MHz system */
 #define BAUD38400  0x04        /* Divisor for 38400  baud 5 MHz system */
 #define BAUD76800  0x02        /* Divisor for 76800  baud 5 MHz system */
 #define BAUD153600 0x01        /* divisor for 153600 baud 5 MHz system */

 #elif defined(CLOCK8)

 #define BAUD300    0x341       /* Divisor for 300    baud 8 MHz system */
 #define BAUD1200   0xd0        /* Divisor for 1200   baud 8 MHz system */
 #define BAUD2400   0x68        /* Divisor for 2400   baud 8 MHz system */
 #define BAUD4800   0x34        /* Divisor for 4800   baud 8 MHz system */
 #define BAUD9600   0x1a        /* Divisor for 9600   baud 8 MHz system */
 #define BAUD19200  0x0d        /* Divisor for 19200  baud 8 MHz system */
 #define BAUD38400  0x06        /* Divisor for 38400  baud 8 MHz system */
 #define BAUD76800  0x03        /* Divisor for 76800  baud 8 MHz system */
 #define BAUD153600 0x01        /* divisor for 153600 baud 8 MHz system */

 #elif defined(CLOCK10)

 #define BAUD300    0x418       /* Divisor for 300    baud 10 MHz system */
 #define BAUD1200   0x164       /* Divisor for 1200   baud 10 MHz system */
 #define BAUD2400   0x82        /* Divisor for 2400   baud 10 MHz system */
 #define BAUD4800   0x42        /* Divisor for 4800   baud 10 MHz system */
 #define BAUD9600   0x20        /* Divisor for 9600   baud 10 MHz system */
 #define BAUD19200  0x10        /* Divisor for 19200  baud 10 MHz system */
 #define BAUD38400  0x08        /* Divisor for 38400  baud 10 MHz system */
 #define BAUD76800  0x04        /* Divisor for 76800  baud 10 MHz system */
 #define BAUD153600 0x02        /* divisor for 153600 baud 10 MHz system */

 #endif

 #if defined(V20)

 #define B300	0xaa
 #define B1200	0xa2
 #define B2400  0x96
 #define B4800  0x8e
 #define B9600  0x86
 #define B19200	0x82

 #endif

/****************************************************************************
 *
 *      SCU function prototypes
 *
 ***************************************************************************/

 void scu_open(unsigned baud_rate);    /* initialize SCU */

 #if defined(V20)

 unsigned int lookup_baud(unsigned baud);

 #endif

 void scu_close(void);				   /* close out scu */
 int scu_check(void);                  /* check for character */
 int scu_getc(void);                   /* get charcter */
 int scu_poll(void);                   /* wait for character */
 void scu_flush(void);                 /* flush characters from receive buffer */
 void scu_putc(int ch);                /* output character to SCU */
 void scu_gets(char *str);             /* input string from receive buffer */
 void scu_puts(char *str);             /* output string to SCU */

/****************************************************************************
 *
 *      SCU function prototype for interrupt service routine
 *
 ***************************************************************************/

 void interrupt far scu_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

/****************************************************************************
 *
 *      SCU data structures
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      The BUFFER_SIZE equate defines the size of the serial port
 *      receive buffer. In order for the code to work as written this
 *      value must be a power of two and less than 65536.
 *
 ***************************************************************************/

 #define BUFFER_SIZE    256     /* DO NOT CHANGE THIS */

 typedef struct {
    unsigned char mode;
    unsigned char command;
    unsigned char status;
    unsigned int  head,tail;
    unsigned int  bufsize;
    unsigned char buffer[BUFFER_SIZE];
    } comport;

/****************************************************************************
 *
 *      SCU global variables
 *
 ***************************************************************************/

 comport scu;                          /* comport structure */

/****************************************************************************
 *
 *
 *      end of global definitions
 *
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *
 *      ********** 8 2 5 1  U A R T   F U N C T I O N S **********
 *
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU initialization routine
 *
 *      DELCARATION:    void scu_open(unsigned int baud_rate)
 *
 *      PARAMETERS:     Required baud rate for SCU
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Uart receive interrupt service routine installed and
 *                      enabled.
 *
 *      LOCALS:         int baud
 *
 *      EXTERNALS
 *       Referenced:    struct comport scu
 *                      void scu_baud(unsigned int baud_rate)
 *                      void _disable(void)
 *                      void install_vector(void far *routine,int irq)
 *                      outp(unsigned portid, int byte_value)
 *                      void int_enable(int irq)
 *                      void scu_flush(void)
 *                      void _enable(void)
 *
 *      HEADERS:        <dos.h>
 *                      <conio.h>
 *                      icu.h
 *                      tcu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      Initialize the 8251 to the desired baud rate with an 8 bit word
 *      no parity, and 1 stop bit
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void scu_open(unsigned int baud_rate)
 {
 #if defined(V20)

 unsigned int uBaud;

 #endif
/****************************************************************************
 *
 *      First set up the desired baud rate
 *
 ***************************************************************************/

 #if defined(V45)

	scu_baud(baud_rate);

 #endif

/****************************************************************************
 *
 *      Install interrupt service routine for receive
 *
 ***************************************************************************/

	_disable();             /* disable interrupts till installation complete */

	install_vector(scu_rx_isr,SCU_IRQ);

/****************************************************************************
 *
 *      Initialize the SCU for 8 bit word, no parity and one stop bit
 *
 ***************************************************************************/

 #if defined(V45)

	outp(SMD,(STL_1 | PS_NONE | CL_8 | BF_16));
                                        /* 1 stop, no parity,8 bits, rtclk/16 */

	outp(SCM,(MRDY_NORMAL | CLEAR_ERROR | RXD_ENABLE | TXD_ENABLE));
                                        /* normal ready, clear error, enable Rx and Tx */

	outp(SIMK,(TM_DISABLE | RM_ENABLE));
                                        /* Receive only interrutps */

 #elif defined(V20)

	/* initialize the 8252 UART */

	uBaud = lookup_baud(baud_rate);
	outp(UCR,0x3c); 		/* 8 bit word, no parity, one stop bit */
	//outp(BRSR,B9600);		/* 9600 baud */
	outp(BRSR,uBaud);		/* set baud rate */
	outp(MCR,0x23);         /* Receiver enable */

 #endif

/****************************************************************************
 *
 *      initialize the structure
 *
 ***************************************************************************/

	scu.head= scu.tail= 0;        /* set buffer pointers */
	scu.bufsize= BUFFER_SIZE;      /* set buffer size */

/****************************************************************************
 *
 *      unmask the interrupts at the controller
 *
 ***************************************************************************/

	int_enable(SCU_IRQ);            /* enable receive interrupts at ICU */

	_enable();                      /* enable interrutps to processor */

	scu_flush();                   /* flush input buffer */
 }

 #pragma page(1)

/*
 *  From an unsigned value, make a baud rate divisor
 *  for the 8250 on a PC.
 */
 #if defined(V20)

 unsigned int lookup_baud(unsigned int baud)
 {
 unsigned int uBaud;

	switch(baud) {
		case   300:
			uBaud = B300;
			break;

		case  1200:
			uBaud = B1200;
			break;

		case  2400:
			uBaud = B2400;
			break;

		case  4800:
			uBaud = B4800;
			break;

		case  9600:
			uBaud = B9600;
			break;

		case 19200:
			uBaud = B19200;
			break;

//		case 38400:
//			uBaud = B38400;
//			break;

//		case 57600:
//			uBaud = B57600;
//			break;

//		case 115:
//			uBaud = B115200;
//			break;

		default:
			uBaud = B9600;	break;
		}

	return uBaud;
 }
 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU close routine
 *
 *      DELCARATION:    void scu_close(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Uart receive interrupts disabled
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void _disable(void)
 *                      outp(unsigned portid, int byte_value)
 *                      void int_disable(int irq)
 *                      void _enable(void)
 *
 *      HEADERS:        <dos.h>
 *                      <conio.h>
 *                      config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      This function disables receive interrupts from the SCU. Under these
 *      conditions the processor is prevented from receiveing any incomming
 *      serial data. It can however still transmit.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void scu_close(void)
 {
	_disable();             /* disable interrupts */

	int_disable(SCU_IRQ);	/* mask SCU receive interrupts */

	_enable();              /* enable interrupts */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU receive interrupt service routine
 *
 *      DELCARATION:    void interrupt far scu_rx_isr(void)
 *
 *      PARAMETERS:     Processor's registers
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    Receive interrupt processed by the ICU
 *
 *      EXIT
 *       Conditions:    Received data placed in receive buffer, and incomming
 *                      character buffer pointer is incremented
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    struct comport scu
 *                      unsigned int inp(unsigned portid)
 *                      outp(unsigned portid, int byte_value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      This function is invoked by the hardware whenever a serial character
 *      has been received. We simply reterive the character from the SCU and
 *      place it in the receive buffer and update the tail pointer for the
 *      next incoming character.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void interrupt far scu_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	_enable();		/* enable interrupts */

 #if defined(V45)

	scu.buffer[scu.tail++]=inp(SCU_DATA);
					/* get the character, place in buffer */
 #elif defined(V20)

	scu.buffer[scu.tail++]=inp(UDATA);
					/* get the character, place in buffer */

 #endif

	if(scu.tail == scu.bufsize)     /* check for end of buffer */
		scu.tail = 0;           /* reset buffer pointer */

	outp(ICU_BASE,SCU_EOI);  /* send specific EOI to interrupt controller */
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 * ******** G E N E R A L   U A R T   C H A R   F U N C T I O N S ********
 *
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU Check for Received Character
 *
 *      DELCARATION:    int scu_check(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        TRUE if character waiting, otherwise ERR
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    struct comport scu
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      This routine simply returns TRUE (1) if there are any characters
 *      available in the receive buffer, or ERR (-1) if there are none.
 *
 *      written by Denton Marlowe
 *		(c)1990-1993 HAYES TARGETS
 *
 ************************************************************************@H*/

 #pragma page(1)

 int scu_check(void)
 {
	if(scu.head == scu.tail)
		return(ERR);      /* no character available */

	return(TRUE);             /* yes, we have some */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU polling receive routine
 *
 *      DELCARATION:    int scu_getc(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        character received
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         char c
 *
 *      EXTERNALS
 *       Referenced:    int scu_poll(void)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      Wait indefinitely for a character from the port.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int scu_getc(void)
 {
	int c;

	while(TRUE) {
		if( (c = scu_poll()) != ERR)
			break;
		}
	return(c & 0xff);               /* return character */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU flush input buffer routine
 *
 *      DELCARATION:    void scu_flush(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Uart receive buffer empty
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    int scu_poll(void)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      Throw out characters until there are no more (either
 *      in the ring buffer or waiting at the port).
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void scu_flush(void)
 {
	while( scu_poll() != ERR)
		;                       /* toss the character */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU polling receive routine
 *
 *      DELCARATION:    int scu_poll(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        character waiting in buffer, otherwise ERR
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    None
 *
 *      LOCALS:         unsigned int c
 *
 *      EXTERNALS
 *       Referenced:    struct comport scu
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      Grab a character out of the ring buffer.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 int scu_poll(void)
 {
	unsigned c;                     /* must be integer for return */

	if(scu.head!=scu.tail) {
		c = scu.buffer[scu.head++];
		if(scu.head == scu.bufsize)
			scu.head = 0;
		return(c&0xff);         /* mask high byte and return character */
		}
	return(ERR);                    /* ring buffer empty */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU polling transmit routine
 *
 *      DELCARATION:    void scu_check(int c)
 *
 *      PARAMETERS:     character to be output
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Character output to SCU
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    unsigned int inp(unsigned portid)
 *                      outp(unsigned portid, int byte_value)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      Write a single character to the serial port
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void scu_putc(int c)
 {
 #if defined(V45)

	while((inp(SCU_STAT) & 0x01) == 0) /* wait till SCU register ready */
		;
	outp(SCU_DATA,c);                  /* give the character to the SCU */

 #elif defined(V20)

 static unsigned char ustat = 0;

	while(TRUE)
	{
		ustat = ustat | inp(USR);
		if(ustat & 0x40)
		{                               /* trans buffer is empty */
			outp(UDATA,c);
			ustat = ustat & 0xbf;   /* clear the tx empty bit */
			return;
		}
	}

 #endif

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
 *      8251 SCU string input routine
 *
 *      DELCARATION:    void scu_gets(char *str)
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
 *       Referenced:    int scu_getc(void)
 *                      void scu_putc(int c)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      This function retrieves a string from the serial port. It terminates
 *      on the carriage return character (0x0d). Does NOT recognize a
 *      backspace character.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void scu_gets(char *str)
 {
	int c;

	while((c = scu_getc()) != '\r')         /* Until a carriage return */
		{
		scu_putc(c);                    /* Echo characters typed */
		*str++ = c;                     /* store it in the string */
		}

	*str = '\0';                            /* Terminate the string */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 SCU string output routine
 *
 *      DELCARATION:    void scu_puts(char *str)
 *
 *      PARAMETERS:     pointer to Null terminated character string
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Character string output to SCU
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void scu_putc(int c)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         scu.c
 *
 *      DESCRIPTION:
 *
 *      Write a string to the serial port, calls scu_putc() repeatedly
 *      untill string is exhausted (NULL terminated).
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void scu_puts(char *str)
 {
	while(*str)                      /* till NULL character */
	scu_putc(*str++);                /* output character */
 }
