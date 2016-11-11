/* file: uart.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Universal Asynchronous Receiver-Transmitter (UART) functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      8251 UART functions
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
 *      I/O addresses of UART control registers
 *
 *      The UART_BASE is the address of the 8251 UART on the SBC40. It is
 *      fixed at address 48H on standard boards.
 *
 ***************************************************************************/

 #if defined(V40)

 #if !defined UART_BASE

 #error Base address of UART not defined

 #endif

 #endif

/****************************************************************************
 *
 *      UART definitions
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      I/O addresses of UART control registers
 *
 ***************************************************************************/

 #if defined(V40)

 #define UART_DATA  UART_BASE           /* R/W character */
 #define UART_CNTRL UART_BASE+1         /* uart control */
 #define UART_STAT  UART_BASE+1         /* uart status */

 #endif

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
 *      UART MODE Register definitions - ASYNCHRONOUS MODE
 *
 ***************************************************************************/

 #define CLOCK_1X    B0
 #define CLOCK_16X   B1
 #define CLOCK_64X   (B1 | B0)

 #define CHAR_5      0x00
 #define CHAR_6      B2
 #define CHAR_7      B3
 #define CHAR_8      (B3 | B2)

 #define NO_PARITY   0x00
 #define ODD_PARITY  B4
 #define EVEN_PARITY (B5 | B4)

 #define STOP_1      B6
 #define STOP_1_2    B7
 #define STOP_2      (B7 | B6)

/****************************************************************************
 *
 *      UART COMMAND Register definitions
 *
 ***************************************************************************/

 #define TXD_ENABLE    0X01
 #define TXD_DISABLE   0X00

 #define DTR_ACTIVE    0x02
 #define DTR_INACTIVE  0x00

 #define RXD_ENABLE    0X04
 #define RXD_DISABLE   0X00

 #define SBRK_ACTIVE   0x08
 #define SBRK_INACTIVE 0x00

 #define ERROR_CLEAR   0x10

 #define RTS_ACTIVE    0X20
 #define RTS_INACTIVE  0X00

 #define RESET         B6

 #define ENTER_HUNT PHASE B7    /* SYNC Mode only */

/****************************************************************************
 *
 *      UART STATUS Register definitions
 *
 ***************************************************************************/

 #define TXRDY 0x01
 #define RXRDY 0x02
 #define TXEMP 0x04
 #define PE    0x08
 #define OVE   0x10
 #define FE    0x20
 #define BRK   0x40
 #define DSR   0x80
 #define ERROR (PE | OVE | FE)

/****************************************************************************
 *
 *      UART general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define ERR  -1

/***************************************************************************
 *
 *      UART Baud Rates definitions for 16X clock
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

/****************************************************************************
 *
 *      UART function prototypes
 *
 ***************************************************************************/

 void uart_open(unsigned baud_rate);    /* initialize UART */
 void uart_close(void);                 /* close out uart */
 int uart_check(void);                  /* check for character */
 int uart_getc(void);                   /* get charcter */
 int uart_poll(void);                   /* wait for character */
 void uart_flush(void);                 /* flush characters from receive buffer */
 void uart_putc(int ch);                /* output character to UART */
 void uart_gets(char *str);             /* input string from receive buffer */
 void uart_puts(char *str);             /* output string to UART */

/****************************************************************************
 *
 *      UART function prototype for interrupt service routine
 *
 ***************************************************************************/

 void interrupt far uart_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

