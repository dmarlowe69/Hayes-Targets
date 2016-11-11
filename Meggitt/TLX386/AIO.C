/* file: analog.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Analog I/O functions")
 #pragma subtitle("(c)1990-1993 Hayes Targets")

/****************************************************************************
 *
 *	Analog I/O functions
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #define LPM_AIO_DC

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

 #include	<dos.h>
 #include	<conio.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include   "config.h"
 #include   "icu.h"

 #ifdef REMOTE_I_O

 #include   "remoteio.h"

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      define A/D board I/O addresses
 *
 ***************************************************************************/

 #define A_D_MUX A_D_BASE               /* address of channel mux */
 #define A_D_BYTE A_D_BASE+1            /* address of 12 bit input word LSB */
 #define A_D_BYTE_LSB A_D_BASE+1        /* address of 12 bit input word LSB */
 #define A_D_BYTE_MSB A_D_BASE+2        /* address of 12 bit input word MSB */
 #define A_D_STATUS A_D_BASE+2          /* address of conversion status word */

 #ifdef LPM_AIO_DC

 #pragma message("AIO A/D CONFIGURATION")

 #define A_D_PG A_D_BASE               /* address of programmable gain */

 #endif

/****************************************************************************
 *
 *      define A/D board status
 *
 ***************************************************************************/

 #define A_D_BUSY	0x80	/* value of status bit for conversion incomplete */
 #define A_D_INT	0x40	/* value of int bit for conversion completed */
							/* int bit is reset by reading of lower byte of */
							/* A/D input */
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
 #define A_D_CAL_LOOP 14

 #ifdef LPM_AIO_DC
 #pragma message("AIO A/D CONFIGURATION")
 #undef D_A_CAL_LOOP
 #define D_A_CAL_LOOP 1
 #undef A_D_CAL_LOOP
 #define A_D_CAL_LOOP 0
 #endif

/****************************************************************************
 *
 *      define D/A board I/O addresses and output ranges
 *
 ***************************************************************************/

 #ifdef LPM_AIO_DC

 #define D_A_CHANNEL_0 D_A_BASE     /* I/O address for D/A channel 0 */
 #define D_A_CH_0_LOWBYTE D_A_BASE	/* I/O address for D/A channel 0 low byte */
 #define D_A_CH_0_HIGHBYTE D_A_BASE+1	/* I/O address for D/A channel 0 high byte */
 #define D_A_CH_0_ZERO 0x0        /* zero volts */
 #define D_A_CH_0_MAX 5        /* max output volts */
 #define D_A_CH_0_MIN 0      /* min output volts */
 #define D_A_CH_0_POLARITY UNIPOLAR  /* unipolar output */

 #define D_A_CHANNEL_1 D_A_BASE+2	/* I/O address for D/A channel 1 */
 #define D_A_CH_1_LOWBYTE D_A_BASE+2	/* I/O address for D/A channel 1 low byte */
 #define D_A_CH_1_HIGHBYTE D_A_BASE+3	/* I/O address for D/A channel 1 high byte */
 #define D_A_CH_1_ZERO 0x0        /* zero volts */
 #define D_A_CH_1_MAX 5        /* max output volts */
 #define D_A_CH_1_MIN 0       /* min output volts */
 #define D_A_CH_1_POLARITY UNIPOLAR  /* unipolar output */

 #else

 #define D_A_CHANNEL_0 D_A_BASE     /* I/O address for D/A channel 0 */
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
 #define D_A_CH_7_POLARITY BIPOLAR  /* bipolar output */

 #endif

