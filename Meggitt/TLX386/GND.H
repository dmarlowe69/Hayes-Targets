/* file: platform.h */

/****************************************************************************
 *
 *	test for ROM or RAM based code
 *
 ***************************************************************************/

 #ifndef PC

 #if defined(ROM_BASED)

 #pragma message("ROM BASED CODE")

 #else

 #pragma message("RAM BASED CODE")

 #endif

 #else

 #pragma message("PC BASED CODE")

 #endif

/****************************************************************************
 *
 *	Test for 240KHz encoder flag
 *
 ***************************************************************************/

 #if defined(FAST_ENCODER)

 #pragma message("USING 240KHz ENCODER")

 #else

 #pragma message("USING STANDARD ENCODER")

 #endif

/****************************************************************************
 *
 *	End of Configuration Definitions
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Debug using LPM-STATUS Definitions
 *
 ***************************************************************************/

 #define DEBUG_STATUS_BANK			0
 #define DEBUG_ERROR_COUNT_BANK		1

 #define DEBUG_TM_DATA_BANK 		2
 #define DEBUG_CMD_DATA_BANK		3

 #define DEBUG_CMD_TM_LED_ERROR		0
 #define DEBUG_CMD_TM_LED_STATUS	1

/****************************************************************************
 *
 *	Local Definitions
 *
 ***************************************************************************/

 #define TRUE 1

 void far brain(void);

 void a_d_config(void); 	/* configure A/D data structure */
 void d_a_config(void); 	/* configure D/A data structure */

 void hardware_config(void);
 void init_brain(void);
 void far brain(void);
 void far house_keep(void);
 void init_systems(void);

 void ramtest(void);

/****************************************************************************
 *
 *	cmd/tm functions
 *
 ***************************************************************************/

 void init_cmd_tm_interface(void);
 void decoder(int on_off);
 void encoder(int on_off);
 void far data_cmd_application(void);
 void far data_tm_application(void);
 void far data_cmd_irq(void);
 void far data_tm_irq(void);
/* file: config.h */

/****************************************************************************
 *
 *      Hardware Specific configuration header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	Define Configuration Flag
 *
 ***************************************************************************/

 #define CONFIG

/****************************************************************************
 *
 *      Check Memory Model
 *
 ***************************************************************************/

 #if defined(M_I86SM)

 #pragma message("Small Memory Model")

 #elif defined(M_I86CM)

 #pragma message("Compact Memory Model")

 #elif defined(M_I86MM)

 #pragma message("Medium Memory Model")

 #elif defined(M_I86LM)

 #pragma message("Large Memory Model")

 #endif

/****************************************************************************
 *
 *      Check CPU Board Configuration and assign default if none selected
 *
 ***************************************************************************/

 #if    !defined(PC)
 #if	!defined(I486)

 #define I486

 #pragma message("Default I486 Processor selected")

 #endif
 #endif

/****************************************************************************
 *
 *      CPU Board Configuration
 *
 ***************************************************************************/


 #if defined(I486)

 #pragma message("i486 Processor selected")

/****************************************************************************
 *
 *	I486/IBM PC Configuration
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	The ICU_BASE definitions is the address of the 8259 ICU
 *
 ***************************************************************************/

 #define ICU_BASE		0x20
 #define ICU_SLAVE_BASE 0xA0

/****************************************************************************
 *
 *      ICU base vector
 *
 ***************************************************************************/

 #define ICU_BASE_IR		0x08
 #define ICU_SLAVE_BASE_IR	0x70

/****************************************************************************
 *
 *      ICU End Of Interrupt Command
 *
 ***************************************************************************/

 #define OCW2_NS_EOI		0x20	/* non-specific EOI command */
 #define OCW2_S_EOI			0x60	/* specific EOI command */

 //#define EOI		OCW2_NS_EOI		/* specific EOI command */
 #define EOI		OCW2_S_EOI		/* specific EOI command */

 //#define SLAVE_EOI	OCW2_NS_EOI		/* nospecific EOI command */
 #define SLAVE_EOI	OCW2_S_EOI		/* nospecific EOI command */

 //#define MASTER_EOI OCW2_NS_EOI		/* nospecific EOI command */
 #define MASTER_EOI OCW2_S_EOI		/* nospecific EOI command */

/***************************************************************************
 *
 *		ICU interrupt number definitions for PC
 *
 *		interrupt number 0	  ICU timer 0
 *		interrupt number 1	  ICU keyboard
 *		interrupt number 2	  ICU slave 8259
 *		interrupt number 3	  ICU com2
 *		interrupt number 4	  ICU com1
 *		interrupt number 5	  ICU lpt2
 *		interrupt number 6	  ICU floppy disk
 *		interrupt number 7	  ICU lpt1
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      TCU Timer 0 Interrupt
 *
 ***************************************************************************/

 #define TCT0_IRQ 0x00          /* TCT0 tied to ICU number 0 */
 #define TCT0_EOI (EOI | TCT0_IRQ)

