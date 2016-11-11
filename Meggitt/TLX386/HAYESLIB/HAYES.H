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
/* file: cmos.h */

/***************************************************************************
 *
 *      BIRD CMOS RAM header file
 *
 *      written by denton marlowe
 *      (c)1989,1990 Hayes Targets
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
 *      CMOS definitions
 *
 ***************************************************************************/

 #define OK   0
 #define ERR -1

/****************************************************************************
 *
 *      cmos ram structure
 *
 ***************************************************************************/

 typedef volatile struct {
	int checksum;
	int seconds;
	int minutes;
	int hours;
	int day;
	int month;
	int year;
	int stat0;
	int stat1;
	int stat2;
	int stat3;
	} cmos;

/****************************************************************************
 *
 *      define pointer to structure
 *      cmos ram structure is FAR DATA (not in our DATA SEGMENT)
 *
 ***************************************************************************/

 extern cmos far *cmosptr;           /* define pointer to far structure */

/****************************************************************************
 *
 *      CMOS RAM functions prototypes
 *
 ***************************************************************************/

 void init_cmos(void);

 int battery_backup_check(void);
 void battery_backup_set(void);
/* file: hardware.h */

/****************************************************************************
 *
 *	hardware header file
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_HARDWARE

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	Define Hardware configuration options
 *
 ***************************************************************************/

 #define RAM_ENV	0x00
 #define ROM_ENV	0x01
 #define PC_ENV 	0x02

/****************************************************************************
 *
 *      Hardware initialization function prototypes
 *
 ***************************************************************************/

 void init_hardware(unsigned int configuration);	/* initialize hardware */

 void de_init_hardware(unsigned int configuration);	/* deinitialize hardware */

 void reset_boot(); 								/* cold boot */

 void (interrupt far *funptr_reset)(void);			/* pointer to function */

 #define _INC_HARDWARE
 #endif /* ifndef _INC_HARDWARE */
/* file: v40init.h */

/****************************************************************************
 *
 *      v40 hardware header file
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
 *      V40 Hardware initialization function prototypes
 *
 ***************************************************************************/

 void v40_init(void);           /* initialize V40 specific hardware */
 void v20_init(void);           /* initialize V20 specific hardware */
/* file: led.h */

/****************************************************************************
 *
 *      LED header file
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
 *      general hardware definitions
 *
 ***************************************************************************/

 #define LED_ON  1
 #define LED_OFF 0

 #ifdef V50

 #define LED_BIT 0

 #endif

/****************************************************************************
 *
 *      Status LED function prototypes
 *
 ***************************************************************************/

 void led_init(void);           /* initialize status LED on SBC40 */
 int led_toggle(void);          /* toggle status LED on SBC40 */
/* file: watchdog.h */

/****************************************************************************
 *
 *      WatchDog timer header file
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
 *		02/06/90		D.marlowe		Original release
 *		05/12/94		D.marlowe		Added I486 routines
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      WatchDog timer function prototypes
 *
 ***************************************************************************/

 void init_the_dog(void);       /* initialize watch dog timer */
 void kick_the_dog(void);       /* toggle watch dog timer */
 void deinit_the_dog(void);		/* deinitialize watch dog timer */
/* file: delay.h */

/****************************************************************************
 *
 *	Utility header file
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
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

/****************************************************************************
 *
 *	utility function prototypes
 *
 ***************************************************************************/

 void delay(unsigned count);    /* delay for count , 32000 = 0.25 seconds */

 void multi_delay(unsigned count);
/* file: 8255.h */

/***************************************************************************
 *
 *	8255 PPI header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/***************************************************************************
 *
 *	General Bit defintions
 *
 ***************************************************************************/

 #define BIT0	0x00
 #define BIT1	0x01
 #define BIT2	0x02
 #define BIT3	0x03
 #define BIT4	0x04
 #define BIT5	0x05
 #define BIT6	0x06
 #define BIT7	0x07

/***************************************************************************
 *
 *      82c55A PPI MODE 0 port configurations Control words
 *
 ***************************************************************************/

                              /* port i/o direction      A  CL CH B */
 #define ppi_mode_0_0 0x80    /* mode 0 control word #0  o  o  o  o */
 #define ppi_mode_0_1 0x81    /* mode 0 control word #1  o  i  o  o */
 #define ppi_mode_0_2 0x82    /* mode 0 control word #2  o  o  o  i */
 #define ppi_mode_0_3 0x83    /* mode 0 control word #3  o  i  o  i */
 #define ppi_mode_0_4 0x88    /* mode 0 control word #4  o  o  i  o */
 #define ppi_mode_0_5 0x89    /* mode 0 control word #5  o  i  i  o */
 #define ppi_mode_0_6 0x8a    /* mode 0 control word #6  o  o  i  i */
 #define ppi_mode_0_7 0x8b    /* mode 0 control word #7  o  i  i  i */
 #define ppi_mode_0_8 0x90    /* mode 0 control word #8  i  o  o  o */
 #define ppi_mode_0_9 0x91    /* mode 0 control word #9  i  i  o  o */
 #define ppi_mode_0_10 0x92   /* mode 0 control word #10 i  o  o  i */
 #define ppi_mode_0_11 0x93   /* mode 0 control word #11 i  i  o  i */
 #define ppi_mode_0_12 0x98   /* mode 0 control word #12 i  o  i  o */
 #define ppi_mode_0_13 0x99   /* mode 0 control word #13 i  i  i  o */
 #define ppi_mode_0_14 0x9a   /* mode 0 control word #14 i  o  i  i */
 #define ppi_mode_0_15 0x9b   /* mode 0 control word #15 i  i  i  i */

