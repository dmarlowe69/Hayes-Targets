/* file: design.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void design(void)
 {
 double l0;
 double droop;
 double x;
 double w;
 double v;
 double p2;
 double temp;
 int index;
 int index_1;

 double q;
 double d;
 double d1;
 double d2;
 double cdc;
 double cf;
 double seglength;
 double segweight;
 double segvolume;
 double seglift;
 double segdrag;
 double tension;

 double mn;
 double mach;
 double cmac;
 double picf;

 int flag;

 #define DESIGN_OK			0
 #define END_OF_DESIGN		1
 #define END_OF_SEGMENT 	2
 #define END_OF_VOLUME		3
 #define END_OF_STRENGTH	4
 #define END_OF_LEADER		5

/****************************************************************************
 *
 *	print header
 *
 ***************************************************************************/

	temp = loads.theta[0] * RAD_TO_DEG;

	printf("\n");
	printf("\n");
	printf("                   TARGET CABLE STEADY STATE DESIGN\n");
	printf("\n");
	printf("  DIA        LENGTH    TENSION        SAFETY FACTOR        ANGLE   SEG WT\n");
	printf("  (IN)        (FT)      (LBS)                              (DEG)   (LBS)");
	printf("\n");
	printf("\n");
	printf(" TARGET                 %4.0f                               %5.2f\n", loads.tension[0], temp);

/****************************************************************************
 *
 *	initialize variables
 *
 ***************************************************************************/

	l0 = 0.0;
	droop = 0.0;
	x = 0.0;
	w = 0.0;
	v = 0.0;
	p2 = 0.0;

	index = 0;
	index_1 = 1;

	q = design_data.q;
	seglength = design_data.seglength;

/****************************************************************************
 *
 *	flange and revs
 *
 ***************************************************************************/

	loads.target_wind_dia = 0.5;
	loads.target_revs = 0.0;
	loads.external_wind_dia = 0.625;
	loads.external_revs = 0.0;

/****************************************************************************
 *
 *	leader design
 *
 ***************************************************************************/

	if(stricmp(design_data.design,"TLX")==0) {
		design_data.d1 = 0.097;
		design_data.leader_length = 50.0;
		}

	if(stricmp(design_data.design,"MTR-101")==0) {
		design_data.d1 = 0.097;
		design_data.leader_length = 50.0;
		}

