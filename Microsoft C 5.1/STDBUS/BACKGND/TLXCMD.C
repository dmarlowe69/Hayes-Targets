/* file: tlx.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("TLX function")
 #pragma subtitle("(c)1989-1992 Hayes Targets")

/****************************************************************************
 *
 *  BIRD BRAIN SERIAL COMMUNICATIONS
 *
 *  written by Denton Marlowe
 *	(c)1989-1992 HAYES TARGETS
 *
 ***************************************************************************/

/****************************************************************************
 *
 *	This Software was Designed using the following development tools:
 *
 *	MS-DOS Verison 5.0
 *	Microsoft C Version 6.00a
 *	MASM Version 5.1
 *	Datalight C_Thru_ROM Version 1.70c
 *
 *		This Software is dedicated to the memory of
 *
 *			NYSSA ANN MARLOWE
 *		    17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdio.h>
 #include	<stdlib.h>
 #include	<conio.h>
 #include	<string.h>
 #include	<graph.h>

/****************************************************************************
 *
 *	Include Hayes definitions
 *
 ***************************************************************************/

 #include	<hayes.h>

 #include	"cmd.h"
 #include	"key.h"

 #include	"tlx_disp.h"
 #include	"hud_scan.h"
 #include	"hud_plot.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 #pragma page(1)

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
 *      Prototypes
 *
 ***************************************************************************/

 void tlx_init(void);
 void recovery_mode(void);
 void set_preset(void);
 void fly_mode(void);
 void std_prog(void);
 void gain_prog(void);
 void alt_prog(void);
 void manual_prog(void);
 void fire_control(void);

 char tlx_key(void);
 void tlx_key_action(char c);

 void manual_wing_position(unsigned char position);
 void manual_wing_up(unsigned char inc);
 void manual_wing_down(unsigned char inc);
 int manual_fly_action(char c);

/***************************************************************************
 *
 *	definitions
 *
 ***************************************************************************/

 #define TLX_INIT_FLAG	0xaa55

 #define RCVY_FLY_BITS	0x01
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
 #define OFF			0

 #pragma page(1)

/****************************************************************************
 *
 *	TLX Initialization
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void tlx_init(void)
 {
	tlx.init_flag = TLX_INIT_FLAG;

	tlx.eng_panel_flag = ON;

	tm_invert_flag = INVERT;				/* test configuration */
	cmd_invert_flag = INVERT;				/* test configuration */

	tm_buffer.data[0]=0x00; 				/* preset 1's */
    tm_buffer.data[1]=0x00;                 /* preset 10's */
    tm_buffer.data[2]=0x00;                 /* preset 100's */
    tm_buffer.data[3]=KP_DEFAULT;           /* KP */
    tm_buffer.data[4]=KV_DEFAULT;           /* KV */
    tm_buffer.data[5]=KI_DEFAULT;           /* KS */
    tm_buffer.data[6]=KS_DEFAULT;           /* scale */
    tm_buffer.data[7]=MR_DEFAULT;           /* MR */
    tm_buffer.data[8]=RCVY_MODE | PROG_STD; /* command */

	tlx.prog_flag = PROG_STD;
	tlx.fire_flag = 0;
	tlx.fire_1 = 0;
	tlx.fire_2 = 0;
	tlx.fire_3 = 0;
	tlx.fire_4 = 0;

	tm_buffer.data[8]=(tm_buffer.data[8] & ~RCVY_FLY_BITS) | RCVY_MODE;
	send_tm_packet(DATA_SYNC,0);	/* load telemetry buffer */
	tlx.recovery_fly_flag = RCVY_MODE;

	tlx.preset_flag = 0;					/* clear preset flag */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	RECOVERY MODE
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void recovery_mode(void)
 {
	tm_buffer.data[8]=(tm_buffer.data[8] & ~RCVY_FLY_BITS) | RCVY_MODE;
 }

 #pragma page(1)

