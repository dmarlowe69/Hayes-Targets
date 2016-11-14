/* file: revs.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *	REEL FLANGE CLEARENCE FOR TARGET
 *
 ***************************************************************************/
 void flange(int index, double volume, double diameter)
 {
 double flange_clearance;

	flange_clearance = volume + 0.8835729;
	flange_clearance = sqrt(flange_clearance / 3.1416 / 4.5);

	if(design_data.s0 > 171.0)
		flange_clearance = (9.0 / 2.0) - flange_clearance - 0.07;
	else
		flange_clearance = (7.125 / 2.0) - flange_clearance - 0.0725;

	revs(index, diameter);

	loads.flange[index] = flange_clearance;
 }
/****************************************************************************
 *
 *	REEL FLANGE CLEARENCE FOR LTC-6
 *
 ***************************************************************************/
 void flange6(int index, double volume, double diameter)
 {
 double flange_clearance;

	flange_clearance = volume + 1.84078125 - loads.cable_tgt_volume;
	flange_clearance = sqrt(flange_clearance / 3.1416 / 6.0);

	flange_clearance = (11.0 / 2.0) - flange_clearance;

	ltc_6_revs(index, diameter);

	loads.flange[index] = flange_clearance;
 }
/****************************************************************************
 *
 *	CALCULATE NUMBER OF REVS
 *
 ***************************************************************************/
 void revs(int index, double diameter)
 {
 double W6;
 double C6;
 double N8;
 double segment_length;

	segment_length = design_data.length[index-1];
	W6 = 4.5 / diameter;

	do {
		loads.target_wind_dia = loads.target_wind_dia + 2.0 * diameter - (diameter / 6.0955);
		C6 = loads.target_wind_dia * 3.1416 / 12.0;
		loads.target_revs = loads.target_revs + W6;
		segment_length = segment_length - C6 * W6;
		}while(segment_length > 0.0);

	N8 = segment_length / C6;
	loads.target_revs = loads.target_revs + N8;

	loads.revs[index] = loads.target_revs;
 }
/****************************************************************************
 *
 *	REVS ON LTC-6 REEL
 *
 ***************************************************************************/
 void ltc_6_revs(int index, double diameter)
 {
 double W7;
 double C7;
 double N8;
 double external_segment_length;

	external_segment_length = design_data.length[index-1];
	W7 = 6.0 / diameter;

	do {
		loads.external_wind_dia = loads.external_wind_dia + 2.0 * diameter - (diameter / 6.0955);
		C7 = loads.external_wind_dia * 3.1416 / 12.0;
		loads.target_revs = loads.target_revs + W7;
		external_segment_length= external_segment_length - C7 * W7;
		}while(external_segment_length > 0.0);

	N8 = external_segment_length / C7;
	loads.target_revs = loads.target_revs + N8;

	loads.revs[index] = loads.target_revs;
 }
