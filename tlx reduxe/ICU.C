// file: icu.c

//////////////////////////////////////////////////////////////////////////////
//
//  	Interrupt Control Unit (ICU) functions
//
//  	written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include definitions
//

 #include   <dos.h>
 #include   <conio.h>
 #include	"config.h"
 #include   "borland.h"


 #define SLAVE_ICU
 #pragma message("SLAVE_ICU")

//
//  Check Base address of ICU
//

 #ifndef ICU_BASE

 #error Base address of ICU not defined

 #endif

 #ifdef SLAVE_ICU

 #ifndef ICU_SLAVE_BASE

 #error Base address of SLAVE ICU not defined

 #endif

 #endif

//
//  ICU base vector
//

 #ifndef ICU_BASE_IR

 #error Base vector for ICU not defined

 #endif

 #ifdef SLAVE_ICU

 #ifndef ICU_SLAVE_BASE_IR

 #error Base address of SLAVE ICU not defined

 #endif

 #endif

//
//  ICU I/O port addresses
//


 #define ICU_MASKPT ICU_BASE+1

 #ifdef SLAVE_ICU

 #define ICU_SLAVE_MASKPT ICU_SLAVE_BASE+1

 #endif

//
//      ICU End Of Interrupt (EOI) macro
//

 #define _EOI(irq) outp(ICU_BASE,(OCW2_S_EOI | irq))

 #define _EOI_MASTER(irq) outp(ICU_BASE,(OCW2_S_EOI | irq))

 #define _EOI_SLAVE(irq) outp(ICU_SLAVE_BASE,(OCW2_S_EOI | (irq&0x7)))

//
//  ICU function prototypes
//

 void init_icu(void);           /* initializes interrupt controler unit */
 void install_vector(void (interrupt far *routine)(),int irq);
                                /* install isr address for specified irq */
 void int_enable(int irq);      /* enable specific interrupt */
 void int_disable(int irq);     /* disable specific interrupt */
 void int_disable_all(void);    /* disable all interrupts at icu */
 int int_toggle(int irq);       /* toggle interrupt mask */
 int int_status(int irq);       /* get enable/disable status of irq */

 void set_vector(void (interrupt far *routine)(void),int irq);
                                /* install isr address for specified int */
 volatile void (interrupt far *get_vector(unsigned int irq))();
                                /* return pointer to given interrupt function */
 void init_exception();         /* initialize exception vectors */

//
//      Special function prototypes
//

 #if defined(CASCADE)

 #pragma message ("Cascade Interrupt Controller")

 void init_cascade_icu(void);
 void chain_interrupt(void);
 void far interrupt new_handler(void);

 #endif

//
//      ICU function prototype for interrupt service routines
//

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

//
//      ICU definitions
//

 #ifndef MK_FP

 #define MK_FP(seg,ofs)  ((void far *) \
			   (((unsigned long)(seg) << 16) | (unsigned)(ofs)))
 #endif

//
//      interrupt vector definitions
//

 #define IR0 0x00                       /* interrupt number 0 */
 #define IR1 0x01                       /* interrupt number 1 */
 #define IR2 0x02                       /* interrupt number 2 */
 #define IR3 0x03                       /* interrupt number 3 */
 #define IR4 0x04                       /* interrupt number 4 */
 #define IR5 0x05                       /* interrupt number 5 */
 #define IR6 0x06                       /* interrupt number 6 */
 #define IR7 0x07                       /* interrupt number 7 */

 #define IR0_MASK 0x01                  /* mask for interrupt number 0 */
 #define IR1_MASK 0x02                  /* mask for interrupt number 1 */
 #define IR2_MASK 0x04                  /* mask for interrupt number 2 */
 #define IR3_MASK 0x08                  /* mask for interrupt number 3 */
 #define IR4_MASK 0x10                  /* mask for interrupt number 4 */
 #define IR5_MASK 0x20                  /* mask for interrupt number 5 */
 #define IR6_MASK 0x40                  /* mask for interrupt number 6 */
 #define IR7_MASK 0x80                  /* mask for interrupt number 7 */

