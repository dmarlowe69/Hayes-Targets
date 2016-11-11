// file: pilot.c
//

/****************************************************************************
 *
 *  Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include   <dos.h>
 #include   <conio.h>

/****************************************************************************
 *
 *	Include definitions
 *
 ***************************************************************************/

 #include	<hayes.h>
 #include   <gnd.h>

/****************************************************************************
 *
 *      define data structures for sensor inputs
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
 *      define data structures for sensor inputs
 *
 ***************************************************************************/

 struct SENSOR {
        signed int alt_lock;                            // ch0
        signed int alt_full_scale;                      // ch1
        signed int alt_ex_scale;                        // ch2
        signed int accelerometer;                       // ch3
        signed int wing_position;                       // ch4
        signed int batt;                                // ch5
        signed int spare_1;                             // ch6
        signed int spare_2;                             // ch7
        signed int spare_3;                             // ch8
        signed int spare_4;                             // ch9
        signed int spare_5;                             // ch10
        signed int spare_6;                             // ch11
        signed int spare_7;                             // ch12
        signed int spare_8;                             // ch13
        signed int calibration_loop;                    // ch14
    	signed int reference_gnd;
    };

 struct SENSOR_BYTE {
        unsigned char alt_lock_lo;
        unsigned char alt_lock_hi;
        unsigned char alt_full_scale_lo;
        unsigned char alt_full_scale_hi;
        unsigned char alt_ex_scale_lo;
        unsigned char alt_ex_scale_hi;
        unsigned char accelerometer_lo;
        unsigned char accelerometer_hi;
        unsigned char wing_position_lo;
        unsigned char wing_position_hi;
        unsigned char batt_lo;
        unsigned char batt_hi;
        unsigned char spare_1_lo;
        unsigned char spare_1_hi;
        unsigned char spare_2_lo;
        unsigned char spare_2_hi;
        unsigned char spare_3_lo;
        unsigned char spare_3_hi;
        unsigned char spare_4_lo;
        unsigned char spare_4_hi;
        unsigned char spare_5_lo;
        unsigned char spare_5_hi;
        unsigned char spare_6_lo;
        unsigned char spare_6_hi;
        unsigned char spare_7_lo;
        unsigned char spare_7_hi;
        unsigned char spare_8_lo;
        unsigned char spare_8_hi;
    	unsigned char calibration_loop_lo;
    	unsigned char calibration_loop_hi;
    	unsigned char reference_gnd_lo;
    	unsigned char reference_gnd_hi;
	};

/****************************************************************************
 *
 *      define data structure union for sensor inputs
 *
 ***************************************************************************/

 union INPUT {
    struct A_D_INPUT a_d_input;                 /* from hayes.h */
    struct A_D_INPUT_BYTE a_d_input_byte;       /* from hayes.h */
    struct SENSOR sensor;
    struct SENSOR_BYTE sensor_byte;
    };

 union INPUT input;                             /* assign storage */

/****************************************************************************
 *
 *      define data structure for scaled sensor inputs
 *
 ***************************************************************************/

 struct CONVERT {
    double alt_lock;
    double alt_full_scale;
    double alt_ex_scale;
    double acceleromter;
    double wing_position;
    double batt;
    double spare_1;
    double spare_2;
    double spare_3;
    double spare_4;
    double spare_5;
    double spare_6;
    double spare_7;
    double spare_8;
    double calibration_loop;
    double reference_gnd;
	};

 struct CONVERT convert;

/****************************************************************************
 *
 *      define data structures for autopilots
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      define data structures for autopilot outputs
 *
 ***************************************************************************/

 struct ACTUATOR {
    double rudder;
    double elevator;
    double aileron;
    double delta_rc_manual;
    double delta_ec_manual;
    double delta_ac_manual;
    };

 struct ACTUATOR actuator;

/****************************************************************************
 *
 *      define data structures for scaled autopilot outputs
 *
 ***************************************************************************/

 struct CONTROLS {
	signed int actuator_1;
	signed int actuator_2;
	signed int actuator_3;
	};

 struct CONTROLS controls;

