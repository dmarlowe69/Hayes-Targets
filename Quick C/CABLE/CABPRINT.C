/* file: cabprint.c */

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

 #include	<float.h>
 #include	<math.h>
 #include	<stdio.h>
 #include	<process.h>
 #include	"cable.h"
 #include	"msc.h"

/****************************************************************************
 *
 *      print header
 *
 ***************************************************************************/
 void print_head(FILE *stream)
 {
 /*
	if(stream == stdprn )
		fprintf(stream,"\f");
 */
	fprintf(stream,"\n");
	fprintf(stream,"\tTRX-12 three dimensional cable analysis");
	fprintf(stream,"\n");
	fprintf(stream,"%s",title);

	return;
 }
/****************************************************************************
 *
 *      print aero data to stream
 *
 ***************************************************************************/
 void print_aero(FILE *stream)
 {
	fprintf(stream,"\n\t");
	fprintf(stream,"          aerodynamic coefficients");
	fprintf(stream,"\n");

	fprintf(stream,"%s",title12);

	fprintf(stream,"\n\t");
	fprintf(stream,"cnaw = % 8.4f",cnaw);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"cnat = % 8.4f",cnat);

	fprintf(stream,"\n\t");
	fprintf(stream,"cna  = % 8.4f",cna);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"cnd  = % 8.4f",cnd);

	fprintf(stream,"\n\t");
	fprintf(stream,"cma  = % 8.4f",cma);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"cmd  = % 8.4f",cmd);

	fprintf(stream,"\n\t");
	fprintf(stream,"aref = % 8.4f",aref);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"dref = % 8.4f",dref);

	fprintf(stream,"\n\t");
	fprintf(stream,"bow  = % 8.4f",bow);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"bot  = % 8.4f",bot);

	fprintf(stream,"\n\t");
	fprintf(stream,"crw  = % 8.4f",crw);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"crt  = % 8.4f",crt);

	fprintf(stream,"\n\t");
	fprintf(stream,"sigma= % 8.4f",sigma);
	fprintf(stream,"\t\t\t");
	fprintf(stream,"smar = % 8.4f",smar);

	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print cable analysis parameters
 *
 ***************************************************************************/
 void print_para(FILE *stream)
 {
	fprintf(stream,"\n\t");
	fprintf(stream,"          cable analysis parameters");
	fprintf(stream,"\n");

	fprintf(stream,"\n\t");
	fprintf(stream,"alt   = % 8.4f feet",alt);
	fprintf(stream,"\t\t");
	fprintf(stream,"rho   = % 8.6f lb-sec**2/ft**4",rho);

	fprintf(stream,"\n\t");
	fprintf(stream,"v1    = % 8.4f ft/sec",v1);
	fprintf(stream,"\t");
	fprintf(stream,"vfl   = % 8.4f ft/sec",vfl);

	fprintf(stream,"\n\t");
	fprintf(stream,"q     = % 8.4f lb/ft**2",q);
	fprintf(stream,"\t");
	fprintf(stream,"mach  = % 8.4f",mach);

	fprintf(stream,"\n\t");
	fprintf(stream,"sos   = % 8.4f ft/sec",sos);
	fprintf(stream,"\t");
	fprintf(stream,"kcas  = % 8.4f kcas",kcas);

	fprintf(stream,"\n\t");
	fprintf(stream,"qsref = % 8.4f ft**2",qsref);
	fprintf(stream,"\t\t");
	fprintf(stream,"sarea = % 8.4f ft**2",sarea);

	fprintf(stream,"\n\t");
	fprintf(stream,"dc    = % 8.4f inches",dc);
	fprintf(stream,"\t\t");
	fprintf(stream,"dia   = % 8.4f inches",dia);

	fprintf(stream,"\n\t");
	fprintf(stream,"tgwt  = % 8.4f lbs",tgwt);
	fprintf(stream,"\t\t");
	fprintf(stream,"xlb   = % 8.4f inches",xlb);

	fprintf(stream,"\n\t");
	fprintf(stream,"cablen= % 8.4f feet",cablen);
	fprintf(stream,"\t\t");
	fprintf(stream,"xm1   = % 8.4f lbs/1000ft",xm1);
	fprintf(stream,"\n");

	fprintf(stream,"\n\tstep\tdiameter\tlength\t\tweight\n");
	for(step=0;step < cab_steps;step++) {
		fprintf(stream,"\t % 2d\t",step);
		fprintf(stream," % 8.4f\t", cab_dia[step]);
		fprintf(stream," % 8.4f\t", cab_len[step]);
		fprintf(stream," % 8.4f\n", cab_wt[step]);
		}

	fprintf(stream,"\n\t\t");
	fprintf(stream,"cable rts = % 8.4f lbs",max_tension);
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print target trim conditions
 *
 ***************************************************************************/
 void print_trim(FILE *stream)
 {
	fprintf(stream,"\n\t");
	fprintf(stream,"          target trim conditions");
	fprintf(stream,"\n");

	fprintf(stream,"\n\t\t");
	fprintf(stream,"station number = %d",position);
	fprintf(stream,"\n");

	fprintf(stream,"\n\t");
	fprintf(stream,"alpha = % 8.4f degrees",alpad);
	fprintf(stream,"\t");
	fprintf(stream,"beta  = % 8.4f degrees",betad);

	fprintf(stream,"\n\t");
	fprintf(stream,"cna   = % 8.4f",cna);
	fprintf(stream,"\t\t");
	fprintf(stream,"cnb   = % 8.4f",cnb);

	fprintf(stream,"\n\t");
	fprintf(stream,"cma   = % 8.4f",cma);
	fprintf(stream,"\t\t");
	fprintf(stream,"cmb   = % 8.4f",cmb);

	fprintf(stream,"\n\t");
	fprintf(stream,"dlpich= % 8.4f degrees",dlpichd);
	fprintf(stream,"\t");
	fprintf(stream,"dlyaw = % 8.4f degrees",dlyawd);

	fprintf(stream,"\n\t");
	fprintf(stream,"cnt   = % 8.4f",cnt);
	fprintf(stream,"\t\t");
	fprintf(stream,"cyt   = % 8.4f",cyt);

	fprintf(stream,"\n\t");
	fprintf(stream,"lift  = % 8.4f lbs",lift);
	fprintf(stream,"\t\t");
	fprintf(stream,"side  = % 8.4f lbs",side);

	fprintf(stream,"\n\t");
	fprintf(stream,"cd    = % 8.4f",cd);
	fprintf(stream,"\t\t");
	fprintf(stream,"drag  = % 8.4f lbs",drag);
	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print conditions at target
 *
 ***************************************************************************/
 void print_tgt(FILE *stream)
 {
	fprintf(stream,"\n\t");
	fprintf(stream,"          initial cable trim conditions");
	fprintf(stream,"\n");

	fprintf(stream,"\n\t");
	fprintf(stream,"phi   = % 8.4f degrees",phid[i]);
	fprintf(stream,"\t");
	fprintf(stream,"si    = % 8.4f degrees",sid[i]);

	fprintf(stream,"\n\t");
	fprintf(stream,"theta = % 8.4f degrees",thetd);
	fprintf(stream,"\t");
	fprintf(stream,"tx    = % 8.4f lbs",tx[i]);

	fprintf(stream,"\n\t");
	fprintf(stream,"thets = % 8.4f degrees",thetsd[i]);
	fprintf(stream,"\t");
	fprintf(stream,"ty    = % 8.4f lbs",ty[i]);

	fprintf(stream,"\n\t");
	fprintf(stream,"tenson= % 8.4f lbs",tension[i]);
	fprintf(stream,"\t\t");
	fprintf(stream,"tz    = % 8.4f lbs",tz[i]);

	fprintf(stream,"\n\t");
	fprintf(stream,"cdc   = % 8.4f",cdc0);
	fprintf(stream,"\t\t");
	fprintf(stream,"picf  = % 8.4f",picf0);

	fprintf(stream,"\n\t");
	fprintf(stream,"roll   = % 8.4f",rolld);

	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print cable analysis header
 *
 ***************************************************************************/
 void print_legend(FILE *stream)
 {
 /*
	if(stream == stdprn )
		fprintf(stream,"\f");
 */

	fprintf(stream,"\n\n");
	fprintf(stream,"   xtg     ytg     ztg");
	fprintf(stream,"     tx       ty       tz     tension");
	fprintf(stream," thetss   si    phi");
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print cable analysis
 *
 ***************************************************************************/
 void print_analysis(FILE *stream)
 {
	fprintf(stream,"% 7.2f",xtg[i]);
	fprintf(stream," ");
	fprintf(stream,"% 7.2f",ytg[i]);
	fprintf(stream," ");
	fprintf(stream,"% 7.2f",ztg[i]);
	fprintf(stream," ");

	fprintf(stream,"% 8.2f",tx[i]);
	fprintf(stream," ");
	fprintf(stream,"% 8.2f",ty[i]);
	fprintf(stream," ");
	fprintf(stream,"% 8.2f",tz[i]);
	fprintf(stream," ");
	fprintf(stream,"% 8.2f",tension[i]);
	fprintf(stream," ");

	fprintf(stream,"% 6.2f",thetsd[i]);
	fprintf(stream," ");
	fprintf(stream,"% 6.2f",sid[i]);
	fprintf(stream," ");
	fprintf(stream,"% 6.2f",phid[i]);

	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print cone angle
 *
 ***************************************************************************/
 void print_cone(FILE *stream)
 {
	fprintf(stream,"\ncone angle = % 7.3f\n",cone);

	return;
 }
/****************************************************************************
 *
 *      print cable analysis
 *
 ***************************************************************************/
 void print_all(FILE *stream)
 {
/****************************************************************************
 *
 *      printheader
 *
 ***************************************************************************/

	print_head(stream);
	print_aero(stream);

/****************************************************************************
 *
 *      print parameters
 *
 ***************************************************************************/

	print_para(stream);

/****************************************************************************
 *
 *      print trim conditions for the target
 *
 ***************************************************************************/

	print_trim(stream);

/****************************************************************************
 *
 *      print conditions at the target
 *
 ***************************************************************************/

	i = 0;

	print_tgt(stream);
	fprintf(stream,"\f");

/****************************************************************************
 *
 *      cable analysis function
 *
 ***************************************************************************/

	print_legend(stream);

	i = 0;

	while( i <= num_segments )
		{
		print_analysis(stream);
		i += 1;
		}

/****************************************************************************
 *
 *      print cone angle
 *
 ***************************************************************************/

	print_cone(stream);
	fprintf(stream,"\f");

/****************************************************************************
 *
 *      return to caller
 *
 ***************************************************************************/

	return;
 }
/****************************************************************************
 *
 *      print cable analysis
 *
 ***************************************************************************/
 void print_most(FILE *stream)
 {
/****************************************************************************
 *
 *      printheader
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      print parameters
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      print trim conditions for the target
 *
 ***************************************************************************/

	print_trim(stream);

/****************************************************************************
 *
 *      print conditions at the target
 *
 ***************************************************************************/

	i = 0;

	print_tgt(stream);
	fprintf(stream,"\f");

/****************************************************************************
 *
 *      cable analysis function
 *
 ***************************************************************************/

	print_legend(stream);

	i = 0;

	while( i <= num_segments )
		{
		print_analysis(stream);
		i += 1;
		}

/****************************************************************************
 *
 *      print cone angle
 *
 ***************************************************************************/

	print_cone(stream);
	fprintf(stream,"\f");

/****************************************************************************
 *
 *      return to caller
 *
 ***************************************************************************/

	return;
 }
/****************************************************************************
 *
 *      print trim analysis header
 *
 ***************************************************************************/
 void print_trim_legend(FILE *stream)
 {
 /*
	if(stream == stdprn )
		fprintf(stream,"\f");
 */

	fprintf(stream,"%s",title);

	fprintf(stream,"\n");
	fprintf(stream,"            trx-12 trim analysis parameters");
	fprintf(stream,"\n");

	fprintf(stream,"\n");
	fprintf(stream,"kcas\talpha\tdelta\ttheta");
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print target trim analysis
 *
 ***************************************************************************/
 void print_trim_analysis(FILE *stream)
 {
	fprintf(stream,"% 7.2f",kcas);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",alpad);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",dlpichd);
	fprintf(stream,"\t");

	fprintf(stream,"% 6.2f",thetd);
	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print reel analysis header
 *
 ***************************************************************************/
 void print_reel_legend(FILE *stream)
 {
 /*
	if(stream == stdprn )
		fprintf(stream,"\f");
 */

	fprintf(stream,"%s",title);

	fprintf(stream,"\n\t\t");
	fprintf(stream,"station number = %d",position);
	fprintf(stream,"\n");

	fprintf(stream,"\n");
	fprintf(stream,"            trx-12 tow reel parameters");
	fprintf(stream,"\n");
	fprintf(stream,"                                      ");
	fprintf(stream,"                        cable angles\n");
	fprintf(stream,"   kcas   alpha   beta");
	fprintf(stream,"     tx       ty       tz     tension");
	fprintf(stream,"  x-z    x-y    y-z");
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print tow reel parameters
 *
 ***************************************************************************/
 void print_reel(FILE *stream)
 {
	float angle_xz;
	float angle_xy;
	float angle_yz;

	fprintf(stream,"% 7.2f",kcas);
	fprintf(stream," ");
	fprintf(stream,"% 7.2f",alpad);
	fprintf(stream," ");
	fprintf(stream,"% 7.2f",betad);
	fprintf(stream," ");

	fprintf(stream,"% 8.2f",tx[num_segments]);
	fprintf(stream," ");
	fprintf(stream,"% 8.2f",ty[num_segments]);
	fprintf(stream," ");
	fprintf(stream,"% 8.2f",tz[num_segments]);
	fprintf(stream," ");
	fprintf(stream,"% 8.2f",tension[num_segments]);
	fprintf(stream," ");

	angle_xz = atan2( tz[num_segments],tx[num_segments] ) * 57.29578;
	angle_xy = atan2( ty[num_segments],tx[num_segments] ) * 57.29578;
	angle_yz = atan2( tz[num_segments],ty[num_segments] ) * 57.29578;

	fprintf(stream,"% 6.2f",angle_xz);
	fprintf(stream," ");
	fprintf(stream,"% 6.2f",angle_xy);
	fprintf(stream," ");
	fprintf(stream,"% 6.2f",angle_yz);

	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print cone angle analysis header
 *
 ***************************************************************************/
 void print_cone_legend(FILE *stream)
 {
 /*
	if(stream == stdprn )
		fprintf(stream,"\f");
 */

	fprintf(stream,"\n");
	fprintf(stream,"        trx-12 maximum cone angles");
	fprintf(stream,"\n");
	fprintf(stream,"%s",title);
	fprintf(stream,"\n");
	fprintf(stream,"%s",title12);

	fprintf(stream,"\n\t\t");
	fprintf(stream,"station number = %d scale = %f",position,scale_area);
	fprintf(stream,"\n");

	fprintf(stream," kcas\t alpha\t beta\t delta_ec\t delta_rc\t tension\t cone angle\tZ");
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print cone angles
 *
 ***************************************************************************/
 void print_cone_max(FILE *stream)
 {

	fprintf(stream,"% 7.2f",kcas);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",alpad);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",betad);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",dlpichd);
	fprintf(stream,"\t\t");
	fprintf(stream,"% 7.2f",dlyawd);
	fprintf(stream,"\t\t");

	fprintf(stream,"% 8.2f",tension[num_segments]);
	fprintf(stream,"\t");

	fprintf(stream,"% 8.3f",cone);
	fprintf(stream,"\t");

	i = num_segments;
	fprintf(stream,"% 7.2f",ztg[i]);
	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

/****************************************************************************
 *
 *      print actuator accuracy analysis header
 *
 ***************************************************************************/
 void print_accuracy_legend(FILE *stream)
 {
 /*
	if(stream == stdprn )
		fprintf(stream,"\f");
 */

	fprintf(stream,"\n");
	fprintf(stream,"        trx-12 actuator accuracy");
	fprintf(stream,"\n");
	fprintf(stream,"%s",title);
	fprintf(stream,"\n");
	fprintf(stream,"%s",title12);

	fprintf(stream,"\n\t\t");
	fprintf(stream,"station number = %d",position);
	fprintf(stream,"\t\t");
	fprintf(stream,"airspeed = % 4.2f KCAS",kcas);
	fprintf(stream,"\n");

	fprintf(stream,"delta_ec\t delta_rc\t xtg\t ytg\t ztg\t cone angle\t station angle \ttension");
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print actuator accuracy data
 *
 ***************************************************************************/
 void print_accuracy(FILE *stream)
 {

	fprintf(stream,"% 7.2f",dlpichd);
	fprintf(stream,"\t\t");
	fprintf(stream,"% 7.2f",dlyawd);
	fprintf(stream,"\t\t");

	fprintf(stream,"% 7.2f",xtg[num_segments]);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",ytg[num_segments]);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",ztg[num_segments]);
	fprintf(stream,"\t");

	fprintf(stream,"% 8.3f",cone);

	fprintf(stream,"\t");
	fprintf(stream,"% 8.3f",angle_station);

	fprintf(stream,"\t");
	fprintf(stream,"% 8.3f",tension[num_segments]);

	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print cable analysis
 *
 ***************************************************************************/
 void print_all_brief(FILE *stream)
 {
/****************************************************************************
 *
 *      printheader
 *
 ***************************************************************************/

	print_head(stream);
 /*
	print_aero(stream);
 */
/****************************************************************************
 *
 *      print parameters
 *
 ***************************************************************************/
 /*
	print_para(stream);
 */
/****************************************************************************
 *
 *      print trim conditions for the target
 *
 ***************************************************************************/

	print_trim(stream);

/****************************************************************************
 *
 *      print conditions at the target
 *
 ***************************************************************************/

	i = 0;

	print_tgt(stream);

/****************************************************************************
 *
 *      cable analysis function
 *
 ***************************************************************************/

	print_legend(stream);

	i = 0;
	print_analysis(stream);
	i = num_segments;
	print_analysis(stream);

/****************************************************************************
 *
 *      print cone angle
 *
 ***************************************************************************/

	print_cone(stream);

/****************************************************************************
 *
 *      return to caller
 *
 ***************************************************************************/

	return;
 }
/****************************************************************************
 *
 *	print cable.out
 *
 ***************************************************************************/
 void print_cable_out(void)
 {
	system("hard cable.out");
 }
/****************************************************************************
 *
 *	print cone.out
 *
 ***************************************************************************/
 void print_cone_out(void)
 {
	system("hard cone.out");
 }
/****************************************************************************
 *
 *	print reel.out
 *
 ***************************************************************************/
 void print_reel_out(void)
 {
	system("hard reel.out");
 }
/****************************************************************************
 *
 *	print trim.out
 *
 ***************************************************************************/
 void print_trim_out(void)
 {
	system("hard trim.out");
 }
/****************************************************************************
 *
 *	print menu
 *
 ***************************************************************************/
 void print_menu(void)
 {
	msc_print();
 }
/****************************************************************************
 *
 *	print options
 *
 ***************************************************************************/
 void print_option(void)
 {
	int print_opt;

	printf("Print Options:");
	scanf("%d",&print_opt);
 }
