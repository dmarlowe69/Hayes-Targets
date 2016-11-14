/* file: analysis.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void analysis(void)
 {
 double l0;
 double droop;
 double x;
 double w;
 double v;
 double p2;
 double temp;
 double q;
 double d;
 double cdc;
 double cf;
 double seglength;
 double segweight;
 double seglift;
 double segdrag;

 double mn;
 double mach;
 double cmac;
 double picf;

 int index;
 int index_1;

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
 *	load first cable segment in design
 *
 ***************************************************************************/

	getcable(design_data.diameter[index],design_data.type[index]);

 #ifdef DEBUG

	printf("\nFirst Cable Segment\n");
	dump_look();
	printf("l0 %f\n",l0);
	getch();

 #endif

	d = design_data.diameter[index];
	cdc = cable_look.cdc;
	cf = cable_look.cf;

/****************************************************************************
 *
 *	analysis loop
 *
 ***************************************************************************/

	while ( l0 < design_data.total_length ) {

		l0 = l0 + design_data.seglength;

		loads.p0 = atan2(loads.tz, loads.tx);

		if( (loads.theta[index_1-1] - loads.p0) < .0005) {
			printf("!");
			loads.p0 = loads.theta[index_1-1];
		}
		else {
			printf(".");
		}

		if( l0 > design_data.length_run[index]) {
			l0 = l0 - design_data.seglength;


			loads.mass[index_1] = w;
			temp = pow(loads.tx,2.0) + pow(loads.tz,2.0);
			loads.tension[index_1] = sqrt(temp);
			loads.theta[index_1] = loads.p0;
			loads.rts[index_1] = cable_look.strength;
			loads.sf[index_1] = cable_look.strength / loads.tension[index_1];

			index_1++;
			index++;
			getcable(design_data.diameter[index],design_data.type[index]);

 #ifdef DEBUG

			dump_look();
			printf("l0 %f\n",l0);
			getch();

 #endif

			d = design_data.diameter[index];
			cdc = cable_look.cdc;
			cf = cable_look.cf;
			}

/****************************************************************************
 *
 *	new coefficients per trx-12/b study
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
				picf = .013 + .0395 * pow(cmac - .85,2);
				}
			cf = (picf / 3.1416);
			}

		segweight = cable_look.weight * seglength / 1000.0;

		seglift = d * seglength * q * cdc * pow(sin(loads.p0),2) * cos(loads.p0) / 12.0;
		segdrag = d * seglength * q * (cdc * pow(sin(loads.p0),3) + 3.141592654 * cf) / 12.0;
		loads.tz = loads.tz	+ segweight - seglift;
		loads.tx = loads.tx	+ segdrag;

		droop = droop + seglength * sin(loads.p0);
		x = x + seglength * cos(loads.p0);
		w = w + segweight;

		v = v + d * d * seglength * 12.0;

 #ifdef DEBUG

		printf("\n");
		printf("d %f\n",d);
		printf("q %f\n",q);
		printf("cdc %f\n",cdc);
		printf("cf %f\n",cf);
		printf("loads.p0 %f\n",loads.p0);
		printf("segweight %f\n",segweight);
		printf("seglift %f\n",seglift);
		printf("segdrag %f\n",segdrag);
		printf("loads.tz %f\n",loads.tz);
		printf("loads.tx %f\n",loads.tx);
		printf("droop %f\n",droop);
		printf("x %f\n",x);
		printf("w %f\n",w);
		printf("v %f\n",v);
		getch();

 #endif
		}

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
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void print_cable(FILE *stream)
 {
 int index;
 int index_1;
 double temp;
 time_t ltime;

	fprintf(stream,"\n");
	fprintf(stream,"\n");
	fprintf(stream,"\n");
	fprintf(stream,"\n");
	fprintf(stream,"                   TOW CABLE STEADY STATE ANALYSIS\n");
	fprintf(stream,"                            HAYES METHOD\n");
	fprintf(stream,"\n");

	time( &ltime );
	fprintf(stream,"%s", ctime( &ltime ) );

	fprintf(stream,"PROJECT: %s",design_data.project);
	fprintf(stream,"          ");
	fprintf(stream,"DESIGN: %s\n",design_data.design);

	fprintf(stream,"TARGET: %s\n",design_data.airframe);

	fprintf(stream,"WEIGHT (LBS): %4.1f\n",design_data.mass);

	fprintf(stream,"AIRSPEED (KIAS): %4.0f",design_data.airspeed);
	fprintf(stream,"          ");
	fprintf(stream,"DRAG COEFFICIENT: %5.3f\n",design_data.cd0);

	fprintf(stream,"WING ANGLE (DEG): %5.2f",design_data.delta);
	fprintf(stream,"          ");
	fprintf(stream,"TARGET LIFT (LBS):%5.2f\n",design_data.lift);

	temp = 999.0;
	for(index=1;index<=design_data.index;index++) {
		if(loads.sf[index] < temp ) {
			temp = loads.sf[index];
			}
		}

	fprintf(stream,"SAFETY FACTOR: %5.2f",temp);
	fprintf(stream,"          ");
	fprintf(stream,"G FACTOR: %5.2f\n",design_data.g);

	temp = loads.theta[0] * RAD_TO_DEG;
	index = 0;

	fprintf(stream,"\n");
	fprintf(stream,"  DIA        LENGTH    TENSION        SAFETY FACTOR        ANGLE   SEG WT\n");
	fprintf(stream,"  (IN)        (FT)      (LBS)                              (DEG)   (LBS)");
	fprintf(stream,"\n");
	fprintf(stream," TARGET                 %4.0f                               %5.2f\n", loads.tension[0], temp);
	temp = loads.rts[index+1]/loads.tension[index];
	fprintf(stream,"                                      %5.2f",temp);
	fprintf(stream," ");
	temp = (1.0 / (loads.rts[index+1] / loads.tension[index])) * 100.0;
	fprintf(stream,"%5.1f",temp);
	fputc('%',stream);
	fprintf(stream,"\n");

	for(index=1;index<=design_data.index;index++) {
		index_1 = index - 1;

		fprintf(stream," %5.3f ",design_data.diameter[index_1]);
		if(design_data.type[index_1]==TYPE_STRAND) {
			fprintf(stream,"1X7");
			}
		else {
			fprintf(stream,"   ");
			}

		fprintf(stream," ");
		fprintf(stream,"%5.0f",design_data.length[index_1]);

		fprintf(stream,"        ");
		fprintf(stream,"%4.0f",loads.tension[index]);

		fprintf(stream," ");
		fprintf(stream,"%4.0f RTS",loads.rts[index]);

		fprintf(stream," ");
		fprintf(stream,"%5.2f",loads.sf[index]);

		temp =	(1.0 / loads.sf[index]) * 100.0;
		fprintf(stream," ");
		fprintf(stream,"%5.1f",temp);
		fputc('%',stream);

		temp = loads.theta[index] * RAD_TO_DEG;
		fprintf(stream,"         ");
		fprintf(stream,"%5.2f",temp);

		temp = loads.mass[index] - loads.mass[index-1];
		fprintf(stream," ");
		fprintf(stream,"%6.2f\n",temp);

		}

	fprintf(stream,"\n");
	fprintf(stream,"CABLE WEIGHT (LBS): %6.1f",loads.cable_weight);
	fprintf(stream,"\t");
	fprintf(stream,"VOLUME (CUBIC INCHES): %6.1f",loads.cable_volume);
	fprintf(stream,"\n");
	fprintf(stream,"CABLE DROOP  (FT) : %6.0f",loads.cable_droop);
	fprintf(stream,"\t");
	fprintf(stream,"DESIGN LENGTH (FT)   :%6.0f",design_data.length_run[index_1]);
	fprintf(stream,"\n");
	fprintf(stream,"CABLE TRAIL  (FT) :%6.0f",loads.cable_trail);

 #ifndef _WINDOWS

	if(stream == stdprn )
		fprintf(stream,"\f");

 #endif

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void direct_output(void)
 {
 char dev;
 int flag;
 FILE *outfile;
 char outfile_name[80];
 
 #ifdef _WINDOWS
 #define OPENFLAGS "w"             		/* Access permission */
    struct _wopeninfo wininfo;     		/* Open information */
    char wintitle[32]="CABLE ANALYSIS";	/* Title for window */
    FILE *wp;                      		/* FILE ptr to window */
    int nRes;                     		/* I/O result */
 #endif
 
	fflush( stdin );
	flag = 0;
	do {

 #ifdef _WINDOWS

		printf("\n(S)creen or (F)ile): ");
		dev = getchar();

 #else

		printf("\n(S)creen or (P)rinter or (F)ile: ");
		dev = getch();

 #endif

		switch(toupper(dev)) {

			case 'S':
				if(stricmp(design_data.design,"LTC-6") == 0) {
 #ifdef _WINDOWS
/* Set up window info structure for _fwopen */
    				wininfo._version = _QWINVER;
    				wininfo._title = wintitle;
				    wininfo._wbufsize = _WINBUFDEF;
/* Create a new window */
/* NULL second argument accepts default size/position */
    				wp = _fwopen( &wininfo, NULL, OPENFLAGS );
    				if( wp == NULL ) {
        				printf( "***ERROR: _fwopen\n" );
        				exit( -1 );
				    }
					print_cable6(wp);
/* Close the window */
    				//nRes = fclose( wp );
 #else
					print_cable6(stdout);
 #endif					
					}
				else {
 #ifdef _WINDOWS
/* Set up window info structure for _fwopen */
	    		wininfo._version = _QWINVER;
    			wininfo._title = wintitle;
    			wininfo._wbufsize = _WINBUFDEF;
/* Create a new window */
/* NULL second argument accepts default size/position */
		    	wp = _fwopen( &wininfo, NULL, OPENFLAGS );
    			if( wp == NULL ) {
        			printf( "***ERROR: _fwopen\n" );
        			exit( -1 );
    				}
					print_cable(wp);
/* Close the window */
    				//nRes = fclose( wp );
 #else
					print_cable(stdout);
 #endif					
					}
				flag = 1;
				break;

 #ifndef _WINDOWS

			case 'P':
				if(stricmp(design_data.design,"LTC-6") == 0) {
					print_cable6(stdprn);
					}
				else {
					print_cable(stdprn);
					}
				flag = 1;
				break;
 #endif

			case 'F':
				printf("Enter output file name: ");
				scanf("%s",&outfile_name);
				if ((outfile = fopen(outfile_name,"w")) == NULL) {
					printf("error opening output file\n");
					break;
					}
				if(stricmp(design_data.design,"LTC-6") == 0) {
					print_cable6(outfile);
					}
				else {
					print_cable(outfile);
					}
				fclose(outfile);
				flag = 1;
				break;

			default:
				break;
			}

		} while(flag == 0 );

 }
