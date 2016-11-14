//	file: dspio.C

//////////////////////////////////////////////////////////////////////////////
//
//      DSPIO UART functions
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include definitions
//

 #include       <stdio.h>
 #include       <stdarg.h>

 #include       <dos.h>
 #include       <conio.h>

 #include       "config.h"
 #include       "icu.h"

//
//      I/O addresses of UART control registers
//

 #define DSPIO_COMM1 0x3F8              /* com 1 */
 #define DSPIO_COMM2 0x2F8              /* com 2 */

 #define DSPIO_COMM3 0x3E8              /* com 3 */
 #define DSPIO_COMM4 0x2E8              /* com 4 */

 #define DSPIO_LPT1  0x378              /* lpt1 */
 #define DSPIO_LPT2  0x278				/* lpt2 */

 #define DSPIO_COMM_BASE_1      DSPIO_COMM1
 #define DSPIO_COMM_BASE_2      DSPIO_COMM2
 #define DSPIO_LPT_BASE 		DSPIO_LPT2

//
//      general bit position definitions
//

 #define B0 0x01
 #define B1 0x02
 #define B2 0x04
 #define B3 0x08
 #define B4 0x10
 #define B5 0x20
 #define B6 0x40
 #define B7 0x80

//
//      UART general defintions
//

 #define TRUE  1
 #define ERR  -1


 #define DATAREG_1              (DSPIO_COMM_BASE_1)
 #define BAUDLOW_1              (DSPIO_COMM_BASE_1)
 #define BAUDHIGH_1             (DSPIO_COMM_BASE_1+1)
 #define INTENABLE_1    (DSPIO_COMM_BASE_1+1)
 #define INTID_1                (DSPIO_COMM_BASE_1+2)
 #define LINECONTROL_1  (DSPIO_COMM_BASE_1+3)
 #define MODEMCONTROL_1 (DSPIO_COMM_BASE_1+4)
 #define LINESTATUS_1   (DSPIO_COMM_BASE_1+5)
 #define MODEMSTATUS_1  (DSPIO_COMM_BASE_1+6)

 #define DATAREG_2              (DSPIO_COMM_BASE_2)
 #define BAUDLOW_2              (DSPIO_COMM_BASE_2)
 #define BAUDHIGH_2             (DSPIO_COMM_BASE_2+1)
 #define INTENABLE_2    (DSPIO_COMM_BASE_2+1)
 #define INTID_2                (DSPIO_COMM_BASE_2+2)
 #define LINECONTROL_2  (DSPIO_COMM_BASE_2+3)
 #define MODEMCONTROL_2 (DSPIO_COMM_BASE_2+4)
 #define LINESTATUS_2   (DSPIO_COMM_BASE_2+5)
 #define MODEMSTATUS_2  (DSPIO_COMM_BASE_2+6)

//
//      Stop/Data/Parity Bits for 8250 UART
//
//      Bits 43210
//               ---xx  Data Bits (00=5, 01=6, 10=7, 11=8)
//               --x--  Stop Bits (0=1, 1=2)
//               xx---  Parity (00=None, 01=Odd, 11=Even)
//

 #define N81            3
 #define E71            26

 #define DEFPARMS       N81

//
//      8250 (PC) UART Baud Rate Divisors
//

 #define B300  384
 #define B1200  96
 #define B2400  48
 #define B4800  24
 #define B9600  12
 #define B19200  6
 #define B38400  3
 #define B57600  2
 #define B115200 1                      /* highest PC clock rate */

//
//      UART function prototypes
//

 void dspio_1_open(unsigned baud_rate); /* initialize UART */
 void dspio_1_close(void);                              /* close out uart */
 int dspio_1_check(void);                               /* check for character */
 int dspio_1_getc(void);                                /* get charcter */
 int dspio_1_poll(void);                                /* wait for character */
 void dspio_1_flush(void);                              /* flush characters from receive buffer */
 void dspio_1_putc(int ch);                             /* output character to UART */
 void dspio_1_gets(char *str);                  /* input string from receive buffer */
 void dspio_1_puts(char *str);                  /* output string to uart */

 void dspio_2_open(unsigned baud_rate); /* initialize UART */
 void dspio_2_close(void);                              /* close out uart */
 int dspio_2_check(void);                               /* check for character */
 int dspio_2_getc(void);                                /* get charcter */
 int dspio_2_poll(void);                                /* wait for character */
 void dspio_2_flush(void);                              /* flush characters from receive buffer */
 void dspio_2_putc(int ch);                             /* output character to UART */
 void dspio_2_gets(char *str);                  /* input string from receive buffer */
 void dspio_2_puts(char *str);                  /* output string to uart */

 static int dspio_makebaud(unsigned baud);

