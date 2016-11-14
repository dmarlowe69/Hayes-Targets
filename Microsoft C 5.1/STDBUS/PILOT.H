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
	signed int pitch_angle; 			// ch0
	signed int roll_angle;				// ch1
	signed int yaw_rate;				// ch2 - was pitch_rate
	signed int pitch_rate;				// ch3 - was yaw_rate
	signed int lateral_accel;			// ch4 - was vertical_accel
	signed int roll_rate;				// ch5 - was vertical_accel_raw
	signed int vertical_accel;			// ch6 - was lateral_accel
	signed int ax_accel;				// ch7 - was lateral_accel_raw
	signed int cable_tension;			// ch8 - was cable_angle;
	signed int elevator;				// ch9 - was rudder
	signed int rudder;					// ch10- was elevator
	signed int aileron; 				// ch11
	signed int power_main;				// ch12
	signed int cable_pitch; 			// ch13
	signed int calibration_loop;		// ch14
	signed int reference_gnd;
	};

 struct SENSOR_BYTE {
	unsigned char pitch_angle_lo;
	unsigned char pitch_angle_hi;
	unsigned char roll_angle_lo;
	unsigned char roll_angle_hi;
	unsigned char yaw_rate_lo;
	unsigned char yaw_rate_hi;
	unsigned char pitch_rate_lo;
	unsigned char pitch_rate_hi;
	unsigned char lateral_accel_lo;
	unsigned char lateral_accel_hi;
	unsigned char roll_rate_lo;
	unsigned char roll_rate_hi;
	unsigned char vertical_accel_lo;
	unsigned char vertical_accel_hi;
	unsigned char ax_accel_lo;
	unsigned char ax_accel_hi;
    unsigned char cable_tension_lo;     // cable_angle_lo;
    unsigned char cable_tension_hi;     // cable_angle_hi;
	unsigned char elevator_lo;
	unsigned char elevator_hi;
	unsigned char rudder_lo;
	unsigned char rudder_hi;
	unsigned char aileron_lo;
	unsigned char aileron_hi;
	unsigned char power_main_lo;
	unsigned char power_main_hi;
    unsigned char cable_pitch_lo;
    unsigned char cable_pitch_hi;
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
    double pitch_angle;
    double roll_angle;
    double pitch_rate;
    double yaw_rate;
    double vertical_accel;
	double roll_rate;			// was vertical_accel_raw
	double lateral_accel;
	double ax_accel;			// was lateral_accel_raw
	double cable_tension;		// was cable_angle
    double rudder;
    double elevator;
    double aileron;
    double power_main;
    double cable_pitch;
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

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////

 void autopilot_keys(void);
 void autopilot_menu(void);
 void set_autopilot_mode(long mode);
 long get_autopilot_mode(void);
 void set_pilot_safe_mode(int flag);
 int get_pilot_safe_mode(void);

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////

 void set_pilot_debugging_messages(int flag);
 int  get_pilot_debugging_messages(void);