/****************************************************************************
 *
 *		Key Board 1 Interrupt
 *
 ***************************************************************************/

 #define KYBRD_IRQ 0x02			/* Key Board tied to ICU number 1 */
 #define KYBRD_EOI (EOI | KYBRD_IRQ)

/****************************************************************************
 *
 *		Slave ICU 2 Interrupt
 *
 ***************************************************************************/

 #define ICU_IRQ 0x02			/* Slave ICU tied to ICU number 2 */
 #define ICU_EOI (EOI | ICU_IRQ)

/****************************************************************************
 *
 *		82C50 UART Receive Interrupt (comm 1)
 *
 ***************************************************************************/

 #define COMM1_IRQ 0x04			/* UART tied to ICU interrupt number 4 */
 #define COMM1_EOI (EOI | COMM1_IRQ)

/****************************************************************************
 *
 *		82C50 UART Receive Interrupt (comm 2)
 *
 ***************************************************************************/

 #define COMM2_IRQ 0x03			/* UART tied to ICU interrupt number 3 */
 #define COMM2_EOI (EOI | COMM2_IRQ)

/****************************************************************************
 *
 *      A/D conversion complete Interrupt
 *
 ***************************************************************************/

 #define AD_IRQ 0x07		   /* A/D tied to ICU interrupt number 7 */
 #define AD_EOI (EOI | AD_IRQ)

/****************************************************************************
 *
 *      6264 RTC Interrupt (not normally used)
 *
 ***************************************************************************/

 #define CLOCK_IRQ 0x08		   /* RTC tied to ICU #2 interrupt number 0 */
 #define CLOCK_EOI (SLAVE_EOI | (CLOCK_IRQ&0x7))

/****************************************************************************
 *
 *      REMTRON TM Interrupt
 *
 ***************************************************************************/

 #define TM_IRQ 0x05		   /* CMD tied to ICU interrupt number 5 */
 #define TM_EOI (EOI | TM_IRQ)

/****************************************************************************
 *
 *      REMTRON CMD Interrupt
 *
 ***************************************************************************/

 #define CMD_IRQ 0x06		   /* CMD tied to ICU interrupt number 6 */
 #define CMD_EOI (EOI | CMD_IRQ)

/****************************************************************************
 *
 *      assign lowest priorty IRQ
 *
 ***************************************************************************/

 //#define LOW_IRQ 0x07			/* assign IR7 lowerest priorty */
 //								/* which makes IR0 highest priorty */

 #define LOW_IRQ 0x04			/* assign IR4 lowerest priorty */
								/* which makes IR5 highest priorty */

 #define LOW_SLAVE_IRQ 0x07		/* assign IR7 lowerest priorty */
								/* which makes IR0 highest priorty */

/****************************************************************************
 *
 *	Real TIme Clock
 *
 ***************************************************************************/

 #define CLOCK_BASE 0x70

/****************************************************************************
 *
 *	I/O addresses of TCU control registers
 *
 ***************************************************************************/

 #define TIMER_BASE 0x40

/****************************************************************************
 *
 *      define BASE address of Watch Dog timer
 *
 ***************************************************************************/

 #define WATCH_DOG_INIT 0x1d0	/* address of watch dog timer port */
 #define WATCH_DOG_KICK 0x1d8	/* address of watch dog timer port */

/****************************************************************************
 *
 *		define BASE addresses PPI's on the PC/104 module
 *
 ***************************************************************************/

 #define PCM_BASE	0x178			/* address of ppi number 1 on iSBX board */
 #define MCS_0_BASE PCM_BASE		/* address of ppi number 1 on iSBX board */
 #define MCS_1_BASE (PCM_BASE+4)	/* address of ppi number 0 on iSBX board */

/****************************************************************************
 *
 *      define BASE address of IOEXP
 *
 ***************************************************************************/

 #define IOEXP 0x0100		/* address of active IOEXP control line */

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #endif

/****************************************************************************
 *
 *      Other Configuration Information
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      application loop timer rate
 *
 ***************************************************************************/

 #define SAMPLE_RATE 100		 /* 100 Hz sample rate */

/****************************************************************************
 *
 *      define analog I/O boards
 *
 ***************************************************************************/

 #define LPM_A_DM12_DC          /* winsystems analog input board */
 #define LPM_D_A12_8_DC         /* winsystems analog output board */
 #define LPM_AIO_DC             /* winsystems analog output board */

/****************************************************************************
 *
 *      define I/O data bit path size
 *
 ***************************************************************************/

 #pragma message("16 bit I/O instructions allowed")

/****************************************************************************
 *
 *      define A/D board I/O base address
 *
 ***************************************************************************/

 #define A_D_BASE 0x13c         /* base I/O address of A/D board */

/****************************************************************************
 *
 *      define A/D board input range and gain factor
 *
 **************************************************************************/

 #define A_D_MAX  10            /* max input volts */
 #define A_D_MIN -10            /* min input volts */
 #define A_D_ZERO 0x0           /* zero volts into A/D */
 #define A_D_GAIN  1            /* gain of input amplifier */