//
//      UART function prototype for interrupt service routine
//

 void interrupt far dspio_1_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 void interrupt far dspio_2_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

//
//      UART data structures
//

//
//      The BUFFER_SIZE equate defines the size of the serial port
//      receive buffer. In order for the code to work as written this
//      value must be a power of two and less than 65536.
//

 #define BUFFER_SIZE    256     /* DO NOT CHANGE THIS */

 typedef struct {
	unsigned char mode;
	unsigned char command;
	unsigned char status;
	unsigned int  head,tail;
	unsigned int  bufsize;
	unsigned char buffer[BUFFER_SIZE];
	} comport;

//
//      UART global variables
//

 comport dspio_1;                                       /* comport structure */

 comport dspio_2;                                       /* comport structure */

//
//      LPT Port Addresses
//

 #define DSPIO_DATA				(DSPIO_LPT_BASE)
 #define READ_DATA				(DSPIO_LPT_BASE)
 #define READ_STATUS			(DSPIO_LPT_BASE+1)
 #define READ_CNTRL 			(DSPIO_LPT_BASE+2)

 #define WRITE_DATA 			(DSPIO_LPT_BASE)
 #define WRITE_CNTRL			(DSPIO_LPT_BASE+2)

 #define READ_STATUS_BUSY		B7
 #define READ_STATUS_ACK		B6
 #define READ_STATUS_PE 		B5
 #define READ_STATUS_SLCT		B4
 #define READ_STATUS_ERROR		B3

 #define READ_CNTRL_IRQ_ENB     B4
 #define READ_CNTRL_SLIN		B3
 #define READ_CNTRL_IRQ_INIT    B2
 #define READ_CNTRL_AUTOFD		B1
 #define READ_CNTRL_STROBE		B0

 #define WRITE_CNTRL_IRQ_ENB    B4
 #define WRITE_CNTRL_SLIN		B3
 #define WRITE_CNTRL_IRQ_INIT   B2
 #define WRITE_CNTRL_AUTOFD     B1
 #define WRITE_CNTRL_STROBE     B0

//
//  PIO data structures
//

 typedef struct {
	unsigned char port;
	} lptport;

//
//  PIO global variables
//

 lptport lpt_1;										/* comport structure */

//
//      lpt function prototypes
//

 int dspio_init_port(void);
 int dspio_assign_lpt_port(unsigned int port);

 void dspio_outp(unsigned char);
 unsigned char dspio_inp(void);

 void dspio_clear_all_bits(void);
 void dspio_set_all_bits(void);
 void dspio_set_bit(unsigned char bit0_7);
 void dspio_clear_bit(unsigned char bit0_7);
 void dspio_set_bit_pattern(unsigned char pattern);
 void dspio_set_masked_bits(unsigned char mask);
 void dspio_clear_masked_bits(unsigned char mask);
 unsigned char dspio_read_output_byte(void);
 unsigned char dspio_read_output_bit(unsigned char bit0_7);
 unsigned char dspio_input_byte(void);
 unsigned char dspio_input_bit(unsigned char bit0_7);

 int dspio_toggle_bit(int bit0_7);

//
//  Embedded Printf Commands
//

 #define MAXSTR 256               /* maximum string length used by printf */

 void (*dspio_printfptr)(char *string);             /* function pointer */
 void dspio_printf_init(void (*ptr)(char *string));
 int dspio_printf(char const *,...);                /* same as microsoft function */