/****************************************************************************
 *
 *      A/D input function prototypes
 *
 ***************************************************************************/

 void init_A_D(void);                           /* initialize A/D interrupt routine */
 void de_init_A_D(void);

 int test_A_D(char mode,volatile struct watch_AD *wad);
						/* test A/D converter */
 int a_d_get(unsigned int);                     /* input 12 bit value from A/D card */
 void sensor_scan(struct A_D_INPUT *a_d_input); /* scan all input channels */
 void sensor_poll(struct A_D_INPUT *a_d_input); /* scan all input channels */

 void interrupt far A_D_int_handler(void);      /* A/D input interrupt service routine */
 void interrupt far A_D_U_int_handler(void);	/* A/D input interrupt service routine */

 void (__interrupt __far *a_d_old_vector)();	/* pointer to old IRQ handler */

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
	signed int A_D[16];			   /* input data */
	unsigned int channel;		   /* channel number */
	unsigned int MUX_ADDR;		/* I/O addresses */
	unsigned int STATUS_ADDR;	/* I/O addresses */
	unsigned int DATA_ADDR;		/* I/O addresses */
	unsigned int ZERO;		/* input for zero volts */
	signed int MAX;				   /* maximum input voltage */
	signed int MIN; 		/* minimum input voltage */
	signed int GAIN;		/* input gain factor */
	signed int POLARITY;		/* channel polarity */
        };

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

 struct A_D_INPUT {
    signed int a_d[16];
    };

 struct A_D_INPUT_BYTE {
    unsigned char a_d_byte[32];
    };

/****************************************************************************
 *
 *      define data structures for A/D
 *
 ***************************************************************************/

 volatile struct ANALOG_TO_DIGITIAL adc = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},	/* input array */
	{0},								/* channel */
	{A_D_MUX},							/* mux address */
	{A_D_STATUS},						/* status address */
	{A_D_BYTE},							/* data address */
	{A_D_ZERO},							/* zero */
	{A_D_MAX},							/* max */
	{A_D_MIN},							/* min */
	{A_D_GAIN},							/* gain */
	{BIPOLAR},							/* polarity */
 };

 volatile struct watch_AD wad;

/****************************************************************************
 *
 *      define data structures for D/A
 *
 ***************************************************************************/

 #ifdef LPM_AIO_DC

// volatile struct DIGITIAL_TO_ANALOG dac[NUM_D_A_CH] = {
//    {D_A_CH_0_ZERO,D_A_CHANNEL_0,D_A_CH_0_ZERO,D_A_CH_0_MAX,D_A_CH_0_MIN,D_A_CH_0_POLARITY},
//    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
// };

 volatile struct DIGITIAL_TO_ANALOG dac[8] = {
    {D_A_CH_0_ZERO,D_A_CHANNEL_0,D_A_CH_0_ZERO,D_A_CH_0_MAX,D_A_CH_0_MIN,D_A_CH_0_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
 };

 #else

 volatile struct DIGITIAL_TO_ANALOG dac[NUM_D_A_CH] = {
    {D_A_CH_0_ZERO,D_A_CHANNEL_0,D_A_CH_0_ZERO,D_A_CH_0_MAX,D_A_CH_0_MIN,D_A_CH_0_POLARITY},
    {D_A_CH_1_ZERO,D_A_CHANNEL_1,D_A_CH_1_ZERO,D_A_CH_1_MAX,D_A_CH_1_MIN,D_A_CH_1_POLARITY},
    {D_A_CH_2_ZERO,D_A_CHANNEL_2,D_A_CH_2_ZERO,D_A_CH_2_MAX,D_A_CH_2_MIN,D_A_CH_2_POLARITY},
    {D_A_CH_3_ZERO,D_A_CHANNEL_3,D_A_CH_3_ZERO,D_A_CH_3_MAX,D_A_CH_3_MIN,D_A_CH_3_POLARITY},
    {D_A_CH_4_ZERO,D_A_CHANNEL_4,D_A_CH_4_ZERO,D_A_CH_4_MAX,D_A_CH_4_MIN,D_A_CH_4_POLARITY},
    {D_A_CH_5_ZERO,D_A_CHANNEL_5,D_A_CH_5_ZERO,D_A_CH_5_MAX,D_A_CH_5_MIN,D_A_CH_5_POLARITY},
    {D_A_CH_6_ZERO,D_A_CHANNEL_6,D_A_CH_6_ZERO,D_A_CH_6_MAX,D_A_CH_6_MIN,D_A_CH_6_POLARITY},
    {D_A_CH_7_ZERO,D_A_CHANNEL_7,D_A_CH_7_ZERO,D_A_CH_7_MAX,D_A_CH_7_MIN,D_A_CH_7_POLARITY},
 };

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

 #pragma page(1)

/***************************************************************************
 *
 *
 *              A N A L O G   T O   D I G I T I A L
 *
 *
 **************************************************************************/

 #pragma page(1)

/***************************************************************************
 *
 *	ANALOG-to-DIGITIAL Converter Initialization
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 #ifndef REMOTE_I_O

 void init_A_D(void)
 {
/***************************************************************************
 *
 *	install interrupt service routine
 *
 ***************************************************************************/

	_disable();                     /* disable interrupts till installation complete */

 #ifdef PC

	a_d_old_vector = _dos_getvect(AD_VEC);

 #endif

	if(adc.POLARITY == BIPOLAR) {

 #ifdef PC

		_dos_setvect(AD_VEC,A_D_int_handler);

 #else

		install_vector(A_D_int_handler,AD_IRQ);

 #endif

		}
	else {

 #ifdef PC

		_dos_setvect(AD_VEC,A_D_U_int_handler);

 #else

 #pragma message("AIO UNIPOLAR ISR")
        install_vector(A_D_U_int_handler,AD_IRQ);

 #endif

		}

	adc.channel = 0;                /* set input channel to zero */

	a_d_get(adc.channel);           /* poll converter */

	outp(A_D_MUX,adc.channel);      /* output channel select to MUX */
									/* to start first conversion */

	int_enable(AD_IRQ);             /* enable interrupts from A/D */

	_enable();                      /* enable interrupts to processor */

 }

 #endif

 #pragma page(1)