/****************************************************************************
 *
 *      define D/A board I/O base address
 *
 ***************************************************************************/

 #define D_A_BASE 0x100                /* I/O base address of D/A board */

/****************************************************************************
 *
 *      define D/A board output range and gain factor
 *
 **************************************************************************/

 #define D_A_MAX  10            /* max input volts */
 #define D_A_MIN -10            /* min input volts */
 #define D_A_ZERO 0x0800        /* zero volts out of D/A */
 #define D_A_GAIN  1            /* gain of output amplifier */

/****************************************************************************
 *
 *	Polarity Definitions
 *
 ***************************************************************************/

 #define UNIPOLAR 0
 #define BIPOLAR  1

/****************************************************************************
 *
 *	Calibration Loop defintions
 *
 ***************************************************************************/

 #define D_A_CAL_LOOP 7

/****************************************************************************
 *
 *      define D/A board I/O addresses and output ranges
 *
 ***************************************************************************/

 #define D_A_CHANNEL_0 D_A_BASE		/* I/O address for D/A channel 0 */
 #define D_A_CH_0_LOWBYTE D_A_BASE	/* I/O address for D/A channel 0 low byte */
 #define D_A_CH_0_HIGHBYTE D_A_BASE+1	/* I/O address for D/A channel 0 high byte */
 #define D_A_CH_0_ZERO 0x800		/* zero volts */
 #define D_A_CH_0_MAX 10		/* max output volts */
 #define D_A_CH_0_MIN -10		/* min output volts */
 #define D_A_CH_0_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_1 D_A_BASE+2	/* I/O address for D/A channel 1 */
 #define D_A_CH_1_LOWBYTE D_A_BASE+2	/* I/O address for D/A channel 1 low byte */
 #define D_A_CH_1_HIGHBYTE D_A_BASE+3	/* I/O address for D/A channel 1 high byte */
 #define D_A_CH_1_ZERO 0x800		/* zero volts */
 #define D_A_CH_1_MAX 10		/* max output volts */
 #define D_A_CH_1_MIN -10		/* min output volts */
 #define D_A_CH_1_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_2 D_A_BASE+4	/* I/O address for D/A channel 2 */
 #define D_A_CH_2_LOWBYTE D_A_BASE+4	/* I/O address for D/A channel 2 low byte */
 #define D_A_CH_2_HIGHBYTE D_A_BASE+5	/* I/O address for D/A channel 2 high byte */
 #define D_A_CH_2_ZERO 0x800		/* zero volts */
 #define D_A_CH_2_MAX 10		/* max output volts */
 #define D_A_CH_2_MIN -10		/* min output volts */
 #define D_A_CH_2_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_3 D_A_BASE+6	/* I/O address for D/A channel 3 */
 #define D_A_CH_3_LOWBYTE D_A_BASE+6	/* I/O address for D/A channel 3 low byte */
 #define D_A_CH_3_HIGHBYTE D_A_BASE+7	/* I/O address for D/A channel 3 high byte */
 #define D_A_CH_3_ZERO 0x800		/* zero volts */
 #define D_A_CH_3_MAX 10		/* max output volts */
 #define D_A_CH_3_MIN -10		/* min output volts */
 #define D_A_CH_3_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_4 D_A_BASE+8	/* I/O address for D/A channel 4 */
 #define D_A_CH_4_LOWBYTE D_A_BASE+8	/* I/O address for D/A channel 4 low byte */
 #define D_A_CH_4_HIGHBYTE D_A_BASE+9	/* I/O address for D/A channel 4 high byte */
 #define D_A_CH_4_ZERO 0x800		/* zero volts */
 #define D_A_CH_4_MAX 10		/* max output volts */
 #define D_A_CH_4_MIN -10		/* min output volts */
 #define D_A_CH_4_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_5 D_A_BASE+10	/* I/O address for D/A channel 5 */
 #define D_A_CH_5_LOWBYTE D_A_BASE+10	/* I/O address for D/A channel 5 low byte */
 #define D_A_CH_5_HIGHBYTE D_A_BASE+11	/* I/O address for D/A channel 5 high byte */
 #define D_A_CH_5_ZERO 0x800		/* zero volts */
 #define D_A_CH_5_MAX 10		/* max output volts */
 #define D_A_CH_5_MIN -10		/* min output volts */
 #define D_A_CH_5_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_6 D_A_BASE+12	/* I/O address for D/A channel 6 */
 #define D_A_CH_6_LOWBYTE D_A_BASE+12	/* I/O address for D/A channel 6 low byte */
 #define D_A_CH_6_HIGHBYTE D_A_BASE+13	/* I/O address for D/A channel 6 high byte */
 #define D_A_CH_6_ZERO 0x800		/* zero volts */
 #define D_A_CH_6_MAX 10		/* max output volts */
 #define D_A_CH_6_MIN -10		/* min output volts */
 #define D_A_CH_6_POLARITY BIPOLAR	/* bipolar output */

 #define D_A_CHANNEL_7 D_A_BASE+14	/* I/O address for D/A channel 7 */
 #define D_A_CH_7_LOWBYTE D_A_BASE+14	/* I/O address for D/A channel 7 low byte */
 #define D_A_CH_7_HIGHBYTE D_A_BASE+15	/* I/O address for D/A channel 7 high byte */
 #define D_A_CH_7_ZERO 0x800		/* zero volts */
 #define D_A_CH_7_MAX 10		/* max output volts */
 #define D_A_CH_7_MIN -10		/* min output volts */
 #define D_A_CH_7_POLARITY BIPOLAR	/* bipolar output */

