/* file: cabcalc.c */

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

 #include	"float.h"
 #include	"math.h"
 #include	"stdio.h"
 #include	"cable.h"

/****************************************************************************
 *
 *	compute cable analysis parameters
 *
 ***************************************************************************/
 void para(void)
 {
 long double exp_temp;

	rad_to_deg = 180.0/3.141592654;

	/*temp = ( 1.0 - 0.006944 * alt );*/
	/*rho = 0.002377 * pow ( temp, 4.195555 );*/
	exp_temp = (-0.00003*alt) - (3.04E-15*(alt*alt*alt));
	temp = (double)(exp(exp_temp));
	rho = 0.002377 * temp;

	v1 = kcas * 1.689;

	q = 0.5 * 0.002377 * v1 * v1;
	dc = cab_dia[0];
	qsref = q * dc * seglen / 12.0;

	temp = 2.0 * q / rho;
	vfl = sqrt( temp );

	temp = ( 1116.89 * 1116.89 ) - ( 8.576488 * alt );
	sos = sqrt( temp );

	mach = vfl / sos;
	sara = 3.14159 * 0.25 * dia * dia;
	sarea = (sara / 144.0) * scale_area;

	xm1 = cab_wt[0];

	return;
 }
/****************************************************************************
 *
 *	lift and side force computations
 *
 ***************************************************************************/
 void trim_loads(void)
 {
	lift = q * sarea * cnt;
	side = q * sarea * cyt;

/****************************************************************************
 *
 *	drag computations
 *
 ***************************************************************************/
	switch(drag_index) {
	    case 0:
/* TRX-12 Drag data estimate prior to wind tunnel test */
		{
		con1 = aref / ( 0.95 * 3.14159 * bow * bow );
		con2 = aref / ( 0.95 * 3.14159 * bot * bot );

		cd1 = ( cnaw * cnaw * ( ( altrm + dlpitch ) * ( altrm + dlpitch ) ) ) * con1;
		cd2 = ( cnaw * cnaw * ( ( beta + dlyaw ) * ( beta + dlyaw ) ) ) * con1;
		cd3 = ( cnat * cnat * ( altrm * altrm ) ) * con2;
		cd4 = ( cnat * cnat * ( beta * beta ) ) * con2;
		cd = (cd0 + cd1 + cd2 + cd3 + cd4);
		break;
		}
	    case 1:
		{
/* TRX-12 Drag data from wind tunnel test */
		cd1 = 0.0;
		cd2 = 0.0197 * ( (alpad * alpad) + (betad * betad) );
		cd3 = 0.00237 * ( (dlpichd * dlpichd) + (dlyawd * dlyawd) );
		cd4 = 0.0;
		cd = (cd0 + cd1 + cd2 + cd3 + cd4) / drag_factor;
		break;
		}
	    case 2:
		cd = cd0;
		break;
		}

	drag = q * sarea * cd;

/****************************************************************************
 *
 *	compute angle between t2 and x axis
 *
 ***************************************************************************/

	i=0;
	phi = atan2( side, drag );
	phid[i] =phi * 57.29578;

/****************************************************************************
 *
 *	compute angle between t and t2
 *
 ***************************************************************************/

	i=0;
	temp = ( tgwt - lift ) * cos( phi );
	si = atan2( temp, drag );
	sid[i] = si * 57.29578;

/****************************************************************************
 *
 *	compute angle between t and missile body axis
 *
 ***************************************************************************/

	term1 = cos( si ) * cos( phi ) * cos( altrm ) * cos( beta );
	term2 =-cos( si ) * sin( phi ) * sin( beta );
	term3 = sin( si ) * sin( altrm ) * cos( beta );
	temp = term1 + term2 + term3;
	theta = acos( temp );
	thetd = theta * 57.29578;

/****************************************************************************
 *
 *	compute angle between t and x axis
 *
 ***************************************************************************/

	i=0;
	temp = cos( si ) * cos( phi );
	thetss = acos( temp );
	thetsd[i] = thetss * 57.29578;

/****************************************************************************
 *
 *	compute tension in the cable at the missile body
 *
 ***************************************************************************/

	i=0;
	tension[i] = drag / ( cos( si ) * cos( phi ) );
	tx[i] = tension[i] * cos( si ) * cos( phi );
	ty[i] = tension[i] * cos( si ) * sin( phi );
	tz[i] = tension[i] * sin( si );

/****************************************************************************
 *
 *	compute cable roll angle between t and missile body axis
 *
 ***************************************************************************/

	if(ty[i] != 0) {
		term1 = tz[i]/ty[i];
		roll = atan( term1 );
		rolld = roll * 57.29578;
		}
	else
		rolld = 90.0;

/****************************************************************************
 *
 *	compute cross flow coeffient of first cable segment
 *
 ***************************************************************************/

	s1 = sin( thetss );
	mn = v1 * s1 / sos;
	cdc0 = 1.17 + (0.025 * mn) - (0.25 * mn * mn) + (0.625 * mn * mn * mn);

/****************************************************************************
 *
 *	compute skin friction coeffient of first cable segment
 *
 ***************************************************************************/

	cmac = mach * cos( thetss );
	if( cmac <= 0.4 )
		picf0 = 0.038 - (0.0425 * cmac);
	if( cmac > 0.4 )
		picf0 = 0.013 + (0.0395 * ( ( cmac - 0.85 ) * ( cmac - 0.85 ) ) );

/****************************************************************************
 *
 *	modifiy cable drag coefficents based on option
 *
 ***************************************************************************/

	switch(cable_index) {
	    case 0:	break;
	    case 1:	cdc0 = cdc0/cdc_correct;
			picf0 = picf0/picf_correct;
			break;
	    case 2:	cdc0 = 2.0;
			picf0 = 0.02199;
			break;
	    case 3:	cdc0 = cdc_correct;
			picf0 = picf_correct;
			break;
		}

	return;
 }
