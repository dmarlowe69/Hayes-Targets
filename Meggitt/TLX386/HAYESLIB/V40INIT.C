/* file: v40init.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("V40/V50 Initialization functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/*@H*************************************************************************
 *
 *      V40/V50 Initialization functions
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

 #include       "clock.h"
 #include       "config.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      V40 initialization function prototypes
 *
 ***************************************************************************/

 void v40_init(void);           /* initialize V40 specific hardware */
 void v20_init(void);           /* initialize V20 specific hardware */

/****************************************************************************
 *
 *
 *      V40/V50 Internal Hardware registers
 *
 *
 ***************************************************************************/

 #define OPCN    0xfffe      /* On chip peripheral Connection register */
 #define OPSEL   0xfffd      /* On chip Peripheral Selection Register */
 #define OPHA    0xfffc      /* On chip Peripheral High address register */
 #define DULA    0xfffb      /* DMA unit low address register */
 #define IULA    0xfffa      /* Interrupt unit low address register */
 #define TULA    0xfff9      /* Timer unit low address register */
 #define SULA    0xfff8      /* Serial unit low address register */
 #define WCY2    0xfff6      /* Wait cycle 2 register */
 #define WCY1    0xfff5      /* Wait cycle 1 register */
 #define WMB     0xfff4      /* Wait state memory boundary register */
 #define RFC     0xfff2      /* Refresh control register */
 #define TCKS    0xfff0      /* Timer clock Selection register */

/****************************************************************************

 #pragma page(1)

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
 *      OPCN definitions
 *
 ***************************************************************************/

 #define DMARQ3_DMAAK3_INTAK 0x00
 #define DMARQ3_DMAAK3_TOUT1 B0
 #define RXD_TXD_INTAK       B1
 #define RXD_TXD_SRDY        (B1 | B0)

 #define INTP1_INTP2 0x00
 #define SCU_INTP2   B2
 #define INTP1_TOUT2 B3
 #define SCU_TOUT2   (B3 | B2)

/****************************************************************************
 *
 *      OPSEL definitions
 *
 ***************************************************************************/

 #define DMAU_DISABLE 0X00
 #define DMAU_ENABLE  B0
 #define ICU_DISABLE 0X00
 #define ICU_ENABLE  B1
 #define TCU_DISABLE 0X00
 #define TCU_ENABLE  B2
 #define SCU_DISABLE 0X00
 #define SCU_ENABLE  B3

/****************************************************************************
 *
 *      WMB definitions
 *
 ***************************************************************************/

 #define UMB_32  0x00
 #define UMB_64  B0
 #define UMB_96  B1
 #define UMB_128 (B1 | B0)
 #define UMB_192 B2
 #define UMB_256 (B2 | B0)
 #define UMB_384 (B2 | B1)
 #define UMB_512 (B2 | B1 | B0)

 #define LMB_32  0x00
 #define LMB_64  B4
 #define LMB_96  B5
 #define LMB_128 (B5 | B4)
 #define LMB_192 B6
 #define LMB_256 (B6 | B4)
 #define LMB_384 (B6 | B5)
 #define LMB_512 (B6 | B5 | B4)

/****************************************************************************
 *
 *      WCY1 definitions
 *
 ***************************************************************************/

 #define LMW_0 0X00
 #define LMW_1 B0
 #define LMW_2 B1
 #define LMW_3 (B1 | B0)

 #define MMW_0 0X00
 #define MMW_1 B2
 #define MMW_2 B3
 #define MMW_3 (B3 | B2)

 #define UMW_0 0X00
 #define UMW_1 B4
 #define UMW_2 B5
 #define UMW_3 (B5 | B4)

 #define IOW_0 0X00
 #define IOW_1 B6
 #define IOW_2 B7
 #define IOW_3 (B7 | B6)

/****************************************************************************
 *
 *      WCY2 definitions
 *
 ***************************************************************************/

 #define RFW_0 0X00
 #define RFW_1 B0
 #define RFW_2 B1
 #define RFW_3 (B1 | B0)

 #define DMAW_0 0X00
 #define DMAW_1 B2
 #define DMAW_2 B3
 #define DMAW_3 (B3 | B2)

/****************************************************************************
 *
 *      RFC definitions
 *
 ***************************************************************************/

 #define RFC_COUNT_17 0X00
 #define RFC_COUNT_18 B0
 #define RFC_COUNT_19 B1
 #define RFC_COUNT_20 (B1 | B0)
 #define RFC_COUNT_5  B2
 #define RFC_COUNT_6  (B2 | B0)

 #define RFC_COUNT_31 (B7 | B6 | B5 | B4 | B3 | B2 | B1 )
 #define RFC_COUNT_32 (B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0)

 #define RFC_DISABLE 0X00
 #define RFC_ENABLE  B7