/****************************************************************************
 *
 *	LPM-SBX-1
 *
 ***************************************************************************/

 //#define LPM_SBX_BASE 0x140
 #define LPM_SBX_BASE 0x180

/****************************************************************************
 *
 *      LPM-STATUS
 *
 **************************************************************************/


 #define STATUS_BASE_ADDRESS	(IOEXP+0x20)

/****************************************************************************
 *
 *      LPM-DSPI/O
 *
 **************************************************************************/

 #define COMM1_BASE 0x3f8
 #define COMM2_BASE 0x2f8
 #define COMM3_BASE 0x3e8
 #define COMM4_BASE 0x2e8
 #define LPT1_BASE  0x378
 #define LPT2_BASE  0x278

 #define COMM_PORT_0_BASE	COMM1_BASE
 #define COMM_PORT_1_BASE	COMM2_BASE
 #define PRINTER_PORT_BASE	LPT1_BASE

/****************************************************************************
 *
 *      MATRIX-RLC
 *
 **************************************************************************/

 #define RLC_BASE_ADDRESS   (IOEXP+0x10)

/****************************************************************************
 *
 *      configuration macros
 *
 ***************************************************************************/
/* file: dspio.h */

/***************************************************************************
 *
 *      UART header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/****************************************************************************
 *
 *	UART function prototypes
 *
 ***************************************************************************/

 void dspio_1_open(unsigned baud_rate);	/* initialize UART */
 void dspio_1_close(void);				/* close out uart */
 int dspio_1_check(void);				/* check for character */
 int dspio_1_getc(void);				/* get charcter */
 int dspio_1_poll(void);				/* wait for character */
 void dspio_1_flush(void);				/* flush characters from receive buffer */
 void dspio_1_putc(int ch);				/* output character to UART */
 void dspio_1_gets(char *str);			/* input string from receive buffer */
 void dspio_1_puts(char *str);			/* output string to uart */

 void dspio_2_open(unsigned baud_rate);	/* initialize UART */
 void dspio_2_close(void);				/* close out uart */
 int dspio_2_check(void);				/* check for character */
 int dspio_2_getc(void);				/* get charcter */
 int dspio_2_poll(void);				/* wait for character */
 void dspio_2_flush(void);				/* flush characters from receive buffer */
 void dspio_2_putc(int ch);				/* output character to UART */
 void dspio_2_gets(char *str);			/* input string from receive buffer */
 void dspio_2_puts(char *str);			/* output string to uart */

 static int dspio_makebaud(unsigned baud);

/****************************************************************************
 *
 *	lpt function prototypes
 *
 ***************************************************************************/

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
 int toggle_bit(int bit0_7);
/* file: icu.h */

/***************************************************************************
 *
 *      BIRD ICU header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

 #ifndef _INC_ICU

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      ICU general definitions
 *
 ***************************************************************************/

 #define ENABLE 0
 #define DISABLE 1

/****************************************************************************
 *
 *      ICU End Of Interrupt Command
 *
 ***************************************************************************/

 #define OCW2_NS_EOI			0x20	/* non-specific EOI command */
 #define OCW2_NOP               0x40    /* no operation */
 #ifndef OCW2_S_EOI
 #define OCW2_S_EOI             0x60    /* specific EOI command */
 #endif

/****************************************************************************
 *
 *      ICU End Of Interrupt (EOI) macro
 *
 ***************************************************************************/

 #define _EOI(irq) outp(ICU_BASE,(OCW2_S_EOI | irq))

 #define _EOI_MASTER(irq) outp(ICU_BASE,(OCW2_S_EOI | irq))

 #define _EOI_SLAVE(irq) outp(ICU_SLAVE_BASE,(OCW2_S_EOI | (irq&0x7)))

