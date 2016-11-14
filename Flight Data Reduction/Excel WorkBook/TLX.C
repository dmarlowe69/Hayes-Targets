/****************************************************************************
 *
 *	general defintions
 *
 ***************************************************************************/

 #define DEBUG
 #define OUTPUT

 #define NUM_ALT	16
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
 float	faccel;

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
 void tlx_test(void);

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

	preset = (int)(fpreset * 256.0/100.0);

	KP = 0x66;	/* 102 - 2.00	volts */
	KV = 0x4c;	/*	76 - 1.50	volts */
	KI = 0x33;	/*	52 - 1.00	volts */
	KS = 0x50;	/*	80 - 1.568	volts */
	MR = 0x66;	/* 102 - 2.00	volts */

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
 *	update sensor reading
 *
 **************************************************************************/
 void tlx_sensor(void)
 {
	alt[0] = (int)( falt * 256.0/100.0);
	accel[0] = (int)(faccel * 256.0/1.0);
 }