/////////////////////////////////////////////////////////////////////////////
//
//  function prototypes
//
/////////////////////////////////////////////////////////////////////////////

 void autopilot_init(void);
 void autopilot(void);
 void actuator_convert(struct ACTUATOR *actuator, struct CONTROLS *controls);
 void sensor_convert(struct SENSOR *sensor,struct CONVERT *convert);
				/* convert sensor inputs to real units */
 void actuator_position(struct CONTROLS *controls);

/****************************************************************************
 *
 *      general definitions
 *
 ***************************************************************************/

 #define INITIALIZE  -1
 #define STANDBY      0
 #define CLOSED       1
 #define CAPTIVE      2
 #define MANUAL       3
 #define AUTO_DEBUG   4

 #define MANUAL_OFF 0
 #define MANUAL_ON  1

/////////////////////////////////////////////////////////////////////////////
//
//  TLX Specific Definitions
//
/////////////////////////////////////////////////////////////////////////////

 #define TLX_INIT_FLAG  0xaa55

 #define RCVY_FLY_BITS  0x01
 #define RCVY_MODE      0x01
 #define FLY_MODE		0x00

 #define PROG_BITS		0x06
 #define PROG_1 		0x02
 #define PROG_2 		0x04
 #define PROG_STD       0x00
 #define PROG_GAIN		0x02
 #define PROG_ALT		0x04
 #define PROG_MANUAL	0x06

 #define KP_DEFAULT		0x66
 #define KV_DEFAULT		0x4c
 #define KI_DEFAULT		0x33
 #define KS_DEFAULT		0x50
 #define MR_DEFAULT		0x66

 #define MR_MIN         0x00
 #define MR_MAX 		0xff
 #define MR_REC 		0xbf

 #define FIRE_BITS		0x78
 #define FIRE_1 		0x08
 #define FIRE_2			0x10
 #define FIRE_3 		0x20
 #define FIRE_4 		0x40

 #define ON 			1
 #define OFF            0

/****************************************************************************
 *
 *	general defintions
 *
 ***************************************************************************/

 #define NUM_ALT    16
 #define NUM_ACCEL	16

 #define FNUM_ALT_STEPS 	15.0
 #define DELTA_T			0.0625

/****************************************************************************
 *
 *	general defintions
 *
 ***************************************************************************/

 int alt[NUM_ALT];
 int accel[NUM_ACCEL];

 int average_alt;
 int alt_error;
 int deviation;
 int preset;
 int height_rate;
 long int integrate_accel;
 int wing;
 long int sum;
 int KP,KV,KI,KS,MR;
 int loop_count;

 float	fpreset;
 double dwing;
 float	falt;
 float  faccel;

/****************************************************************************
 *
 *	Data Structures
 *
 ***************************************************************************/

 struct TLX {
	int preset;
	int preset_1;
	int preset_10;
	int preset_100;
	int preset_flag;
	int kp;
	int kv;
	int ki;
	int ks;
	int mr;
	int command;
	int recovery_fly_flag;
	int prog_flag;
	int fire_flag;
	int fire_1;
	int fire_2;
	int fire_3;
	int fire_4;

	int init_flag;
	int eng_panel_flag;
	};

 struct TLX_TM {
	int recovery_fly_flag;
	int prog_flag;
	int fire_1;
	int fire_2;
	int fire_3;
	int fire_4;
	};

 struct TLX tlx;

 struct TLX_TM tlx_tm;

/****************************************************************************
 *
 *	function prototypes
 *
 ***************************************************************************/

 void tlx_init(void);
 void tlx_pilot(void);
 void alt_stack_out(void);
 void accel_stack_out(void);
 void tlx_sensor(void);

