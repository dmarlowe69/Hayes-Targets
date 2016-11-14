/* file: cable.c */

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
 #include	"stdlib.h"

/****************************************************************************
 *
 *	define variables
 *
 ***************************************************************************/

 #define NUM_SEGMENTS 101
 #define NUM_STEPS 20
 //#define NUM_SEGMENTS 2001
 //#define NUM_STEPS 20

	int num_speeds;
	double airspeed[] = {
	300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300 };
	int k;

	char title[81];
	char title12[81];
	char aerocode[81];
	float kcas;
	float dc,seglen,xm1,alt;
	float tgwt,cd0,xlb,dia,cablen;
	float offset,max_tension,max_alpha;
	float alpad,betad;

	float scale_area;
	float scale[] = { 1.0F,0.75F,0.50F,0.25F,0.10F };
	int karea;

 #define num_area 5

	float crw,dref,cna,cnd,cma,cmd,sigma;
	float crt,aref,bow,bot,cnaw,cnat,smar;

	double rho,v1,q,qsref,vfl,sos,mach,sara,sarea;

	double cnb,cmb,altrm,beta,dlpitch,dlpichd;
	double cnt,dlyaw,dlyawd,cyt,lift,side;

	double con1,con2,cd1,cd2,cd3,cd4,cd,drag;

	double phi,phid[NUM_SEGMENTS];

	double si,sid[NUM_SEGMENTS];

	double term1,term2,term3,theta,thetd;

	double thetss,thetsd[NUM_SEGMENTS];

	double tension[NUM_SEGMENTS],tx[NUM_SEGMENTS],ty[NUM_SEGMENTS],tz[NUM_SEGMENTS];

	double l0,xtg[NUM_SEGMENTS],ytg[NUM_SEGMENTS],ztg[NUM_SEGMENTS],w,vol;
	double segwgt,cmac,picf,ap,s1,mn,cdc,ac,s2,s3,c1,c2,c3;
	double dllift,dldrag,dlside;
	double t1,t2;
	double cdc0,picf0;
	double dum,cone;

	double rad_to_deg;

	double temp,temp2;

	int i,j,num_segments;

	int option;
	int position;

	double roll,rolld;

	double delta_e1,delta_e2,delta_e3;
	double delta_r1,delta_r2,delta_r3;
	double angle_station;
	double z_station,y_station;

	int cab_steps;
	int step;
	float cab_dia[NUM_STEPS];
	float cab_len[NUM_STEPS];
	float cab_wt[NUM_STEPS];
	float seg_len;
	float rts;
	int drag_index;
	float drag_factor;
	int cable_index;
	float cdc_correct;
	float picf_correct;
	float user_delta_ec;
	float user_delta_rc;

	float initial_tx;
	float initial_ty;
	float initial_tz;

/* New Stuff */

	float seglen_out;

	double _tx,_ty,_tz;
	double _tension;
	double _ztg,_ytg,_xtg;
	double pcablen;

	int pflag;

 #define PI 3.141592654

/****************************************************************************
 *
 *	include files
 *
 ***************************************************************************/

 void init_aero(void);
 void init_para(void);
 int init_user(void);
 void init_aero_name(void);

 void para(void);
 void trim_loads(void);
 void trim_canard(void);
 void trim(void);
 void cableanalysis(void);
 double tmax(void);
 void trim_init_loads(void);

 void print_legend_excel(FILE *stream);
 void print_analysis_excel(FILE *stream);
 void print_all_excel(FILE *stream);
 void print_head(FILE *stream);
 void print_aero(FILE *stream);
 void print_para(FILE *stream);
 void print_trim(FILE *stream);
 void print_tgt(FILE *stream);
 void print_legend(FILE *stream);
 void print_analysis(FILE *stream);
 void print_cone(FILE *stream);
 void print_all(FILE *stream);
 void print_most(FILE *stream);
 void print_trim_legend(FILE *stream);
 void print_trim_analysis(FILE *stream);
 void print_reel_legend(FILE *stream);
 void print_reel(FILE *stream);
 void print_cone_legend(FILE *stream);
 void print_cone_max(FILE *stream);
 void print_accuracy_legend(FILE *stream);
 void print_accuracy(FILE *stream);
 void print_all_brief(FILE *stream);
 void print_cable_out(void);
 void print_cone_out(void);
 void print_reel_out(void);
 void print_trim_out(void);
 void print_menu(void);
 void print_option(void);

 void station_keep(int station_num);
 void reel_in(void);

 int cable_info(float diameter);
 void cable_trim(float attack, float side_slip);
 void cable_trim_canard(float delta_ec, float delta_rc);

 void cable_plot(void);

