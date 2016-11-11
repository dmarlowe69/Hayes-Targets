/* file: config.h */

/****************************************************************************
 *
 *      Hardware Specific configuration header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *		05/12/94		D.marlowe		Added I486 routines
 *
 ************************************************************************@R*/

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

 #if	!defined(V48)
 #if	!defined(V40)
 #if    !defined(V50)
 #if    !defined(V20)
 #if	!defined(PC)
 #if	!defined(I486)

 #define V40

 #pragma message("Default V40 Processor selected")

 #endif
 #endif
 #endif
 #endif
 #endif
 #endif

/****************************************************************************
 *
 *	Set define for V40 series processors
 *
 ***************************************************************************/

 #if defined(V48)

 #define V40

 #endif

/****************************************************************************
 *
 *      Set define for V40/V50 series processors
 *
 ***************************************************************************/

 #if defined(V40)

 #define V45

 #elif defined(V50)

 #define V45

 #endif

/****************************************************************************
 *
 *      CPU Board Configuration
 *
 ***************************************************************************/

 #if defined(V40)

 #pragma message("V40 Processor selected")

/****************************************************************************
 *
 *      LPM-SBC40 CPU Board Configuration
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      define address of battery backed CMOS ram
 *
 ***************************************************************************/

 #define CMOS_RAM 0X00000300L   /* CMOS battery-backed RAM */

/****************************************************************************
 *
 *      I/O addresses of UART control registers
 *
 *      The UART_BASE is the address of the 8251 UART on the SBC40. It is
 *      fixed at address 48H on standard boards.
 *
 ***************************************************************************/

 #define UART_BASE  0x48                /* 8251 uart base address */

/****************************************************************************
 *
 *      The OKI 6264 Real time clock chip is mapped at address 50H on all
 *      standard SBC40 boards
 *
 ***************************************************************************/

 #define CLOCK_BASE 0x50

/****************************************************************************
 *
 *      I/O addresses of TCU control registers
 *
 *      The TIMER_BASE defines the address of the counter/timer (8254
 *      equivalent) present in the V40 and is controlled by the OPHA register
 *      (FFFCH) and the TULA register (FFF9H).
 *
 ***************************************************************************/

 #define TIMER_BASE 0x80

/****************************************************************************
 *
 *      The SCU_BASE definition is the address of the 8251 equivalent within
 *      the V40. It's address is determined by the OPHA register (FFFCH) and
 *      the IULA register (FFFAH) programmed by the hardware init routine.
 *
 ***************************************************************************/

 #define SCU_BASE  0x90

/****************************************************************************
 *
 *      The DMA_BASE definitions is the address of the 8257 equivalent within
 *      the V40. It's address is determined by the OPHA register (FFFCH) and
 *      the IULA register (FFFAH) programmed by the hardware init routine.
 *
 ***************************************************************************/

 #define DMA_BASE  0xa0

/****************************************************************************
 *
 *      The ICU_BASE definitions is the address of the 8259 equivalent within
 *      the V40. It's address is determined by the OPHA register (FFFCH) and
 *      the IULA register (FFFAH) programmed by the hardware init routine.
 *
 ***************************************************************************/

 #define ICU_BASE 0xb0

/****************************************************************************
 *
 *      ICU base vector
 *
 ***************************************************************************/

 #define ICU_BASE_IR 0xE0

/****************************************************************************
 *
 *      ICU End Of Interrupt Command
 *
 ***************************************************************************/

 #define OCW2_S_EOI 0x60                /* specific EOI command */

/***************************************************************************
 *
 *      ICU interrupt number definitions for LPM-SBC40 CPU
 *
 *      interrupt number 0    ICU (internal - TOUT0)
 *      interrupt number 1    ICU (internal or externel - SINT or SBX INTR1)
 *      interrupt number 2    ICU (external - 6264 calendar clock or TOUT1)
 *      interrupt number 3    ICU (external - INTRQ or J13 pin 2)
 *      interrupt number 4    ICU (external - INTRQ1 or J13 pin 4)
 *      interrupt number 5    ICU (external - INTRQ2 or J13 pin 6)
 *      interrupt number 6    ICU (external - UART RXRDY or J13 pin 8)
 *      interrupt number 7    ICU (external - UART TXRDY or SBX INTR0)
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      TCU Timer 0 Interrupt
 *
 ***************************************************************************/

 #define TCT0_IRQ 0x00          /* TCT0 tied to ICU number 0 */
 #define TCT0_EOI (OCW2_S_EOI | TCT0_IRQ)