/****************************************************************************
 *
 *	compute trim conditions for the target given dlpichd and dlyawd
 *
 ***************************************************************************/
 void trim_canard(void)
 {
	cnb = cna;
	cmb = cma;

	dlpitch = dlpichd / 57.29578;
	dlyaw = dlyawd / 57.29578;

	altrm = dlpitch * cmd / -cma;
	alpad = altrm * 57.29578;

	cnt = cna * altrm + cnd * dlpitch;

	beta = dlyaw * cmd / -cmb;
	betad = beta * 57.29578;

	cyt = cnb * beta + cnd * dlyaw;

	trim_loads();

	return;
 }
/****************************************************************************
 *
 *	compute trim conditions for the target given alpha and beta
 *
 ***************************************************************************/
 void trim(void)
 {
	cnb = cna;
	cmb = cma;

	altrm = alpad / 57.29578;
	beta = betad / 57.29578;

	dlpitch = -cma * altrm / cmd;
	dlpichd = dlpitch * 57.29578;

	cnt = cna * altrm + cnd * dlpitch;

	dlyaw = -cmb * beta / cmd;
	dlyawd = dlyaw * 57.29578;

	cyt = cnb * beta + cnd * dlyaw;

	trim_loads();

	return;
 }
/****************************************************************************
 *
 *	cable analysis function
 *
 ***************************************************************************/
 void cableanalysis(void)
 {
	l0 = 0.0;
	i = 0;
	step = 0;
	xtg[i] = 0.0;
	ytg[i] = 0.0;
	ztg[i] = 0.0;
	dc = cab_dia[step];
	seg_len = cab_len[step];
	xm1 = cab_wt[step];
	segwgt = ( xm1 * seglen ) / 1000.0;
	qsref = q * dc * seglen / 12.0;
	w = 0.0;
	vol = 0.0;

	while ( l0 < ( cablen ) )
	{
		i += 1;

		cmac = mach * cos( thetss );
		if( cmac <= 0.4 )
			picf = 0.038 - ( 0.0425 * cmac );
		if( cmac > 0.4 )
			picf = 0.013 + ( 0.0395 * ( ( cmac - 0.85 ) * ( cmac - 0.85 ) ) );

		s1 = sin( thetss );
		mn = v1 * s1 / sos;
		cdc = 1.17 + ( 0.025 * mn ) - ( 0.25 * mn * mn ) + ( 0.625 * mn * mn * mn );

		switch(cable_index) {
			case 0:	break;
			case 1:	cdc = cdc/cdc_correct;
				picf = picf/picf_correct;
				break;
			case 2:	cdc = 2.0;
				picf = 0.02199;
				break;
			case 3:	cdc = cdc_correct;
				picf = picf_correct;
				break;
			}

		ap = picf * qsref;
		ac = cdc * qsref;

		s2 = sin( phi );
		s3 = sin( si );
		c1 = cos( thetss );
		c2 = cos( phi );
		c3 = cos( si );

/***************************************************************************/

		dllift = (ac * s1 * c3 * s3 * c2) - (ap * c1 * c1 * s3);
		dldrag = (ap * c1 * c1 * c3 * c2) - (ac * s1 * ( (c3 * c3 * c2 * c2) - 1.0 ));
		dlside = (ac * s1 * c3 * c3 * c2 * s2) - (ap * c1 * c1 * c3 * s2);

/***************************************************************************/

		tx[i] = tx[i-1] + dldrag;
		ty[i] = ty[i-1] - dlside;
		tz[i] = tz[i-1] + segwgt - dllift;

		temp = ( (tx[i] * tx[i]) + (ty[i] * ty[i]) + (tz[i] * tz[i]));
		tension[i] = sqrt( temp );

/***************************************************************************/

		ztg[i] = ztg[i-1] + seglen * sin( si );
		ytg[i] = ytg[i-1] + seglen * cos( si ) * sin( phi );
		xtg[i] = xtg[i-1] + seglen * cos(si) * cos( phi );

/***************************************************************************/

		w += segwgt;
		vol += dc * dc * seglen * 12.0;

/***************************************************************************/

		temp = (tz[i] * tz[i]) + (ty[i] * ty[i]);
		t1 = sqrt( temp );

		temp = (tx[i] * tx[i]) + (ty[i] * ty[i]);
		t2 = sqrt( temp );

		si = atan2( tz[i], t2 );
		sid[i] = si * 57.29578;

		phi = atan2( ty[i], tx[i] );
		phid[i] = phi * 57.29578;

		thetss = atan2( t1, tx[i] );
		thetsd[i] = thetss * 57.29578;

/****************************************************************************
 *
 *	increase cable length
 *
 ***************************************************************************/

		l0 = l0 + seglen;

		if(l0 > seg_len) {
			step++;
			dc = cab_dia[step];
			seg_len = seg_len + cab_len[step];

			if(cable_info(dc)!=0) {
				printf("cable size % 4.2f not found\n",dc);
				xm1 = cab_wt[step];
				}
			segwgt = ( xm1 * seglen ) / 1000.0;
			qsref = q * dc * seglen / 12.0;
			/*printf("next seg\n");*/
			/*printf("step %d dc %f xm1 %f seglen %f\n",step,dc,xm1,seglen);*/
			/*printf("l0 %f seg_len %f %f\n\n",l0,seg_len,cab_len[step]);*/
			}
	}

/****************************************************************************
 *
 *	save number of segemnts
 *
 ***************************************************************************/

	num_segments = i;

/****************************************************************************
 *
 *	compute cone angle
 *
 ***************************************************************************/

	temp = (ytg[i] * ytg[i]) + (ztg[i] * ztg[i]);
	temp2 = sqrt( temp );

	cone = 57.29578 * atan2( temp2, xtg[i] );

	return;
 }