/***************************************************************************
 *
 *	ANALOG-to-DIGITIAL Converter de-Initialization
 *
 *	written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 #ifndef REMOTE_I_O

 void de_init_A_D(void)
 {

	_disable(); 					/* disable interrupts till installation complete */

	int_disable(AD_IRQ);			/* disable interrupts from A/D */

 #ifdef PC

	_dos_setvect(AD_VEC,a_d_old_vector);

 #endif

	_enable();						/* enable interrupts to processor */

 }

 #endif

 #pragma page(1)

/***************************************************************************
 *
 *      A/D Input Interrupt Service Routine
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 #ifndef REMOTE_I_O

 void interrupt far A_D_int_handler(void)
 {
	_enable();                      /* enable interrutps */

    //
    // following code added for i486, where A/D is on IRQ7
    // which can be called by an incomplete interrupt
    //
    if( (inp(A_D_STATUS) & A_D_INT) != A_D_INT) {
        goto end_of_A_D_int;
		}

 #if !defined(DATA_16)

	adc.A_D[adc.channel++] = ( ( (signed)(inpw(A_D_BYTE)) & 0x0fff ) << 4) >> 4;
									/* input data, mask high nibble, adjust sign */
 #else

	adc.A_D[adc.channel++] = ( ( (signed)(inp(A_D_BYTE_LSB)|(inp(A_D_BYTE_MSB)<<8)) & 0x0fff ) << 4) >> 4;
                        
 #endif

	if(adc.channel > 15)            /* bounds check on channel */
		adc.channel = 0;			/* reset to first channel */

	outp(A_D_MUX,adc.channel);      /* output channel select to MUX */

    outp(ICU_BASE,AD_EOI);          /* specific EOI command */

 end_of_A_D_int:  ;             	/* end of interrupt service routine */

 }									/* return from interrupt */

 #endif

 #pragma page(1)

