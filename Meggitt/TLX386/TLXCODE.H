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

 extern struct TLX tlx;

 extern struct TLX_TM tlx_tm;

/****************************************************************************
 *
 *      Prototypes
 *
 ***************************************************************************/

 void tlx_init(void);
 void tlx_pilot(void);
 void tlx_test(void);


 void recovery_mode(void);
 void set_preset(void);
 void fly_mode(void);
 void std_prog(void);
 void gain_prog(void);
 void alt_prog(void);
 void manual_prog(void);
 void fire_control(void);