/***************************************************************************
 *
 *      82c55A PPI MODE 0 port configurations Control words
 *
 ***************************************************************************/

 #define PPI_MODE_0	 0x80	/* base for MODE 0 control code */

 #define PORTA_OUT	 0x00	/* port a output */
 #define PORTB_OUT	 0x00	/* port b output */
 #define PORTC_LOWER_OUT 0x00	/* port c lower nibble output */
 #define PORTC_UPPER_OUT 0x00	/* port c upper nibble output */

 #define PORTA_IN	 0x10	/* port a output */
 #define PORTC_UPPER_IN  0x08	/* port c upper nibble output */
 #define PORTB_IN	 0x02	/* port b output */
 #define PORTC_LOWER_IN  0x01	/* port c lower nibble output */

/****************************************************************************
 *
 *      PPI data structure
 *
 ***************************************************************************/

 #ifndef PPI_STRUCT

 struct PPI {
        unsigned int ppi_base_addr;         /* base I/O address of PPI */
        unsigned int port_a_addr;           /* I/O address of port a */
        unsigned int port_b_addr;           /* I/O address of port b */
        unsigned int port_c_addr;           /* I/O address of port c */
        unsigned int cntrl_addr;            /* I/O address of control port */
        unsigned char port_a_direction;     /* input or output status */
        unsigned char port_b_direction;     /* input or output status */
        unsigned char port_c_low_direction; /* input or output status */
        unsigned char port_c_hi_direction;  /* input or output status */
        unsigned char port_a;               /* current value of port a */
        unsigned char port_b;               /* current value of port b */
        unsigned char port_c;               /* current value of port c */
	unsigned char mode;                 /* current value of mode */
	unsigned int port_addr[4];          /* I/O address array */
    };

 #define PPI_STRUCT

 #endif

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define BIT_SET        1
 #define BIT_RESET      0

 #define PPI_INPUT      0
 #define PPI_OUTPUT     1

 #define PORT_ERROR     -1

 #define PORT_A         0
 #define PORT_B         1
 #define PORT_C         2
 #define PORT_CNTRL     3
/* file: ppi.h */

/***************************************************************************
 *
 *      PPI header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	define addresses for LPM-SBC40-8 ppi
 *
 ***************************************************************************/

 #if !defined(V50)

 #define ppi_porta PPI_BASE      /* address of port a */
 #define ppi_portb PPI_BASE+1    /* address of port b */
 #define ppi_portc PPI_BASE+2    /* address of port c */
 #define ppi_control PPI_BASE+3  /* address of control register */

 #else

 #define ppi_porta PPI_BASE      /* address of port a */
 #define ppi_portb PPI_BASE+2    /* address of port b */
 #define ppi_portc PPI_BASE+4    /* address of port c */
 #define ppi_control PPI_BASE+6  /* address of control register */

 #endif

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI ppi;

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_ppi(void);           /* initialize ppi on LPM-SBC40 card */
 int toggle_bit(int bit);       /* toggle PPI port c bits */
 int test_bit(int bit);         /* test PPI port c bit */
 int set_bit(int bit);          /* set PPI port c bit */
 int reset_bit(int bit);        /* reset PPI port c bit */

 int read_porta(void);          /* read byte from port A */
 int read_portb(void);          /* read byte from port A */
 int read_portc(void);          /* read byte from port A */

 int write_porta(int byte);     /* write byte from port A */
 int write_portb(int byte);     /* write byte from port A */
 int write_portc(int byte);     /* write byte from port A */

 int toggle_bit_port(int bit, int port);/* toggle PPI port bit */
 int test_bit_port(int bit, int port);  /* test PPI port bit */
 int set_bit_port(int bit, int port);   /* set PPI port bit */
 int reset_bit_port(int bit, int port); /* reset PPI port bit */
/* file: lbx_pio.h */

/***************************************************************************
 *
 *	LBX-PIO PPI header file
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	define addresses for iSBX ppi number 1
 *
 ***************************************************************************/

 #if !defined(V50)

 #define mcs0_porta MCS_0_BASE     /* address of port a */
 #define mcs0_portb MCS_0_BASE+1   /* address of port b */
 #define mcs0_portc MCS_0_BASE+2   /* address of port c */
 #define mcs0_control MCS_0_BASE+3 /* address of control register */

 #else

 #define mcs0_porta MCS_0_BASE     /* address of port a */
 #define mcs0_portb MCS_0_BASE+2   /* address of port b */
 #define mcs0_portc MCS_0_BASE+4   /* address of port c */
 #define mcs0_control MCS_0_BASE+6 /* address of control register */

 #endif