/***************************************************************************
 *
 *      A/D Input Interrupt Service Routine
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 #ifndef REMOTE_I_O

 void interrupt far A_D_U_int_handler(void)
 {
	_enable();					/* enable interrutps */


    if( (inp(A_D_STATUS) & A_D_INT) != A_D_INT) {
		goto end_of_A_D_U_int;
		}


 #if !defined(DATA_16)

	adc.A_D[adc.channel++] = ( (signed)(inpw(A_D_BYTE)) & 0x0fff );
								/* input data, mask high nibble, adjust sign */
 #else

	adc.A_D[adc.channel++] = ( (signed)(inp(A_D_BYTE_LSB)|(inp(A_D_BYTE_MSB)<<8)) & 0x0fff );
                        
 #endif

	if(adc.channel > 15)		/* bounds check on channel */
		adc.channel = 0;        /* reset to first channel */

	outp(A_D_MUX,adc.channel);	/* output channel select to MUX */

	outp(ICU_BASE,AD_EOI);		/* specific EOI command */

 end_of_A_D_U_int:  ;           /* end of interrupt service routine */

 }								/* return from interrupt */

 #endif

 #pragma page(1)

/***************************************************************************
 *
 *      A/D Polling Input
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 int a_d_get(unsigned channel)
 {
	int input;                      /* local variable for input data */
	int time_out;                   /* time out counter */

	outp(A_D_MUX,channel);          /* output channel select to MUX */

	time_out = 0;                   /* set time out */

	while( (inp(A_D_STATUS) & A_D_BUSY) ) {
		time_out++; 				/* increment time out */
		if(time_out>TIME_OUT) {
			return(INVALID);
			}						/* conversion time out */
		}

 #if !defined(DATA_16)

        input = ( inpw(A_D_BYTE) & 0x0fff );
                                    /* input and mask high nibble */
 #else

        input = ( (inp(A_D_BYTE_LSB) | (inp(A_D_BYTE_MSB)<<8)) & 0xfff);
                        
 #endif

	return(input);                  /* return value to caller */
 }

 #pragma page(1)

