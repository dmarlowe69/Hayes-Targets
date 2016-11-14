//  file: AIOTCU.C

//////////////////////////////////////////////////////////////////////////////
//
//      AIO Timer/Counter Unit (TCU) functions
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include Header file definitions
//

 #include   <dos.h>
 #include   <stdio.h>
 #include   <conio.h>
 #include   "config.h"
 #include   "borland.h"

//
//      Timer Clock Rate Defintions
//

 #define PRESCALE 1L			/* Prescale in TCKS register */

 #define AIO_CLOCK_RATE 100000L /* standard PC rate */

//
//      TCU I/O port addresses
//

 #define AIO_BASE   0x0300
 #define AIO_TIMER_BASE AIO_BASE+12
 #define TCT0    AIO_TIMER_BASE     /* Lower address of timer unit 0 */
 #define TCT1    AIO_TIMER_BASE+1   /* Lower address of timer unit 1 */
 #define TCT2    AIO_TIMER_BASE+2   /* Lower address of timer unit 2 */
 #define TCTCMD  AIO_TIMER_BASE+3   /* Lower address of command register */

//
//      TCU command words
//

 #define SC_0 0x00              /* select counter 0 */
 #define SC_1 0x40              /* select counter 1 */
 #define SC_2 0x80              /* select counter 2 */
 #define SC_READ_BACK 0xc0      /* read-back command */

 #define RW_LATCH 0x00          /* counter latch command */
 #define RW_LSB 0x10            /* read/write least significant byte only */
 #define RW_MSB 0x20            /* read/write most significant byte only */
 #define RW_LSB_MSB 0x30        /* read/write LSB first, then MSB */

 #define MODE_0 0x00            /* counter mode 0 */
 #define MODE_1 0x02            /* counter mode 1 */
 #define MODE_2 0x04            /* counter mode 2 */
 #define MODE_3 0x06            /* counter mode 3 */
 #define MODE_4 0x08            /* counter mode 4 */
 #define MODE_5 0x0a            /* counter mode 5 */

 #define BCD_BINARY 0x00        /* binary counter - 16 bits */
 #define BCD_BCD 0x01           /* binary coded decimal (4 decades) */

//
//      TCU function prototypes
//

 void init_aio_timer(void);
 void aio_timer(unsigned int baud_rate);
 void deinit_aio_timer(void);

//////////////////////////////////////////////////////////////////////////////
//
//      Timer/counter initialization - 82c54 Compatiable
//
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_aio_timer(void)
 {
//
//      Setup Counter/Timer Configuration
//
    outp(AIO_BASE+10,0xF2); //11110010b

//
//      Select Page 0 I/O on AIO Board
//

    outp(AIO_BASE+8,0x00);

 }
//////////////////////////////////////////////////////////////////////////////
//
//      Timer/counter Frequency Output - 82c54 Compatiable
//
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void aio_timer(unsigned int baud_rate)
 {
	int baud;               /* timer frequency */
//
//      calculate the necessary divisor given the desired baud rate and the
//      input frequency to the counter/timer
//

    baud =(unsigned) (AIO_CLOCK_RATE / (long) baud_rate);

//
//      Select Page 0 I/O on AIO Board
//

    outp(AIO_BASE+8,0x00);

//
//      timer zero - sample rate timer
//

	outp(TCTCMD,SC_0 | RW_LSB_MSB | MODE_3 | BCD_BINARY);
				/* setup sample time counter */
				/* timer 0,lo/hi, mode 0,binary */

	outp(TCT0,(baud & 0xff));       /* low byte of counter */
	outp(TCT0,(baud >> 8) & 0xff);  /* high byte of counter */

 }
//////////////////////////////////////////////////////////////////////////////
//
//      Timer/counter De-Initialization - 82c54 Compatiable
//
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void deinit_aio_timer(void)
{

}