/****************************************************************************
 *
 *	define addresses for iSBX ppi number 0
 *
 ***************************************************************************/

 #if !defined(V50)

 #define mcs1_porta MCS_1_BASE     /* address of port a */
 #define mcs1_portb MCS_1_BASE+1   /* address of port b */
 #define mcs1_portc MCS_1_BASE+2   /* address of port c */
 #define mcs1_control MCS_1_BASE+3 /* address of control register */

 #else

 #define mcs1_porta MCS_1_BASE     /* address of port a */
 #define mcs1_portb MCS_1_BASE+2   /* address of port b */
 #define mcs1_portc MCS_1_BASE+4   /* address of port c */
 #define mcs1_control MCS_1_BASE+6 /* address of control register */

 #endif

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI sbx[2];

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_ppi_sbx(int port_0, int port_1);
					/* initialize ppi on LBX-PIO CARD */
 int toggle_bit_sbx(int bit, int port);	/* toggle PPI port c bits */
 int test_bit_sbx(int bit, int port);	/* test PPI port c bit */
 int set_bit_sbx(int bit, int port);	/* set PPI port c bit */
 int reset_bit_sbx(int bit, int port);	/* reset PPI port c bit */

 int read_porta_sbx(int port);			/* read byte from port A */
 int read_portb_sbx(int port);			/* read byte from port A */
 int read_portc_sbx(int port);			/* read byte from port A */

 int write_porta_sbx(int byte, int port);	/* write byte from port A */
 int write_portb_sbx(int byte, int port);	/* write byte from port A */
 int write_portc_sbx(int byte, int port);	/* write byte from port A */
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
 #define OCW2_NOP				0x40	/* no operation */
 #define OCW2_S_EOI				0x60	/* specific EOI command */

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
 void uart_baud(unsigned int baud_rate);        /* setup baud rate for UART */
 void scu_baud(unsigned int baud_rate);         /* setup baud rate for serial control unit (SCU) */
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

/* file: scu.h */

/***************************************************************************
 *
 *      SCU header file
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
 *      scu function prototypes
 *
 ***************************************************************************/

 void scu_open(unsigned baud_rate);    /* initialize SCU */

 #if defined(V20)

 unsigned int lookup_baud(unsigned baud);

 #endif

 void scu_close(void);				   /* close out SCU */
 int scu_check(void);                  /* check for character */
 int scu_getc(void);                   /* get charcter */
 int scu_poll(void);                   /* wait for character */
 void scu_flush(void);                 /* flush characters from receive buffer */
 void scu_putc(int ch);                /* output character to SCU */
 void scu_gets(char *str);             /* input string from receive buffer */
 void scu_puts(char *str);             /* output string to SCU */
/* file: rtc.h */

/***************************************************************************
 *
 *      RTC header file
 *
 *      written by denton marlowe
 *      (c)1989,1990 Hayes Targets
 *
 **************************************************************************/

/****************************************************************************
 *
 *      RTC function prototypes
 *
 ***************************************************************************/

 void init_clock(void);         /* initialize RTC */
 void clock_init(void);         /* Initialize the 6264 clock chip */
 void clock_int_setup(void);    /* Set up the 1 second interrupt from the clock */
 void set_clock_time(int hrs,int mins,int secs);         /* set time */
 void set_clock_date(int month,int day,int year);        /* set date */
 void read_clock_time(volatile struct RTC *rtc);         /* read time */
 void read_clock_date(volatile struct RTC *rtc);         /* read date */

 void install_rtc_application(void (far *routine)());    /* install pointer to main timing loop application */
 void far dummy_rtc_application(void);                   /* dummy main timing loop application */

/****************************************************************************
 *
 *      Structures
 *
 ***************************************************************************/

 volatile struct RTC {
		int ticks;
		int seconds;
		int minutes;
		int hours;
		int day;
		int month;
		int year;
		};

/****************************************************************************
 *
 *      define data which will be externel to other modules
 *
 ***************************************************************************/

 #if !defined(NO_RTC)

 extern volatile struct RTC rtc;				/* RTC data structure */

 #endif
/* file: uart.h */

/***************************************************************************
 *
 *      UART header file
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
 int uart_printf(char const *,...);                 /* same as microsoft function */
 int scu_printf(char const *,...);                  /* same as microsoft function */
 int com_printf(char const *,...);                  /* same as microsoft function */
 int printf(char const *,...);						/* same as microsoft function */

/****************************************************************************
 *
 *      ROMable function prototypes - Code in file ST.ASM (startup code)
 *
 ***************************************************************************/

 void exit(int exit_status);                        /* same as microsoft function */
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
/* file: remtron.h */

