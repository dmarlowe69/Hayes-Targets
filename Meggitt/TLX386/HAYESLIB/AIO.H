/* file: aio.h */

/****************************************************************************
 *
 *	ANALOG hardware header file
 *
 *  written by denton marlowe
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
