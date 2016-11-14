/* file: designst.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void designst(void)
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

 double d1_save;

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
	printf("                             MTR-101/RM-30\n");
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

	d1_save = design_data.d1;

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

			if(stricmp(design_data.design,"MTR-101") == 0) {
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

		  if(_strnicmp(design_data.airframe,"TLX",3) == 0) {
			cdc = cdc/2.0;
			cf = cf/2.0;
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

	print_segment_design(index, index_1);

/****************************************************************************
 *
 *	second leader design
 *
 ***************************************************************************/

	if(stricmp(design_data.design,"TLX")==0) {
		design_data.d1 = 0.064;
		design_data.leader_length = 100.0;
		}

	if(stricmp(design_data.design,"MTR-101")==0) {
		design_data.d1 = 0.064;
		design_data.leader_length = 100.0;
		}

	index = index + 1;
	index_1 = index_1 + 1;

/****************************************************************************
 *
 *	second leader design
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

		if((index_1 == 2	) & (loads.p0 < design_data.cut_off_angle ) & ( design_data.cut_off_angle > 0)) {
			flag = END_OF_SEGMENT;
			break;
			}
		if((index_1 == 2) & (l0 > design_data.leader_length) & (design_data.d1 != 0.0) & (design_data.leader_length != 0.0)) {
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
 *	end of second leader
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
	design_data.length[index] = l0 - design_data.length[index-1];

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

	print_segment_design(index, index_1);

/****************************************************************************
 *
 *	segment loop
 *
 ***************************************************************************/

	design_data.d1 = d1_save;

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

		if(l0 > 0) {

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
		loads.cable_droop = droop;
		loads.cable_trail = x;

		print_segment_design(index, index_1);
		}

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