/****************************************************************************
 *
 *      UART data structures
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
 *      UART global variables
 *
 ***************************************************************************/

 #if defined(V40)

 comport uart;                          /* comport structure */

 #endif

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
 *      8251 UART initialization routine
 *
 *      DELCARATION:    void uart_open(unsigned int baud_rate)
 *
 *      PARAMETERS:     Required baud rate for UART
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
 *       Referenced:    struct comport uart
 *                      void uart_baud(unsigned int baud_rate)
 *                      void _disable(void)
 *                      void install_vector(void far *routine,int irq)
 *                      outp(unsigned portid, int byte_value)
 *                      void int_enable(int irq)
 *                      void uart_flush(void)
 *                      void _enable(void)
 *
 *      HEADERS:        <dos.h>
 *                      <conio.h>
 *                      icu.h
 *                      tcu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
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

 void uart_open(unsigned int baud_rate)
 {

 #if defined(V40)

/****************************************************************************
 *
 *      First set up the desired baud rate
 *
 ***************************************************************************/

	uart_baud(baud_rate);

/****************************************************************************
 *
 *      reset the UART by writing three zeroes to the command register
 *
 ***************************************************************************/

	outp(UART_CNTRL,0x0);
	outp(UART_CNTRL,0x0);
	outp(UART_CNTRL,0x0);

/****************************************************************************
 *
 *      Install interrupt service routine for receive
 *
 ***************************************************************************/

	_disable();             /* disable interrupts till installation complete */

	install_vector(uart_rx_isr,RX_IRQ);

/****************************************************************************
 *
 *      Initialize the UART for 8 bit word, no parity and one stop bit
 *
 ***************************************************************************/

	uart.mode = STOP_1 | NO_PARITY | CHAR_8 | CLOCK_16X;
	uart.command = ERROR_CLEAR | RXD_ENABLE | TXD_ENABLE;
	uart.status = 0;

	outp(UART_CNTRL,RESET);         /* internal Reset */

	outp(UART_CNTRL,uart.mode);     /* mode word */

	outp(UART_CNTRL,uart.command);  /* command word */

/****************************************************************************
 *
 *      initialize the structure
 *
 ***************************************************************************/

	uart.head= uart.tail= 0;        /* set buffer pointers */
	uart.bufsize= BUFFER_SIZE;      /* set buffer size */

/****************************************************************************
 *
 *      unmask the interrupts at the controller
 *
 ***************************************************************************/

	int_enable(RX_IRQ);             /* enable receive interrupts at ICU */

	_enable();                      /* enable interrutps to processor */

	uart_flush();                   /* flush input buffer */

 #endif

 }

 #pragma page(1)

 #if defined(V40)

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART close routine
 *
 *      DELCARATION:    void uart_close(void)
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
 *      SOURCE:         uart.c
 *
 *      DESCRIPTION:
 *
 *      This function disables receive interrupts from the UART. Under these
 *      conditions the processor is prevented from receiveing any incomming
 *      serial data. It can however still transmit.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void uart_close(void)
 {
	_disable();             /* disable interrupts */

	int_disable(RX_IRQ);    /* mask UART receive interrupts */

	_enable();              /* enable interrupts */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART receive interrupt service routine
 *
 *      DELCARATION:    void interrupt far uart_rx_isr(void)
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
 *       Referenced:    struct comport uart
 *                      unsigned int inp(unsigned portid)
 *                      outp(unsigned portid, int byte_value)
 *
 *      HEADERS:        <conio.h>
 *                      config.h
 *                      icu.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
 *
 *      DESCRIPTION:
 *
 *      This function is invoked by the hardware whenever a serial character
 *      has been received. We simply reterive the character from the UART and
 *      place it in the receive buffer and update the tail pointer for the
 *      next incoming character.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void interrupt far uart_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	_enable();		/* enable interrupts */

	uart.buffer[uart.tail++]=inp(UART_DATA);
				/* get the character, place in buffer */

	if(uart.tail == uart.bufsize) /* check for end of buffer */
		uart.tail = 0;        /* reset buffer pointer */

	uart.status = inp(UART_STAT); /* load status */

	if(uart.status & ERROR != 0)  /* check error flags */
		outp(UART_CNTRL,uart.command);  /* reset error flag */

	outp(ICU_BASE,RX_EOI);  /* send specific EOI to interrupt controller */
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
 *      8251 UART Check for Received Character
 *
 *      DELCARATION:    int uart_check(void)
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
 *       Referenced:    struct comport uart
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
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

 int uart_check(void)
 {
	if(uart.head == uart.tail)
		return(ERR);      /* no character available */

	return(TRUE);             /* yes, we have some */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART polling receive routine
 *
 *      DELCARATION:    int uart_getc(void)
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
 *       Referenced:    int uart_poll(void)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
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

 int uart_getc(void)
 {
	int c;

	while(TRUE) {
		if( (c = uart_poll()) != ERR)
			break;
		}
	return(c & 0xff);               /* return character */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART flush input buffer routine
 *
 *      DELCARATION:    void uart_flush(void)
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
 *       Referenced:    int uart_poll(void)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
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

 void uart_flush(void)
 {
	while( uart_poll() != ERR)
		;                       /* toss the character */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART polling receive routine
 *
 *      DELCARATION:    int uart_poll(void)
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
 *       Referenced:    struct comport uart
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
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

 int uart_poll(void)
 {
	unsigned c;                     /* must be integer for return */

	if(uart.head!=uart.tail) {
		c = uart.buffer[uart.head++];
		if(uart.head == uart.bufsize)
			uart.head = 0;
		return(c&0xff);         /* mask high byte and return character */
		}
	return(ERR);                    /* ring buffer empty */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART polling transmit routine
 *
 *      DELCARATION:    void uart_check(int c)
 *
 *      PARAMETERS:     character to be output
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Character output to UART
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
 *      SOURCE:         uart.c
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

 void uart_putc(int c)
 {
	while((inp(UART_STAT) & 0x01) == 0) /* wait till UART register ready */
		;
	outp(UART_DATA,c);                  /* give the character to the UART */
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
 *      8251 UART string input routine
 *
 *      DELCARATION:    void uart_gets(char *str)
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
 *       Referenced:    int uart_getc(void)
 *                      void uart_putc(int c)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
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

 void uart_gets(char *str)
 {
	int c;

	while((c = uart_getc()) != '\r')        /* Until a carriage return */
		{
		uart_putc(c);                   /* Echo characters typed */
		*str++ = c;                     /* store it in the string */
		}

	*str = '\0';                            /* Terminate the string */
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *      8251 UART string output routine
 *
 *      DELCARATION:    void uart_puts(char *str)
 *
 *      PARAMETERS:     pointer to Null terminated character string
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    Character string output to UART
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void uart_putc(int c)
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         uart.c
 *
 *      DESCRIPTION:
 *
 *      Write a string to the serial port, calls uart_putc() repeatedly
 *      untill string is exhausted (NULL terminated).
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 void uart_puts(char *str)
 {
	while(*str)                 /* till NULL character */
        uart_putc(*str++);          /* output character */
 }

 #endif
