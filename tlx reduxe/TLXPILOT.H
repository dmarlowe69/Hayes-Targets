// file: TLXPILOT.H

//////////////////////////////////////////////////////////////////////////////
//
//      TLX AUTOPILOT header file
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////

 #ifndef _INC_TLXPILOT

/////////////////////////////////////////////////////////////////////////////
//
//  TLX Specific Definitions
//
/////////////////////////////////////////////////////////////////////////////

//
//  Autopilot Gains
//

 #define KP_DEFAULT     0x66
 #define KV_DEFAULT		0x4c
 #define KI_DEFAULT		0x33
 #define KS_DEFAULT		0x50
 #define MR_DEFAULT		0x66
 #define KP_ALTERNATE   0x33

//
//  Maximum, Minimum and Recovery Mode Wing Position Commands
//

 #define MR_MIN         0x00
 #define MR_MAX 		0xff
 #define MR_REC 		0xbf

//
//  Altimeter Status
//

 #define LOCK   0x01
 #define UNLOCK 0x00

//
//  Internal Data Definitions
//

 #define NUM_ALT    16
 #define NUM_ACCEL	16

 extern int alt[NUM_ALT];
 extern int accel[NUM_ACCEL];

 extern int altitude_valid_flag;
 extern int altitude;
 extern int average_alt;
 extern int alt_error;
 extern int average_accel;
 extern int deviation;
 extern int preset;
 extern int height_rate;
 extern long int integrate_accel;
 extern int wing;
 extern long int sum;
 extern int KP,KV,KI,KS,MR;
 extern int loop_count;
 extern long kp_alt_error;
 extern long kv_height_rate;
 extern long alt_sum;
 extern long integrate_accel_dt;
 extern long ki_integrate_accel;
 extern int scale_wing;
 extern int hard_wing;

//
//  Autopilot Debugging Data Defintions
//

 #define FNUM_ALT_STEPS     15.0
 #define DELTA_T            0.0625

 extern float  fpreset;
 extern float  fvalid;
 extern float  falt;
 extern float  faccel;
 extern double dpreset;
 extern double dvalid;
 extern double dalt;
 extern double daccel;
 extern double dwing;

//
//  Command Byte Bit Defintions
//

 #define RCVY_MODE      0x01
 #define FLY_MODE		0x00

 #define PROG_STD       0x00
 #define PROG_GAIN		0x02
 #define PROG_ALT		0x04
 #define PROG_MANUAL	0x06

 #define ON             1
 #define OFF            0

//
//  Internal Command Data Structure
//

 struct TLX_CMD_DATA {
    signed int preset;              // 100ft/255 bits
    signed int proportional_gain;   // 5V/255 bits
    signed int rate_gain;           // 5V/255 bits
    signed int integral_gain;       // 5V/255 bits
    signed int delta_scale;         // 5V/255 bits
    signed int delta_ec_manual;     // 5V/255 bits
    signed int command;             // Command Byte

    signed int rcvy_fly;
    signed int prog_mode;
    };

 extern struct TLX_CMD_DATA tlx_cmd_data;  /* assign storage */

 struct TLX_SENSOR_DATA {
    signed int altitude_valid;
    signed int altitude;
    signed int accelerometer;
    signed int wing_command;
    };

 extern struct TLX_SENSOR_DATA tlx_sensor_data;     /* assign storage */

//
//  Function Prototypes
//

 void autopilot_init(void);
 void autopilot(void);
 void tlx_init(void);
 void tlx_pilot(void);
 void tlx_pilot_manual(void);
 void tlx_pilot_recovery(void);
 void tlx_sensor(void);
 void tlx_servo(void);
 void Abort_Processor(void);

 void autopilot_dump_header(void);
 void autopilot_dump(void);
 void preset_dump(void);
 void gain_dump(void);
 void alt_stack_dump(void);
 void accel_stack_dump(void);

 #define _INC_TLXPILOT
 #endif /* _INC_TLXPILOT */