/***************************************************************************
 *
 *	define decoded command variables
 *
 **************************************************************************/

 struct CMD_DATA {
    signed int preset;
    signed int proportional_gain;
    signed int rate_gain;
    signed int integral_gain;
    signed int delta_scale;
    signed int delta_ec_manual;

    signed int rcvy_fly;
    signed int prog_mode;
    signed int fire_mode;

    unsigned char command;          /* command channel */

    unsigned char channel_mode;     /* rte1/rte2 channel mode */

    unsigned int frame_count;       /* command data frame count */

    unsigned char tm_mode;          /* telemetry mode */
    unsigned char cmd_echo;         /* command word echo */
    unsigned char status_flag;      /* status word */

    unsigned int uart_cmd;                  /* uart command mode flag */

    unsigned long int watch_dog;    /* watch dog counter for cmd/tm */
    unsigned long int watch_count;  /* watch dog counter for cmd/tm */
    };

 struct CMD_DATA cmd_data;          /* assign storage */

 #pragma page(1)

/***************************************************************************
 *
 *      Initialize AUTOPILOT
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

 void autopilot_init(void)
 {

    dspio_1_puts("AUTO Init\r\n");

    tlx_init();

 }

 #pragma page(1)

/***************************************************************************
 *
 *      AUTOPILOT
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

 void autopilot(void)
 {
/****************************************************************************
 *
 *      input sensor data and convert to units
 *
 ***************************************************************************/

	sensor_scan(&input.a_d_input);				  /* sensor scan */

    sensor_convert(&input.sensor,&convert);       /* scale sensor input */

/****************************************************************************
 *
 *      call the autopilot
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      scale position of actuators
 *
 ***************************************************************************/

    actuator_convert(&actuator,&controls);  /* command converison */

/****************************************************************************
 *
 *      position actuators
 *
 ***************************************************************************/

     actuator_position(&controls);            /* actuator position controller */

 }

 #pragma page(1)

/***************************************************************************
 *
 *	sensor input conversion
 *
 *	written by denton marlowe
 *	(c)1990 Hayes Targets
 *
 **************************************************************************/

 void sensor_convert(struct SENSOR *sensor,struct CONVERT *convert)
 {
 }

 #pragma page(1)

/***************************************************************************
 *
 *      Actuator command converison function
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/

 void actuator_convert(struct ACTUATOR *actuator, struct CONTROLS *controls)
 {
 }

 #pragma page(1)

/***************************************************************************
 *
 *      Position actuator through D/A board
 *
 *      written by denton marlowe
 *      (c)1994 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void actuator_position(struct CONTROLS *controls)
 {
    //D_A_channel_out(0,(*controls).actuator_1);      /* rudder */
    //D_A_channel_out(1,(*controls).actuator_2);      /* elevator */
    //D_A_channel_out(2,(*controls).actuator_3);      /* aileron */
 }

