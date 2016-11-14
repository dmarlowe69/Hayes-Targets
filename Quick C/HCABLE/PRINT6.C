/* file: print6.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void print_cable6(FILE *stream)
 {
 int index;
 int index_1;
 int jindex;
 double temp;
 time_t ltime;

/****************************************************************************
 *
 *	print header for target cable design
 *
 ***************************************************************************/

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

	for(index=1;index<=design_data.j1;index++) {
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
	fprintf(stream,"CABLE WEIGHT (LBS): %6.1f",loads.cable_tgt_weight);
	fprintf(stream,"\t");
	fprintf(stream,"VOLUME (CUBIC INCHES): %6.1f",loads.cable_tgt_volume);
	fprintf(stream,"\n");
	fprintf(stream,"CABLE DROOP  (FT) : %6.0f",loads.cable_tgt_droop);
	fprintf(stream,"\t");
	fprintf(stream,"DESIGN LENGTH (FT)   :%6.0f",design_data.length_run[index_1]);
	fprintf(stream,"\n");
	fprintf(stream,"CABLE TRAIL  (FT) :%6.0f",loads.cable_tgt_trail);

/****************************************************************************
 *
 *	print header for ltc-6
 *
 ***************************************************************************/

	fprintf(stream,"\n");
	fprintf(stream,"\n");
	fprintf(stream,"                   LTC-6 CABLE STEADY STATE DESIGN\n");
	fprintf(stream,"\n");
	fprintf(stream,"  DIA        LENGTH    TENSION        SAFETY FACTOR        ANGLE   SEG WT\n");
	fprintf(stream,"  (IN)        (FT)      (LBS)                              (DEG)   (LBS)");
	fprintf(stream,"\n");
	fprintf(stream,"\n");

/****************************************************************************
 *
 *	ltc-6 design
 *
 ***************************************************************************/
	jindex = (int)(design_data.j1) + 1;

	for(index=jindex;index<=design_data.index;index++) {
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
	temp = loads.cable_weight - loads.cable_tgt_weight;
	fprintf(stream,"CABLE WEIGHT (LBS): %6.1f",temp);
	fprintf(stream,"\t");
	temp = loads.cable_volume - loads.cable_tgt_volume;
	fprintf(stream,"VOLUME (CUBIC INCHES): %6.1f",temp);
	fprintf(stream,"\n");
	temp = design_data.length_run[index_1] - design_data.length_run[jindex-2];
	fprintf(stream,"DESIGN LENGTH (FT):%6.0f",temp);
	fprintf(stream,"\n");

/****************************************************************************
 *
 *	total design
 *
 ***************************************************************************/

	fprintf(stream,"\n");
	fprintf(stream,"                    TOTAL CABLE DESIGN\n");
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
	fprintf(stream,"\n");

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 #ifndef _WINDOWS

	if(stream == stdprn )
		fprintf(stream,"\f");

 #endif
 }