/****************************************************************************
 *
 *	REMTRON CMD/TM functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_REMTRON

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/***************************************************************************
 *
 *	data structures for REMTRON data link
 *
 ***************************************************************************/

 #define NUM_CMD_TM_CHANNELS    16
 #define NUM_TM_CHANNELS        16
 #define NUM_CMD_CHANNELS       9

 #define TM_FRAME_COUNT         15

 struct CMD_TM {
	unsigned char addr;
	unsigned char bit[8];
	unsigned char num_channels;
	unsigned int frame_count;
	unsigned char data[NUM_CMD_TM_CHANNELS];
	};

/***************************************************************************
 *
 *	define variables for command link interrupt service routine
 *
 ***************************************************************************/

 extern volatile struct CMD_TM cmd;

/***************************************************************************
 *
 *	define variables for telemetry link interrupt service routine
 *
 ***************************************************************************/

 extern volatile struct CMD_TM tm;

/****************************************************************************
 *
 *	CMD/TM function prototypes
 *
 ***************************************************************************/

 void init_REMTRON(void);       /* initialize CMD/TM system */
 void init_REMTRON16(void);     /* initialize CMD/TM system */
 void de_init_REMTRON(void);	/* deinitialize CMD/TM system */

 void init_mcs(void);			/* initialize CMD/TM interface hardware */

 void cmd_tm_reset(void);       /* reset CMD/TM system */
 void cmd_tm_enable(void);      /* enable CMD/TM system */
 void cmd_tm_disable(void);     /* disable CMD/TM system */
 void enable_cmd(void);         /* enable CMD system */
 void disable_cmd(void);		/* disable CMD system */
 void enable_tm(void);          /* enable TM system */
 void disable_tm(void);         /* disable TM system */

 void echo_cmd_to_tm(void);     /* echo CMD data to TM output */

 void install_cmd_application(void (far *routine)());
						    /* install pointer to main timing loop application */
 void far dummy_cmd_application(void);              /* dummy main timing loop application */

 void install_tm_application(void (far *routine)());
						    /* install pointer to main timing loop application */
 void far dummy_tm_application(void);			 /* dummy main timing loop application */

/****************************************************************************
 *
 *	TCU function prototype for interrupt service routine
 *
 ***************************************************************************/

 void interrupt far cmd_int_handler(unsigned int es, unsigned int ds,
                      unsigned int di, unsigned int si,
                      unsigned int bp, unsigned int sp,
                      unsigned int bx, unsigned int dx,
                      unsigned int cx, unsigned int ax,
                      unsigned int ip, unsigned int cs,
                      unsigned int flags);
                                            /* CMD interrupt service routine */

 void interrupt far tm_int_handler(unsigned int es, unsigned int ds,
                      unsigned int di, unsigned int si,
                      unsigned int bp, unsigned int sp,
                      unsigned int bx, unsigned int dx,
                      unsigned int cx, unsigned int ax,
                      unsigned int ip, unsigned int cs,
                      unsigned int flags);
                                            /* TM interrupt service routine */

 void interrupt far cmd16_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
					    /* CMD interrupt service routine */

 void interrupt far tm16_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
					    /* TM interrupt service routine */

/****************************************************************************
 *
 *	CMD function pointer to command decoder function
 *
 ***************************************************************************/

 extern void (far *funptr_cmd_application)(void);	/* pointer to function */

 extern void (far *funptr_tm_application)(void);	/* pointer to function */

 extern void (__interrupt __far *cmd_vector)();		/* pointer to current IRQ handler */

 extern void (__interrupt __far *tm_vector)();		/* pointer to current IRQ handler */

 extern void (__interrupt __far *cmd_old_vector)();	/* pointer to old IRQ handler */

 extern void (__interrupt __far *tm_old_vector)();	/* pointer to old IRQ handler */

 #define _INC_REMTRON
 #endif /* ifndef _INC_REMTRON */
/* file: cmd_tmio.h */

/****************************************************************************
 *
 *	REMTRON CMD/TM functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_CMD_TMIO

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *	CMD/TM definitions
 *
 ***************************************************************************/

 #define ID_MODE_SINGLE 	0
 #define ID_MODE_DOUBLE		0

 #define ID_NO_PIO			-1

 #define ID_PIO				0

 #define ID_SBX_PIO 		1

 #define ID_LPM_SBX_PIO 	2
 #define ID_LPM_SBX_PIO_J1	3
 #define ID_LPM_SBX_PIO_J2	4

/****************************************************************************
 *
 *	single pio mode definitions
 *
 ***************************************************************************/

 #define ID_PCM_PIO 		5
 #define ID_PCM_PIO_J1		6
 #define ID_PCM_PIO_J2		7

 #define ID_SBX_PIO_J1			8
 #define ID_SBX_PIO_J2			9

 #define ID_LPM_SBX_PIO_J1_J1	10 // first slot j1
 #define ID_LPM_SBX_PIO_J1_J2	11 // first slot j2
 #define ID_LPM_SBX_PIO_J2_J1	12 // second slot j1
 #define ID_LPM_SBX_PIO_J2_J2	13 // second slot j2

 #define ID_ONBOARD_PIO			14 // OnBoard PIO (V40/V50)

