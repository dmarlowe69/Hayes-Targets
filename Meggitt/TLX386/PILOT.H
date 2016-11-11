// file: pilot.h
//

 #pragma message("PILOT.H")

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

 extern union INPUT input;                       /* assign storage */

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

 extern struct CONVERT convert;

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

 extern struct ACTUATOR actuator;

/****************************************************************************
 *
 *      general definitions
 *
 ***************************************************************************/

 #define INITIALIZE  -1
 #define STANDBY	  0
 #define CLOSED 	  1
 #define CAPTIVE      2
 #define MANUAL 	  3
 #define AUTO_DEBUG   4

 #define MANUAL_OFF 0
 #define MANUAL_ON  1

/////////////////////////////////////////////////////////////////////////////
//
//  TLX Specific Definitions
//
/////////////////////////////////////////////////////////////////////////////

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

 extern struct CMD_DATA cmd_data;          /* assign storage */

 void far decode_cmd_data(void);

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

 #undef com_puts
 #undef com_getc
 #undef com_gets
 #undef com_putc
 #undef com_check

 #define com_puts(x) dspio_1_puts(x)
 #define com_getc()  dspio_1_getc()
 #define com_gets(x) dspio_1_gets(x)
 #define com_putc(x) dspio_1_putc(x)
 #define com_check() dspio_1_check()