//
//      general bit definitions
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
//      ICU command words
//

 #define ICW1_BASE				B4		/* base initialization word */

 #define ICW1_NO_ICW4			0x00	/* no initialization word number 4 */
 #define ICW1_YES_ICW4			B0		/* require initialization word number 4 */
 #define ICW1_SINGLE			B1		/* single icu system */
 #define ICW1_CASCADE			0x00	/* multiple icu's in system */
 #define ICW1_INTERVAL_8		0x00	/* vector address 8 intervals */
 #define ICW1_INTERVAL_4		B2		/* vector address 4 intervals */
 #define ICW1_EDGE_TRIGGERED	0x00	/* edge triggered interrupts */
 #define ICW1_LEVEL_TRIGGERED	B3		/* level triggered interrupts */

 #define ICW4_MCS80_85			0x00	/* MCS-80/85 mode */
 #define ICW4_M8086 			B0		/* 8086 mode */
 #define ICW4_NORMAL_EOI		0x00	/* normal EOI mode */
 #define ICW4_AUTO_EOI			B1		/* auto EOI mode */
 #define ICW4_NON_BUFFERED		0x00	/* no-buffered mode */
 #define ICW4_BUFFERED_SLAVE	B3		/* buffered mode/slave */
 #define ICW4_BUFFERED_MASTER	(B3|B2) /* buffered mode/master */
 #define ICW4_NOT_SPECIAL		0x00	/* not special fully nested mode */
 #define ICW4_SPECIAL 0x10		B4		/* special fully nested mode */

 #define OCW2_NS_EOI			0x20	/* non-specific EOI command */
 #define OCW2_NOP				0x40	/* no operation */
 #define OCW2_S_EOI				0x60	/* specific EOI command */

 #define OCW2_ROTATE_AUTO_SET 0x80		/* rotate in auto EOI mode - set */
 #define OCW2_ROTATE_NS_EOI   0xa0      /* rotate on non-specific EOI command */
 #define OCW2_ROTATE_AUTO_CLR 0x00      /* rotate in auto EOI mode - clear */
 #define OCW2_SET_PRIORITY    0xc0      /* set priority command */
 #define OCW2_ROTATE_S_EOI    0xe0      /* rotate on specific EOI command */

//
//      ICU command words
//

 #define icw1_1 ICW1_BASE | ICW1_YES_ICW4 | ICW1_CASCADE | ICW1_INTERVAL_8 | ICW1_EDGE_TRIGGERED
 #define icw2_1 ICU_BASE_IR
 #define icw3_1 IR4
 #define icw4_1 ICW4_M8086 | ICW4_NORMAL_EOI | ICW4_NON_BUFFERED | ICW4_NOT_SPECIAL

 #define icw1_2 ICW1_BASE | ICW1_YES_ICW4 | ICW1_CASCADE | ICW1_INTERVAL_8 | ICW1_EDGE_TRIGGERED
 #define icw2_2 ICU_SLAVE_BASE_IR
 #define icw3_2 IR2
 #define icw4_2 ICW4_M8086 | ICW4_NORMAL_EOI | ICW4_NON_BUFFERED | ICW4_NOT_SPECIAL

 #define ocw1 IR0_MASK | IR1_MASK | IR2_MASK | IR3_MASK | IR4_MASK | IR5_MASK | IR6_MASK | IR7_MASK

//
//      CASACADED ICU Configuration defintions
//

 #if defined(CASCADE)

 #define MASTER_ICU_BASE_IR ICU_BASE_IR	/* base vector for master ICU */
 #define SLAVE_ICU_BASE_IR  (ICU_BASE_IR+0x08)
					/* base vector for slave ICU */
 #define SLAVE_IRQ          0x10    /* master INTP 4 has slave */
 #define SLAVE_IRQ_ID       0x04    /* slave in tied to master INTP 4 */

//
//      Master ICU command words
//

 #define master_icw1 ICW1_BASE | ICW1_YES_ICW4 | ICW1_CASCADE | ICW1_INTERVAL_4 | ICW1_EDGE_TRIGGERED
 #define master_icw2 MASTER_ICU_BASE_IR
 #define master_icw3 SLAVE_IRQ
 #define master_icw4 ICW4_M8086 | ICW4_NORMAL_EOI | ICW4_BUFFERED_MASTER | ICW4_NOT_SPECIAL
 #define master_ocw1 IR0_MASK | IR1_MASK | IR2_MASK | IR3_MASK | IR4_MASK | IR5_MASK | IR6_MASK | IR7_MASK

