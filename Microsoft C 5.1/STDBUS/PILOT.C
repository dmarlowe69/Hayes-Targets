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

 #include   "v_str.h"
 #include   "utility.h"
 #include   "cmd_tm.h"
 #include   "ground.h"

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

 union INPUT input;                             /* assign storage */

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
 *      general definitions
 *
 ***************************************************************************/

 #define INITIALIZE  -1
 #define STANDBY	  0
 #define CLOSED 	  1
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

/****************************************************************************
 *
 *  local global variable (not public)
 *
 ***************************************************************************/

 static int pilot_safe_mode          = DISABLE;
 static int pilot_debugging_messages = DISABLE;

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

	set_pilot_debugging_messages(DISABLE);

    com_puts("AUTO Init\r\n");

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

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 void autopilot_keys(void)
 {
 #ifndef _EPROM
 #pragma message("run time debugging included")

 int key;

    _PRINT_APPLICATION_NAME();
 	_PRINT_AUTHOR_NAME();
 	_PRINT_HAYES_COPYRIGHT();

    _PRINT_VERSION_STRING();

    com_puts("\r\n>");

        key = com_getc();

        switch(toupper(key)) {

            case 0x1b:
            	autopilot_menu();
				break;

			default:
				com_puts("[?]");
                com_puts("\r\nExiting Autopilot Menu\r\n");
                break;
			}
 #endif
 }

 #pragma page(1)

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 void autopilot_menu(void)
 {
 #ifndef _EPROM
 #pragma message("run time debugging included")

	int c;

	while(TRUE)
	{
        com_puts("\r\n\r\nAUTOPILOT FUNCUTIONS\r\n");

		com_puts("0. autopilot init\r\n");
		com_puts("1. standy mode\r\n");
		com_puts("2. closed mode\r\n");
		com_puts("3. manual mode\r\n");
		com_puts("4. Debug  mode\r\n");
		com_puts("5. actuator init\r\n");
		com_puts("6. actuator flex\r\n");
        com_puts("9. exit\r\n");
        com_puts("G. Ground Menu\r\n");

        com_puts("\r\n    Make Selection ");

		c = com_getc();         /* get a character from the serial port */

		switch(toupper(c))
		{
            case '0':
					set_autopilot_mode(-1L);
					break;

            case '1':
					set_autopilot_mode(0L);
					break;

            case '2':
					set_autopilot_mode(1L);
					break;

            case '3':
					set_autopilot_mode(3L);
					break;

            case '4':
					set_autopilot_mode(4L);
					break;

            case '5':
					actuator_init();
					break;

            case '6':
					int_disable(TCT0_IRQ);
					int_enable(TCT0_IRQ);
					actuator_flex();
                    actuator_init();
					int_enable(TCT0_IRQ);
					break;

            case '9':
					com_puts("\r\nExiting Autopilot Menu\r\n");
                    return;

            case 'G':
                    ground_menu();
                    break;

            default:
					break;
		}
	}
 #endif
 }

 #pragma page(1)

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 void set_autopilot_mode(long mode)
 {
    switch(mode) {

		case -1L:					 // init mode
			int_disable(TCT0_IRQ);
            autopilot_init();
            int_enable(TCT0_IRQ);
            break;

        case 0L:            	     // standby mode
			com_puts("AUTO STANDBY\r\n");
			int_disable(TCT0_IRQ);
            //ap_mode=mode;
            int_enable(TCT0_IRQ);
            break;

        case 1L:
            com_puts("AUTO ENGAGED\r\n");
			int_disable(TCT0_IRQ);
            //ap_mode=mode;
            int_enable(TCT0_IRQ);
            break;

        case 2L:
            com_puts("AUTO CAPTIVE\r\n");
			int_disable(TCT0_IRQ);
            //ap_mode=mode;
            int_enable(TCT0_IRQ);
            break;

        case 3L:
            com_puts("AUTO MANUAL\r\n");
			int_disable(TCT0_IRQ);
            //ap_mode=mode;
            int_enable(TCT0_IRQ);
            break;

        case 4L:
			com_puts("DEBUGGING NESSAGES ");
			int_disable(TCT0_IRQ);
			if(get_pilot_debugging_messages()==DISABLE) {
				set_pilot_debugging_messages(ENABLE);
				com_puts("ENABLED\r\n");
				}
			else {
				set_pilot_debugging_messages(DISABLE);
				com_puts("DISABLED\r\n");
				}
			int_enable(TCT0_IRQ);
            break;

		default:
            com_puts("AUTO NOT_A_MODE\r\n");
			break;
        }
 }

 #pragma page(1)

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 long get_autopilot_mode(void)
 {
    //return(ap_mode);
 }

 #pragma page(1)

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 void set_pilot_safe_mode(int flag)
 {
	pilot_safe_mode = flag;
	if (flag == ENABLE)
		cmd_data.status_flag = 0x0A;
	else
		cmd_data.status_flag = 0x00;
 }

 int get_pilot_safe_mode(void)
 {
	return(pilot_safe_mode);
 }

 #pragma page(1)

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////
 void set_pilot_debugging_messages(int flag)
 {
	pilot_debugging_messages = flag;
 }

 int get_pilot_debugging_messages(void)
 {
	return(pilot_debugging_messages);
 }