/****************************************************************************
 *
 *	leader design
 *
 ***************************************************************************/

	flag = 0;

	while ( 1 ) {

		if(design_data.d1 == 0.0) {
			lookupmono(design_data.d2,design_data.sf,loads.tension[0]);
			d = cable_look.diameter;
			design_data.type[index]=TYPE_MONO;
			}
		else {
			lookupstrand(design_data.d1,design_data.sf,loads.tension[0]);
			d = cable_look.diameter;
			design_data.type[index]=TYPE_STRAND;
			}

		if(cable_look.diameter > 900.0) {
			flag = END_OF_STRENGTH;
			return;
			}

		segweight = cable_look.weight * seglength / 1000.0;
		segvolume = d * d * seglength * 12.0;

		do {
			l0 = l0 + design_data.seglength;

			loads.p0 = atan2(loads.tz, loads.tx);
			if((loads.theta[index_1-1] - loads.p0) < .0005) {
				loads.p0 = loads.theta[index_1-1];
				}

			w = w + segweight;
			v = v + segvolume;
			droop = droop + seglength * sin(loads.p0);
			x = x + seglength * cos(loads.p0);

			if(l0 > design_data.l) {
				flag = END_OF_DESIGN;
				break;
				}

			if(v > design_data.s0) {
				flag = END_OF_VOLUME;
				break;
				}
/****************************************************************************
 *
 *	get cable coefficients
 *
 ***************************************************************************/

			if(stricmp(design_data.design,"TLX") == 0) {
				mn = design_data.airspeed_fps * sin(loads.p0) / 1116.89;
				cdc = 1.17 + .25 * mn - .25 * mn * mn + .625 * mn * mn * mn;
				mach = design_data.airspeed_fps	/ 1116.89;
				cmac = mach * cos(loads.p0);
				if(cmac <= .4) {
					picf = .038 - .0425 * cmac;
					}
				else {
					picf = .013 + .0395 * pow(cmac - .85,2.0);
					}
				cf = (picf / 3.1416);
				}
			else {
				cdc = cable_look.cdc;
				cf = cable_look.cf;
				}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

			seglift = d * seglength * q * cdc * pow(sin(loads.p0),2) * cos(loads.p0) / 12.0;
			segdrag = d * seglength * q * (cdc * pow(sin(loads.p0),3) + 3.141592654 * cf) / 12.0;
			loads.tz = loads.tz	+ segweight - seglift;
			loads.tx = loads.tx	+ segdrag;

			tension = pow(loads.tx,2.0) + pow(loads.tz,2.0);
			tension = sqrt(tension);

/****************************************************************************
 *
 *	end of segment loop
 *
 ***************************************************************************/

		if((index_1 == 1 ) & (loads.p0 < design_data.cut_off_angle ) & ( design_data.cut_off_angle > 0)) {
			flag = END_OF_SEGMENT;
			break;
			}
		if((index_1 == 1) & (l0 > design_data.leader_length) & (design_data.d1 != 0.0) & (design_data.leader_length != 0.0)) {
			flag = END_OF_SEGMENT;
			break;
			}
		} /* do */
		while(tension < (cable_look.strength / design_data.sf));

		if(flag != 0)
			break;

	} /* while */

/****************************************************************************
 *
 *	end of leader
 *
 ***************************************************************************/

	l0 = l0 - design_data.seglength;

	loads.tz = loads.tz	- segweight + seglift;
	loads.tx = loads.tx	- segdrag;
	w = w - segweight;
	loads.mass[index_1] = w;
	v = v - d * d * seglength * 12.0;
	droop = droop - seglength * sin(loads.p0);
	x = x - seglength * cos(loads.p0);

	design_data.diameter[index] = d;
	design_data.length_run[index] = l0;
	design_data.length[index] = l0;

	loads.mass[index_1] = w;
	temp = pow(loads.tx,2.0) + pow(loads.tz,2.0);
	loads.tension[index_1] = sqrt(temp);
	loads.theta[index_1] = loads.p0;
	loads.rts[index_1] = cable_look.strength;
	loads.sf[index_1] = cable_look.strength / loads.tension[index_1];

	loads.cable_weight = w;
	loads.cable_volume = v;
	loads.cable_droop = droop;
	loads.cable_trail = x;

	flange(index_1,v,d);

//	print_segment_design(index, index_1);
	print_segment_oneway_design(index, index_1);

//	printf("end of leader press any key\n");
//	getch();

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	if(flag == END_OF_DESIGN) {
		loads.rts[index_1] = cable_look.strength - 15;
		loads.sf[index_1] = (cable_look.strength -15) / loads.tension[index_1];

		printf("\n");
		printf("\tCABLE DIAMETER %5.3f RTS REDUCED 15 LBS FOR SWAGE",design_data.diameter[index]);

		design_data.index = index_1;

		printf("\n");
		printf("CABLE WEIGHT (LBS): %6.1f",loads.cable_weight);
		printf("\t");
		printf("VOLUME (CUBIC INCHES): %6.1f",loads.cable_volume);
		printf("\n");
		printf("CABLE DROOP  (FT) : %6.0f",loads.cable_droop);
		printf("\t");
		printf("DESIGN LENGTH (FT)   :%6.0f",design_data.length_run[index]);
		printf("\n");
		printf("CABLE TRAIL  (FT) :%6.0f",loads.cable_trail);

		return;
		}

