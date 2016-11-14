/* file: design6.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void design6(void)
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
 int jindex;

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
 *	do target design
 *
 ***************************************************************************/

	design();

/****************************************************************************
 *
 *	print header
 *
 ***************************************************************************/

	printf("\n");
	printf("\n");
	printf("                   LTC-6 CABLE STEADY STATE DESIGN\n");
	printf("\n");
	printf("  DIA        LENGTH    TENSION        SAFETY FACTOR        ANGLE   SEG WT\n");
	printf("  (IN)        (FT)      (LBS)                              (DEG)   (LBS)");
	printf("\n");
	printf("\n");

/****************************************************************************
 *
 *	initialize variables
 *
 ***************************************************************************/

	l0 = design_data.length_run[design_data.index-1];
	droop = loads.cable_droop;
	x = loads.cable_trail;
	w = loads.cable_weight;
	v = loads.cable_volume;

	loads.cable_tgt_weight = loads.cable_weight;
	loads.cable_tgt_volume = loads.cable_volume;
	loads.cable_tgt_droop  = loads.cable_droop;
	loads.cable_tgt_trail  = loads.cable_trail;

	index = design_data.index;
	index_1 = design_data.index;

	design_data.j1 = design_data.index;
	jindex = (int)(design_data.j1) - 1;

	q = design_data.q;
	seglength = design_data.seglength;

/****************************************************************************
 *
 *	segment loop
 *
 ***************************************************************************/

	d1 = design_data.d1;
	d2 = design_data.d2;

	tension = pow(loads.tx,2.0) + pow(loads.tz,2.0);
	tension = sqrt(tension);

	flag = 0;

	while ( 1 ) {

		index = index + 1;
		index_1 = index_1 + 1;

		if(design_data.d2 != 0.0) {
			lookupmono(d2,design_data.sf,tension);
			d = cable_look.diameter;
			design_data.type[index-1]=TYPE_MONO;
			d2 = d;
			}
		else {
			lookupstrand(d1,design_data.sf,tension);
			d = cable_look.diameter;
			design_data.type[index-1]=TYPE_STRAND;
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

			if(v > (design_data.s0 + 513.0 - 6.28)) {
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

		design_data.diameter[index-1] = d;
		design_data.length_run[index-1] = l0;
		design_data.length[index-1] = l0 - design_data.length_run[index - 2];

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

		flange6(index_1,v,d);

//		print_segment_design((index-1), index_1);
		print_segment_oneway_design((index-1), index_1);

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
	printf("\tCABLE DIAMETER %5.3f RTS REDUCED 15 LBS FOR SWAGE",design_data.diameter[index-1]);

/****************************************************************************
 *
 *	end of design
 *
 ***************************************************************************/

	design_data.index = index_1;

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	printf("\n");
	temp = loads.cable_weight - loads.cable_tgt_weight;
	printf("CABLE WEIGHT (LBS): %6.1f",temp);
	printf("\t");
	temp = loads.cable_volume - loads.cable_tgt_volume;
	printf("VOLUME (CUBIC INCHES): %6.1f",temp);
	printf("\n");
	temp = design_data.length_run[index-1] - design_data.length_run[jindex];
	printf("DESIGN LENGTH (FT):%6.0f",temp);
	printf("\n");

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	printf("\n");
	printf("                    TOTAL CABLE DESIGN\n");
	printf("\n");

	printf("CABLE WEIGHT (LBS): %6.1f",loads.cable_weight);
	printf("\t");
	printf("VOLUME (CUBIC INCHES): %6.1f",loads.cable_volume);
	printf("\n");
	printf("CABLE DROOP  (FT) : %6.0f",loads.cable_droop);
	printf("\t");
	printf("DESIGN LENGTH (FT)   :%6.0f",design_data.length_run[index-1]);
	printf("\n");
	printf("CABLE TRAIL  (FT) :%6.0f",loads.cable_trail);
	printf("\n");

 }