/****************************************************************************
 *
 *	definitions for REMTRON data link interface
 *
 ***************************************************************************/

 #define CMD_ENABLE 	0x01
 #define CMD_DISABLE	0x00

 #define TM_ENABLE		0x00
 #define TM_DISABLE 	0x01

 #define SINGLE_PORT	0x00
 #define DUAL_PORT		0x01

/****************************************************************************
 *
 *	data structures for REMTRON data link
 *
 ***************************************************************************/

 struct CMD_TM_PORT {
	unsigned int cntrl;
	unsigned int addr_port;
	unsigned int data_port;
	unsigned int aux_port;
	unsigned int mode;
	unsigned int single_dual;
	};

 extern volatile struct CMD_TM_PORT cmd_port;
 extern volatile struct CMD_TM_PORT tm_port;

/****************************************************************************
 *
 *	CMD/TM Application function prototypes
 *
 ***************************************************************************/

 #define FUNCTION_OFF		0
 #define FUNCTION_ON		1

 #define FRAME_COUNT_OFF	0
 #define FRAME_COUNT_ON		1

 struct CMD_TM_APP {
	int frame_count_flag;					// frame counting enable switch
	int function_flag;						// function enable switch
	int irq_function_flag;					// function enable switch
	void (far *funptr_application)(void);	// pointer to function
	void (far *funptr_irq)(void);			// pointer to function
	};


 extern volatile struct CMD_TM_APP cmdApp;
 extern volatile struct CMD_TM_APP tmApp;

/****************************************************************************
 *
 *	CMD/TM function prototypes
 *
 ***************************************************************************/

 void set_cmdtm_port_mode(int mode);
 //void open_cmdtm_port(int port_id, int mode);
 void open_cmdtm_port(int port_id);
 void init_cmdtm(void);
 void init_single_cmd_tm(void);

 void init_dual_msc(void);			/* initialize CMD/TM interface hardware */
 void deinit_dual_msc(void);			/* deinitialize CMD/TM interface hardware */

 void init_single_msc(void);			/* initialize CMD/TM interface hardware */
 void deinit_single_msc(void);			/* deinitialize CMD/TM interface hardware */

/****************************************************************************
 *
 *	CMD/TM function prototypes
 *
 ***************************************************************************/

 int cmd_addr_data_dual(struct CMD_TM *cmd);
 int cmd_address_dual(void);
 int cmd_data_dual(void);

 int tm_addr_data_dual(struct CMD_TM *tm);
 int tm_address_dual(void);
 int tm_data_dual(int data);

/****************************************************************************
 *
 *	REMTRON function prototype for interrupt service routines
 *
 ***************************************************************************/

 void interrupt far rtd1_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* CMD interrupt service routine */

 void interrupt far rte2_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* TM interrupt service routine */

 void interrupt far rtd1s_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* CMD interrupt service routine */

 void interrupt far rte2s_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
                                            /* TM interrupt service routine */

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_cmd_application(void);
 void disable_cmd_application(void);
 void enable_tm_application(void);
 void disable_tm_application(void);

/****************************************************************************
 *
 * interrupt functions
 *
 ***************************************************************************/

 void enable_cmd_irq_function(void);
 void disable_cmd_irq_function(void);
 void enable_tm_irq_function(void);
 void disable_tm_irq_function(void);

 void install_cmd_irq(void (far *routine)(void));
 void far dummy_cmd_irq(void);
 void install_tm_irq(void (far *routine)(void));
 void far dummy_tm_irq(void);

 extern void (far *funptr_cmd_irq)(void);	/* pointer to function */

 extern void (far *funptr_tm_irq)(void);	/* pointer to function */

/****************************************************************************
 *
 * frame count functions
 *
 ***************************************************************************/

 void enable_cmd_frame_count(void);
 void disable_cmd_frame_count(void);
 void enable_tm_frame_count(void);
 void disable_tm_frame_count(void);

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI cmd_tm_ppi[2];

 #define _INC_CMD_TMIO
 #endif /* ifndef _INC_CMD_TMIO */
/* file: rdeb.h */

/***************************************************************************
 *
 *      RDEB interface functions header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

/****************************************************************************
 *
 *      startup function prototypes
 *
 ***************************************************************************/

 void far _astart(void);                        /* C startup code function */

/****************************************************************************
 *
 *      C_THRU_ROM Boot fucntion prototype
 *
 ***************************************************************************/

 void C_THRU_ROM(void);                 /* Resident remote debugger */

/****************************************************************************
 *
 *      C_THRU_ROM resident kernel function prototype
 *
 ***************************************************************************/

 void r_main(void);             /* main function of C_THRU_ROM kernel */

