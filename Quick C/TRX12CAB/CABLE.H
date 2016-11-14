/* file: cable.h */

/****************************************************************************
 *
 *	3D Cable Analysis header file
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 ***************************************************************************/

/*@R*************************************************************************
 *
 *      Revision History
 *
 *      Date            Engineer        Description
 *
 *	02/06/91	D.marlowe	Original release
 *
 ************************************************************************@R*/

 #define NUM_SEGMENTS 101
 #define NUM_STEPS 20
 //#define NUM_SEGMENTS 2001
 //#define NUM_STEPS 20

 extern int num_speeds;
 extern double airspeed[];
 extern int k;

 extern char title[81];
 extern char title12[81];
 extern char aerocode[81];
 extern float kcas;
 extern float dc,seglen,xm1,alt;
 extern float tgwt,cd0,xlb,dia,cablen;
 extern float offset,max_tension,max_alpha;
 extern float alpad,betad;

 extern float scale_area;
 extern float scale[];
 extern int karea;

 #define num_area 5

 extern float crw,dref,cna,cnd,cma,cmd,sigma;
 extern float crt,aref,bow,bot,cnaw,cnat,smar;

 extern double rho,v1,q,qsref,vfl,sos,mach,sara,sarea;

 extern double cnb,cmb,altrm,beta,dlpitch,dlpichd;
 extern double cnt,dlyaw,dlyawd,cyt,lift,side;

 extern double con1,con2,cd1,cd2,cd3,cd4,cd,drag;

 extern double phi,phid[NUM_SEGMENTS];

 extern double si,sid[NUM_SEGMENTS];

 extern double term1,term2,term3,theta,thetd;

 extern double thetss,thetsd[NUM_SEGMENTS];

 extern double tension[NUM_SEGMENTS],tx[NUM_SEGMENTS],ty[NUM_SEGMENTS],tz[NUM_SEGMENTS];

 extern double l0,xtg[NUM_SEGMENTS],ytg[NUM_SEGMENTS],ztg[NUM_SEGMENTS],w,vol;
 extern double segwgt,cmac,picf,ap,s1,mn,cdc,ac,s2,s3,c1,c2,c3;
 extern double dllift,dldrag,dlside;
 extern double t1,t2;
 extern double cdc0,picf0;
 extern double dum,cone;

 extern double rad_to_deg;

 extern double temp,temp2;

 extern int i,j,num_segments;

 extern int option;
 extern int position;

 extern double roll,rolld;

 extern double delta_e1,delta_e2,delta_e3;
 extern double delta_r1,delta_r2,delta_r3;
 extern double angle_station;
 extern double z_station,y_station;

 extern int cab_steps;
 extern int step;
 extern float cab_dia[NUM_STEPS];
 extern float cab_len[NUM_STEPS];
 extern float cab_wt[NUM_STEPS];
 extern float seg_len;
 extern float rts;
 extern int drag_index;
 extern float drag_factor;
 extern int cable_index;
 extern float cdc_correct;
 extern float picf_correct;
 extern	float user_delta_ec;
 extern	float user_delta_rc;

 extern float initial_tx;
 extern float initial_ty;
 extern float initial_tz;

 #define PI 3.141592654

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
