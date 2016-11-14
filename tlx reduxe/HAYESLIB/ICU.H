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