/****************************************************************************
 *
 *	determine max tension in towline
 *
 ***************************************************************************/
 double tmax(void)
 {
	double t_max;

	t_max=0.0;

	for( j = 0; j <= num_segments; ++j)
	{
		if(tension[j] > t_max)
			t_max = tension[j];
	}
	return(t_max);
 }
/****************************************************************************
 *
 *	compute cable at trim state
 *
 ***************************************************************************/
 void cable_trim(float attack, float side_slip)
 {
	alpad = attack;
	betad = side_slip;
	para();
	trim();
	cableanalysis();
 }
/****************************************************************************
 *
 *	compute cable at trim state
 *
 ***************************************************************************/
 void cable_trim_canard(float delta_ec, float delta_rc)
 {
	dlpichd = delta_ec;
	dlyawd = delta_rc;
	para();
	trim_canard();
	cableanalysis();
 }
/****************************************************************************
 *
 *	lift and side force computations
 *
 ***************************************************************************/
 void trim_init_loads(void)
 {
	lift = initial_tz;
	side = initial_ty;
	drag = initial_tx;

	altrm = 0.0;
	beta = 0.0;

/****************************************************************************
 *
 *	compute angle between t2 and x axis
 *
 ***************************************************************************/

	i=0;
	phi = atan2( side, drag );
	phid[i] =phi * 57.29578;

/****************************************************************************
 *
 *	compute angle between t and t2
 *
 ***************************************************************************/

	i=0;
	temp = ( - lift ) * cos( phi );
	si = atan2( temp, drag );
	sid[i] = si * 57.29578;

/****************************************************************************
 *
 *	compute angle between t and missile body axis
 *
 ***************************************************************************/

	term1 = cos( si ) * cos( phi ) * cos( altrm ) * cos( beta );
	term2 =-cos( si ) * sin( phi ) * sin( beta );
	term3 = sin( si ) * sin( altrm ) * cos( beta );
	temp = term1 + term2 + term3;
	theta = acos( temp );
	thetd = theta * 57.29578;

/****************************************************************************
 *
 *	compute angle between t and x axis
 *
 ***************************************************************************/

	i=0;
	temp = cos( si ) * cos( phi );
	thetss = acos( temp );
	thetsd[i] = thetss * 57.29578;

/****************************************************************************
 *
 *	compute tension in the cable at the missile body
 *
 ***************************************************************************/

	i=0;
	tension[i] = drag / ( cos( si ) * cos( phi ) );
	tx[i] = tension[i] * cos( si ) * cos( phi );
	ty[i] = tension[i] * cos( si ) * sin( phi );
	tz[i] = tension[i] * sin( si );

/****************************************************************************
 *
 *	compute cable roll angle between t and missile body axis
 *
 ***************************************************************************/

	if(ty[i] != 0) {
		term1 = tz[i]/ty[i];
		roll = atan( term1 );
		rolld = roll * 57.29578;
		}
	else
		rolld = 90.0;

/****************************************************************************
 *
 *	compute cross flow coeffient of first cable segment
 *
 ***************************************************************************/

	s1 = sin( thetss );
	mn = v1 * s1 / sos;
	cdc0 = 1.17 + (0.025 * mn) - (0.25 * mn * mn) + (0.625 * mn * mn * mn);

/****************************************************************************
 *
 *	compute skin friction coeffient of first cable segment
 *
 ***************************************************************************/

	cmac = mach * cos( thetss );
	if( cmac <= 0.4 )
		picf0 = 0.038 - (0.0425 * cmac);
	if( cmac > 0.4 )
		picf0 = 0.013 + (0.0395 * ( ( cmac - 0.85 ) * ( cmac - 0.85 ) ) );

/****************************************************************************
 *
 *	modifiy cable drag coefficents based on option
 *
 ***************************************************************************/

	switch(cable_index) {
	    case 0:	break;
	    case 1:	cdc0 = cdc0/cdc_correct;
			picf0 = picf0/picf_correct;
			break;
	    case 2:	cdc0 = 2.0;
			picf0 = 0.02199;
			break;
	    case 3:	cdc0 = cdc_correct;
			picf0 = picf_correct;
			break;
		}

	return;
 }