/****************************************************************************
 *
 *      SCU Interrupt
 *
 ***************************************************************************/

 #define SCU_IRQ 0x01          /* SCU tied to ICU number 1 */
 #define SCU_EOI (OCW2_S_EOI | SCU_IRQ)

/****************************************************************************
 *
 *      6264 RTC Interrupt
 *
 ***************************************************************************/

 #define CLOCK_IRQ 0x02         /* RTC tied to ICU interrupt number 2 */
 #define CLOCK_EOI (OCW2_S_EOI | CLOCK_IRQ)

/****************************************************************************
 *
 *      A/D conversion complete Interrupt
 *
 ***************************************************************************/

 #define AD_IRQ 0x03            /* A/D tied to ICU interrupt number 3 */
 #define AD_EOI (OCW2_S_EOI | AD_IRQ)

/****************************************************************************
 *
 *      REMTRON TM Interrupt
 *
 ***************************************************************************/

 #define TM_IRQ 0x04            /* CMD tied to ICU interrupt number 4 */
 #define TM_EOI (OCW2_S_EOI | TM_IRQ)

/****************************************************************************
 *
 *      REMTRON CMD Interrupt
 *
 ***************************************************************************/

 #define CMD_IRQ 0x05           /* CMD tied to ICU interrupt number 5 */
 #define CMD_EOI (OCW2_S_EOI | CMD_IRQ)

/****************************************************************************
 *
 *      82C51 UART Receive Interrupt
 *
 ***************************************************************************/

 #define RX_IRQ 0x06            /* UART tied to ICU interrupt number 6 */
 #define RX_EOI (OCW2_S_EOI | RX_IRQ)

/****************************************************************************
 *
 *      82C51 UART Transmit Interrupt
 *
 ***************************************************************************/

 #define TX_IRQ 0x07            /* UART tied to ICU interrupt number 7 */
 #define TX_EOI (OCW2_S_EOI | TX_IRQ)

/****************************************************************************
 *
 *      assign lowest priorty IRQ
 *
 ***************************************************************************/

 #define LOW_IRQ AD_IRQ                 /* assign IR3 lowerest priorty */
                                        /* which makes IR4 highest priorty */
/****************************************************************************
 *
 *      define BASE addresse of on board PPI
 *
 ***************************************************************************/

 #define PPI_BASE 0x4c       /* address of on board ppi */

/****************************************************************************
 *
 *      define BASE addresses PPI's on the iSBX module
 *
 ***************************************************************************/

 #define MCS_0_BASE 0x60     /* address of ppi number 1 on iSBX board */
 #define MCS_1_BASE 0x68     /* address of ppi number 0 on iSBX board */

/****************************************************************************
 *
 *      define BASE address of STATUS LED
 *
 ***************************************************************************/

 #define LED_PORT 0x40       /* LED Port on SBC40 Board */

/****************************************************************************
 *
 *      define BASE address of Watch Dog timer
 *
 ***************************************************************************/

 #define WATCH_DOG 0x44         /* address of watch dog timer port */

/****************************************************************************
 *
 *      define BASE address of IOEXP
 *
 ***************************************************************************/

 //#define IOEXP 0x2000		/* address of active IOEXP control line */
 #define IOEXP 0x0000		/* address of active IOEXP control line */
 #define NO_IOEXP 0x2000	/* address of inactive IOEXP control line */

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #elif defined(V50)

 #pragma message("V50 Processor selected")

/****************************************************************************
 *
 *      LPM-SBC50 CPU Board Configuration
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      The OKI 6264 Real time clock chip is mapped at address 7f80H on all
 *      standard SBC50 boards
 *
 ***************************************************************************/

 #define CLOCK_BASE 0x7f80

/****************************************************************************
 *
 *      I/O addresses of TCU control registers
 *
 *      The TIMER_BASE defines the address of the counter/timer (8254
 *      equivalent) present in the V50 and is controlled by the OPHA register
 *      (FFFCH) and the TULA register (FFF9H).
 *
 ***************************************************************************/

 #define TIMER_BASE 0x80

