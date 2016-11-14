/* file: cabwork.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("3D Cable Analysis")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *	3D Cable analysis
 *
 *      written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *      02/06/90        D.marlowe       Original release
 *
 ************************************************************************@R*/

/****************************************************************************
 *
 *      This Software was Designed using the following development tools:
 *
 *      MS-DOS Verison 3.3
 *	Microsoft C Version 6.00a
 *
 *              This Software is dedicated to the memory of
 *
 *                      NYSSA ANN MARLOWE
 *                  17 OCT 1988 - 03 NOV 1988
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	 "float.h"
 #include        "math.h"
 #include        "stdio.h"
 #include	 "cable.h"

/****************************************************************************
 *
 *      compute analysis for station
 *
 ***************************************************************************/

 void station_keep(int station_num)
 {
	double temp_zy;
	float zero_z;

	switch(station_num)
	{
	case 0: goto station_0;
	case 1: goto station_1;
	case 2: goto station_2;
	case 3: goto station_3;
	case 4: goto station_4;
	case 5: goto station_5;
	}
	return;
/****************************************************************************
 *
 *      check trim conditions for desired station number (minimum drag)
 *
 ***************************************************************************/
 station_0:

	alpad=0;
	betad=0;

 loop_0a:
	trim();
	cableanalysis();

	if(thetd < 40.0)
		if( fabs(alpad) < max_alpha )
			{
			alpad -= 0.01;
			goto loop_0a;
			}
	return;

/****************************************************************************
 *
 *      check trim conditions for desired station number (1)
 *
 ***************************************************************************/
 station_1:

	alpad=0;
	betad=0;

 loop_1a:
	trim();
	cableanalysis();

	if( cone < offset )
		if( tmax() < max_tension )
			if( fabs(alpad) < max_alpha )
				{
				alpad -= 0.01;
				goto loop_1a;
				}
	return;

/****************************************************************************
 *
 *      check trim conditions for desired station number (2)
 *
 ***************************************************************************/
 station_2:

	alpad=0;
	betad=max_alpha;
 loop_2a:
	trim();
	cableanalysis();

	temp_zy = cone - offset;
	if( fabs(temp_zy) > 2.5 )
		if( tmax() < max_tension )
			temp_zy = ztg[i] - ytg[i];
				if( fabs(temp_zy) > 25.0)
					if( fabs(alpad) < max_alpha )
					{
					alpad -= 0.01;
					goto loop_2a;
					}
	return;

/****************************************************************************
 *
 *      check trim conditions for desired station number (3)
 *
 ***************************************************************************/
 station_3:

	alpad=0;
	betad=0;

	zero_z = 0.0;

 loop_3:
	trim();
	cableanalysis();

	if( ztg[i] > zero_z )
		if( tmax() < max_tension )
			{
			alpad += 0.01;
			goto loop_3;
			}

 loop_3a:
	trim();
	cableanalysis();

	if( cone < offset )
		if( tmax() < max_tension )
			if( betad < max_alpha )
				{
				betad += 0.01;
				goto loop_3a;
				}
	return;

/****************************************************************************
 *
 *      check trim conditions for desired station number (4)
 *
 ***************************************************************************/
 station_4:

	alpad=0;
	betad=max_alpha * 0.75;

	zero_z = 0.0;

 loop_4:
	trim();
	cableanalysis();

	if( cone < offset )
		if( tmax() < max_tension )
			temp_zy = ztg[i] - ytg[i];
				if( fabs(temp_zy) > 25.0)
					if( fabs(alpad) < max_alpha )
					{
					alpad += 0.01;
					goto loop_4;
					}
	return;

/****************************************************************************
 *
 *      check trim conditions for desired station number (5)
 *
 ***************************************************************************/
 station_5:

	alpad=0;
	betad=0;

	zero_z = 0.0;

 loop_5:
	trim();
	cableanalysis();

	if( ztg[i] > zero_z )
		if( tmax() < max_tension )
			if( alpad < max_alpha )
				{
				alpad += 0.01;
				goto loop_5;
				}

 loop_5a:
	trim();
	cableanalysis();

	if( cone < offset )
		if( tmax() < max_tension )
			if( alpad < max_alpha )
				{
				alpad += 0.05;
				goto loop_5a;
				}
	return;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

 void reel_in(void)
 {
	double accel;
	double t;

	accel = 12.0;

	alpad=0;
	betad=0;

	trim();
	cableanalysis();

	for(t=0.0;t<=5.0;t+=0.01)
		{
		kcas = kcas + 1.0;
		}
 }
