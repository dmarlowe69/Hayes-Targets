//  file: config.h

//////////////////////////////////////////////////////////////////////////////
//
//      Hardware Specific Configuration Header File
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//  	Define Configuration Flag
//

 #define CONFIG

//
//      ICU interrupt number definitions for PC
//
//      interrupt number 0    ICU timer 0
//      interrupt number 1    ICU keyboard
//      interrupt number 2    ICU slave 8259
//      interrupt number 3    ICU com2
//      interrupt number 4    ICU com1
//      interrupt number 5    ICU lpt2
//      interrupt number 6    ICU floppy disk
//      interrupt number 7    ICU lpt1
//
//
//  	The ICU_BASE definition is the address of the 8259 ICU
//

 #define ICU_BASE		0x20
 #define ICU_SLAVE_BASE 0xA0

//
//      ICU base vector
//

 #define ICU_BASE_IR		0x08
 #define ICU_SLAVE_BASE_IR	0x70

//
//      ICU End Of Interrupt Command
//

 #define OCW2_NS_EOI        0x20    /* non-specific EOI command */
 #define OCW2_S_EOI			0x60	/* specific EOI command */

 //#define EOI		OCW2_NS_EOI		/* specific EOI command */
 #define EOI		OCW2_S_EOI		/* specific EOI command */

 //#define MASTER_EOI OCW2_NS_EOI       /* nospecific EOI command */
 #define MASTER_EOI OCW2_S_EOI      /* nospecific EOI command */

 //#define SLAVE_EOI    OCW2_NS_EOI     /* nospecific EOI command */
 #define SLAVE_EOI	OCW2_S_EOI		/* nospecific EOI command */

//
//      TCU Timer 0 Interrupt
//

 #define TCT0_IRQ 0x00          /* TCT0 tied to ICU number 0 */
 #define TCT0_EOI (EOI | TCT0_IRQ)

//
//      Key Board 1 Interrupt
//

 #define KYBRD_IRQ 0x02			/* Key Board tied to ICU number 1 */
 #define KYBRD_EOI (EOI | KYBRD_IRQ)

//
//      Slave ICU 2 Interrupt
//

 #define ICU_IRQ 0x02			/* Slave ICU tied to ICU number 2 */
 #define ICU_EOI (EOI | ICU_IRQ)

//
//      82C50 UART Receive Interrupt (comm 1)
//

 #define COMM1_IRQ 0x04			/* UART tied to ICU interrupt number 4 */
 #define COMM1_EOI (EOI | COMM1_IRQ)

//
//  	82C50 UART Receive Interrupt (comm 2)
//

 #define COMM2_IRQ 0x03			/* UART tied to ICU interrupt number 3 */
 #define COMM2_EOI (EOI | COMM2_IRQ)

//
//  	A/D conversion complete Interrupt
//

 #define AD_IRQ 0x07		   /* A/D tied to ICU interrupt number 7 */
 #define AD_EOI (EOI | AD_IRQ)

//
//      6264 RTC Interrupt (not normally used)
//

 #define CLOCK_IRQ 0x08		   /* RTC tied to ICU #2 interrupt number 0 */
 #define CLOCK_EOI (SLAVE_EOI | (CLOCK_IRQ&0x7))

//
//      Assign Lowest Priorty IRQ
//

 #define LOW_IRQ 0x07           /* assign IR7 lowerest priorty */
                                /* which makes IR0 highest priorty */

 #define LOW_SLAVE_IRQ 0x07		/* assign IR7 lowerest priorty */
								/* which makes IR0 highest priorty */

//
//  	Real TIme Clock
//

 #define CLOCK_BASE 0x70

//
//	I/O addresse of TCU control registers
//

 #define TIMER_BASE 0x40

//
//      Timer Clock Rate Defintions
//

 #define PRESCALE 1L			/* Prescale in TCKS register */

 #define CLOCK_RATE 1193181L	/* standard PC rate */

//
//      Serial I/O
//

 #define COMM1_BASE 0x3f8
 #define COMM2_BASE 0x2f8
 #define COMM3_BASE 0x3e8
 #define COMM4_BASE 0x2e8
 #define LPT1_BASE  0x378
 #define LPT2_BASE  0x278

 #define COMM_PORT_0_BASE	COMM1_BASE
 #define COMM_PORT_1_BASE	COMM2_BASE
 #define PRINTER_PORT_BASE	LPT1_BASE