/****************************************************************************
 *
 *      The SCU_BASE definition is the address of the 8251 equivalent within
 *      the V50. It's address is determined by the OPHA register (FFFCH) and
 *      the IULA register (FFFAH) programmed by the hardware init routine.
 *
 ***************************************************************************/

 #define SCU_BASE  0x90

/****************************************************************************
 *
 *      The DMA_BASE definitions is the address of the 8257 equivalent within
 *      the V40. It's address is determined by the OPHA register (FFFCH) and
 *      the IULA register (FFFAH) programmed by the hardware init routine.
 *
 ***************************************************************************/

 #define DMA_BASE  0xa0

/****************************************************************************
 *
 *      The ICU_BASE definitions is the address of the 8259 equivalent within
 *      the V50. It's address is determined by the OPHA register (FFFCH) and
 *      the IULA register (FFFAH) programmed by the hardware init routine.
 *
 ***************************************************************************/

 #define ICU_BASE 0xb0

/****************************************************************************
 *
 *      ICU base vector
 *
 ***************************************************************************/

 #define ICU_BASE_IR 0xE0

/****************************************************************************
 *
 *      ICU End Of Interrupt Command
 *
 ***************************************************************************/

 #define OCW2_S_EOI 0x60                /* specific EOI command */

/***************************************************************************
 *
 *      ICU interrupt number definitions for LPM-SBC50 CPU
 *
 *      interrupt number 0    ICU (internal - TOUT0)
 *      interrupt number 1    ICU (internal or externel - SINT,TOUT1,RTC or SBX INTR1)
 *      interrupt number 2    ICU (external - SBX INTR0 )
 *      interrupt number 3    ICU (external - coprocessor or STD BUS INTRQ)
 *      interrupt number 4    ICU (external - J4 pin 2)
 *      interrupt number 5    ICU (external - J4 pin 4)
 *      interrupt number 6    ICU (external - J4 pin 6)
 *      interrupt number 7    ICU (external - J4 pin 8)
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      TCU Timer 0 Interrupt
 *
 ***************************************************************************/

 #define TCT0_IRQ 0x00          /* TCT0 tied to ICU number 0 */
 #define TCT0_EOI (OCW2_S_EOI | TCT0_IRQ)

/****************************************************************************
 *
 *      SCU Interrupt
 *
 ***************************************************************************/

 #define SCU_IRQ 0x01          /* SCU tied to ICU number 1 */
 #define SCU_EOI (OCW2_S_EOI | SCU_IRQ)

/****************************************************************************
 *
 *      6264 RTC Interrupt (not normally used)
 *
 ***************************************************************************/

 #define CLOCK_IRQ 0x01         /* RTC tied to ICU interrupt number 2 */
 #define CLOCK_EOI (OCW2_S_EOI | CLOCK_IRQ)

/****************************************************************************
 *
 *      A/D conversion complete Interrupt
 *
 ***************************************************************************/

 #define AD_IRQ 0x03            /* A/D tied to ICU interrupt number 3 */
 #define AD_EOI (OCW2_S_EOI | AD_IRQ)

/****************************************************************************
 *
 *      REMTRON TM Interrupt
 *
 ***************************************************************************/

 #define TM_IRQ 0x05            /* CMD tied to ICU interrupt number 4 */
 #define TM_EOI (OCW2_S_EOI | TM_IRQ)

/****************************************************************************
 *
 *      REMTRON CMD Interrupt
 *
 ***************************************************************************/

 #define CMD_IRQ 0x06           /* CMD tied to ICU interrupt number 5 */
 #define CMD_EOI (OCW2_S_EOI | CMD_IRQ)

/****************************************************************************
 *
 *      assign lowest priorty IRQ
 *
 ***************************************************************************/

 #define LOW_IRQ 0x04                   /* assign IR4 lowerest priorty */
					/* which makes IR5 highest priorty */
/****************************************************************************
 *
 *		82C51 UART Receive Interrupt (taken from v40 code, for dspio)
 *
 ***************************************************************************/

 #define RX_IRQ 0x06            /* UART tied to ICU interrupt number 6 */
 #define RX_EOI (OCW2_S_EOI | RX_IRQ)

/****************************************************************************
 *
 *      define BASE addresse of on board PPI
 *
 ***************************************************************************/

 #define PPI_BASE 0x7fa0     /* address of on board ppi */

