// file: TLXPILOT.C */

//////////////////////////////////////////////////////////////////////////////
//
//      TLX Autopilot functions
//
//      written by denton marlowe
//      (c)1990-1993 Hayes Targets
//
//////////////////////////////////////////////////////////////////////////////

//
//      Include definitions
//

 #include   <stdlib.h>
 #include   <dos.h>
 #include   <conio.h>
 #include   "ground.h"
 #include   "cmd_tm.h"

/////////////////////////////////////////////////////////////////////////////
//
//  TLX Specific Definitions
//
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
//
// WING LIMITS
//
// HARDWARE WING POSITION
// BASED ON FOLLOWING CALIBRATION
//
// ANGLE  HEX  DEC  VOLTS
//
// +1     FF   255   2.5
//  0     E5   229   2.25
// -1     CC   204   2.0
// -1.5   BF   191   1.875
// -2     B2   178   1.75
// -3     99   153   1.5
// -4     7F   127   1.25
// -5     66   102   1.0
// -6     4C    76   0.75
// -7     33    51   0.50
// -8     19    25   0.25
// -9     00     0   0.0
//
// SOFTWARE WING POSITION
// BASED ON THE FOLLOWING CONVENTION
//
// ANGLE    HEX
//
// +8       00CC + 00E5 = 01B1
//  0       0000 + 00E5 = 00E5
// -8       FF34 + 00E5 = 0019
//
//////////////////////////////////////
// WING POSITON EQUATES
//////////////////////////////////////
 #define WNGPOS  0xE5      // MAX POSITIVE (0)
 #define WNGZER  0xE5      // ZERO ANGLE
 #define WNGREC  0xBF      // RECOVERY
 #define WNGNEG  0x19      // MAX NEGATIVE (-8)

 #define WNGMAX  0x000000CC    // ABS POS LIMIT (+8)
 #define WNGXRI  0xFFFFFF34    // ABS NEG LIMIT (-8)
//////////////////////////////////////
//
// ACCELEROMETER SCALING
//
// G'S    HEX    DEC    VOLTS
//
// +1.5   FF     255     2.5
// +1.25  BF     191     1.875
// +1     7F     127     1.25
// +0.75  3F      63     0.625
// +0.5   00       0     0.0
//
//////////////////////////////////////

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

 int alt[NUM_ALT];
 int accel[NUM_ACCEL];

 int altitude_valid_flag;
 int altitude;
 int average_alt;
 int average_accel;
 int alt_error;
 int deviation;
 int preset;
 int height_rate;
 long int integrate_accel;
 int wing;
 long int sum;
 int KP,KV,KI,KS,MR;
 int loop_count;
 long kp_alt_error;
 long kv_height_rate;
 long alt_sum;
 long integrate_accel_dt;
 long ki_integrate_accel;
 int scale_wing;
 int hard_wing;

//
//  Autopilot Debugging Data Defintions
//

 #define FNUM_ALT_STEPS     15.0
 #define DELTA_T            0.0625

 float  fpreset;
 float  fvalid;
 float  falt;
 float  faccel;
 double dpreset;
 double dvalid;
 double dalt;
 double daccel;
 double dwing;
 long debug_counter;
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
    signed int fire_mode;
    };

 struct TLX_CMD_DATA tlx_cmd_data;	/* assign storage */

 struct TLX_SENSOR_DATA {
    signed int altitude_valid;
    signed int altitude;
    signed int accelerometer;
    signed int wing_command;
    };

 struct TLX_SENSOR_DATA tlx_sensor_data;     /* assign storage */

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
//////////////////////////////////////////////////////////////////////////////
//
//      Initialize AUTOPILOT
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void autopilot_init(void)
 {

    printf("AUTOPILOT Init\n");

    //fvalid = 5.0;
    //falt = 75.0;
    //faccel = 0.0;

    tlx_init();

 }
//////////////////////////////////////////////////////////////////////////////
//
//      AUTOPILOT
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void autopilot(void)
 {
    if(tlx_cmd_data.rcvy_fly == RCVY_MODE) {
        tlx_pilot_recovery();
        return;
    	}

	switch(tlx_cmd_data.prog_mode) {
        case PROG_STD:
            {
    	    tlx_pilot();
            break;
        	}
        case PROG_GAIN:
            {
            tlx_pilot();
            break;
            }
        case PROG_ALT:
            {
            tlx_pilot();
            break;
            }
        case PROG_MANUAL:
            {
            tlx_pilot_manual();
            break;
        	}
        default:
            {
            tlx_pilot_recovery();
            break;
            }
        }
 }