/****************************************************************************
 *
 *      RDEB interface definitions
 *
 ***************************************************************************/

 #define KERNEL_PUTS 0xF0       /* The interrupt # handled by the Kernel */

/****************************************************************************
 *
 *      RDEB interface function definitions
 *
 ***************************************************************************/

 int rdeb_putstr(char *str);    /* output string to RDEB console*/
 void rdeb_puts(char *string);  /* output string with newline */
 void rdeb_putch(char c);       /* output character to RDEB console */

/****************************************************************************
 *
 *		CALIBRATE Boot fucntion prototype
 *
 ***************************************************************************/

 void CALIBRATE(void);				   /* Resident remote calibration */

/****************************************************************************
 *
 *      end of RDEB interface functions header file
 *
 ***************************************************************************/
/* file: rlc.h */

/***************************************************************************
 *
 *      RLC header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      RLC data structure
 *
 ***************************************************************************/

 #ifndef RLC_STRUCT

 struct RLC {
	unsigned int rlc_addr;          /* base I/O address of PPI */
	unsigned int relay_byte;
	unsigned int relay_read;
	unsigned int relay_write;
	};

 extern struct RLC rlc;

 #define RLC_STRUCT

 #endif

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define RELAY_SET      1
 #define RELAY_RESET    0

 #define RELAY_ERROR    -1

/****************************************************************************
 *
 *      RLC initialization function prototypes
 *
 ***************************************************************************/

 void init_rlc(void);           /* initialize RLC card */
 int toggle_relay(int relay);   /* toggle RLC relay */
 int test_relay(int relay);     /* test RLC relay */
 int set_relay(int relay);      /* set RLC relay */
 int reset_relay(int relay);    /* reset RLC relay */

 int write_relay(int relay);	/* write RLC control byte */
 int read_relay(void);		/* read RLC relay status */
/* file: status.h */

/***************************************************************************
 *
 *      STATUS header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *  STATUS data structure
 *
 ***************************************************************************/

 #ifndef STATUS_STRUCT

 struct STATUS_LED_SW {
	unsigned int status_addr;	/* base I/O address of PPI */
	unsigned int led_addr[4];	/* array of LED bank addresses */
	unsigned int sw_addr[4];	/* array of switch bank addresses */
	unsigned int led[4];		/* status led banks */
	unsigned int sw[4];		/* status switch bank */
	unsigned int byte;		/* I/O byte */
    };

 #define STATUS_STRUCT

 #endif

 extern struct STATUS_LED_SW status_led_sw;
					/* define structure storage */

/****************************************************************************
 *
 *      general hardware definitions
 *
 ***************************************************************************/

 #define LED_SET  1
 #define LED_RESET    0
 #define SWITCH_SET  1
 #define SWITCH_RESET    0

 #define LED_ERROR    -1
 #define SWITCH_ERROR    -1

 #define NUM_BANKS 4

/****************************************************************************
 *
 *  STATUS initialization function prototypes
 *
 ***************************************************************************/

 void init_status(void);                /* initialize STATUS card */
 int toggle_LED(int bank, int LED);     /* toggle STATUS LED */
 int test_LED(int bank, int LED);       /* return state of LED  */
 int set_LED(int bank, int LED);        /* set STATUS LED */
 int reset_LED(int bank, int LED);      /* reset STATUS LED */

 int read_switch(int bank, int SW);     /* read status of switch */
 int set_LED_switch(int bank, int SW);  /* set led to state of switch */

 int write_LED(int bank, int LED);	/* update bank of LEDs */
 int read_LED(int bank);		/* read bank of LEDs */

 int read_SW(int bank); 		/* read bank of Switches */
/* file: lpm_sbx.h */

/***************************************************************************
 *
 *      LBX-PIO PPI header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      define base addresses of LPM-SBX-1 devices
 *
 ***************************************************************************/

 #define SBX_0_MCS0 LPM_SBX_BASE
 #define SBX_0_MCS1 (LPM_SBX_BASE+0x08)

 #define SBX_1_MCS0 (LPM_SBX_BASE+0x10)
 #define SBX_1_MCS1 (LPM_SBX_BASE+0x18)

 #define LBX_ICU_BASE (LPM_SBX_BASE+0x20)

/****************************************************************************
 *
 *      define addresses for iSBX #1 ppi at J2
 *
 ***************************************************************************/

 #define mcs0_0_porta SBX_0_MCS0     /* address of port a */
 #define mcs0_0_portb SBX_0_MCS0+1   /* address of port b */
 #define mcs0_0_portc SBX_0_MCS0+2   /* address of port c */
 #define mcs0_0_control SBX_0_MCS0+3 /* address of control register */

/****************************************************************************
 *
 *      define addresses for iSBX #1 ppi at J1
 *
 ***************************************************************************/

 #define mcs1_0_porta SBX_0_MCS1     /* address of port a */
 #define mcs1_0_portb SBX_0_MCS1+1   /* address of port b */
 #define mcs1_0_portc SBX_0_MCS1+2   /* address of port c */
 #define mcs1_0_control SBX_0_MCS1+3 /* address of control register */

