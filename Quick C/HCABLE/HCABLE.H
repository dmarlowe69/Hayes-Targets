/* file: hcable.h */

/****************************************************************************
 *
 *	include files
 *
 ***************************************************************************/

 #include	<stdio.h>
 #include	<stdlib.h>
 #include	<math.h>
 #include	<string.h>
 #include	<time.h>

 #ifdef _WINDOWS

 #include	<io.h>

 #else

 #include	<conio.h>

 #endif

/****************************************************************************
 *
 *	definitions
 *
 ***************************************************************************/

 #define RAD_TO_DEG (180.0/3.141592654)

 #define MAX_INDEX		100
 #define MAX_TYPES		5

 #define TYPE_MONO		0
 #define TYPE_STRAND	1
 #define TYPE_SPECIAL	2
 #define TYPE_OUTPUT	3
 #define TYPE_INPUT 	4

/****************************************************************************
 *
 *	data structures
 *
 ***************************************************************************/

 struct CABLE_DATA {
	double diameter[MAX_INDEX];
	double length[MAX_INDEX];
	double strength[MAX_INDEX];
	double weight[MAX_INDEX];
	double cf[MAX_INDEX];
	double cdc[MAX_INDEX];
	double elongation[MAX_INDEX];
	int type[MAX_INDEX];
	int number;
	};

 extern struct CABLE_DATA cable_data[MAX_TYPES];

 struct CABLE_LOOK {
	double diameter;
	double length;
	double strength;
	double weight;
	double cf;
	double cdc;
	double elongation;
	int type;
	int number;
	};

 extern struct CABLE_LOOK cable_look;

 struct DESIGN_DATA {
	double airspeed;
	double airspeed_fps;
	double q;
	double ref_dia;
	double ref_area;
	double mass;
	double cd0;
	double g;
	double s0;
	double l;
	double j1;
	double seglength;
	double d1;
	double d2;

	double sf;
	double leader_length;
	double cut_off_angle;

	int index;

	double cl;
	double cl_delta;
	double delta;
	double lift;
	double cd0_wing;
	double cdi_wing;
	double cdi;
	double cd_total;
	double drag;

	double total_length;

	char project[81];
	char design[81];
	char airframe[81];
	char opt[81];
	double diameter[100];
	double length[100];
	double length_run[100];
	int type[100];
	};

 extern struct DESIGN_DATA design_data;

 struct LOADS {
	double tx;
	double ty;
	double tz;
	double p0;
	double p1;

	double cable_weight;
	double cable_volume;
	double cable_droop;
	double cable_trail;
	double cable_tgt_weight;
	double cable_tgt_volume;
	double cable_tgt_droop;
	double cable_tgt_trail;

	double target_revs;
	double target_wind_dia;
	double external_revs;
	double external_wind_dia;

	double tension[MAX_INDEX];
	double theta[MAX_INDEX];
	double mass[MAX_INDEX];
	double rts[MAX_INDEX];
	double sf[MAX_INDEX];

	double flange[MAX_INDEX];
	double revs[MAX_INDEX];
	};

 extern struct LOADS loads;

/****************************************************************************
 *
 *	cabdata.in prototypes
 *
 ***************************************************************************/

 void read_mono(void);
 void dump_mono(void);
 void read_strand(void);
 void dump_strand(void);
 void read_data(void);
 void dump_data(void);
 void load_design_data(void);
 void write_data(void);

/****************************************************************************
 *
 *	lookup.in prototypes
 *
 ***************************************************************************/

 int getmono(double diameter);
 int getstrand(double diameter);
 int getcable(double diameter, int type);

 void lookupmono(double diameter, double sf, double tension);
 void lookupstrand(double diameter, double sf, double tension);
 void lookupcable(double diameter, double sf, double tension, int type);

 void load_cable_data(int type,int index);

 void dump_look(void);

/****************************************************************************
 *
 *	tlxloads.in prototypes
 *
 ***************************************************************************/

 void targetloads(void);
 void tlx_targetloads(void);

 void dump_targetloads(void);

 void trx9_loads(void);

/****************************************************************************
 *
 *	analysis.in prototypes
 *
 ***************************************************************************/

 void analysis(void);

/****************************************************************************
 *
 *	print.in prototypes
 *
 ***************************************************************************/

 void print_cable(FILE *stream);
 void direct_output(void);

/****************************************************************************
 *
 *	desinput.in prototypes
 *
 ***************************************************************************/

 void input_design(void);
 void in_target(void);
 void in_cable(void);

 int scope_it(void);
 void testdata(int test_id);

/****************************************************************************
 *
 *	desopt.in prototypes
 *
 ***************************************************************************/

 #define OPT_QUIT	0
 #define OPT_AGAIN	1

 int design_opt(void);

 void start_strand(void);
 void start_mono(void);
 void new_speed(void);
 void new_safety(void);
 void new_length(void);

/****************************************************************************
 *
 *	design.in prototypes
 *
 ***************************************************************************/

 void design(void);

 void print_segment_design(int index,int index_1);

 void print_segment_oneway_design(int index,int index_1);

/****************************************************************************
 *
 *	design6.in prototypes
 *
 ***************************************************************************/

 void design6(void);

/****************************************************************************
 *
 *	print6.in prototypes
 *
 ***************************************************************************/

 void print_cable6(FILE *stream);

/****************************************************************************
 *
 *	revs.in prototypes
 *
 ***************************************************************************/

 void flange(int index, double volume, double diameter);
 void flange6(int index, double volume, double diameter);
 void revs(int index, double diameter);
 void ltc_6_revs(int index, double diameter);

/****************************************************************************
 *
 *	design.in prototypes
 *
 ***************************************************************************/

 void designst(void);

/****************************************************************************
 *
 *	main function prototypes
 *
 ***************************************************************************/

 void input_data(void);
 void design_driver(void);
 void analysis_driver(void);
 void debug_main(void);