/****************************************************************************
 *
 *      define BASE addresses PPI's on the iSBX module
 *
 ***************************************************************************/

 #define MCS_0_BASE 0x7fb0   /* address of ppi number 1 on iSBX board */
 #define MCS_1_BASE 0x7fc0   /* address of ppi number 0 on iSBX board */

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

 #elif defined(V20)

 #pragma message("V20 Processor selected")

/****************************************************************************
 *
 *      LPM-SBC8 CPU Board Configuration
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      I/O addresses of TCU control registers
 *
 *      The TIMER_BASE defines the address of the counter/timer (8254
 *      equivalent) present in the V50 and is controlled by the OPHA register
 *      (FFFCH) and the TULA register (FFF9H).
 *
 ***************************************************************************/

 #define TIMER_BASE 0x80

/****************************************************************************
 *
 *      I/O addresses of UART control registers
 *
 *      The UART_BASE is the address of the 8252 UART on the SBC8. It is
 *      fixed at address 90H on standard boards.
 *
 ***************************************************************************/

 #define SCU_BASE 0x90

/****************************************************************************
 *
 *      The ICU_BASE is the address of the 8259 ICU on the SBC8. It is
 *      fixed at address 88H on standard boards.
 *
 ***************************************************************************/

 #define ICU_BASE 0x88

/****************************************************************************
 *
 *      ICU base vector
 *
 ***************************************************************************/

 #define ICU_BASE_IR 0xE0

/****************************************************************************
 *
 *      ICU End Of Interrupt Command
 *
 ***************************************************************************/

 #define OCW2_S_EOI 0x60                /* specific EOI command */

/***************************************************************************
 *
 *      ICU interrupt number definitions for LPM-SBC8 CPU
 *
 *      interrupt number 0    ICU (TOUT0 or J3 pin 2)
 *      interrupt number 1    ICU (TOUT1 or int 8087)
 *      interrupt number 2    ICU (TOUT2 or J3 pin 4)
 *      interrupt number 3    ICU (82c52 TBRE or J3 pin 6)
 *      interrupt number 4    ICU (82c52 DR or J3 pin 8)
 *      interrupt number 5    ICU (STD BUS INTRQ or J3 pin 10)
 *      interrupt number 6    ICU (82c52 INTR or isbx MINTR0)
 *      interrupt number 7    ICU (STD BUS INTRQ3, isbx MINTR1 or STD BUS INTRQ2)
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      TCU Timer 0 Interrupt
 *
 ***************************************************************************/

 #define TCT0_IRQ 0x00          /* TCT0 tied to ICU number 0 */
 #define TCT0_EOI (OCW2_S_EOI | TCT0_IRQ)

/****************************************************************************
 *
 *      REMTRON TM Interrupt
 *
 ***************************************************************************/

 #define TM_IRQ 0x02            /* CMD tied to ICU interrupt number 2 */
 #define TM_EOI (OCW2_S_EOI | TM_IRQ)

/****************************************************************************
 *
 *      REMTRON CMD Interrupt
 *
 ***************************************************************************/

 #define CMD_IRQ 0x03           /* CMD tied to ICU interrupt number 3 */
 #define CMD_EOI (OCW2_S_EOI | CMD_IRQ)

/****************************************************************************
 *
 *      SCU Interrupt
 *
 ***************************************************************************/

 #define SCU_IRQ 0x04          /* SCU tied to ICU number 4 */
 #define SCU_EOI (OCW2_S_EOI | SCU_IRQ)

/****************************************************************************
 *
 *      A/D conversion complete Interrupt
 *
 ***************************************************************************/

 #define AD_IRQ 0x05            /* A/D tied to ICU interrupt number 5 */
 #define AD_EOI (OCW2_S_EOI | AD_IRQ)

/****************************************************************************
 *
 *      assign lowest priorty IRQ
 *
 ***************************************************************************/

 #define LOW_IRQ 0x01                   /* assign IR1 lowerest priorty */
					/* which makes IR2 highest priorty */
/****************************************************************************
 *
 *      define BASE addresse of on board PPI
 *
 ***************************************************************************/

 #define PPI_BASE 0x0118                /* address ppi on LPM-SBX card */

/****************************************************************************
 *
 *      define BASE addresses PPI's on the iSBX module
 *
 ***************************************************************************/

 #define MCS_0_BASE 0x98   /* address of ppi number 1 on iSBX board */
 #define MCS_1_BASE 0xa0   /* address of ppi number 0 on iSBX board */

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

 #elif defined(PC)

 #pragma message("PC Processor selected")