/****************************************************************************
 *
 *      define addresses for iSBX #2 ppi at J2
 *
 ***************************************************************************/

 #define mcs0_1_porta SBX_1_MCS0     /* address of port a */
 #define mcs0_1_portb SBX_1_MCS0+1   /* address of port b */
 #define mcs0_1_portc SBX_1_MCS0+2   /* address of port c */
 #define mcs0_1_control SBX_1_MCS0+3 /* address of control register */

/****************************************************************************
 *
 *      define addresses for iSBX #2 ppi at J1
 *
 ***************************************************************************/

 #define mcs1_1_porta SBX_1_MCS1     /* address of port a */
 #define mcs1_1_portb SBX_1_MCS1+1   /* address of port b */
 #define mcs1_1_portc SBX_1_MCS1+2   /* address of port c */
 #define mcs1_1_control SBX_1_MCS1+3 /* address of control register */

/****************************************************************************
 *
 *      assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI lpm_sbx[4];

/****************************************************************************
 *
 *      PPI initialization function prototypes
 *
 ***************************************************************************/

 void init_lpm_sbx(int port_0, int port_1, int port_2, int port_3);
						/* initialize ppi on LBX-PIO CARD */
 int toggle_bit_lpm_sbx(int bit, int port); 	/* toggle PPI port c bits */
 int test_bit_lpm_sbx(int bit, int port);       /* test PPI port c bit */
 int set_bit_lpm_sbx(int bit, int port);        /* set PPI port c bit */
 int reset_bit_lpm_sbx(int bit, int port);      /* reset PPI port c bit */

 int read_porta_lpm_sbx(int port);              /* read byte from port A */
 int read_portb_lpm_sbx(int port);              /* read byte from port A */
 int read_portc_lpm_sbx(int port);              /* read byte from port A */

 int write_porta_lpm_sbx(int byte, int port);   /* write byte from port A */
 int write_portb_lpm_sbx(int byte, int port);   /* write byte from port A */
 int write_portc_lpm_sbx(int byte, int port);   /* write byte from port A */
/* file: asm.h */

/****************************************************************************
 *
 *      Assembly Language Function header file
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
 *	Processor Register Functions
 *
 ***************************************************************************/

 int check_IF(void);
 int _get_flags(void);

 unsigned int _getDS(void);

 unsigned int _getCS(void);

/****************************************************************************
 *
 *      Memory function prototypes
 *
 ***************************************************************************/

 void _install(int vect,void (interrupt far *handler)());

 unsigned int _peek(unsigned int segment, int unsigned offset);

 void _poke(unsigned char value, unsigned int segment, unsigned int offset);

 int _ramtest(unsigned int segment, unsigned int offset_start, unsigned int offset_stop);
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
/* file: checksum.h */

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

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void makechecksum(void);
 void Xmakechecksum(void);
 unsigned char checksum(unsigned char *p,int len);
/* file: mon_i_or.h */

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

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void mon_out(void);
 void mon_in(void);
/* file: mon_puts.h */

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

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void mon_puts(char *str);
/* file: cmdtmbuf.h */

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
 *	defintions
 *
 ***************************************************************************/

 #define NORMAL 	0
 #define INVERT 	1

/***************************************************************************
 *
 *	data structures
 *
 ***************************************************************************/

 extern struct CMD_TM cmd_buffer;
 extern struct CMD_TM tm_buffer;

 extern int tm_invert_flag;
 extern int cmd_invert_flag;

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
 *	Remote buffer Routines
 *
 ***************************************************************************/

 void load_cmd_pack_buffer(char sync);
 void load_tm_buffer(void);			/* load telemetry transmit buffer */

 void echo_packet_to_xpacket(void);

/****************************************************************************
 *
 *	serial command/telemetry data transfer functions
 *
 ***************************************************************************/

 void load_cmd_data(void);			/* load cmd.data from pack_buffer */
 void buffer_tm_data(char sync);	/* load Xpack_buffer from tm.data */
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
/* file: dspio.h */

/***************************************************************************
 *
 *      UART header file
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 **************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/91        D.marlowe       Original release
 *
 ************************************************************************@R*/

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
/* file: mit_io.h */

/****************************************************************************
 *
 *	MIT/LL I/O	functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #ifndef _INC_MIT_IO

 #define _INC_MIT_IO
 #pragma message("_INC_MIT_IO")

/*@R*************************************************************************
 *
 *	Revision History
 *
 *	Date		Engineer	Description
 *
 *	02/06/94	D.marlowe	Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *		MIT TM Interrupt
 *
 ***************************************************************************/

 #define MIT_IRQ	14				/* MIT tied to ICU interrupt number 14 */
									/* MIT tied to ICU interrupt number 6 of slave ICU */
 #define MIT_EOI (EOI | (MIT_IRQ&0x07))