/***************************************************************************
 *
 *	initialize autopilot
 *
 **************************************************************************/
 void tlx_init(void)
 {
 int i;

	for(i=0;i<NUM_ALT;i++) {
		alt[i] = 0;
		}

	for(i=0;i<NUM_ACCEL;i++) {
		accel[i] = 0;
		}

	loop_count = 0;

    //preset = (int)(fpreset * 256.0/100.0);

    preset = cmd_data.preset;

    switch(cmd_data.prog_mode) {
        case PROG_STD:
            {
        	KP = 0x66;  /* 102 - 2.00   volts */
        	KV = 0x4c;  /*  76 - 1.50   volts */
        	KI = 0x33;  /*  52 - 1.00   volts */
        	KS = 0x50;  /*  80 - 1.568  volts */
            MR = 0x66;  /* 102 - 2.00   volts */
            break;
        	}
        case PROG_GAIN:
            {
    	    KP = cmd_data.proportional_gain;
        	KV = cmd_data.rate_gain;
        	KI = cmd_data.integral_gain;
        	KS = cmd_data.delta_scale;
        	MR = cmd_data.delta_ec_manual;
            break;
            }
        case PROG_ALT:
            {
            KP = 0x66;  /* 102 - 2.00   volts */
        	KV = 0x4c;  /*  76 - 1.50   volts */
        	KI = 0x33;  /*  52 - 1.00   volts */
        	KS = 0x50;  /*  80 - 1.568  volts */
        	MR = 0x66;  /* 102 - 2.00   volts */
            break;
            }
        case PROG_MANUAL:
            {
            KP = 0x66;  /* 102 - 2.00   volts */
        	KV = 0x4c;  /*  76 - 1.50   volts */
        	KI = 0x33;  /*  52 - 1.00   volts */
        	KS = 0x50;  /*  80 - 1.568  volts */
        	MR = 0x66;  /* 102 - 2.00   volts */
            break;
        	}
        default:
            {
            KP = 0x66;  /* 102 - 2.00   volts */
        	KV = 0x4c;  /*  76 - 1.50   volts */
        	KI = 0x33;  /*  52 - 1.00   volts */
        	KS = 0x50;  /*  80 - 1.568  volts */
        	MR = 0x66;  /* 102 - 2.00   volts */
            break;
            }
        }


    dspio_1_puts("tlx_init()\r\n");
    printf("KP %x\n",KP);
    printf("KV %x\n",KV);
    printf("KI %x\n",KI);
    printf("KS %x\n",KS);
    printf("MR %x\n",MR);

 }

/***************************************************************************
 *
 *	TLX autopilot
 *
 **************************************************************************/
 void tlx_pilot(void)
 {
 int i;

/***************************************************************************
 *
 *	Shift ALT stack down one entry
 *
 **************************************************************************/

	for(i=NUM_ALT-1;i>0;i--) {
		alt[i] = alt[i-1];
		}
/*
	alt[15] = alt[14];
	alt[14] = alt[13];
	alt[13] = alt[12];
	alt[12] = alt[11];
	alt[11] = alt[10];
	alt[10] = alt[ 9];
	alt[ 9] = alt[ 8];
	alt[ 8] = alt[ 7];
	alt[ 7] = alt[ 6];
	alt[ 6] = alt[ 5];
	alt[ 5] = alt[ 4];
	alt[ 4] = alt[ 3];
	alt[ 3] = alt[ 2];
	alt[ 2] = alt[ 1];
	alt[ 1] = alt[ 0];
*/

/***************************************************************************
 *
 *	Shift ACCEL stack down one entry
 *
 **************************************************************************/

	for(i=NUM_ACCEL-1;i>0;i--) {
		accel[i] = accel[i-1];
		}
/*
	accel[15] = accel[14];
	accel[14] = accel[13];
	accel[13] = accel[12];
	accel[12] = accel[11];
	accel[11] = accel[10];
	accel[10] = accel[ 9];
	accel[ 9] = accel[ 8];
	accel[ 8] = accel[ 7];
	accel[ 7] = accel[ 6];
	accel[ 6] = accel[ 5];
	accel[ 5] = accel[ 4];
	accel[ 4] = accel[ 3];
	accel[ 3] = accel[ 2];
	accel[ 2] = accel[ 1];
	accel[ 1] = accel[ 0];
*/

/***************************************************************************
 *
 *	update readings
 *
 **************************************************************************/

	tlx_sensor();

/***************************************************************************
 *
 *	average last four altitude samples
 *
 **************************************************************************/

	average_alt = 0;

	for(i=0;i<4;i++) {
		average_alt = average_alt + alt[i];
	}

	average_alt = average_alt >> 2;

/***************************************************************************
 *
 *	put average into alt stack
 *
 **************************************************************************/

	alt[0] = average_alt;

/***************************************************************************
 *
 *	calculate altitude error
 *
 **************************************************************************/

	alt_error = preset - average_alt;

	sum = (long)(KP * alt_error);

/***************************************************************************
 *
 *	calculate altitude deviation
 *
 **************************************************************************/

	deviation = -alt_error;

	if(deviation > 127 )
		deviation = 127;

	if(deviation < -128)
		deviation = 128;

	deviation = deviation + 128;

/***************************************************************************
 *
 *	determine height rate
 *
 **************************************************************************/

	height_rate = alt[NUM_ALT-1] - alt[0];

	height_rate = height_rate;

	sum = sum + (long)(KV * height_rate);

/***************************************************************************
 *
 *	check loop counter
 *
 **************************************************************************/

	loop_count++;
	if(loop_count == 4) {
		loop_count = 0;

/***************************************************************************
 *
 *	integrate accelerometer
 *
 **************************************************************************/

		integrate_accel = 0;
		for(i=0;i<NUM_ACCEL;i++) {
			integrate_accel = integrate_accel + (NUM_ACCEL - i) * accel[i];
		}

		integrate_accel = integrate_accel >> 8;

		sum = sum + (long)(KI) * integrate_accel;

/***************************************************************************
 *
 *	scale result
 *
 **************************************************************************/

		wing = (int)(sum >> 5);

/***************************************************************************
 *
 *	add in hardware offset
 *
 **************************************************************************/

		wing = wing + 0;

/***************************************************************************
 *
 *	subtrack manual reset
 *
 **************************************************************************/

		wing = wing - MR;

/***************************************************************************
 *
 *	limit wing
 *
 **************************************************************************/

		if(wing < -255)
			wing = 255;
		if(wing > 255)
			wing = 255;

		dwing = (double)(wing) * 10.0/256.0;

	} /* if loop_count */

 }

 #pragma page(1)