/****************************************************************************
 *
 *	SET PRESET
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void set_preset(void)
 {
	tm_buffer.data[0] = (tlx.preset_1*28) ^ 0xff;
	tm_buffer.data[1] = tlx.preset_10*28 ^ 0xff;
	tm_buffer.data[2] = tlx.preset_100*28 ^ 0xff;

	tlx.preset_flag = 1;
 }

 #pragma page(1)

/****************************************************************************
 *
 *	FLY MODE
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void fly_mode(void)
 {
	if(tlx.preset_flag ==0) {
		putchar('\a');
		return;
	}
	tm_buffer.data[8]=(tm_buffer.data[8] & ~RCVY_FLY_BITS) | FLY_MODE;

	tlx.recovery_fly_flag = FLY_MODE;
 }

 #pragma page(1)

/****************************************************************************
 *
 *	STANDARD PROGRAM MODE
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void std_prog(void)
 {
	if(tlx.recovery_fly_flag == FLY_MODE)
		return;

	tm_buffer.data[8]=(tm_buffer.data[8] & ~PROG_BITS) | PROG_STD;

	tlx.prog_flag = PROG_STD;

 }

 #pragma page(1)

/****************************************************************************
 *
 *	GAINABLE PROGRAM MODE
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void gain_prog(void)
 {
 int data;
 char buffer[81];

	if(tlx.recovery_fly_flag == FLY_MODE)
		return;

	_outtext("\nGAINABLE PROGRAM MODE\n");

	tm_buffer.data[8]=(tm_buffer.data[8] & ~PROG_BITS) | PROG_GAIN;
									/* set PROG 1 and 2 mode */
	tm_buffer.data[3]=KP_DEFAULT;			/* KP */
	tm_buffer.data[4]=KV_DEFAULT;			/* KV */
	tm_buffer.data[5]=KI_DEFAULT;			/* KI */
	tm_buffer.data[6]=KS_DEFAULT;			/* KS */
	tm_buffer.data[7]=MR_DEFAULT;			/* MR */

	data = tm_buffer.data[3];		/* set default gain */
	_outtext("\nenter gain KP ");
	gscanf("%x",&data);
	tm_buffer.data[3]=data;			/* KP */

	data = tm_buffer.data[4];		/* set default gain */
	_outtext("\nenter gain KV ");
	gscanf("%x",&data);
	tm_buffer.data[4]=data;			/* KV */

	data = tm_buffer.data[5];		/* set default gain */
	_outtext("\nenter gain KI ");
	gscanf("%x",&data);
	tm_buffer.data[5]=data;			/* KI */

	data = tm_buffer.data[6];		/* set default gain */
	_outtext("\nenter gain KS ");
	gscanf("%x",&data);
	tm_buffer.data[6]=data;			/* KS */

	data = tm_buffer.data[7];		/* set default gain */
	_outtext("\nenter gain MR ");
	gscanf("%x",&data);
	tm_buffer.data[7]=data;			/* MR */

	_outtext("\nGAINABLE PROGRAM MODE\n");

	sprintf(buffer,"gain KP %x\n",tm_buffer.data[3]);
	_outtext(buffer);

	sprintf(buffer,"gain KV %x\n",tm_buffer.data[4]);
	_outtext(buffer);

	sprintf(buffer,"gain KI %x\n",tm_buffer.data[5]);
	_outtext(buffer);

	sprintf(buffer,"gain KS %x\n",tm_buffer.data[6]);
	_outtext(buffer);

	sprintf(buffer,"gain MR %x\n",tm_buffer.data[7]);
	_outtext(buffer);

	tlx.prog_flag = PROG_GAIN;
 }

 #pragma page(1)

/****************************************************************************
 *
 *	ALTERNATE PROGRAM MODE
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void alt_prog(void)
 {
	if(tlx.recovery_fly_flag == FLY_MODE)
		return;

	tm_buffer.data[8]=(tm_buffer.data[8] & ~PROG_BITS) | PROG_ALT;

	tlx.prog_flag = PROG_ALT;

 }

 #pragma page(1)

/****************************************************************************
 *
 *	MANUAL CONTROL MODE
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void manual_prog(void)
 {
	if(tlx.recovery_fly_flag == FLY_MODE)
		return;

	tm_buffer.data[8]=(tm_buffer.data[8] & ~PROG_BITS) | PROG_MANUAL;

	tm_buffer.data[7]=MR_REC;		/* MR */

	tlx.prog_flag = PROG_MANUAL;

 }

 #pragma page(1)

