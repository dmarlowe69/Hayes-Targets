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
 int alt_error;
 int deviation;
 int preset;
 int height_rate;
 long int integrate_accel;
 int wing;
 long int sum;
 int KP,KV,KI,KS,MR;
 int loop_count;

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

//
//  Command Byte Bit Defintions
//

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

 #define FIRE_BITS		0x78
 #define FIRE_1 		0x08
 #define FIRE_2			0x10
 #define FIRE_3 		0x20
 #define FIRE_4 		0x40

 #define ON 			1
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
 void tlx_init(void);
 void autopilot(void);
 void tlx_pilot(void);
 void tlx_sensor(void);
 void alt_stack_out(void);
 void accel_stack_out(void);

 void far decode_cmd_data(void);

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

    printf("AUTOPILOT Init\r\n");
    dspio_printf("AUTOPILOT Init\r\n");

    fvalid = 5.0;
    falt = 50.0;
    faccel = 0.0;

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
    printf("AUTOPILOT\r\n");
    dspio_printf("AUTOPILOT\r\n");

    tlx_pilot();
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

    printf("tlx_init()\r\n");
    dspio_printf("tlx_init()\r\n");

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
            KS = KS_DEFAULT;  /* 0x50 -  80 - 1.568  volts */
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
            KS = KS_DEFAULT;  /* 0x50 80 - 1.568  volts */
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
            KS = KS_DEFAULT;
            MR = MR_DEFAULT;
            break;
            }
        }

    printf("KP %x\n",KP);
    dspio_printf("KP %x\n",KP);
    printf("KV %x\n",KV);
    dspio_printf("KV %x\n",KV);
    printf("KI %x\n",KI);
    dspio_printf("MR %x\n",MR);
    printf("KS %x\n",KS);
    dspio_printf("KS %x\n",KS);
    printf("MR %x\n",MR);
    dspio_printf("KI %x\n",KI);

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
    if(tlx_sensor_data.altitude_valid > 127) {
            altitude_valid_flag = LOCK;
            }
         else {
            altitude_valid_flag = UNLOCK;
            }

    alt[0] = tlx_sensor_data.altitude;
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
//    	put average into alt stack
//

    	alt[0] = average_alt;

//
//  calculate altitude error
//

	alt_error = preset - average_alt;

	sum = (long)(KP * alt_error);

//
//  calculate altitude deviation
//

	deviation = -alt_error;

	if(deviation > 127 )
		deviation = 127;

	if(deviation < -128)
		deviation = 128;

	deviation = deviation + 128;

//
//  determine height rate
//

	height_rate = alt[NUM_ALT-1] - alt[0];

	height_rate = height_rate;

	sum = sum + (long)(KV * height_rate);

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

		integrate_accel = integrate_accel >> 8;

		sum = sum + (long)(KI) * integrate_accel;

//
//  scale result
//

        wing = (int)(sum >> KS);

//
//  add in hardware offset
//

		wing = wing + 0;

//
//  subtrack manual reset
//

		wing = wing - MR;

//
//  limit wing
//

		if(wing < -255)
			wing = 255;
		if(wing > 255)
			wing = 255;

        tlx_servo();

	} /* if loop_count */

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

    tlx_sensor_data.altitude_valid = (int)( dvalid * 255.0/5.0);
    tlx_sensor_data.altitude = (int)( dalt * 255.0/100.0);
    tlx_sensor_data.accelerometer = (int)(daccel * 255.0/1.0);

 }
//////////////////////////////////////////////////////////////////////////////
//
//  Output Sevro Commands
//
//////////////////////////////////////////////////////////////////////////////
 void tlx_Servo(void)
 {
        dwing = (double)(wing) * 10.0/256.0;

        tlx_sensor_data.wing_command = wing;
 }
//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////
 void alt_stack_out(void)
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
 void accel_stack_out(void)
 {
 int i;

	for(i=0;i<NUM_ACCEL;i++) {
        printf("%d %d\n",i,accel[i]);
    }

 }
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
    tlx_cmd_data.preset = ((cmd.data[1]^0xff)/27)*1 \
                      + ((cmd.data[2]^0xff)/27)*10 \
                      + ((cmd.data[3]^0xff)/27)*100;
    //preset = (int)(fpreset * 256.0/100.0);

    //
    //  ch4 - proportional gain
    //
    tlx_cmd_data.proportional_gain = cmd.data[4];

    //
    //  ch5 - rate gain
    //
    tlx_cmd_data.rate_gain = cmd.data[5];

    //
    //  ch6 - integral gain
    //
    tlx_cmd_data.integral_gain = cmd.data[6];

    //
    //  ch7 - scale factor
    //
    tlx_cmd_data.delta_scale = ( (cmd.data[7] & 0xf0) >> 4 ) + 1;

    //
    //  ch8 - manual reset
    //
    tlx_cmd_data.delta_ec_manual = cmd.data[8];

    //
    //  ch9 - command channel
    //
    tlx_cmd_data.command = cmd.data[9];

    //
    //  RCVY/FLY
    //
    tlx_cmd_data.rcvy_fly = cmd.data[9] & RCVY_FLY_BITS;

    //
    //  PROG
    //
    tlx_cmd_data.prog_mode = cmd.data[9] & PROG_BITS;

    //
    //  FIRE
    //
    tlx_cmd_data.fire_mode = cmd.data[9] & FIRE_BITS;

 }