/***************************************************************************
 *
 *
 *
 **************************************************************************/
 void alt_stack_out(void)
 {
 int i;

	for(i=0;i<NUM_ALT;i++) {
        printf("%d %d\n",i,alt[i]);
    }

 }

 #pragma page(1)

/***************************************************************************
 *
 *
 *
 **************************************************************************/
 void accel_stack_out(void)
 {
 int i;

	for(i=0;i<NUM_ACCEL;i++) {
        printf("%d %d\n",i,accel[i]);
    }

 }

 #pragma page(1)

/***************************************************************************
 *
 *	update sensor reading
 *
 **************************************************************************/
 void tlx_sensor(void)
 {
	alt[0] = (int)( falt * 256.0/100.0);
	accel[0] = (int)(faccel * 256.0/1.0);
 }

 #pragma page(1)

/*@H*************************************************************************
 *
 *  Command data decoder
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 ************************************************************************@H*/
 void far decode_cmd_data(void)
 {
    //
    //  Thumb wheel is ch1, ch2 and ch3
    //
    cmd_data.preset = ((cmd.data[1]^0xff)/27)*1 \
                      + ((cmd.data[2]^0xff)/27)*10 \
                      + ((cmd.data[3]^0xff)/27)*100;

    //
    //  ch4 - proportional gain
    //
    cmd_data.proportional_gain = cmd.data[6];

    //
    //  ch5 - rate gain
    //
    cmd_data.rate_gain = cmd.data[5];

    //
    //  ch6 - integral gain
    //
    cmd_data.integral_gain = cmd.data[6];

    //
    //  ch7 - scale factor
    //
    cmd_data.delta_scale = ( (cmd.data[7] & 0xf0) >> 4 ) + 1;

    //
    //  ch8 - manual reset
    //
    cmd_data.delta_ec_manual = cmd.data[8] * cmd_data.delta_scale;

    //
    //  ch9 - command channel
    //
    cmd_data.command = cmd.data[9];

    //
    //  RCVY/FLY
    //
    cmd_data.rcvy_fly = cmd.data[9] & RCVY_FLY_BITS;

    //
    //  PROG
    //
    cmd_data.prog_mode = cmd.data[9] & PROG_BITS;

    //
    //  FIRE
    //
    cmd_data.fire_mode = cmd.data[9] & FIRE_BITS;

    //
    //  frame counter
    //
    cmd_data.frame_count++;
 }
