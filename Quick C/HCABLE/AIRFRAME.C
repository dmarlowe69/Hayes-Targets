/* file: airframe.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void targetloads(void)
 {
 double temp;

      if(_strnicmp(design_data.airframe,"TLX",3) == 0) {
		tlx_targetloads();
		return;
		}

      if(stricmp(design_data.airframe,"TRX-9") == 0) {
		trx9_loads();
		return;
		}

	design_data.airspeed_fps = design_data.airspeed * 1.689;
	design_data.q = 0.5 * 0.002378 * pow(design_data.airspeed_fps,2.0);
	design_data.ref_area = 3.1416 * pow(design_data.ref_dia,2.0) / 4.0;

	design_data.drag = design_data.cd0 * design_data.q * design_data.ref_area / 144.0;

	loads.tz = design_data.mass * design_data.g;
	loads.tx = design_data.drag;

	temp = pow(loads.tx,2.0) + pow(loads.tz,2.0);
	loads.tension[0] = sqrt(temp);

	temp = loads.tz/loads.tx;
	loads.theta[0] = atan(temp);

	loads.mass[0] = 0.0;
	loads.rts[0] = 0.0;
	loads.sf[0] = 0.0;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void tlx_targetloads(void)
 {
 float temp;

	if(stricmp(design_data.airframe,"TLX-TMS")==0) {
		design_data.mass = 60.0;
		design_data.cd0 = 0.36127;
		design_data.delta = -4.0;
		design_data.cl_delta = 0.1679;
		design_data.cd0_wing = 0.006736/1.875;
		design_data.cdi_wing = 0.0013334/1.875;
		printf("TMS\n");
		}

	if(stricmp(design_data.airframe,"TLX-1")==0) {
		design_data.mass = 68.0;
		design_data.cd0 = 0.36127;
		design_data.delta = -8.0;
		design_data.cl_delta = 0.31285;
		design_data.cd0_wing = 0.006736;
		design_data.cdi_wing = 0.0013334;
		printf("TLX-1\n");
		}

	if(stricmp(design_data.airframe,"TLX-19")==0) {
		design_data.mass = 63.0;
		design_data.cd0 = 0.36127;
		design_data.delta = -8.0;
		design_data.cl_delta = 0.31285;
		design_data.cd0_wing = 0.006736;
		design_data.cdi_wing = 0.0013334;
		printf("TLX-19\n");
		}

	if(stricmp(design_data.airframe,"TLX-29")==0) {
		design_data.mass = 60.0;
		design_data.cd0 = 0.36127;
		design_data.delta = -8.0;
		design_data.cl_delta = 0.31285;
		design_data.cd0_wing = 0.006736;
		design_data.cdi_wing = 0.0013334;
		printf("TLX-29\n");
		}

	if(stricmp(design_data.airframe,"TLX-IR")==0) {
		design_data.mass = 126.0;
		design_data.cd0 = 0.36127 + 0.1082;
		design_data.delta = -8.0;
		design_data.cl_delta = 0.31285;
		design_data.cd0_wing = 0.006736;
		design_data.cdi_wing = 0.0013334;
		}

	if(stricmp(design_data.airframe,"TLX-SIR")==0) {
		design_data.mass = 217.0;
		design_data.cd0 = 0.44411 + 0.12189;
		design_data.delta = -8.0;
		design_data.cl_delta = 0.31285;
		design_data.cd0_wing = 0.006736;
		design_data.cdi_wing = 0.008088;
		}

	if(stricmp(design_data.airframe,"TLX-FUSE")==0) {
		design_data.mass = 84.0;
		design_data.cd0 = 0.36127 + 0.1082;
		design_data.delta = -8.0;
		design_data.cl_delta = 0.31285;
		design_data.cd0_wing = 0.006736;
		design_data.cdi_wing = 0.0013334;
		}

	printf("Enter TLX wing angle (deg): ");
	scanf("%f",&temp);
	design_data.delta = (double)temp;
	printf("delta %f\n",design_data.delta);

	design_data.airspeed_fps = design_data.airspeed * 1.689;
	design_data.q = 0.5 * 0.002378 * pow(design_data.airspeed_fps,2.0);
	design_data.ref_area = 3.1416 * pow(design_data.ref_dia,2.0) / 4.0;

	design_data.cl = design_data.cl_delta * design_data.delta;
	design_data.lift = design_data.cl * design_data.q * design_data.ref_area / 144.0;

	design_data.cd_total = design_data.cd0;
	design_data.cd_total += design_data.cd0_wing;
	design_data.cd_total += (design_data.cdi_wing * pow(design_data.delta,2.0));

	design_data.drag = design_data.cd_total * design_data.q * design_data.ref_area / 144.0;

	loads.tz = design_data.mass * design_data.g - design_data.lift;
	loads.tx = design_data.drag;

	temp = pow(loads.tx,2.0) + pow(loads.tz,2.0);
	loads.tension[0] = sqrt(temp);

	temp = loads.tz/loads.tx;
	loads.theta[0] = atan(temp);

	loads.mass[0] = 0.0;
	loads.rts[0] = 0.0;
	loads.sf[0] = 0.0;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void dump_targetloads(void)
 {
 double angle;

	printf("airspeed_fps %f\n",design_data.airspeed_fps);
	printf("q %f\n",design_data.q);
	printf("ref_area %f\n",design_data.ref_area);

	printf("drag %f\n",design_data.drag);

	printf("tz %f\n",loads.tz);
	printf("tx %f\n",loads.tx);

	printf("tension %f\n",loads.tension[0]);

	angle = loads.theta[0] * RAD_TO_DEG;
	printf("theta %f rad %f deg\n",loads.theta[0],angle);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void trx9_loads(void)
 {
 int index;
 double xt;
 double yt;
 double xtp;
 double cna;
 double xcp;
 double lift;
 double phi;
 double tension;
 double temp,temp1,temp2,temp3;
 double theta,thetap;

	xt = 0.0;
	yt = .375;
	cna = 16.415;
	xcp = 1.4167;

	design_data.airspeed_fps = design_data.airspeed * 1.689;
	design_data.q = 0.5 * 0.002378 * pow(design_data.airspeed_fps,2.0);
	design_data.ref_area = 3.1416 * pow(design_data.ref_dia,2.0) / 4.0;

	theta = 0.0;

	for(index=0;index<100;index++) {
		lift = -cna * theta * design_data.q * design_data.ref_area;

		design_data.drag = design_data.cd0 * design_data.q * design_data.ref_area / 144.0;
		loads.tx = design_data.drag;

		temp = (design_data.mass - lift) / loads.tx;
		phi = atan(temp);
		temp = phi - theta;
		xtp = xt + yt / tan(temp);

		temp = ((design_data.mass - lift) * (design_data.mass - lift)) + (loads.tx * loads.tx);
		tension = sqrt(temp);

		temp1 = loads.tx * xcp * sin(theta);
		temp = phi - theta;
		temp2 = tension * sin(temp) * xtp;
		temp3 = -cna * design_data.q * design_data.ref_area * xcp * cos(theta);
		thetap = (temp1 - temp2) / temp3;
       
		theta = thetap;
       
		}

	loads.tz = design_data.mass * design_data.g - lift;

	temp = pow(loads.tx,2.0) + pow(loads.tz,2.0);
	loads.tension[0] = sqrt(temp);

	temp = loads.tz/loads.tx;
	loads.theta[0] = atan(temp);

	loads.mass[0] = 0.0;
	loads.rts[0] = 0.0;
	loads.sf[0] = 0.0;
 }