//
//      SLAVE ICU command words
//

 #define slave_icw1 ICW1_BASE | ICW1_NO_ICW4 | ICW1_SINGLE | ICW1_INTERVAL_4 | ICW1_EDGE_TRIGGERED
 #define slave_icw2 SLAVE_ICU_BASE_IR
 #define slave_icw3 SLAVE_IRQ_ID
 #define slave_icw4 ICW4_M8086 | ICW4_NORMAL_EOI | ICW4_BUFFERED_SLAVE | ICW4_NOT_SPECIAL
 #define slave_ocw1 IR0_MASK | IR1_MASK | IR2_MASK | IR3_MASK | IR4_MASK | IR5_MASK | IR6_MASK | IR7_MASK

 #endif

//
//      ICU general definitions
//

 #define ENABLE 0
 #define DISABLE 1

//
//      interrupt routine data structure
//
//      This data structure, maintained by the ICU interface functions,
//      is an array of pointers to the installed interrupts service routines
//      and their enable/disable status. Also a counter for the number of
//      times the incomplete interrupt handler is accessed by the ICU.
//

 #ifndef SLAVE_ICU

 struct IRQ_TABLE {
	void (interrupt far *funptrs[8])();     /* array of pointers to interrupt functions */
	int status[8];                          /* array of enable/disable flags */
	unsigned int incomplete;                /* number times INT 7 serviced */
	};

 #else

 struct IRQ_TABLE {
	void (interrupt far *funptrs[16])();	/* array of pointers to interrupt functions */
	int status[16];							/* array of enable/disable flags */
	unsigned int incomplete;                /* number times INT 7 serviced */
	};

 #endif

 struct IRQ_TABLE irq_table;				/* define data structre */

//
//      define pointer to an array of function pointers
//      (the interrupt vector table)
//

 volatile void (interrupt far *( ( far *int_vector)[0x100] ) ) (void);

//
//      define pointer to an interrupt function
//

 void (interrupt far *int_funptr)(void);

//
//      define pointer to interrupt function
//

 #if defined(CASCADE)

 void (interrupt far *chain_int_handler)(void); /* pointer to IRQ function */

 #endif