/****************************************************************************
 *
 *	single pio mode definitions
 *
 ***************************************************************************/

 #define ID_LPM_SBX_PIO_J1_J1	10 // first slot j1
 #define ID_LPM_SBX_PIO_J1_J2	11 // first slot j2
 #define ID_LPM_SBX_PIO_J2_J1	12 // second slot j1
 #define ID_LPM_SBX_PIO_J2_J2	13 // second slot j2

/****************************************************************************
 *
 *	definitions for MIT/LL data link interface
 *
 ***************************************************************************/

 #define MIT_ENABLE		0x00
 #define MIT_DISABLE	0x01

 #define MIT_DATA_READY 	0x09	/* port c, bit 4 on */
 #define MIT_DATA_NOT_READY 0x08	/* port c, bit 4 off */

/****************************************************************************
 *
 *	data structures for MIT/LL data link
 *
 ***************************************************************************/

 #define NUM_MIT_CHANNELS	  16

 #define MIT_FRAME_COUNT	  15

 struct MIT_TM {
	unsigned int frame_count;
	unsigned char addr;
	unsigned int data[NUM_MIT_CHANNELS];
	};

 extern volatile struct MIT_TM mit_tm;

/****************************************************************************
 *
 *	data structures for MIT/LL data link
 *
 ***************************************************************************/

 struct MIT_PORT {
	unsigned int cntrl;
	unsigned int addr_port;
	unsigned int ldata_port;
	unsigned int hdata_port;
	unsigned int aux_port;
	unsigned int mode;
	unsigned int single_dual;
	};

 extern volatile struct MIT_PORT mit_port;

/****************************************************************************
 *
 *	MIT Application function prototypes
 *
 ***************************************************************************/

 #define FUNCTION_OFF		0
 #define FUNCTION_ON		1

 #define FRAME_COUNT_OFF	0
 #define FRAME_COUNT_ON		1

 struct MIT_APP {
	int frame_count_flag;					// frame counting enable switch
	int function_flag;						// function enable switch
	int irq_function_flag;					// function enable switch
	void (far *funptr_application)(void);	// pointer to function
	void (far *funptr_irq)(void);			// pointer to function
	};

 extern volatile struct MIT_APP mitApp;

/****************************************************************************
 *
 *	MIT function prototypes
 *
 ***************************************************************************/

 void open_mit_port(int port_id);

 void init_mit(void);

 void init_mit_msc(void);			/* initialize MIT interface hardware */
 void deinit_mit_msc(void);			/* deinitialize MIT interface hardware */

/****************************************************************************
 *
 *	MIT/LL function prototype for interrupt service routines
 *
 ***************************************************************************/

 void interrupt far mit_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags);
											/* MIT interrupt service routine */

 extern void (interrupt far *mit_int_funptr)(void);
											/* pointer to interrupt function */

/****************************************************************************
 *
 * application functions
 *
 ***************************************************************************/

 void enable_mit_application(void);
 void disable_mit_application(void);

 void install_mit_application(void (far *routine)(void));
 void far dummy_mit_application(void);
 void echo_to_mit(void);
 void enable_mit(void);			/* enable TM system */
 void disable_mit(void);		/* disable TM system */

/****************************************************************************
 *
 * interrupt functions
 *
 ***************************************************************************/

 void enable_mit_irq_function(void);
 void disable_mit_irq_function(void);

 void install_mit_irq(void (far *routine)(void));
 void far dummy_mit_irq(void);

 void (far *funptr_mit_irq)(void);	/* pointer to function */

/****************************************************************************
 *
 * frame count functions
 *
 ***************************************************************************/

 void enable_mit_frame_count(void);
 void disable_mit_frame_count(void);
/****************************************************************************
 *
 *	MIT function to telemetry encoder function
 *
 ***************************************************************************/

 void install_mit_application(void (far *routine)(void));
						    /* install pointer to main timing loop application */
 void far dummy_mit_application(void);				/* dummy main timing loop application */

/****************************************************************************
 *
 *	MIT function pointer to telemetry encoder function
 *
 ***************************************************************************/

 void (far *funptr_mit_application)(void);	/* pointer to function */

 void (__interrupt __far *mit_vector)();		/* pointer to current IRQ handler */

 void (__interrupt __far *mit_old_vector)();	/* pointer to old IRQ handler */

/****************************************************************************
 *
 *	runtime debugger
 *
 ***************************************************************************/

 void echo_to_mit(void);
 void enable_mit(void);			/* enable TM system */
 void disable_mit(void);		/* disable TM system */

 void load_mit_tm_addr(int addr,unsigned int data);
 void load_mit_tm_array(unsigned int data[]);

/****************************************************************************
 *
 *	assign data storage for structure
 *
 ***************************************************************************/

 extern struct PPI mit_ppi[2];

 void mit_icu(void);

 #pragma message("_INC_MIT_IO")
 #endif /* ifndef _INC_MIT_IO */