/****************************************************************************
 *
 *	FIRE CONTROL
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void fire_control(void)
 {
	int c;
	int fire;
	char on[] = "ON\n";
	char off[] = "OFF\n";
	char buffer[81];

	while(TRUE)
	{
		_outtext("\n\nFIRE CONTROL MENU\n");

		buffer[0] = '\0';
		strcat(buffer,"\n1. FIRE 1 ");
		if(tlx.fire_1 == FIRE_1)
			strcat(buffer,off);
		else
			strcat(buffer,on);

		_outtext(buffer);

		buffer[0] = '\0';
		strcat(buffer,"\n2. FIRE 2 ");
		if(tlx.fire_2 == FIRE_2)
			strcat(buffer,off);
		else
			strcat(buffer,on);

		_outtext(buffer);

		buffer[0] = '\0';
		strcat(buffer,"\n3. FIRE 3 ");
		if(tlx.fire_3 == FIRE_3)
			strcat(buffer,off);
		else
			strcat(buffer,on);

		_outtext(buffer);

		buffer[0] = '\0';
		strcat(buffer,"\n4. FIRE 4 ");
		if(tlx.fire_4 == FIRE_4)
			strcat(buffer,off);
		else
			strcat(buffer,on);

		_outtext(buffer);


		_outtext("\n9. EXIT\n");
		_outtext("\n   Make Selection ");

		c = getch();

		switch(c) {
			case '1':	if(tlx.fire_1 == FIRE_1)
							tlx.fire_1 = 0x00;
						else
							tlx.fire_1 = FIRE_1;

						break;

			case '2':	if(tlx.fire_2 == FIRE_2)
							tlx.fire_2 = 0x00;
						else
							tlx.fire_2 = FIRE_2;

						break;

			case '3':	if(tlx.fire_3 == FIRE_3)
							tlx.fire_3 = 0x00;
						else
							tlx.fire_3 = FIRE_3;

						break;

			case '4':	if(tlx.fire_4 == FIRE_4)
							tlx.fire_4 = 0x00;
						else
							tlx.fire_4 = FIRE_4;

						break;

			case '9':
						return;

			default:
						break;
			} /* switch */

		fire = tlx.fire_1 | tlx.fire_2 | tlx.fire_3 | tlx.fire_4;

		tm_buffer.data[8]=(tm_buffer.data[8] & ~FIRE_BITS) | fire;

		send_tm_packet(DATA_SYNC,0);	/* load telemetry buffer */

		tlx.fire_flag = tlx.fire_1 | tlx.fire_2 | tlx.fire_3 | tlx.fire_4;

		} /* while */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	process keyboard inputs
 *
 ***************************************************************************/
 char tlx_key(void)
 {
 char key;
 char ex_key;

	key = 0;
	if(kbhit()) {
		key = getch();
		if(key == 0x00) {
			ex_key = getch();
			tlx_key_action(ex_key);
			return(ex_key);
		}

	} /* if */

	return(key);
 }

 #pragma page(1)

/****************************************************************************
 *
 *	process keyboard inputs
 *
 ***************************************************************************/
 void tlx_key_action(char c)
 {
	/*sprintf(buffer,"\n%d action key\n",c);*/
	/*_outtext(buffer);*/
 }

 #pragma page(1)

/****************************************************************************
 *
 *	manual wing position
 *
 ***************************************************************************/
 void manual_wing_position(unsigned char position)
 {
	if(tlx.prog_flag != PROG_MANUAL) {
		return;
    }
    else {
		tm_buffer.data[7] = (unsigned char)position;
		que_command();
    }
 }

 #pragma page(1)

/****************************************************************************
 *
 *	manual wing increment
 *
 ***************************************************************************/
 void manual_wing_up(unsigned char inc)
 {
	if(tlx.prog_flag != PROG_MANUAL) {
		return;
    }
    else {
        if(tm_buffer.data[7] < (MR_MAX-inc) ) {
			tm_buffer.data[7] = (unsigned char)(tm_buffer.data[7] + inc);
			que_command();
		}
        else {
			tm_buffer.data[7] = (unsigned char)MR_MAX;
			que_command();
		}
	}
 }

 #pragma page(1)

/****************************************************************************
 *
 *	manual wing decrement
 *
 ***************************************************************************/
 void manual_wing_down(unsigned char inc)
 {
	if(tlx.prog_flag != PROG_MANUAL) {
		return;
    }
    else {
        if(tm_buffer.data[7] > (MR_MIN+inc) ) {
			tm_buffer.data[7] = (unsigned char)(tm_buffer.data[7] - inc);
			que_command();
		}
        else {
			tm_buffer.data[7] = (unsigned char)MR_MIN;
			que_command();
		}
    }
 }

 #pragma page(1)

/****************************************************************************
 *
 *	manual control
 *
 ***************************************************************************/
 int manual_fly_action(char c)
 {
 int action;

	if(tlx.prog_flag != PROG_MANUAL)
		return(0);

	action = 0;

	switch(c) {

		case KEY_UP_ARROW:
				manual_wing_up(1);
				action = 1;
				break;

		case KEY_PGUP:
				manual_wing_up(10);
				action = 1;
				break;

		case KEY_DOWN_ARROW:
				manual_wing_down(1);
				action = 1;
				break;

		case KEY_PGDN:
				manual_wing_down(10);
				action = 1;
				break;

		case KEY_HOME:
				manual_wing_position(MR_REC);
				action = 1;
				break;

		case KEY_END:
				manual_wing_position(MR_MIN);
				action = 1;
				break;

		default:
				break;
		}

	return(action);
 }