//////////////////////////////////////////////////////////////////////////////
//
//      Initialize TLX AUTOPILOT
//
//      written by denton marlowe
//      (c)1990 Hayes Targets
//      (c)1999 Meggitt Defense Systems
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_init(void)
 {
 int i;

//
//  Zero Altitude Stack
//
    for(i=0;i<NUM_ALT;i++) {
		alt[i] = 0;
		}
//
//  Zero Accelerometer Stack
//
    for(i=0;i<NUM_ACCEL;i++) {
		accel[i] = 0;
		}
//
//  Zero Internal Autopilot Loop Count
//
    loop_count = 0;
//
//  Load Internal Preset Altitude
//
    preset = tlx_cmd_data.preset;
//
//  Check Autopilot Mode
//
    switch(tlx_cmd_data.prog_mode) {
        case PROG_STD:
            {
            KP = KP_DEFAULT;  /* 0x66 - 102 - 2.00   volts */
            KV = KV_DEFAULT;  /* 0x4c -  76 - 1.50   volts */
            KI = KI_DEFAULT;  /* 0x33 -  52 - 1.00   volts */
            KS = ( (KS_DEFAULT & 0xf0) >> 4 );
                              /* 0x50 -  80 - 1.568  volts */
            MR = MR_DEFAULT;  /* 0x66 -  102 - 2.00  volts */
            break;
        	}
        case PROG_GAIN:
            {
            KP = tlx_cmd_data.proportional_gain;
            KV = tlx_cmd_data.rate_gain;
            KI = tlx_cmd_data.integral_gain;
            KS = tlx_cmd_data.delta_scale;
            MR = tlx_cmd_data.delta_ec_manual;
            break;
            }
        case PROG_ALT:
            {
            KP = KP_ALTERNATE;/* 0x33 52 - 1.00   volts */
            KV = KV_DEFAULT;  /* 0x4c 76 - 1.50   volts */
            KI = KI_DEFAULT;  /* 0x33 52 - 1.00   volts */
            KS = ( (KS_DEFAULT & 0xf0) >> 4 );
                              /* 0x50 -  80 - 1.568  volts */
            MR = MR_DEFAULT;  /* 0x66102 - 2.00   volts */
            break;
            }
        case PROG_MANUAL:
            {
            MR = tlx_cmd_data.delta_ec_manual;
            break;
        	}
        default:
            {
            KP = KP_DEFAULT;
            KV = KV_DEFAULT;
            KI = KI_DEFAULT;
            KS = ( (KS_DEFAULT & 0xf0) >> 4 );
                              /* 0x50 -  80 - 1.568  volts */
            MR = MR_DEFAULT;
            break;
            }
        }
 }