//////////////////////////////////////////////////////////////////////////////
//
//
//              ********** 8 2 5 0       U A R T   F U N C T I O N S **********
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_1_open(unsigned baud)
 {
 unsigned parms;

//
//      First set up the desired baud rate
//

	parms = DEFPARMS;

	baud = dspio_makebaud(baud);

//
//      Install interrupt service routine for receive
//

	_disable();             /* disable interrupts till installation complete */

	install_vector(dspio_1_rx_isr,COMM1_IRQ);

//
//      initialize the structure
//

	dspio_1.head= dspio_1.tail= 0;  /* set buffer pointers */
	dspio_1.bufsize= BUFFER_SIZE;   /* set buffer size */

//
//      Initialize the UART for 8 bit word, no parity and one stop bit
//

	outp(LINECONTROL_1,0x80);                 /* set high bit, so we can set baud */
	outp(BAUDLOW_1,baud & 0xFF);      /* baud rate */
	outp(BAUDHIGH_1,baud >> 8);
	outp(LINECONTROL_1,parms);                /* stop/data/parity bits */

	outp(MODEMCONTROL_1,0x0B);                /* enable interrupts (bit3), and pins high */
	outp(INTENABLE_1,0x01);                   /* enable receive interrupt */

//
//      unmask the interrupts at the controller
//

	int_enable(COMM1_IRQ);                  /* enable receive interrupts at ICU */

	_enable();                      /* enable interrutps to processor */

	dspio_1_flush();                                /* flush input buffer */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_1_close(void)
 {
	_disable();             /* disable interrupts */

	int_disable(COMM1_IRQ); /* mask UART receive interrupts */

	_enable();              /* enable interrupts */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far dspio_1_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
    _enable();                              /* enable interrupts */

	dspio_1.buffer[dspio_1.tail++]=inp(DATAREG_1);
				/* get the character, place in buffer */

	if(dspio_1.tail >= dspio_1.bufsize)             /* check for end of buffer */
		dspio_1.tail = 0;                                       /* reset buffer pointer */

	//dspio_1.status = inp(UART_STAT);              /* load status */

	//if(dspio_1.status & ERROR != 0)               /* check error flags */
		//outp(UART_CNTRL,dspio_1.command);     /* reset error flag */

	outp(ICU_BASE,COMM1_EOI);  /* send specific EOI to interrupt controller */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
// ******** G E N E R A L   U A R T   C H A R   F U N C T I O N S ********
//
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_1_check(void)
 {
	if(dspio_1.head == dspio_1.tail)
		return(ERR);                    /* no character available */

	return(TRUE);                                           /* yes, we have some */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_1_getc(void)
 {
	int c;

	while(TRUE) {
		if( (c = dspio_1_poll()) != ERR)
			break;
		}
	return(c & 0xff);                                       /* return character */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_1_flush(void)
 {
	while( dspio_1_poll() != ERR)
		;                                                               /* toss the character */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_1_poll(void)
 {
	unsigned c;                                             /* must be integer for return */

	if(dspio_1.head!=dspio_1.tail) {
		c = dspio_1.buffer[dspio_1.head++];
		if(dspio_1.head >= dspio_1.bufsize)
			dspio_1.head = 0;
		return(c&0xff);                 /* mask high byte and return character */
		}
	return(ERR);                                            /* ring buffer empty */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_1_putc(int c)
 {
	unsigned char val;

	while(TRUE) {
		val=inp(LINESTATUS_1);
		if(val & 0x20) {                /* trans buffer is empty */
			outp(DATAREG_1,c);
			return;
			}
		}
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_1_gets(char *str)
 {
	int c;

	while((c = dspio_1_getc()) != '\r')     /* Until a carriage return */
		{
		dspio_1_putc(c);                                /* Echo characters typed */
		*str++ = c;                     /* store it in the string */
		}

	*str = '\0';                                            /* Terminate the string */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_1_puts(char *str)
 {
	while(*str)                                             /* till NULL character */
		dspio_1_putc(*str++);                   /* output character */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//  ********** 8 2 5 0       U A R T   F U N C T I O N S **********
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_2_open(unsigned baud)
 {
 unsigned parms;

//
//      First set up the desired baud rate
//

	parms = DEFPARMS;

	baud = dspio_makebaud(baud);

//
//      Install interrupt service routine for receive
//

	_disable();             /* disable interrupts till installation complete */

	install_vector(dspio_2_rx_isr,COMM2_IRQ);

//
//      initialize the structure
//

	dspio_2.head= dspio_2.tail= 0;  /* set buffer pointers */
	dspio_2.bufsize= BUFFER_SIZE;   /* set buffer size */

//
//      Initialize the UART for 8 bit word, no parity and one stop bit
//

	outp(LINECONTROL_2,0x80);                 /* set high bit, so we can set baud */
	outp(BAUDLOW_2,baud & 0xFF);      /* baud rate */
	outp(BAUDHIGH_2,baud >> 8);
	outp(LINECONTROL_2,parms);                /* stop/data/parity bits */

	outp(MODEMCONTROL_2,0x0B);                /* enable interrupts (bit3), and pins high */
	outp(INTENABLE_2,0x01);                   /* enable receive interrupt */

//
//      unmask the interrupts at the controller
//

	int_enable(COMM2_IRQ);                  /* enable receive interrupts at ICU */

	_enable();                      /* enable interrutps to processor */

	dspio_2_flush();                                /* flush input buffer */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_2_close(void)
 {
	_disable();             /* disable interrupts */

	int_disable(COMM2_IRQ); /* mask UART receive interrupts */

	_enable();              /* enable interrupts */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far dspio_2_rx_isr(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
    _enable();                              /* enable interrupts */

	dspio_2.buffer[dspio_2.tail++]=inp(DATAREG_2);
				/* get the character, place in buffer */

	if(dspio_2.tail >= dspio_2.bufsize)             /* check for end of buffer */
		dspio_2.tail = 0;                                       /* reset buffer pointer */

	//dspio_2.status = inp(UART_STAT);              /* load status */

	//if(dspio_2.status & ERROR != 0)               /* check error flags */
		//outp(UART_CNTRL,dspio_2.command);     /* reset error flag */

	outp(ICU_BASE,COMM2_EOI);  /* send specific EOI to interrupt controller */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
// ******** G E N E R A L   U A R T   C H A R   F U N C T I O N S ********
//
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_2_check(void)
 {
	if(dspio_2.head == dspio_2.tail)
		return(ERR);                    /* no character available */

	return(TRUE);                                           /* yes, we have some */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_2_getc(void)
 {
	int c;

	while(TRUE) {
		if( (c = dspio_2_poll()) != ERR)
			break;
		}
	return(c & 0xff);                                       /* return character */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_2_flush(void)
 {
	while( dspio_2_poll() != ERR)
		;                                                               /* toss the character */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_2_poll(void)
 {
	unsigned c;                                             /* must be integer for return */

	if(dspio_2.head!=dspio_2.tail) {
		c = dspio_2.buffer[dspio_2.head++];
		if(dspio_2.head >= dspio_2.bufsize)
			dspio_2.head = 0;
		return(c&0xff);                 /* mask high byte and return character */
		}
	return(ERR);                                            /* ring buffer empty */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_2_putc(int c)
 {
	unsigned char val;

	while(TRUE) {
		val=inp(LINESTATUS_2);
		if(val & 0x20) {                /* trans buffer is empty */
			outp(DATAREG_2,c);
			return;
			}
		}
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_2_gets(char *str)
 {
	int c;

	while((c = dspio_2_getc()) != '\r')     /* Until a carriage return */
		{
		dspio_2_putc(c);                                /* Echo characters typed */
		*str++ = c;                     /* store it in the string */
		}

	*str = '\0';                                            /* Terminate the string */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_2_puts(char *str)
 {
	while(*str)                                             /* till NULL character */
		dspio_2_putc(*str++);                   /* output character */
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 static int dspio_makebaud(unsigned baud)
 {
	switch(baud) {
		case   300: baud = B300;        break;
		case  1200: baud = B1200;       break;
		case  2400: baud = B2400;       break;
		case  4800: baud = B4800;       break;
		case  9600: baud = B9600;       break;
		case 19200: baud = B19200;      break;
		case 38400: baud = B38400;      break;
		case 57600: baud = B57600;      break;
		case 115:   baud = B115200; break;
		default:        baud = B9600;   break;
		}
	return baud;
 }
//////////////////////////////////////////////////////////////////////////////
//
//
// ******** G E N E R A L    P I O    F U N C T I O N S ********
//
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_init_port(void)
 {
    dspio_assign_lpt_port(DSPIO_LPT1);
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_assign_lpt_port(unsigned int port)
 {
	lpt_1.port = port;
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_outp(unsigned char data)
 {
	outp(WRITE_DATA,data);
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 unsigned char dspio_inp(void)
 {
	return(inp(READ_DATA));
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void dspio_clear_all_bits(void)
 {
	outp(DSPIO_DATA, 0x00); /* all pins = 0 VDC */
 }

 void dspio_set_all_bits(void)
 {
	outp(DSPIO_DATA, 0xFF); /* all pins = 5 VDC */
 }

 void dspio_set_bit(unsigned char bit0_7)
 {
	dspio_set_masked_bits((unsigned char)(0x01 << bit0_7));
 }

 void dspio_clear_bit(unsigned char bit0_7)
 {
	dspio_clear_masked_bits((unsigned char)(0x01 << bit0_7));
 }

 void dspio_set_bit_pattern(unsigned char pattern)
 {
	outp(DSPIO_DATA, pattern);
 }

 void dspio_set_masked_bits(unsigned char mask)
 {
	outp(DSPIO_DATA, mask | inp(DSPIO_DATA));
 }

 void dspio_clear_masked_bits(unsigned char mask)
 {
	outp(DSPIO_DATA,(mask ^ 0xff) & inp(DSPIO_DATA));
 }

 unsigned char dspio_read_output_byte(void)
 {
	return inp(DSPIO_DATA);
 }

 unsigned char dspio_read_output_bit(unsigned char bit0_7)
 {
	return(inp(DSPIO_DATA) & (0x01 << bit0_7) ? 1:0);
 }

 unsigned char dspio_input_byte(void)
 {
 unsigned char inbyte;

	// Read STATUS port, keep bits 7,6,5,4,3
	inbyte = inp(DSPIO_DATA + 1) & 0xf8;

	// Read CONTROL port, keep bits 2,1,0
	inbyte |= inp(DSPIO_DATA + 2) & 0x07;

	// convert active low inputs to active high
	inbyte ^= 0x83;

	return(inbyte);
 }

 unsigned char dspio_input_bit(unsigned char bit0_7)
 {
	return(dspio_input_byte() & (0x01 << bit0_7) ? 1:0);
 }

 int dspio_toggle_bit(int bit0_7)
 {
 unsigned char state;

	state = inp(DSPIO_DATA) & (0x01 << bit0_7) ? 1:0;

	if(state == 0)
		dspio_set_masked_bits((unsigned char)(0x01 << bit0_7));
	else
		dspio_clear_masked_bits((unsigned char)(0x01 << bit0_7));
 }

//
//  Embedded Printf Commands
//

//////////////////////////////////////////////////////////////////////////////
//
//      ROMable printf - formated string output routine
//
//      DELCARATION:    int dspio_printf(char const *format, ...)
//
//      PARAMETERS:     character string and/or formatting specifications
//
//      RETURNS:        Length of output string
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    Formated string output to UART
//
//      LOCALS:         Char string[MAXSTR]
//                      va_list arglist
//                      int retval
//                      FILE str
//
//      EXTERNALS
//       Referenced:    int outfile(...)
//                      uart_puts(char *string) or scu_puts(char *string)
//
//      INCLUDES:       <stdio.h>
//                      <stdarg.h>
//
//      WARNINGS:       None
//
//      SOURCE:         rom.c
//
//      DESCRIPTION:
//
//      Use printf to write character strings and values of C variables,
//      formated in a specified manner, to the UART. See the description
//      of the microsoft library function printf for further details.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 int dspio_printf(char const *format, ...)
 {
	char string[MAXSTR];
	va_list arglist;
	int retval;
	FILE str;

	FILE *outfile = &str;

	va_start(arglist, format);

	outfile->_flag = _IOWRT|_IOSTRG;
	outfile->_ptr = outfile->_base = string;
	outfile->_cnt = MAXSTR;

	retval = _output(outfile,format,arglist);

	string[retval] = '\0';

    if(dspio_printfptr != NULL)
        (dspio_printfptr)(string);
    else
		dspio_1_puts(string);

	return(retval);
 }

//
//
//
 void dspio_printf_init(void (*ptr)(char *string))
 {
    dspio_printfptr = ptr;
 }