/****************************************************************************
 *
 *	segment loop
 *
 ***************************************************************************/

	d1 = design_data.d1;
	d2 = design_data.d2;

	flag = 0;

	while ( 1 ) {

		index = index + 1;
		index_1 = index_1 + 1;

		if(design_data.d2 != 0.0) {
			lookupmono(d2,design_data.sf,tension);
			d = cable_look.diameter;
			design_data.type[index]=TYPE_MONO;
			d2 = d;
			}
		else {
			lookupstrand(d1,design_data.sf,tension);
			d = cable_look.diameter;
			design_data.type[index]=TYPE_STRAND;
			d1 = d;
			}

		if(cable_look.diameter > 900.0) {
			flag = END_OF_STRENGTH;
			return;
			}

		segweight = cable_look.weight * seglength / 1000.0;
		segvolume = d * d * seglength * 12.0;

		do {
			l0 = l0 + design_data.seglength;

			loads.p0 = atan2(loads.tz, loads.tx);
			if((loads.theta[index_1-1] - loads.p0) < .0005) {
				loads.p0 = loads.theta[index_1-1];
				}

			w = w + segweight;
			v = v + segvolume;
			droop = droop + seglength * sin(loads.p0);
			x = x + seglength * cos(loads.p0);

			if(l0 > design_data.l) {
				flag = END_OF_DESIGN;
				break;
				}

			if(v > design_data.s0) {
				flag = END_OF_VOLUME;
				break;
				}
/****************************************************************************
 *
 *	get cable coefficients
 *
 ***************************************************************************/

			if(stricmp(design_data.design,"TLX") == 0) {
				mn = design_data.airspeed_fps * sin(loads.p0) / 1116.89;
				cdc = 1.17 + .25 * mn - .25 * mn * mn + .625 * mn * mn * mn;
				mach = design_data.airspeed_fps	/ 1116.89;
				cmac = mach * cos(loads.p0);
				if(cmac <= .4) {
					picf = .038 - .0425 * cmac;
					}
				else {
					picf = .013 + .0395 * pow(cmac - .85,2.0);
					}
				cf = (picf / 3.1416);
				}
			else {
				cdc = cable_look.cdc;
				cf = cable_look.cf;
				}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

			seglift = d * seglength * q * cdc * pow(sin(loads.p0),2) * cos(loads.p0) / 12.0;
			segdrag = d * seglength * q * (cdc * pow(sin(loads.p0),3) + 3.141592654 * cf) / 12.0;
			loads.tz = loads.tz	+ segweight - seglift;
			loads.tx = loads.tx	+ segdrag;

			tension = pow(loads.tx,2.0) + pow(loads.tz,2.0);
			tension = sqrt(tension);

/****************************************************************************
 *
 *	end of segment loop
 *
 ***************************************************************************/

		} /* do */
		while(tension < (cable_look.strength / design_data.sf));

/****************************************************************************
 *
 *	end of segment
 *
 ***************************************************************************/

		l0 = l0 - design_data.seglength;

		if(l0 > 0)

		loads.tz = loads.tz	- segweight + seglift;
		loads.tx = loads.tx	- segdrag;
		w = w - segweight;
		loads.mass[index_1] = w;
		v = v - d * d * seglength * 12.0;
		droop = droop - seglength * sin(loads.p0);
		x = x - seglength * cos(loads.p0);

		design_data.diameter[index] = d;
		design_data.length_run[index] = l0;
		design_data.length[index] = l0 - design_data.length_run[index - 1];

		loads.mass[index_1] = w;
		temp = pow(loads.tx,2.0) + pow(loads.tz,2.0);
		loads.tension[index_1] = sqrt(temp);
		loads.theta[index_1] = loads.p0;
		loads.rts[index_1] = cable_look.strength;
		loads.sf[index_1] = cable_look.strength / loads.tension[index_1];

		loads.cable_weight = w;
		loads.cable_volume = v;
		loads.cable_droop = droop * 1.25;
		loads.cable_trail = x;

		flange(index_1,v,d);

//		print_segment_design(index, index_1);
		print_segment_oneway_design(index, index_1);

//		printf("end of segment press any key\n");
//		getch();

		if(flag!=0)
			break;

		} /* while */