/***************************************************************************
 *
 *      sensor input from A/D board
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 #ifndef REMOTE_I_O

 void sensor_scan(struct A_D_INPUT *a_d_input)
 {
 #if !defined(DATA_16)

 int i;

	for(i=0;i<16;i++)
		a_d_input->a_d[i] = adc.A_D[i];
 #else

        a_d_input->a_d[0]  = adc.A_D[0];
        a_d_input->a_d[1]  = adc.A_D[1];
        a_d_input->a_d[2]  = adc.A_D[2];
        a_d_input->a_d[3]  = adc.A_D[3];
        a_d_input->a_d[4]  = adc.A_D[4];
        a_d_input->a_d[5]  = adc.A_D[5];
        a_d_input->a_d[6]  = adc.A_D[6];
        a_d_input->a_d[7]  = adc.A_D[7];
        a_d_input->a_d[8]  = adc.A_D[0];
        a_d_input->a_d[9]  = adc.A_D[9];
        a_d_input->a_d[10] = adc.A_D[10];
        a_d_input->a_d[11] = adc.A_D[11];
        a_d_input->a_d[12] = adc.A_D[12];
        a_d_input->a_d[13] = adc.A_D[13];
        a_d_input->a_d[14] = adc.A_D[14];
        a_d_input->a_d[15] = adc.A_D[15];

 #endif
 }

 #endif

 #pragma page(1)

/***************************************************************************
 *
 *      sensor input from A/D board
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void sensor_poll(struct A_D_INPUT *a_d_input)
 {
 #if !defined(DATA_16)

 int i;

	for(i=0;i<16;i++)
		a_d_input->a_d[i] = a_d_get(i);

 #else

        a_d_input->a_d[0]  = a_d_get(0);
        a_d_input->a_d[1]  = a_d_get(1);
        a_d_input->a_d[2]  = a_d_get(2);
        a_d_input->a_d[3]  = a_d_get(3);
        a_d_input->a_d[4]  = a_d_get(4);
        a_d_input->a_d[5]  = a_d_get(5);
        a_d_input->a_d[6]  = a_d_get(6);
        a_d_input->a_d[7]  = a_d_get(7);
        a_d_input->a_d[8]  = a_d_get(8);
        a_d_input->a_d[9]  = a_d_get(9);
        a_d_input->a_d[10] = a_d_get(10);
        a_d_input->a_d[11] = a_d_get(11);
        a_d_input->a_d[12] = a_d_get(12);
        a_d_input->a_d[13] = a_d_get(13);
        a_d_input->a_d[14] = a_d_get(14);
        a_d_input->a_d[15] = a_d_get(15);

 #endif
 }

 #pragma page(1)

/***************************************************************************
 *
 *      ANALOG-to-DIGITIAL Converter test
 *
 *      mode:   0       initialize test variables
 *              1       enable A/D interrupts and start conversion cycle
 *              2       disable A/D interrupts
 *              3       time out test on conversion
 *              4       time out test on A/D interrupts
 *              5       check A/D calibration
 *              6       step up/down calibration value
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 #ifndef REMOTE_I_O

 int test_A_D(char mode,volatile struct watch_AD *wad)
 {
 int flag;                      /* local variable for return */

	switch(mode)
	{
/***************************************************************************
 *
 *      initial A/D test variables
 *
 ***************************************************************************/
	case 0x00:              /* initial test varaibles */
		{
		wad->seed = 0;
		wad->inc = 1;
		if(dac[D_A_CAL_LOOP].POLARITY == BIPOLAR) {
			wad->seed_max = 4095;
			wad->seed_min = 0;
			}
		else
			{
			wad->seed_max = 2047;
			wad->seed_min = -2048;
			}
		wad->output = D_A_cal_out(wad->seed);
		flag = OK;
		break;
		}
/***************************************************************************
 *
 *      enable A/D interrupts
 *
 ***************************************************************************/
	case 0x01:              /* enable A/D interrupts */
		{
		init_A_D();
		flag = OK;
		break;
		}
/***************************************************************************
 *
 *      disable A/D interrupts
 *
 ***************************************************************************/
	case 0x02:              /* disable A/D interrupts */
		{
		int_disable(AD_IRQ);
		flag = OK;
		break;
		}
/***************************************************************************
 *
 *      time out test
 *
 ***************************************************************************/
	case 0x03:              /* time out test */
		{
		int_disable(AD_IRQ);            /* disable interrupts from A/D */

		outp(A_D_MUX,0x00);             /* output channel select to MUX */

		wad->time_out = 0;              /* set time out */

		while( (inp(A_D_STATUS) & A_D_BUSY) ) {
			wad->time_out++;                /* increment time out */
			if(wad->time_out>TIME_OUT)      /* check for time out */
				break;                  /* conversion time out */
			}

 #if !defined(DATA_16)

                wad->input = ( inpw(A_D_BYTE) & 0x0fff );
                                        /* input and mask high nibble */
 #else

                wad->input = ( (inp(A_D_BYTE_LSB) | (inp(A_D_BYTE_MSB)<<8) ) & 0xfff);
                                        /* input and mask high nibble */
 #endif

		if(wad->time_out>TIME_OUT)
			flag = ERR;
		else
			flag = OK;
		break;
		}
/***************************************************************************
 *
 *      check for A/D interrupts active
 *
 ***************************************************************************/
	case 0x04:              /* A/D interrupts active */
		{
		wad->channel = adc.channel;     /* get current converter channel */

		wad->time_out = 0;              /* set time out */

		while(wad->channel == adc.channel) {
			wad->time_out++;                /* increment time out */
			if(wad->time_out>TIME_OUT)      /* check for time out */
				break;                  /* conversion time out */
			}

		if(wad->time_out>TIME_OUT)
			flag = ERR;
		else
			flag = OK;
		break;
		}
/***************************************************************************
 *
 *      check A/D calibration at current calibration loop output
 *
 ***************************************************************************/
	case 0x05:              /* check A/D calibration */
		{
        wad->input = adc.A_D[A_D_CAL_LOOP];

		if( (wad->input < (wad->seed - 16)) || (wad->input > (wad->seed + 16)) )
			flag = ERR;
		else
			flag = OK;
		break;
		}
/***************************************************************************
 *
 *      step up/down calibration loop value
 *
 ***************************************************************************/
	case 0x06:              /* step up/down calibration */
		{
		wad->seed += wad->inc;

		if(wad->seed == wad->seed_max)
			wad->inc = -1;
		if(wad->seed == wad->seed_min)
			wad->inc = 1;

		wad->output = D_A_cal_out(wad->seed);

		flag = OK;
		break;
		}
/***************************************************************************
 *
 *
 *
 ***************************************************************************/

	} /* switch */

	return(flag);           /* return test status */
 }

 #endif

 #pragma page(1)

