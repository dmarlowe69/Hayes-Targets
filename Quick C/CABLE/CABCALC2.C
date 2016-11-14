/* file: cabcalc2.c */

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
 *	cable analysis function
 *
 ***************************************************************************/
 void cable_analysis(void)
 {

	//seglen = 1.0;
	//seglen_out = 100.0;

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

	pcablen = 0.0;
	pflag = 0;
	_tx = tx[0];
	_ty = ty[0];
	_tz = tz[0];
	_xtg = 0.0;
	_ytg = 0.0;
	_ztg = 0.0;

	while ( l0 < ( cablen ) )
	{
		//putchar('!');
		if(pcablen == seglen_out) {
			i += 1;
			pflag = 1;
			pcablen = 0.0;
			//putchar('*');
			}

		pcablen = pcablen + seglen;


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

		_tx = _tx + dldrag;
		_ty = _ty - dlside;
		_tz = _tz + segwgt - dllift;

		temp = ( (_tx * _tx) + (_ty * _ty) + (_tz * _tz));
		_tension = sqrt( temp );

		_ztg = _ztg + seglen * sin( si );
		_ytg = _ytg + seglen * cos( si ) * sin( phi );
		_xtg = _xtg + seglen * cos(si) * cos( phi );

/***************************************************************************/

		if(pflag == 1) {
			tx[i] = _tx;
			ty[i] = _ty;
			tz[i] = _tz;

			temp = ( (tx[i] * tx[i]) + (ty[i] * ty[i]) + (tz[i] * tz[i]));
			tension[i] = sqrt( temp );

			ztg[i] = _ztg;
			ytg[i] = _ytg;
			xtg[i] = _xtg;
			//putchar('+');
			}

/***************************************************************************/

		w += segwgt;
		vol += dc * dc * seglen * 12.0;

/***************************************************************************/

		temp = (_tz * _tz) + (_ty * _ty);
		t1 = sqrt( temp );

		temp = (_tx * _tx) + (_ty * _ty);
		t2 = sqrt( temp );

		si = atan2( _tz, t2 );

		if(pflag == 1) {
			si = atan2( tz[i], t2 );
			sid[i] = si * 57.29578;
			}

		phi = atan2( _ty, _tx );

		if(pflag == 1) {
			phi = atan2( ty[i], tx[i] );
			phid[i] = phi * 57.29578;
			}

		thetss = atan2( t1, _tx );

		if(pflag == 1) {
			thetss = atan2( t1, tx[i] );
			thetsd[i] = thetss * 57.29578;
			}

		if(pflag == 1) {
			pflag = 0;
			}

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