/****************************************************************************
 *
 *	end of design trim 15 loads from cable rts for swage
 *
 ***************************************************************************/

	loads.rts[index_1] = cable_look.strength - 15;
	loads.sf[index_1] = (cable_look.strength -15) / loads.tension[index_1];

	printf("\n");
	printf("\tCABLE DIAMETER %5.3f RTS REDUCED 15 LBS FOR SWAGE",design_data.diameter[index]);

/****************************************************************************
 *
 *	end of design
 *
 ***************************************************************************/

	design_data.index = index_1;

	printf("\n");
	printf("CABLE WEIGHT (LBS): %6.1f",loads.cable_weight);
	printf("\t");
	printf("VOLUME (CUBIC INCHES): %6.1f",loads.cable_volume);
	printf("\n");
	printf("CABLE DROOP  (FT) : %6.0f",loads.cable_droop);
	printf("\t");
	printf("DESIGN LENGTH (FT)   :%6.0f",design_data.length_run[index]);
	printf("\n");
	printf("CABLE TRAIL  (FT) :%6.0f",loads.cable_trail);

 }
/****************************************************************************
 *
 *	SELECT DESIGN OPTIONS
 *
 ***************************************************************************/
 int design_opt(void)
 {
 char c;
 int option;

	printf("\n");
	printf("YOUR OPTIONS ARE:\n");
	printf(" (P) PRINT OUT (M) SELECT MONOFILAMENT (V) NEW AIRSPEED \n");
	printf(" (N) NEW SAFETY FACTOR (R) select stranded (S) exit (T) ABORT RUN\n");
	printf(" (W) WRITE DESIGN TO FILE\n");

	option = -1;

	while(1) {

 #ifdef _WINDOWS

		c = getchar();

 #else

		c = getch();

 #endif

		switch(toupper(c)) {


			case 'F':
					write_data();
					option = OPT_QUIT;
					break;

			case 'T':
					option = OPT_QUIT;
					break;

			case 'P':
					direct_output();
					option = OPT_QUIT;
					break;

			case 'M':
					start_mono();
					option = OPT_AGAIN;
					break;

			case 'R':
					start_strand();
					option = OPT_AGAIN;
					break;

			case 'S':
					option = OPT_QUIT;
					break;

			case 'L':
					new_length();
					option = OPT_AGAIN;
					break;

			case 'V':
					new_speed();
					option = OPT_AGAIN;
					break;

			case 'W':
					write_data();
					option = OPT_QUIT;
					break;

			case 'N':
					new_safety();
					option = OPT_AGAIN;
					break;

			default:
					putchar('\a');
					break;

			}
		if(option != -1)
			break;
		}

	return(option);
 }
/****************************************************************************
 *
 *	input initial stranded cable diameter
 *
 ***************************************************************************/
 void start_strand(void)
 {
 float a;

	while(1) {
		printf("\nSTRANDED LEADER CABLE DIAMETER (INCHES) (ZERO IF NONE) ");
		scanf("%f",&a);
		design_data.d1 = (double)a;
		if(design_data.d1 == 0.0)
			break;

		if((getstrand(design_data.d1) == 0))
			break;

		}

	if(stricmp(design_data.design,"MTR-101")==0) {
		design_data.cut_off_angle = 0.0;
		design_data.leader_length = design_data.l;
		return;
		}

	if(stricmp(design_data.design,"TLX")==0) {
		design_data.cut_off_angle = 0.0;
		design_data.leader_length = design_data.l;
		return;
		}

	if(design_data.d1 != 0.0) {
		printf("\nCUT OFF ANGLE (DEGREES) OR LEADER LENGTH (FEET): ");
			scanf("%f",&a);
			design_data.leader_length =(double)a;
			if((design_data.leader_length > 100.0) | (design_data.leader_length > 0.0))
				design_data.cut_off_angle = 0.0;
			else
				design_data.cut_off_angle = design_data.leader_length / RAD_TO_DEG;
		}
 }