/****************************************************************************
 *
 *
 *              D I G I T I A L   T O   A N A L O G
 *
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      D/A initialization
 *
 *      Set output to zero volts
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void D_A_init(void)
 {
	int i;

 //#if !defined(DATA_16)
 //
 //       for(i=0;i<NUM_D_A_CH;i++)                /* output data to dacs */
 //       outpw(dac[i].ADDR,dac[i].D_A);
 //
 //#else
 //
 //       for(i=0;i<NUM_D_A_CH;i++) {              /* output data to dacs */
 //       outp(dac[i].ADDR,dac[i].D_A & 0xff);
 //       outp((dac[i].ADDR)+1,(dac[i].D_A >> 8) & 0xff);
 //               }
 //#endif
    outp(0x0104,0x00);
    outp(0x0105,0x00);
    outp(0x0106,0x00);
    outp(0x0107,0x00);
 }

 #pragma page(1)

/***************************************************************************
 *
 *      D/A calibration loop output
 *
 *      Word is a signed integer and is converted to offset binary for output
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 unsigned int D_A_cal_out(signed int word)
 {
	if(dac[D_A_CAL_LOOP].POLARITY == BIPOLAR) {
		dac[D_A_CAL_LOOP].D_A = ((unsigned)(word) ^ 0x800) & 0xfff;
		}		/* convert to unsigned value and fix up sign */
	else
		{
		dac[D_A_CAL_LOOP].D_A = ((unsigned)(word)) & 0xfff;
		}		/* convert to unsigned value */
 #if !defined(DATA_16)

	outpw(dac[D_A_CAL_LOOP].ADDR,dac[D_A_CAL_LOOP].D_A);
				/* output D/A */

 #else

	outp(dac[D_A_CAL_LOOP].ADDR,dac[D_A_CAL_LOOP].D_A & 0xff);
	outp((dac[D_A_CAL_LOOP].ADDR)+1,(dac[D_A_CAL_LOOP].D_A >> 8) & 0xff);

 #endif

	return(dac[D_A_CAL_LOOP].D_A);	/* return converted data */
 }

 #pragma page(1)

/***************************************************************************
 *
 *      D/A selected channel output
 *
 *      written by denton marlowe
 *	(c)1990-1993 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void D_A_channel_out(unsigned int d_a_channel, unsigned data)
 {
	dac[d_a_channel].D_A = data;	   /* store data in array */

 //#if !defined(DATA_16)
 //
 //   outpw(dac[d_a_channel].ADDR,data);     /* output D/A */
 //
 //#else
 //
 //   outp(dac[d_a_channel].ADDR,data & 0xff);
 //   outp((dac[d_a_channel].ADDR)+1,(data >> 8) & 0xff);
 //
 //#endif
    outp(0x0104,data & 0xff);
    outp(0x0105,(data >> 8) & 0xff);
    outp(0x0106,data & 0xff);
    outp(0x0107,(data >> 8) & 0xff);
 }

 void AIO_D_A_channel_out(unsigned int data0, unsigned int data1)
 {
    dac[0].D_A = data0;       /* store data in array */
    dac[1].D_A = data1;       /* store data in array */

 //#if !defined(DATA_16)
 //
 //   outpw(dac[d_a_channel].ADDR,data);     /* output D/A */
 //
 //#else

    outp(0x0104,data0 & 0xff);
    outp(0x0105,(data0 >> 8) & 0xff);
    outp(0x0106,data1 & 0xff);
    outp(0x0107,(data1 >> 8) & 0xff);

 //#endif
 }
