/* file: hcable.c */

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

 struct CABLE_DATA cable_data[MAX_TYPES];

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

 struct CABLE_LOOK cable_look;

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

 struct DESIGN_DATA design_data;

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

 struct LOADS loads;

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

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void main(void)
 {
 char c;
 int i;

 #ifdef _WINDOWS

 int wm;				/* Menu click result */
 int sf, gf;			/* Set/Get focus results */
 FILE *wins;			/* file pointer */
 int nRes;				/* Result */
 FILE *wp;				/* File pointer */
 struct _wsizeinfo ws;	/* Size information */

 #endif

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #ifdef _WINDOWS

	_wabout("Shadow_Ware");

	gf = _wgetfocus();

	ws._version = _QWINVER;
	ws._type = _WINSIZEMAX;
	nRes = _wsetsize( gf , &ws );
   if( nRes == -1 ) {
	  printf( "***ERROR: _wsetsize\n" );
	  exit( -1 );
   }

	_wmenuclick(_WINTILE);

 #endif

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	for(i=0;i<MAX_INDEX;i++) {
		loads.tension[i] = 0.0;
		loads.theta[i] = 0.0;
		loads.mass[i] = 0.0;
		loads.rts[i] = 0.0;
		loads.sf[i] = 0.0;

		loads.flange[i] = 0.0;
		loads.revs[i] = 0.0;
		}

/****************************************************************************
 *
 *	 READ CABLE PARAMETERIS INTO ARRAYS
 *
 ***************************************************************************/

	read_mono();
	read_strand();

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	while(1) {

		fflush( stdin );
		printf("\n");
		printf("HAYES CABLE DESIGN PROGRAM FOR TLX  (C)HAYES TARGETS DIVISION\n");
		printf("WRITTEN BY DENTON MARLOWE NOVEMBER 02 1987\n");
		printf("\n");
		printf(" OPTIONS\n");
		printf("\n");
		printf(" (1) ONE WAY TARGET CABLE DESIGN OR ANALYSIS\n");
		printf("\n");
		printf(" (2) TWO WAY TARGET CABLE DESIGN OR ANALYSIS\n");
		printf("\n");
		printf(" (3) TLX TARGET CABLE DESIGN OR ANALYSIS\n");
		printf("\n");
		printf(" (4) QUIT\n");
		printf("\n");

 #ifdef _WINDOWS

		c = getchar();

 #else

		c = getch();

 #endif

		switch(c) {

			case '1':
					input_data();
					break;

			case '2':
					input_data();
					break;

			case '3':
					input_data();
					break;

			case '4':

 #ifdef _WINDOWS

			_wsetexit(_WINEXITPROMPT);

 #endif

					exit(0);
					break;
		}

	}

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void input_data(void)
 {
 int option;
 int test_id;
 char buffer[81];
 char *user;

	while(1) {
		printf("\nOPTIONS:\n");
		printf(" DESIGN - DESIGN A TOW CABLE\n");
		printf(" DATA - ANALYSIS CABLE DESIGN DATA FROM DATA STATEMENTS\n");
		printf(" FILE - ANALYSIS CABLE DESIGN DATA FROM FILE\n");
		printf(" AGAIN - REPEAT DESIGN ANALYSIS WITH SAME INPUT DATA\n");
		printf(" QUIT - EXIT PROGRAM\n");
		printf(" OPTION: ");
		scanf("%s",buffer);
		user = strtok(buffer,"\n");

		if(_stricmp(user,"DESIGN") == 0) {
			design_driver();
			return;
			}

		if(_stricmp(user,"DATA") == 0) {
			read_data();
			return;
			}

		if(_stricmp(user,"TEST") == 0) {
			printf("enter test case :");
			scanf("%d",&test_id);
			testdata(test_id);
			do {
				design_data.ref_dia = 8.91;
				design_data.ref_dia = 9.00;

				targetloads();

				if(stricmp(design_data.design,"LTC-6") == 0) {
					design6();
					}
				else {

					if(stricmp(design_data.design,"MTR-101") == 0) {
						designst();
						}
					else {
						design();
						}
					}

				option = design_opt();

				}while(option != OPT_QUIT);
				return;
			}

		if(_stricmp(user,"FILE") == 0) {
			analysis_driver();
			return;
			}

		if(_stricmp(user,"AGAIN") == 0) {
			return;
			}

		if(_stricmp(user,"QUIT") == 0) {
			return;
			}

		}

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void design_driver(void)
 {
 int option;

	input_design();

	do {
		design_data.ref_dia = 8.91;
		design_data.ref_dia = 9.00;

		targetloads();

		if(stricmp(design_data.design,"LTC-6") == 0) {
			design6();
			}
		else if(stricmp(design_data.design,"MTR-101") == 0) {
			designst();
			}
		else {
			design();
			}


		option = design_opt();

		}while(option != OPT_QUIT);

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void analysis_driver(void)
 {
// float a;
 char string[81];

	read_data();

	if(scope_it()!=0)
		exit(1);

	printf("ENTER AIRSPEED (KIAS) :");
	fflush( stdin );
	gets( string );
	if(strlen(string) == 0) {
		design_data.airspeed = 300.0;
		printf("airspeed %f\n",design_data.airspeed);
	}
	else {
		design_data.airspeed = atof(string);
	}

//	scanf("%f",&a);

//	design_data.airspeed = (double)a;
	design_data.ref_dia = 8.91;
	design_data.ref_dia = 9.00;

	targetloads();

	analysis();

	direct_output();
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void debug_main(void)
 {
	read_mono();
//	dump_mono();

	read_strand();
//	dump_strand();

	read_data();
//	dump_data();
	write_data();
//	getch();

//	printf("getmono 0.066\n");
//	getmono(0.066);
//	dump_look();
//	getch();

//	printf("getstrand 0.097\n");
//	getstrand(0.097);
//	dump_look();
//	getch();

//	printf("lookupmono 0.028 sf 1.7 t 200\n");
//	lookupmono(0.028, 1.7, 200.0);
//	dump_look();
//	getch();

//	printf("lookupmono 0.035 sf 1.7 t 200\n");
//	lookupstrand(0.035, 1.7, 200.0);
//	dump_look();
//	getch();

	design_data.airspeed = 300.0;
	design_data.ref_dia = 8.91;
	design_data.ref_dia = 9.00;

	targetloads();
	dump_targetloads();

	if(scope_it()!=0)
		exit(1);

//	dump_data();

	analysis();

//	print_cable(stdout);
//	print_cable(stdprn);
	direct_output();


	input_data();
	dump_data();
 }