/****************************************************************************
 *
 *	input initial mono cable diameter
 *
 ***************************************************************************/
 void start_mono(void)
 {
 float a;

	while(1) {
		printf("\nSTARTING MONOFILAMENT DIAMETER (INCHES) (ZERO IF NONE) ");
		scanf("%f",&a);
		design_data.d2 = (double)a;
		if(design_data.d2 == 0.0)
			break;

		if((getmono(design_data.d2) == 0))
			break;

		}
 }
/****************************************************************************
 *
 *	enter new design parameters
 *
 ***************************************************************************/
 void new_speed(void)
 {
 float a;

	printf("\nENTER NEW AIRSPEED (KIAS) :");
	scanf("%f",&a);
	design_data.airspeed = (double)a;
 }
/****************************************************************************
 *
 *	enter new design parameters
 *
 ***************************************************************************/
 void new_safety(void)
 {
 float a;

	printf("\nENTER NEW SAFETY FACTOR :");
	scanf("%f",&a);
	design_data.sf= (double)a;
 }
/****************************************************************************
 *
 *	enter new design parameters
 *
 ***************************************************************************/
 void new_length(void)
 {
 float a;

	printf("\nENTER NEW CABLE LENGHT (FEET) :");
	scanf("%f",&a);
	design_data.l = (double)a;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void print_segment_design(int index,int index_1)
 {
 double temp;

	printf(" %5.3f ",design_data.diameter[index]);
	if(design_data.type[index]==TYPE_STRAND) {
		printf("1X7");
		}
	else {
		printf("   ");
		}

	printf(" ");
	printf("%5.0f",design_data.length[index]);
//	printf("%5.0f %5.0f",design_data.length[index],design_data.length_run[index]);

	printf("        ");
	printf("%4.0f",loads.tension[index_1]);

	printf(" ");
	printf("%4.0f RTS",loads.rts[index_1]);

	printf(" ");
	printf("%5.2f",loads.sf[index_1]);

	temp =	(1.0 / loads.sf[index_1]) * 100.0;
	printf(" ");
	printf("%5.1f",temp);
	putchar('%');

	temp = loads.theta[index_1] * RAD_TO_DEG;
	printf("         ");
	printf("%5.2f",temp);

	temp = loads.mass[index_1] - loads.mass[index_1-1];
	printf(" ");
	printf("%6.2f\n",temp);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void print_segment_oneway_design(int index,int index_1)
 {
 double temp;

	printf(" %5.3f ",design_data.diameter[index]);
	if(design_data.type[index]==TYPE_STRAND) {
		printf("1X7");
		}
	else {
		printf("   ");
		}

	printf(" ");
	printf("%5.0f",design_data.length[index]);

	printf("        ");
	printf("%4.0f",loads.tension[index_1]);

	printf(" ");
	printf("%4.0f RTS",loads.rts[index_1]);

	printf(" ");
	printf("%5.2f",loads.sf[index_1]);

	temp =	(1.0 / loads.sf[index_1]) * 100.0;
	printf(" ");
	printf("%5.1f",temp);
	putchar('%');

	temp = loads.theta[index_1] * RAD_TO_DEG;
	printf("         ");
	printf("%5.2f",temp);

	temp = loads.mass[index_1] - loads.mass[index_1-1];
	printf(" ");
	printf("%6.2f",temp);

	temp = loads.flange[index_1];
	printf(" ");
	printf("%6.2f",temp);

	temp = loads.revs[index_1] - loads.revs[index_1-1];
	printf(" ");
	printf("%6.2f",temp);

	temp = loads.revs[index_1];
	printf(" ");
	printf("%6.2f\n",temp);
 }