/****************************************************************************
 *
 *	entry point
 *
 ***************************************************************************/
 int main()
 {

	FILE *fp;
	FILE *excel;

/****************************************************************************
 *
 *	initailize variables
 *
 ***************************************************************************/

	init_aero_name();
	init_aero();
	init_para();

	while(1) {

/****************************************************************************
 *
 *	user input
 *
 ***************************************************************************/

	option = init_user();

/****************************************************************************
 *
 *	target trim analysis
 *
 ***************************************************************************/

	if( option == 1)
		{
		if ((fp=fopen("trim.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		print_trim_legend(stdout);
		print_trim_legend(fp);
		for( k = 0; k < num_speeds; ++k)
			{
			kcas = airspeed[k];
			para();
			for( alpad = -15; alpad <= 15; alpad += 1.0)
				{
				trim();
				i = 0;
				print_trim_analysis(stdout);
				print_trim_analysis(fp);
				}
			}
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute station trim conditions at tow reel
 *
 ***************************************************************************/

	if( option == 2 )
		{
		if ((fp=fopen("reel.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		print_reel_legend(stdout);
		print_reel_legend(fp);
		for( k = 0; k < num_speeds; ++k)
			{
			kcas = airspeed[k];
			para();
			station_keep(position);
			print_reel(stdout);
			print_reel(fp);
			}
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute station cone angles
 *
 ***************************************************************************/

	if( option == 3 )
		{
		if ((fp=fopen("cone.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		/*for( karea = 0; karea < num_area; ++karea)*/
		/*{*/
		/*scale_area = scale[karea];*/
		/*scale_area = 1.0;*/
		print_cone_legend(stdout);
		print_cone_legend(fp);
		for( k = 0; k < num_speeds; ++k)
			{
			kcas = airspeed[k];
			para();
			station_keep(position);
			print_cone_max(stdout);
			print_cone_max(fp);
			}
		/*}*/
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute station trim conditions
 *
 ***************************************************************************/

	if( option == 4 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		if ((excel=fopen("cable.xls","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		for( k = 0; k < num_speeds; ++k)
			{
			kcas = airspeed[k];
			para();
			station_keep(position);
			print_all(stdout);
			print_all(fp);
			print_all_excel(excel);
			}
		fclose(fp);
		fclose(excel);
		}
/****************************************************************************
 *
 *	compute station trim conditions given user inputs
 *
 ***************************************************************************/

	if( option == 5 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		para();
		trim();
		cableanalysis();
		print_all(stdout);
		print_all(fp);
		/*print_all_brief(fp);*/
		/*print_all_excel(fp);*/
		/*print_all_brief(stdprn);*/
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute station trim conditions given user inputs
 *
 ***************************************************************************/

	if( option == 6 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		para();
		trim_canard();
//		cableanalysis();
		cable_analysis();
		print_all(stdout);
		print_all(fp);
		/*print_all_brief(stdprn);*/
		/*print_all(stdprn);*/
		/*print_all_excel(fp);*/
		fclose(fp);
		cable_plot();
		}
/****************************************************************************
 *
 *	compute station trim conditions for various alpha and beta
 *
 ***************************************************************************/

	if( option == 7 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		alpad = 0.0;
		betad = 0.0;
		para();
		trim();
		print_para(stdout);
		print_para(fp);

		for(alpad=-10.0;alpad<11.0;alpad+=1.0) {
			for(betad=0.0;betad<11.0;betad+=1.0) {
			para();
			trim();
			cableanalysis();
			print_most(stdout);
			print_most(fp);
			}
		}
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute station trim conditions for various canard angles
 *
 ***************************************************************************/

	if( option == 8 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		dlpichd = 0.0;
		dlyawd = 0.0;
		para();
		trim_canard();
		print_para(stdout);
		print_para(fp);

		for(dlpichd=-10.0;dlpichd<11.0;dlpichd+=2.5) {
			for(dlyawd=0.0;dlyawd<11.0;dlyawd+=2.5) {
			para();
			trim_canard();
			cableanalysis();
			print_most(stdout);
			print_most(fp);
			}
		}
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute effect of actuator accuracy
 *
 ***************************************************************************/

	if( option == 9 )
		{
		if ((fp=fopen("actuator.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		print_accuracy_legend(stdout);
		print_accuracy_legend(fp);
		para();
		station_keep(position);

		z_station = ztg[num_segments];
		y_station = ytg[num_segments];

		delta_e1 = dlpichd;
		delta_e2 = delta_e1 - 0.5;
		delta_e3 = delta_e1 + 0.5;

		delta_r1 = dlyawd;
		delta_r2 = delta_r1 - 0.5;
		delta_r3 = delta_r1 + 0.5;

		for(dlpichd=delta_e2;dlpichd<=delta_e3;dlpichd+=0.1) {
			for(dlyawd=delta_r2;dlyawd<=delta_r3;dlyawd+=0.1) {
			para();
			trim_canard();
			cableanalysis();

			temp = ztg[num_segments]/ytg[num_segments];
			angle_station = atan(temp) * 180.0/PI;

			print_accuracy(stdout);
			print_accuracy(fp);
			}
		}
		fclose(fp);
		}
/****************************************************************************
 *
 *	compute station trim conditions given user inputs
 *
 ***************************************************************************/

	if( option == 10 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		user_delta_ec = dlpichd;
		user_delta_rc = dlyawd;

		dlpichd = 0.0;
		dlyawd = 0.0;
		cable_trim_canard(dlpichd,dlyawd);
		print_para(fp);
		print_all_brief(fp);
		/*print_all_brief(stdprn);*/

//		dlpichd = -user_delta_ec;
//		dlyawd = 0.0;
//		cable_trim_canard(dlpichd,dlyawd);
//		print_all_brief(fp);
//		/*print_all_brief(stdprn);*/

//		dlpichd = -user_delta_ec;
//		dlyawd = user_delta_rc;
//		cable_trim_canard(dlpichd,dlyawd);
//		print_all_brief(fp);
//		/*print_all_brief(stdprn);*/

		dlpichd = 0.0;
		dlyawd = user_delta_rc;
		cable_trim_canard(dlpichd,dlyawd);
		print_all_brief(fp);
		/*print_all_brief(stdprn);*/

//		dlpichd = user_delta_ec;
//		dlyawd = user_delta_rc;
//		cable_trim_canard(user_delta_ec,user_delta_rc);
//		print_all_brief(fp);
//		/*print_all_brief(stdprn);*/

//		dlpichd = user_delta_ec;
//		dlyawd = 0.0;
//		cable_trim_canard(dlpichd,dlyawd);
//		print_all_brief(fp);
//		/*print_all_brief(stdprn);*/

		fclose(fp);
		}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	if( option == 11 )
		{
		if ((fp=fopen("reel.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		print_reel_legend(stdout);
		print_reel_legend(fp);
		for( k = 0; k < num_speeds; ++k)
			{
			kcas = airspeed[k];
			alpad = 0.0;
			betad = 0.0;
			para();
			trim();
			cableanalysis();
			print_reel(stdout);
			print_reel(fp);
			}
		fclose(fp);
		}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	if( option == 12 )
		{
		print_menu();
		}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	if( option == 13 )
		{
		exit(0);
		}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	if( option == 14 )
		{
		if ((fp=fopen("cable.out","w")) == NULL)
			{
			printf("error opening output data file\n");
			exit(0);
			}
		dlpichd = 0.0;
		dlyawd = 0.0;
		para();
		trim_init_loads();
		cableanalysis();
		print_all(stdout);
		/*print_all(fp);*/
		print_all_excel(fp);

		fclose(fp);
		}
/****************************************************************************
 *
 *	end of main
 *
 ***************************************************************************/
	} /* while */
 }