/****************************************************************************
 *
 *		IBM PC Configuration
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

 #define OCW2_S_EOI 0x20				  /* nonspecific EOI command */

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
 *      A/D conversion complete Interrupt
 *
 ***************************************************************************/

 #define AD_IRQ 0x03			/* A/D tied to ICU interrupt number 3 - com2	*/
 #define AD_VEC 0x0B			/* A/D tied to ICU interrupt number 3 - com2	*/
 #define AD_EOI (OCW2_S_EOI | AD_IRQ)

/****************************************************************************
 *
 *      REMTRON TM Interrupt
 *
 ***************************************************************************/

 #define TM_IRQ 0x05		   /* CMD tied to ICU interrupt number 5 - lpt2	*/
 #define TM_VEC 0x0D		   /* CMD tied to ICU interrupt number 5 - lpt2	*/
 #define TM_EOI (OCW2_S_EOI | TM_IRQ)

/****************************************************************************
 *
 *      REMTRON CMD Interrupt
 *
 ***************************************************************************/

 #define CMD_IRQ 0x07		   /* CMD tied to ICU interrupt number 7 - lpt1	*/
 #define CMD_VEC 0x0F		   /* CMD tied to ICU interrupt number 7 - lpt1	*/
 #define CMD_EOI (OCW2_S_EOI | CMD_IRQ)

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

 #elif defined(I486)

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

 #if defined(V45)

 #define SAMPLE_RATE 100         /* 100 Hz sample rate */

 #elif defined(V20)

 #define SAMPLE_RATE 125         /* 125 Hz sample rate */

 #elif defined(I486)

 #define SAMPLE_RATE 100		 /* 100 Hz sample rate */

 #endif

/****************************************************************************
 *
 *      define analog I/O boards
 *
 ***************************************************************************/

 #define LPM_A_DM12_DC          /* winsystems analog input board */
 #define LPM_D_A12_8_DC         /* winsystems analog output board */

/****************************************************************************
 *
 *      define I/O data bit path size
 *
 ***************************************************************************/

 #if defined(V50)

 #define DATA_16
 
 #pragma message("WARNING - 16 bit I/O instructions not allowed")

 #else

 #pragma message("16 bit I/O instructions allowed")

 #endif

/****************************************************************************
 *
 *      define A/D board I/O base address
 *
 ***************************************************************************/

 #if defined(PC)

 #define A_D_BASE 0x13c			/* base I/O address of A/D board */

 #elif defined(I486)

 #define A_D_BASE 0x13c			/* base I/O address of A/D board */

 #else

 #define A_D_BASE 0x3c			/* base I/O address of A/D board */

 #endif

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

 #if defined(PC)

 #define D_A_BASE 0x100				   /* I/O base address of D/A board */

 #elif defined(I486)

 #define D_A_BASE 0x100				   /* I/O base address of D/A board */

 #else

 #define D_A_BASE 0x00                 /* I/O base address of D/A board */

 #endif

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

 #if defined(PC)

 #define STATUS_BASE_ADDRESS	(IOEXP+0x20)

 #elif defined(I486)

 #define STATUS_BASE_ADDRESS	(IOEXP+0x20)

 #else

 #define STATUS_BASE_ADDRESS	0x20

 #endif

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

 #if defined(PC)

 #define RLC_BASE_ADDRESS	(IOEXP+0x10)

 #elif defined(I486)

 #define RLC_BASE_ADDRESS	(IOEXP+0x10)

 #else

 #define RLC_BASE_ADDRESS	0x10

 #endif

/****************************************************************************
 *
 *      configuration macros
 *
 ***************************************************************************/

 #if defined(V40)

 #define com_puts(x) uart_puts(x)
 #define com_getc()  uart_getc()
 #define com_gets(x) uart_gets(x)
 #define com_putc(x) uart_putc(x)
 #define com_check() uart_check()

 #elif defined(V50)

 #define com_puts(x) scu_puts(x)
 #define com_getc()  scu_getc()
 #define com_gets(x) scu_gets(x)
 #define com_putc(x) scu_putc(x)
 #define com_check() scu_check()

 #elif defined(V20)

 #define com_puts(x) scu_puts(x)
 #define com_getc()  scu_getc()
 #define com_gets(x) scu_gets(x)
 #define com_putc(x) scu_putc(x)
 #define com_check() scu_check()

 #endif