/****************************************************************************
 *
 *      TCKS definitions
 *
 ***************************************************************************/

 #define PRESCALE_2  0X00
 #define PRESCALE_4  B0
 #define PRESCALE_8  B1
 #define PRESCALE_16 (B1 | B0)

 #define CS0_INTERNAL 0X00
 #define CS0_TCLK     B2

 #define CS1_INTERNAL 0X00
 #define CS1_TCLK     B3

 #define CS2_INTERNAL 0X00
 #define CS2_TCLK     B4

/****************************************************************************
 *
 *      define Prescale command
 *
 ***************************************************************************/

 #if PRESCALE == 2

 #define PRESCALE_X PRESCALE_2

 #elif PRESCALE == 4

 #define PRESCALE_X PRESCALE_4

 #elif PRESCALE == 8

 #define PRESCALE_X PRESCALE_8

 #elif PRESCALE == 16

 #define PRESCALE_X PRESCALE_16

 #endif

 #if !defined(PRESCALE_X)

 #define PRESCALE_X PRESCALE_2

 #endif

/****************************************************************************
 *
 *      end of v40/v50 hardware definitions
 *
 ***************************************************************************/

 #pragma page(1)

/*@H*************************************************************************
 *
 *      V40/V50 specific hardware initialization
 *
 *      DELCARATION:    void v40_init(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    v40 hardware initialized
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:    void _disable(void)
 *                      int outp(unsigned portid, int value)
 *                      void _enable(void)
 *
 *      HEADERS:        <dos.h>
 *                      <conio.h>
 *                      config.h
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         v40init.c
 *
 *      DESCRIPTION:
 *
 *      This function initializes the V40 processor
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 //#if !defined(V20)
 #if defined(V40) || defined(V50)

 void v40_init(void)
 {
	_disable();             /* disable interrupts */

/****************************************************************************
 *
 *      setup v40 multiplexed pins
 *
 ***************************************************************************/
                  
	outp(OPCN,(SCU_INTP2 | RXD_TXD_INTAK)); /* connect RxD,TxD, & INTAK */
						/* int 1 = scu, int2 = INTP2 pin */
/****************************************************************************
 *
 *      setup internal peripherals
 *
 ***************************************************************************/

	outp(OPSEL,(SCU_ENABLE | TCU_ENABLE | ICU_ENABLE));
				/* enable scu & tcu & icu */

	outp(OPHA,0x00);        /* set internal peripheral to 0x00 high address */

	outp(DULA,DMA_BASE);    /* dma lower address */
	outp(IULA,ICU_BASE);    /* interrupt lower address */
	outp(TULA,TIMER_BASE);  /* timer address */
	outp(SULA,SCU_BASE);    /* serial address */

/****************************************************************************
 *
 *      setup wait state generator
 *
 ***************************************************************************/

	outp(WCY2,(DMAW_0 | RFW_0));    /* no dma or refresh wait states */

 #if !defined(V50)

        outp(WCY1,(IOW_1 | UMW_0 | MMW_0 | LMW_0));
					/* no wait states on all memory and 1 on I/O */
 #else

        outp(WCY1,(IOW_3 | UMW_0 | MMW_0 | LMW_0));
                                        /* no wait states on all memory and 3 on I/O */

 #endif

	outp(WMB,(LMB_512 | UMB_512));  /* no boundary needed (all memory) */

/****************************************************************************
 *
 *      setup refresh control unit
 *
 ***************************************************************************/

	outp(RFC,RFC_DISABLE);          /* disable DRAM refresh */

/****************************************************************************
 *
 *      select clock source for TCU and internal clock prescale
 *
 ***************************************************************************/

	outp(TCKS,(CS2_INTERNAL | CS1_INTERNAL | CS0_INTERNAL | PRESCALE_X));

	_enable();                      /* enable interrupts */
 }

 #endif

 #pragma page(1)

/*@H*************************************************************************
 *
 *      V20 specific hardware initialization
 *
 *      DELCARATION:    void v20_init(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
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
 *       Referenced:    None
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         v40init.c
 *
 *      DESCRIPTION:
 *
 *      This function is a dummy initialization routine
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ************************************************************************@H*/

 #pragma page(1)

 #if defined(V20)

 void v20_init(void)
 {

 }

 #endif