//////////////////////////////////////////////////////////////////////////////
//
//      ICU initialization
//
//      DELCARATION:    void init_icu(void)
//
//      PARAMETERS:     None
//
//      RETURNS:        Nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    dummy interrupt service routines installed (int 0..6)
//                      incomplete interrupt service routine installed (int 7)
//                      interrupts enabled
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    void _disable(void)
//                      void install_vector(void far *routine,int irq)
//                      void interrupt far dummy_int_handler(void)
//                      void interrupt far incomplete_int_handler(void)
//                      outp(unsigned portid, int byte_value)
//                      void _enable(void)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       <dos.h>
//                      <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function programs the Interrupt Control Unit (ICU) and installs
//      the addresses (SEGMENT:OFFSET) of dummy interrupt service routines
//      in the interrupt vector table, offset from the base vector programmed
//      into the ICU. A special incomplete interrupt service routine is
//      installed at INT 7.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_icu(void)
 {
//
//  disable interrupts untill things get setup
//

	_disable();                     /* disable interrupts to processor */

//
//  setup pointer to interrupt vector table (note the type casting)
//

	int_vector = (void (interrupt far *( (far *)[0x100] ) ) (void) ) 0x00000000;

//
//  install dummy interrupt service routines
//

//
//  install incomplete interrupt service routine
//

//
//  setup the interrupt control unit - 82c59A compatiable
//

    //outp(ICU_BASE,icw1_1);        /* edge triggered */
	//outp(ICU_MASKPT,icw2_1);		/* base vector */
	//outp(ICU_MASKPT,icw3_1);		/* slave IRQ */
	//outp(ICU_MASKPT,icw4_1);		/* mode	*/

	//outp(ICU_MASKPT,ocw1);			/* mask all interruputs (disable) */

	//outp(ICU_BASE,OCW2_SET_PRIORITY | LOW_IRQ);
									/* assign lowerest priorty IRQ */
	//outp(ICU_BASE,OCW2_NS_EOI);	/* clear any pending with non-specific EOI */

	//outp(ICU_SLAVE_BASE,icw1_2);		/* edge triggered */
    //outp(ICU_SLAVE_MASKPT,icw2_2);    /* base vector */
    //outp(ICU_SLAVE_MASKPT,icw3_2);    /* slave IRQ */
    //outp(ICU_SLAVE_MASKPT,icw4_2);    /* mode */

	//outp(ICU_SLAVE_MASKPT,ocw1);		/* mask all interruputs (disable) */

	//outp(ICU_SLAVE_BASE,OCW2_SET_PRIORITY | LOW_SLAVE_IRQ);
										/* assign lowerest priorty IRQ */
	//outp(ICU_SLAVE_BASE,OCW2_NS_EOI);	/* clear any pending with non-specific EOI */

//
//  enable interrupts
//

	_enable();                      /* enable interrupts to processor */
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Interrupt Vector Installation
//
//      DELCARATION:    void install_vector(void far *routine,int irq)
//
//      PARAMETERS:     far pointer to interrupt service routine
//                      interrupt number ( 0..7)
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    none
//
//      EXIT
//       Conditions:    interrupt service routine installed at int number
//                      interrupts disabled
//
//      LOCALS:         int vector
//
//      EXTERNALS
//       Referenced:    void _disable(void)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       dos.h
//                      conio.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This functions installs the address (segment:offset) in the
//      interrupt vector table, at the interrupt number given, offset from
//      the base vector programmed into the Interrupt Control Unit (ICU).
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void install_vector(void (interrupt far *routine)(),int irq)
 {
	int vector;                     /* interrupt vector */

	_disable();                     /* insure disable interrupts to processor */

	int_disable(irq);               /* insure this interrupt is disabled */

	irq_table.funptrs[irq] = routine;
                                    /* install vector in table */
 #ifdef SLAVE_ICU

	if(irq < 8)
		vector = ICU_BASE_IR | irq;	/* interrupt vector */
	else
		vector = ICU_SLAVE_BASE_IR | (irq&0x07);
									/* interrupt vector */

 #else

	vector = ICU_BASE_IR | irq;			/* interrupt vector */

 #endif


	*(long far *)MK_FP(0,vector*4) = (long)routine;
 }
//////////////////////////////////////////////////////////////////////////////
//      ICU enable interrupts
//
//      DELCARATION:    void int_enable(int irq)
//
//      PARAMETERS:     int irq - Interrupt number (0..7)
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    interrupt number enabled at ICU
//
//      LOCALS:         int mask
//
//      EXTERNALS
//       Referenced:    unsigned int inp(unsigned portid)
//                      outp(unsigned portid, int byte_value)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function enables the given interrupt number by setting the
//      appropiate bit in the interrupt enable mask register in the ICU,
//      to zero.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void int_enable(int irq)
 {
	int mask;                       /* bit mask */

//
//  set status of selected IRQ in table
//

	irq_table.status[irq] = ENABLE;

//
//  determine bit mask from irq number
//

	mask = 0x01 << (irq&0x07);	/* bit mask is 1 left shifted irq number times */

//
//  unmask the interrupts at the controller
//

 #ifdef SLAVE_ICU

	if(irq < 8)
		outp(ICU_MASKPT,inp(ICU_MASKPT) & ~mask);
	else
		outp(ICU_SLAVE_MASKPT,inp(ICU_SLAVE_MASKPT) & ~mask);

 #else

	outp(ICU_MASKPT,inp(ICU_MASKPT) & ~mask);

 #endif

 }
//////////////////////////////////////////////////////////////////////////////
//
//      ICU disable interrupts
//
//      DELCARATION:    void int_disable(int irq)
//
//      PARAMETERS:     int irq - Interrupt number (0..7)
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    interrupt number disabled at ICU
//
//      LOCALS:         int mask
//
//      EXTERNALS
//       Referenced:    unsigned int inp(unsigned portid)
//                      outp(unsigned portid, int byte_value)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function disables the given interrupt number by setting the
//      appropiate bit in the interrupt enable mask register in the ICU,
//      to one.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

 #pragma page(1)

 void int_disable(int irq)
 {
	int mask;                       /* bit mask */

//
//  set status of selected IRQ in table
//

	irq_table.status[irq] = DISABLE;

//
//  determine bit mask from irq number
//

	mask = 0x01 << (irq&0x07);	/* bit mask is 1 left shifted irq number times */

//
//  mask the interrupts at the controller
//

 #ifdef SLAVE_ICU

	if(irq < 8)
		outp(ICU_MASKPT,inp(ICU_MASKPT) | mask);
	else
		outp(ICU_SLAVE_MASKPT,inp(ICU_SLAVE_MASKPT) | mask);

 #else

	outp(ICU_MASKPT,inp(ICU_MASKPT) | mask);

 #endif

 }
//////////////////////////////////////////////////////////////////////////////
//
//      ICU disable interrupts
//
//      DELCARATION:    void int_disable_all(void)
//
//      PARAMETERS:     None
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    all interrupts disabled at ICU
//
//      LOCALS:         int mask
//
//      EXTERNALS
//       Referenced:    unsigned int inp(unsigned portid)
//                      outp(unsigned portid, int byte_value)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function disables all interrupts by setting the appropiate
//      bit in the interrupt enable mask register in the ICU, to one.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 #ifndef PC

 void int_disable_all(void)
 {
	int mask;                       /* bit mask */

//
//  set status of selected IRQ in table
//

	irq_table.status[0] = DISABLE;
	irq_table.status[1] = DISABLE;
	irq_table.status[2] = DISABLE;
	irq_table.status[3] = DISABLE;
	irq_table.status[4] = DISABLE;
	irq_table.status[5] = DISABLE;
	irq_table.status[6] = DISABLE;
	irq_table.status[7] = DISABLE;

 #ifdef SLAVE_ICU

	irq_table.status[8] = DISABLE;
	irq_table.status[9] = DISABLE;
	irq_table.status[10] = DISABLE;
	irq_table.status[11] = DISABLE;
	irq_table.status[12] = DISABLE;
	irq_table.status[13] = DISABLE;
	irq_table.status[14] = DISABLE;
	irq_table.status[15] = DISABLE;

 #endif

//
//      mask the interrupts at the controller
//

	outp(ICU_MASKPT,ocw1);

 #ifdef SLAVE_ICU

	outp(ICU_SLAVE_MASKPT,ocw1);

 #endif

 }

 #endif
//////////////////////////////////////////////////////////////////////////////
//
//      ICU toggle enable/disable interrupts
//
//      DELCARATION:    void int_toggle(int irq)
//
//      PARAMETERS:     int irq - Interrupt number (0..7)
//
//      RETURNS:        if int number was enabled, returns DISABLED (1)
//                      if int number was disabled, returns ENABLED (0)
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    interrupt number status toggled at ICU
//
//      LOCALS:         int mask
//
//      EXTERNALS
//       Referenced:    unsigned int inp(unsigned portid)
//                      outp(unsigned portid, int byte_value)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function toggles the interrupt enable status of the given
//      interrupt number by toggling the appropiate bit in the interrupt
//      enable mask register in the ICU.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 int int_toggle(int irq)
 {
	int mask;                       /* bit mask */

//
//  determine bit mask from irq number
//

	mask = 0x01 << (irq&0x07);	/* bit mask is 1 left shifted irq number times */

//
//  toggle the interrupts at the controller
//

 #ifdef SLAVE_ICU

	if( irq < 8 ) {

		if((inp(ICU_MASKPT) & mask) == mask)
			{
			irq_table.status[irq] = ENABLE;
			outp(ICU_MASKPT,inp(ICU_MASKPT) & ~mask);	/* enable */
			return(ENABLE); 							/* return status*/
			}
		else
			{
			irq_table.status[irq] = DISABLE;
			outp(ICU_MASKPT,inp(ICU_MASKPT) | mask);	/* disable */
			return(DISABLE);							/* return status*/
			}
		}

	else {

		if((inp(ICU_SLAVE_MASKPT) & mask) == mask)
			{
			irq_table.status[irq] = ENABLE;
			outp(ICU_SLAVE_MASKPT,inp(ICU_SLAVE_MASKPT) & ~mask);	/* enable */
			return(ENABLE); 										/* return status*/
			}
		else
			{
			irq_table.status[irq] = DISABLE;
			outp(ICU_SLAVE_MASKPT,inp(ICU_SLAVE_MASKPT) | mask);	/* disable */
			return(DISABLE);										/* return status*/
			}

		}

 #else

	if((inp(ICU_MASKPT) & mask) == mask)
		{
		irq_table.status[irq] = ENABLE;
		outp(ICU_MASKPT,inp(ICU_MASKPT) & ~mask);	/* enable */
		return(ENABLE); 							/* return status*/
		}
	else
		{
		irq_table.status[irq] = DISABLE;
		outp(ICU_MASKPT,inp(ICU_MASKPT) | mask);	/* disable */
		return(DISABLE);							/* return status*/
		}

 #endif

 }
//////////////////////////////////////////////////////////////////////////////
//
//      ICU interrupt enable/disable status
//
//      DELCARATION:    void int_status(int irq)
//
//      PARAMETERS:     int irq - Interrupt number (0..7)
//
//      RETURNS:        if int number is enabled, returns ENABLED (0)
//                      if int number is disabled, returns DISABLED (1)
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    unsigned int inp(unsigned portid)
//                      outp(unsigned portid, int byte_value)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This functions returns the enable/disable status of the given
//      interrupt number.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 int int_status(int irq)
 {
	if(irq_table.status[irq] == ENABLE)
		return(ENABLE);
	else
		return(DISABLE);
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Interrupt Vector Installation
//
//      DELCARATION:    void set_vector(void far *routine,int irq)
//
//      PARAMETERS:     far pointer to interrupt service routine
//                      interrupt number ( 0..255)
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    none
//
//      EXIT
//       Conditions:    interrupt service routine installed at int number
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    void _disable(void)
//                      struct IRQ_TABLE irq_table
//
//      INCLUDES:       <dos.h>
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This functions installs the address (segment:offset) in the
//      interrupt vector table, at the interrupt number given
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void set_vector(void (interrupt far *routine)(void),int irq)
 {
	_disable();                     /* insure disable interrupts to processor */

	(*int_vector)[irq] = routine;

	_enable();                      /* enable interrupts */
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Get Interrupt Vector
//
//      DELCARATION:    void get_vector(unsigned int irq)
//
//      PARAMETERS:     interrupt number ( 0..255)
//
//      RETURNS:        far pointer to interrupt service routine
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         int vector
//
//      EXTERNALS
//       Referenced:    struct IRQ_TABLE irq_table
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This functions fetches the address (segment:offset) in the
//      interrupt vector table, of the interrupt number give.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 volatile void (interrupt far *get_vector(unsigned int irq))()
 {
	return( (*int_vector)[irq] );
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Dummy interrupt service routine
//
//      DELCARATION:    void interrupt far dummy_int_handler(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    int outp(unsigned portid, int value)
//
//      INCLUDES:       <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far dummy_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	outp(ICU_BASE,OCW2_NS_EOI);		/* send NON-Specific End of Interrupt */
 }									/* return from interrupt */
//////////////////////////////////////////////////////////////////////////////
//
//      Dummy Slave interrupt service routine
//
//      DELCARATION:    void interrupt far dummy_slave_int_handler(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    int outp(unsigned portid, int value)
//
//      INCLUDES:       <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 #ifdef SLAVE_ICU

 void interrupt far dummy__slave_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
	outp(ICU_SLAVE_BASE,OCW2_NS_EOI);	/* send NON-Specific End of Interrupt */
	outp(ICU_BASE,OCW2_NS_EOI); 		/* send NON-Specific End of Interrupt */
 }										/* return from interrupt */

 #endif
//////////////////////////////////////////////////////////////////////////////
//
//      incomplete interrupt service routine
//
//      DELCARATION:    void interrupt far incomplete_int_handler(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    Invoked by interrupt 7 from ICU
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    struct IRQ_TABLE irq_table
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function is accessed by the ICU if an incomplete interrupt
//      condition is detected. By default, the ICU issues an interrupt
//      vector number seven under this condition. A variable is the data
//      strucure IRQ_TABLE irq_table is incremented each time the routine
//      is accessed, and bounds checking is done to prevent overflow. Note
//      that is function does not issue an end of interrupt command.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far incomplete_int_handler(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {
    irq_table.incomplete += 1;          /* increment irq error count */

    if(irq_table.incomplete > 32000)    /* bounds check */
		irq_table.incomplete = 0;       /* reset to prevent overflow */

 }                                      /* return from interrupt */

//////////////////////////////////////////////////////////////////////////////
//
//
//  EXCEPTION INTERRUPT SERVICE FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//      BIRD exception interrupt vector initialization
//
//      DELCARATION:     void init_exception(void)
//
//      PARAMETERS:     None
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    Exception vectors installed
//
//      LOCALS:         unsigned far//addr
//                      struct SREGS sregs
//
//      EXTERNALS
//       Referenced:    void segread(struct SREGS//sregs)
//                      void interrupt far exception_0(...)
//                      void interrupt far exception_1(...)
//                      void interrupt far exception_2(...)
//                      void interrupt far exception_3(...)
//                      void interrupt far exception_4(...)
//
//      INCLUDES:       <dos.h>
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function installs the addresses of the processor exception
//      routines in the interrupt vector table
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_exception(void)
 {
 //unsigned far *addr;			/* pointer for vector address */
 //struct SREGS sregs;			/* processor segment registers */
 int vector;					/* interrupt vector */

//
//  setup pointer to vector table
//

	//segread(&sregs);						  /* return contents of processor's location registers */
	//addr= (unsigned far *)(0x00L);		  /* point to vector address */

//
//  Install divide by 0 interrupt vector with address of handler function
//

	//*addr++= (unsigned)exception_0;	/* load offset address */
	//*addr++= sregs.cs;				/* load segment address */

	vector = 0;
	*(long far *)MK_FP(0,vector*4) = (long)exception_0;

//
//  Install trace interrupt vector with address of handler function
//

    //*addr++= (unsigned)exception_1;         /* load offset address */
	//*addr++= sregs.cs;					  /* load segment address */

	vector = 1;
	*(long far *)MK_FP(0,vector*4) = (long)exception_1;

//
//  Install NMI interrupt vector with address of handler function
//

	//*addr++= (unsigned)exception_2;		  /* load offset address */
	//*addr++= sregs.cs;					  /* load segment address */

	vector = 2;
	*(long far *)MK_FP(0,vector*4) = (long)exception_2;

//
//  Install break point interrupt vector with address of handler function
//

	//*addr++= (unsigned)exception_3;		  /* load offset address */
	//*addr++= sregs.cs;					  /* load segment address */

	vector = 3;
	*(long far *)MK_FP(0,vector*4) = (long)exception_3;

//
//  Install overflow interrupt vector with address of handler function
//

	//*addr++= (unsigned)exception_4;	  /* load offset address */
	//*addr= sregs.cs;					  /* load segment address */

	vector = 4;
	*(long far *)MK_FP(0,vector*4) = (long)exception_4;
 }
//////////////////////////////////////////////////////////////////////////////
//
//      BIRD divide by zero exception handler
//
//      DELCARATION:    void interrupt far exception_0(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    None
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far exception_0(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {

 }                                      /* return from interrupt */
//////////////////////////////////////////////////////////////////////////////
//
//      BIRD trace exception handler
//
//      DELCARATION:    void interrupt far exception_1(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    None
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far exception_1(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {

 }                                      /* return from interrupt */
//////////////////////////////////////////////////////////////////////////////
//
//      BIRD NMI exception handler
//
//      DELCARATION:    void interrupt far exception_2(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    None
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far exception_2(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {

 }                                      /* return from interrupt */
//////////////////////////////////////////////////////////////////////////////
//
//      BIRD break point exception handler
//
//      DELCARATION:    void interrupt far exception_3(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    None
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far exception_3(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {

 }                                      /* return from interrupt */
//////////////////////////////////////////////////////////////////////////////
//
//      BIRD overflow exception handler
//
//      DELCARATION:    void interrupt far exception_4(
//                                    unsigned int es, unsigned int ds,
//                                    unsigned int di, unsigned int si,
//                                    unsigned int bp, unsigned int sp,
//                                    unsigned int bx, unsigned int dx,
//                                    unsigned int cx, unsigned int ax,
//                                    unsigned int ip, unsigned int cs,
//                                    unsigned int flags);
//
//      PARAMETERS:     Processor's registers
//
//      RETURNS:        nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    None
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    None
//
//      INCLUDES:       None
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function does nothing - simply executes a return from interrupt
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void interrupt far exception_4(unsigned int es, unsigned int ds,
				      unsigned int di, unsigned int si,
				      unsigned int bp, unsigned int sp,
				      unsigned int bx, unsigned int dx,
				      unsigned int cx, unsigned int ax,
				      unsigned int ip, unsigned int cs,
				      unsigned int flags)
 {

 }                                      /* return from interrupt */

//////////////////////////////////////////////////////////////////////////////
//
//
//  END OF EXCEPTION INTERRUPT SERVICE FUNCTIONS
//
//
//////////////////////////////////////////////////////////////////////////////

 #if defined(CASCADE)

//////////////////////////////////////////////////////////////////////////////
//
//  NOTES:
//
//  In an interrupt service routine that is called by an interrupt to the
//  slave controller, an End Of Interrupt (EOI) must be sent to both
//  controllers. The EOI should be sent to the slave first and then to
//  the master. The slave ICU's interrupt service register can be polled
//  prior to the EOI to the master, and any pending interrupts serviced.
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//      Cascaded ICU initialization
//
//      DELCARATION:    void init_cascade_icu(void)
//
//      PARAMETERS:     None
//
//      RETURNS:        Nothing
//
//      ENTRY
//       Conditions:    None
//
//      EXIT
//       Conditions:    ICU's programmed in cascade mode, all interrupts masked
//
//      LOCALS:         None
//
//      EXTERNALS
//       Referenced:    void _disable(void)
//                      outp(unsigned portid, int byte_value)
//                      void _enable(void)
//
//      INCLUDES:       <dos.h>
//                      <conio.h>
//                      config.h
//
//      WARNINGS:       None
//
//      SOURCE:         icu.c
//
//      DESCRIPTION:
//
//      This function programs two Interrupt Control Units (ICU) in a cascade
//      configuration. On exit interrupts are enabled, but masked at the
//      controllers.
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void init_cascade_icu(void)
 {
//
//  disable interrupts untill things get setup
//

	_disable();					   /* disable interrupts to processor */

//
//  program master ICU
//

	outp(MASTER_ICU_BASE,master_icw1);		/* edge triggered */
	outp(MASTER_ICU_BASE+1,master_icw2);	/* base vector */
	outp(MASTER_ICU_BASE+1,master_icw3);	/* which INTP has slave */
	outp(MASTER_ICU_BASE+1,master_icw4);	/* master controller */
	outp(MASTER_ICU_BASE+1,master_ocw1);	/* mask all interruputs (disable) */
	outp(MASTER_ICU_BASE,OCW2_SET_PRIORITY | LOW_IRQ);
											/* assign lowerest priorty IRQ */
	outp(MASTER_ICU_BASE,OCW2_NS_EOI);		/* clear any pending with non-specific EOI */

//
//  program slave ICU
//

	outp(SLAVE_ICU_BASE,slave_icw1);		/* edge triggered */
	outp(SLAVE_ICU_BASE+1,slave_icw2);		/* base vector */
	outp(SLAVE_ICU_BASE+1,slave_icw3);		/* ID of master INTP */
	outp(SLAVE_ICU_BASE+1,slave_icw4);		/* slave controller */
	outp(SLAVE_ICU_BASE+1,slave_ocw1);		/* mask all interruputs (disable) */
	outp(SLAVE_ICU_BASE,OCW2_SET_PRIORITY | LOW_IRQ);
											/* assign lowerest priorty IRQ */
	outp(SLAVE_ICU_BASE,OCW2_NS_EOI);		/* clear any pending with non-specific EOI */

//
//  enable interrupts
//

	_enable();					   /* enable interrupts to processor */
 }
//////////////////////////////////////////////////////////////////////////////
//
//  chain interrupts
//
//////////////////////////////////////////////////////////////////////////////
 void chain_interrupt(void)
 {
	_disable();									/* disable interrupt */

	chain_int_handler = get_vector(SCU_IRQ);	/* set pointer to function */

	set_vector(new_handler,SCU_IRQ);			/* install new vector */

	_enable();									/* re-enable interrupts */
 }
//////////////////////////////////////////////////////////////////////////////
//
//  chain interrupts
//
//////////////////////////////////////////////////////////////////////////////
 void far interrupt new_handler(void)
 {
	_chain_intr(chain_int_handler);	/* chain to old interrupt handler */
 }

 #endif