//////////////////////////////////////////////////////////////////////////////
//
//	  TLX autopilot
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_pilot(void)
 {
 int i;

//
//  Shift ALT stack down one entry
//

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

//
//  Shift ACCEL stack down one entry
//

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

//
//  	update readings
//

    tlx_sensor();

//
//      Check For Radar Lock
//

    if(tlx_sensor_data.altitude_valid > 127) {
            altitude_valid_flag = LOCK;
            }
         else {
            altitude_valid_flag = UNLOCK;
            Abort_Processor();
            return;
            }

    alt[0] = tlx_sensor_data.altitude;
    if(alt[0] < 0x04) {
            Abort_Processor();
            return;
        }

    accel[0] = tlx_sensor_data.accelerometer;

//
//	  average last four altitude samples
//

	average_alt = 0;

	for(i=0;i<4;i++) {
		average_alt = average_alt + alt[i];
	}

	average_alt = average_alt >> 2;

//
//  put average into alt stack
//

    alt[0] = average_alt;

//
//  calculate altitude error
//

	alt_error = preset - average_alt;

    kp_alt_error = (long)(KP * alt_error);

//
//  calculate altitude deviation
//

	deviation = -alt_error;

	if(deviation > 127 )
		deviation = 127;

	if(deviation < -128)
        deviation = -128;

	deviation = deviation + 128;

//
//  determine height rate
//

	height_rate = alt[NUM_ALT-1] - alt[0];

    kv_height_rate = (long)(KV * height_rate);

//
//
//
    alt_sum = kp_alt_error + kv_height_rate;

//
//	average last four accelerometer samples
//

    average_accel = 0;

	for(i=0;i<4;i++) {
        average_accel = average_accel + accel[i];
	}

    average_accel = average_accel >> 2;

//
//  put average into accel stack
//

    accel[0] = average_accel;
//
//  check loop counter
//

	loop_count++;
	if(loop_count == 4) {
		loop_count = 0;

//
//  integrate accelerometer
//

    integrate_accel = 0;
    for(i=0;i<NUM_ACCEL;i++) {
        integrate_accel = integrate_accel + (NUM_ACCEL - i) * accel[i];
    	}

    integrate_accel_dt = integrate_accel >> 8;

    ki_integrate_accel = (long)(KI) * integrate_accel_dt;

    sum = alt_sum + ki_integrate_accel;

//
//  scale result
//

    scale_wing = (int)(sum >> KS);

//
//  check wing command for overflow
//

//    if(scale_wing < WNGXRI)
//        scale_wing = WNGXRI;
//    if(scale_wing > WNGMAX)
//        scale_wing = WNGMAX;

//
//  add in hardware offset
//

    hard_wing = scale_wing + WNGZER;

//
//  subtrack manual reset
//

    wing = hard_wing - MR;

//
//  limit wing command
//

	if(wing < WNGNEG)
    	wing = WNGNEG;
    if(wing > WNGPOS)
        wing = WNGPOS;

	tlx_servo();

	} /* if loop_count */

 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_pilot_manual(void)
 {
	tlx_sensor_data.wing_command = MR;

	dwing = (double)(MR) * 10.0/255.0;
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_pilot_recovery(void)
 {
 int i;

//
//  Shift ALT stack down one entry
//

	for(i=NUM_ALT-1;i>0;i--) {
		alt[i] = alt[i-1];
        }
//
//  	update readings
//

    tlx_sensor();

    alt[0] = tlx_sensor_data.altitude;

//
//	  average last four altitude samples
//

	average_alt = 0;

	for(i=0;i<4;i++) {
		average_alt = average_alt + alt[i];
	}

    average_alt = average_alt >> 2;

//
//  put average into alt stack
//

    alt[0] = average_alt;

//
//  calculate altitude error
//

	alt_error = preset - average_alt;

//
//  calculate altitude deviation
//

	deviation = -alt_error;

	if(deviation > 127 )
		deviation = 127;

	if(deviation < -128)
        deviation = -128;

    deviation = deviation + 128;

    wing = WNGREC;

    tlx_servo();
 }
//////////////////////////////////////////////////////////////////////////////
//
//  update sensor reading
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_sensor(void)
 {
    dvalid = (double)fvalid;
    dalt = (double)falt;
    daccel = (double)faccel;

    tlx_sensor_data.altitude_valid = (int)((dvalid * 255.0)/5.0);
    tlx_sensor_data.altitude = (int)((dalt * 255.0)/100.0);
    tlx_sensor_data.accelerometer = (int)((daccel * 255.0)/1.0);

 }
//////////////////////////////////////////////////////////////////////////////
//
//  Output Sevro Commands
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_servo(void)
 {
	tlx_sensor_data.wing_command = wing;

    dwing = ((double)(wing) * 10.0)/255.0;
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void Abort_Processor(void)
 {
    tlx_init();
    tlx_pilot_recovery();
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void autopilot_dump_header(void)
 {
    printf("count\t");
    printf("avg_alt\t");
    printf("alt_err\t");
    printf("dev\t");
    printf("kp_alterr\t");
    printf("ht_rate\t");
    printf("kv_htrt\t");
    printf("alt_sum\t");
    printf("int_accel\t");
    printf("s_wing\n");
    printf("h_wing\n");
    printf("wing\n");
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void autopilot_dump(void)
 {
    printf("%2x\t",loop_count);
    printf("%4x\t",average_alt);
    printf("%4x\t",alt_error);
    printf("%4x\t",deviation);
    printf("%8x\t",kp_alt_error);
    printf("%4x\t",height_rate);
    printf("%8x\t",kv_height_rate);
    printf("%8x\t",alt_sum);
    printf("%4x\t",integrate_accel);
    printf("%4x\t",scale_wing);
    printf("%4x\t",hard_wing);
    printf("%4x\t",wing);
    printf("\n");
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void alt_stack_dump(void)
 {
 int i;

	for(i=0;i<NUM_ALT;i++) {
        printf("%d %d\n",i,alt[i]);
    }
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void accel_stack_dump(void)
 {
 int i;

	for(i=0;i<NUM_ACCEL;i++) {
        printf("%d %d\n",i,accel[i]);
    }
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void preset_dump(void)
 {
    printf("Preset %2x\n",preset);
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void gain_dump(void)
 {
    printf("KP %2x\n",KP);

    printf("KV %2x\n",KV);

    printf("KI %2x\n",KI);

    printf("KS %2x\n",KS);

    printf("MR %2x\n",MR);
 }