/****************************************************************************
 *
 *      ICU function prototypes
 *
 ***************************************************************************/

 void init_icu(void);           /* initializes interrupt controler unit */
 void install_vector(void (interrupt far *routine)(),int irq);
				/* install isr address for specified irq */
 void int_enable(int irq);      /* enable specific interrupt */
 void int_disable(int irq);     /* disable specific interrupt */
 void int_disable_all(void);    /* disable all interrupts at icu */
 int int_toggle(int irq);       /* toggle interrupt mask */
 int int_status(int irq);       /* get enable/disable status of irq */

 void set_vector(void (interrupt far *routine)(),int irq);
				/* install isr address for specified int */
 volatile void (interrupt far *get_vector(unsigned int irq))();
				/* return pointer to given interrupt function */
 void init_exception();         /* initialize exception vectors */

/****************************************************************************
 *
 *      ICU function prototype for interrupt service routine
 *
 ***************************************************************************/

 void interrupt far dummy_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 #ifdef SLAVE_ICU

 void interrupt far dummy_slave_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
 #endif

 void interrupt far incomplete_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 void interrupt far exception_0(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 void interrupt far exception_1(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 void interrupt far exception_2(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 void interrupt far exception_3(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 void interrupt far exception_4(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);

 #define _INC_ICU
 #endif	/* _INC_ICU */
/* file: analog.h */

/****************************************************************************
 *
 *	ANALOG hardware header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      A/D input function prototypes
 *
 ***************************************************************************/

 void init_A_D(void);                           /* initialize A/D interrupt routine */
 void de_init_A_D(void);

 void interrupt far A_D_int_handler(void);		/* A/D input interrupt service routine */
 int test_A_D(char mode,volatile struct watch_AD *wad);
						/* test A/D converter */
 int a_d_get(unsigned int);                     /* input 12 bit value from A/D card */
 void sensor_scan(struct A_D_INPUT *a_d_input); /* scan all input channels */
 void sensor_poll(struct A_D_INPUT *a_d_input); /* scan all input channels */

 void interrupt far A_D_int_handler(void);      /* A/D input interrupt service routine */
 void interrupt far A_D_U_int_handler(void);	/* A/D input interrupt service routine */

 extern void (__interrupt __far *a_d_old_vector)();
												/* pointer to old IRQ handler */

/****************************************************************************
 *
 *      define data structures for A/D
 *
 ***************************************************************************/

 struct watch_AD {
		signed int seed;
		signed int inc;
		signed int seed_max;
		signed int seed_min;
		signed int input;
		signed int time_out;
		unsigned int output;
		unsigned int channel;
		};

 struct ANALOG_TO_DIGITIAL {
		signed int A_D[16];             /* input data */
		unsigned int channel;           /* channel number */
		unsigned int MUX_ADDR;		/* I/O addresses */
		unsigned int STATUS_ADDR;	/* I/O addresses */
		unsigned int DATA_ADDR;		/* I/O addresses */
		unsigned int ZERO;		/* input for zero volts */
		signed int MAX;                 /* maximum input voltage */
		signed int MIN;                 /* minimum input voltage */
		signed int GAIN;		/* input gain factor */
		signed int POLARITY;		/* channel polarity */
		};

/****************************************************************************
 *
 *	Polarity Definitions
 *
 ***************************************************************************/

 #define UNIPOLAR 0
 #define BIPOLAR  1

/****************************************************************************
 *
 *	Calibration Loop defintions
 *
 ***************************************************************************/

 #define D_A_CAL_LOOP 7

/****************************************************************************
 *
 *      D/A output function prototypes
 *
 ***************************************************************************/

 void D_A_init(void);
 unsigned int D_A_cal_out(signed int word);
 void D_A_channel_out(unsigned int d_a_channel, unsigned data);

/****************************************************************************
 *
 *      define data structures  for D/A
 *
 ***************************************************************************/

 struct DIGITIAL_TO_ANALOG {
		unsigned int D_A;	/* output data */
		unsigned int ADDR;	/* I/O addresses */
		unsigned int ZERO;	/* output for zero volts */
		signed int MAX;		/* maximum output voltage */
		signed int MIN;		/* minimum output voltage */
		signed int POLARITY;	/* channel polarity */
		};

/****************************************************************************
 *
 *      define data structures for sensor inputs (assignment is externel)
 *
 ***************************************************************************/

 #ifndef ADINPUT

 struct A_D_INPUT {
    signed int a_d[16];
    };

 struct A_D_INPUT_BYTE {
    unsigned char a_d_byte[32];
    };

 #define ADINPUT

 #endif

/****************************************************************************
 *
 *      general definitions
 *
 ***************************************************************************/

 #define TIME_OUT 4096
 #define INVALID  4096
 #define OK   0
 #define ERR -1

/****************************************************************************
 *
 *      define data which will be externel to other modules
 *
 ***************************************************************************/

 extern volatile struct watch_AD wad;

 extern volatile struct ANALOG_TO_DIGITIAL adc;

 extern volatile struct DIGITIAL_TO_ANALOG dac[8];
/* file: ground.h */

/****************************************************************************
 *
 *      BIRD Ground test functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_GROUND

/****************************************************************************
 *
 *	remove defintions from Hayes.h
 *
 ***************************************************************************/

 #undef com_puts
 #undef com_getc
 #undef com_gets
 #undef com_putc
 #undef com_check

/****************************************************************************
 *
 *	check build configuration
 *
 ***************************************************************************/

 //#ifdef PC
 #ifdef PC_CONSOLE

 #pragma message("Using PC STDIN and STDOUT for console I/O")

 #include	<conio.h>
 #include	<stdio.h>

 #define com_puts(x) _cputs(x)
 #define com_getc() getch()
 #define com_gets(x) gets(x)

 #else

 #define UART_PTR

 #if defined(UART_PTR)

 #pragma message("Using Comm_port Function Pointers")

 #ifdef I486_8250
 #define com_open(x,y,z) (*com_open_ptr)(x,y,z)
 #else
 #define com_open(x) (*com_open_ptr)(x)
 #endif
 #define com_close() (*com_close_ptr)()
 #define com_check() (*com_check_ptr)()
 #define com_getc() (*com_getc_ptr)()
 #define com_poll() (*com_poll_ptr)()
 #define com_flush() (*com_flush_ptr)()
 #define com_putc(x) (*com_putc_ptr)(x)
 #define com_gets(x) (*com_gets_ptr)(x)
 #define com_puts(x) (*com_puts_ptr)(x)

 #endif

 #endif // ifdef PC

/***************************************************************************
 *
 *      general defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

/****************************************************************************
 *
 *	function pointers
 *
 ***************************************************************************/

 //#ifndef PC
 #ifndef PC_CONSOLE

 #ifdef I486_8250
 extern void (*com_open_ptr)(unsigned baud_rate, unsigned parms,unsigned port);
 #else
 extern void (*com_open_ptr)(unsigned baud_rate);	/* initialize UART */
 #endif
 extern void (*com_close_ptr)(void);				/* close out uart */
 extern int (*com_check_ptr)(void);					/* check for character */
 extern int (*com_getc_ptr)(void);					/* get charcter */
 extern int (*com_poll_ptr)(void);					/* wait for character */
 extern void (*com_flush_ptr)(void);				/* flush characters from receive buffer */
 #ifdef I486_8250
 extern void (*com_putc_ptr)(char ch);				/* output character to UART */
 #else
 extern void (*com_putc_ptr)(int ch);				/* output character to UART */
 #endif
 extern void (*com_gets_ptr)(char *str);			/* input string from receive buffer */
 extern void (*com_puts_ptr)(char *str);			/* output string to UART */

 #endif

/****************************************************************************
 *
 *      Ground function prototypes
 *
 ***************************************************************************/

 #ifndef PC

 void init_gnd(int port);	/* setup required comm port */

 #endif

 #define _INC_GROUND
 #endif	/* _INC_GROUND */
/* file: menu_ana.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      ANALOG serial interface function prototypes
 *
 ***************************************************************************/

 void analog_menu(void);        /* analog functions menu */
 void display_AD(void);         /* display A/D input data */
 void verify_AD(void);          /* verify A/D operation */
 void calibrate_AD(void);       /* calibrate A/D */
 void display_DA(void);         /* display D/A output data */
 void calibrate_DA(void);	/* calibrate D/A */

 void a_d_config_display(void); /* display A/D data structure */
 void d_a_config_display(void); /* display D/A data structure */
/* file: menu_utl.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      General Serial utilites function prototypes
 *
 ***************************************************************************/

 void int_to_binary(int num,char *str); 	/* 8 bit integer to binary */
 void int_to_binary12(int num,char *str);	/* 12 bit integer to binary */
 void new_line(void);				/* CRLF */
 void pause(void);				/* press any key */
/* file: rom.h */

/***************************************************************************
 *
 *      ROMable functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      ROMable function prototypes
 *
 ***************************************************************************/


 extern void (*printfptr)(char *string);				/* function pointer */
 void printf_init(void (*ptr)(char *string));

 int sprintf(char *string,char const *format, ...); /* same as microsoft function */
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

/****************************************************************************
 *
 *      ROMable function prototypes - Code in file ST.ASM (startup code)
 *
 ***************************************************************************/

 void exit(int exit_status);                        /* same as microsoft function */
/* file: speaker.c */

 void sound_main(void);
 void sound_beeps(int beeps);
 void sound_beep(void);
 void init_sound(unsigned frequency);
 void adjust_sound(unsigned frequency);
 void adjust_sound_union(unsigned frequency);
 void sound_on(void);
 void sound_off(void);
 void sound_delay(unsigned ms_count);
 //void timed_delay(unsigned count);
 int timed_delay(unsigned count);
 unsigned get_count(void);
 void counter_init(void);

/****************************************************************************
 *
 *	sound globals
 *
 ***************************************************************************/

 extern char song[];
 extern char *songpointer;
 extern int octave;
 extern int duration;
 extern int pitch;
 extern int songindex;

 extern void (interrupt far *funptr_sound)(void);	/* pointer to function */

 void main_playsong(void);
 void playtune(char *song);
 void kill_playtune(void);
 void interrupt far beNoisy(void);
 void DoSomething(void);

 void beNoisy_fun(void);

/****************************************************************************
 *
 *	sound functions
 *
 ***************************************************************************/

 void main_laser(void);
 void laser(void);

 void main_siren(void);
 void siren(void);

 void main_music(void);
 void tone(int frequency);

 void main_hearing(void);
 void hear_tone(int frequency);
/* file: terminal.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *       COMM Port test function prototypes
 *
 ***************************************************************************/

 void term(void);			/* terminal loop-back test */
 int com_break(void);			/* test comm ports for access */
/* file: menu_rst.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      hardware reset functions
 *
 ***************************************************************************/

 void reset_menu(void);                 /* reset functions */
/* file: menu_rlc.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	RLC serial interface function prototypes
 *
 ***************************************************************************/

 void rlc_menu(void);
 void rlc_status(void);
 void rlc_test(void);
 void rlc_report(int status);
 void rlc_config_display(void);
 void rlc_sequence_test(void);
/* file: menu_gnd.h */

/****************************************************************************
 *
 *      BIRD Ground test functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/


/****************************************************************************
 *
 *      Ground function prototypes
 *
 ***************************************************************************/

 void gnd_menu(void);		/* UART command processor */
/* file: menu_sbx.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	LPM-SBX function prototypes
 *
 ***************************************************************************/

 void lpm_sbx_menu(void);
 void loop_LPM_SBX(void);
 void lpm_sbx_config_display(void);
/* file: menu_tm.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      CMD function pointer to command decoder function
 *
 ***************************************************************************/

 extern void (far *funptr_decoder_application)(void);	/* pointer to function */

/****************************************************************************
 *
 *      1553 serial interface function prototypes
 *
 ***************************************************************************/

 void cmd_tm_menu(void);		/* CMD/TM functions menu */
 void display_CMD(void);        /* display CMD data */
 void calibrate_CMD(void);      /* calibrate CMD data */
 void display_decode(void);     /* display and decode CMD data */
 void display_TM(void);         /* display TM data */
 void calibrate_TM(void);       /* calibrate TM data */

 void install_decoder_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_decoder_application(void);
							/* dummy main timing loop application */
/* file: menu_sta.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	STATUS serial interface function prototypes
 *
 ***************************************************************************/

 void status_menu(void);
 void status_status(void);
 void status_test(void);
 void led_report(int status);
 void status_config_display(void);
 void status_sequence_test(void);
/* file: menu_rtc.h */

/****************************************************************************
 *
 *	Ground test functions header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      RTC serial interface function prototypes
 *
 ***************************************************************************/

 void rtc_menu(void);           /* RTC functions menu */
 void set_time(void);           /* set time */
 void set_date(void);           /* set data */
 void display_clock(void);      /* display clock */
/* file: menu_pio.h */

 void pio_menu(void);
/* tcu.h */

/****************************************************************************
 *
 *      BIRD Timer/Counter Unit (TCU) header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
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

/****************************************************************************
 *
 *      TCU function prototypes
 *
 ***************************************************************************/

 void init_timer(unsigned int baud_rate);       /* setup main timing loop interrupt */
 void install_application(void (far *routine)());
						/* install pointer to main timing loop application */
 void far dummy_application(void);              /* dummy main timing loop application */

/****************************************************************************
 *
 *      TCU data structure
 *
 ***************************************************************************/

 struct TCU_TABLE {
	unsigned int ticks;             /* timing loop counter */
	};

 extern struct TCU_TABLE tcu_table; /* define data structre */
/* file: clock.h */

/****************************************************************************
 *
 *      Hardware Specific configuration header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
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

/****************************************************************************
 *
 *      The CLOCK_RATE define is the frequency of the signal being applied to
 *      the internal counter/timer of the V40 and is dependent upon not only
 *      the master clock frquency but also upon the value programmed into the
 *      TCKS register (address FFF0H) which holds the prescale value.
 *
 *      The prescale value is set in the STARTUP code file HARDINIT.INC, and
 *      is currently programmed to a value of 0x02.
 *
 ***************************************************************************/

 #if !defined(CLOCK)

 #define CLOCK 10

 #pragma message("default clock speed - 10Mz")

 #endif

/****************************************************************************
 *
 *	Set define for V40 series processors
 *
 ***************************************************************************/

 #if defined(V40)

/****************************************************************************
 *
 *      LPM-SBC40 CPU Board Configuration
 *
 ***************************************************************************/

 #if CLOCK == 5

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 2500000L	/* 8 MHz / PRESCALE */

 #pragma message("V40 - 5Mz")

 #elif CLOCK == 8

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 4000000L	/* 8 MHz / PRESCALE */

 #pragma message("V40 - 8Mz")

 #elif CLOCK == 10

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 5000000L    /* 10 MHz / PRESCALE */

 #pragma message("V40 - 10Mz")

 #endif

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(V50)

/****************************************************************************
 *
 *      LPM-SBC50 CPU Board Configuration
 *
 ***************************************************************************/

 #define PRESCALE 2L		/* Prescale in TCKS register */

 #define CLOCK_RATE 5000000L    /* 10 MHz / PRESCALE */

 #pragma message("V50 - 10Mz")

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(V20)

/****************************************************************************
 *
 *      LPM-SBC8 CPU Board Configuration
 *
 ***************************************************************************/

 #define PRESCALE 1L			/* Prescale in TCKS register */

 #define CLOCK_RATE 8000000L	/* 8 MHz*/

 #pragma message("V20 - 8Mz")

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(I486) || defined(PC)

/****************************************************************************
 *
 *		LPM-486SLC CPU Board Configuration
 *
 ***************************************************************************/

 #define PRESCALE 1L			/* Prescale in TCKS register */

 #define CLOCK_RATE 1193181L	/* standard PC rate */

 #pragma message("I486 - STD PC Rate")

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #endif
/* file: analog.h */

/****************************************************************************
 *
 *	ANALOG hardware header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      A/D input function prototypes
 *
 ***************************************************************************/

 void init_A_D(void);                           /* initialize A/D interrupt routine */
 void de_init_A_D(void);

 void interrupt far A_D_int_handler(void);		/* A/D input interrupt service routine */
 int test_A_D(char mode,volatile struct watch_AD *wad);
						/* test A/D converter */
 int a_d_get(unsigned int);                     /* input 12 bit value from A/D card */
 void sensor_scan(struct A_D_INPUT *a_d_input); /* scan all input channels */
 void sensor_poll(struct A_D_INPUT *a_d_input); /* scan all input channels */

 void interrupt far A_D_int_handler(void);      /* A/D input interrupt service routine */
 void interrupt far A_D_U_int_handler(void);	/* A/D input interrupt service routine */

 extern void (__interrupt __far *a_d_old_vector)();
												/* pointer to old IRQ handler */

/****************************************************************************
 *
 *      define data structures for A/D
 *
 ***************************************************************************/

 struct watch_AD {
		signed int seed;
		signed int inc;
		signed int seed_max;
		signed int seed_min;
		signed int input;
		signed int time_out;
		unsigned int output;
		unsigned int channel;
		};

 struct ANALOG_TO_DIGITIAL {
		signed int A_D[16];             /* input data */
		unsigned int channel;           /* channel number */
		unsigned int MUX_ADDR;		/* I/O addresses */
		unsigned int STATUS_ADDR;	/* I/O addresses */
		unsigned int DATA_ADDR;		/* I/O addresses */
		unsigned int ZERO;		/* input for zero volts */
		signed int MAX;                 /* maximum input voltage */
		signed int MIN;                 /* minimum input voltage */
		signed int GAIN;		/* input gain factor */
		signed int POLARITY;		/* channel polarity */
		};

/****************************************************************************
 *
 *	Polarity Definitions
 *
 ***************************************************************************/

 #define UNIPOLAR 0
 #define BIPOLAR  1

/****************************************************************************
 *
 *	Calibration Loop defintions
 *
 ***************************************************************************/

 #define D_A_CAL_LOOP 7

 #ifdef LPM_AIO_DC
 #pragma message("AIO A/D CONFIGURATION")
 #undef D_A_CAL_LOOP
 #define D_A_CAL_LOOP 1
 #endif

/****************************************************************************
 *
 *      D/A output function prototypes
 *
 ***************************************************************************/

 void D_A_init(void);
 unsigned int D_A_cal_out(signed int word);
 void D_A_channel_out(unsigned int d_a_channel, unsigned data);

/****************************************************************************
 *
 *      define data structures  for D/A
 *
 ***************************************************************************/

 struct DIGITIAL_TO_ANALOG {
		unsigned int D_A;	/* output data */
		unsigned int ADDR;	/* I/O addresses */
		unsigned int ZERO;	/* output for zero volts */
		signed int MAX;		/* maximum output voltage */
		signed int MIN;		/* minimum output voltage */
		signed int POLARITY;	/* channel polarity */
		};

/****************************************************************************
 *
 *      define data structures for sensor inputs (assignment is externel)
 *
 ***************************************************************************/

 #ifndef ADINPUT

 struct A_D_INPUT {
    signed int a_d[16];
    };

 struct A_D_INPUT_BYTE {
    unsigned char a_d_byte[32];
    };

 #define ADINPUT

 #endif

/****************************************************************************
 *
 *      general definitions
 *
 ***************************************************************************/

 #define TIME_OUT 4096
 #define INVALID  4096
 #define OK   0
 #define ERR -1

/****************************************************************************
 *
 *      define data which will be externel to other modules
 *
 ***************************************************************************/

 extern volatile struct watch_AD wad;

 extern volatile struct ANALOG_TO_DIGITIAL adc;

 extern volatile struct DIGITIAL_TO_ANALOG dac[8];
