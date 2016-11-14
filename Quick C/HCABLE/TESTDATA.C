/* file: testdata.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void testdata(int test_id)
 {
	switch(test_id) {

		case 0: {
			strcpy(design_data.project,"TEST");
			strcpy(design_data.opt,"ONEWAY");
			strcpy(design_data.design,"LTC-6");
			strcpy(design_data.airframe,"TPT-3");

			design_data.mass = 88.5;
			design_data.cd0 = 0.34;
			design_data.g =1.0;
			design_data.s0 = 276.0;
			design_data.l = 30000.0;
			design_data.airspeed = 300.0;
			design_data.sf = 1.7;
			design_data.seglength = 10;
			design_data.d2 = 0.024;
			design_data.d1 = 0.035;
			design_data.leader_length = 390.0;

			if((design_data.leader_length > 100) | (design_data.leader_length == 0))
				design_data.cut_off_angle = 0.0;
			else
			design_data.cut_off_angle = design_data.leader_length / RAD_TO_DEG;
			printf("test case %d\n",test_id);
			break;
			}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

		case 1: {
			strcpy(design_data.project,"TEST");
			strcpy(design_data.opt,"TWOWAY");
			strcpy(design_data.design,"MTR-101");
			strcpy(design_data.airframe,"TPT-3");

			design_data.mass = 88.5;
			design_data.cd0 = 0.34;
			design_data.g =1.0;
			design_data.s0 = 1200.0;
			design_data.l = 30000.0;
			design_data.airspeed = 300.0;
			design_data.sf = 1.7;
			design_data.seglength = 10;
			design_data.d2 = 0.0;
			design_data.d1 = 0.050;
			design_data.leader_length = 30000.0;

			if((design_data.leader_length > 100) | (design_data.leader_length == 0))
				design_data.cut_off_angle = 0.0;
			else
				design_data.cut_off_angle = design_data.leader_length / RAD_TO_DEG;
			printf("test case %d\n",test_id);
			break;
			}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

		case 2: {
			strcpy(design_data.project,"TEST");
			strcpy(design_data.opt,"ONEWAY");
			strcpy(design_data.design,"LTC-2");
			strcpy(design_data.airframe,"TGT-12");

			design_data.mass = 60.0;
			design_data.cd0 = 2.10;
			design_data.g =1.0;
			design_data.s0 = 171.0;
			design_data.l = 3000.0;
			design_data.airspeed = 300.0;
			design_data.sf = 1.7;
			design_data.seglength = 10;
			design_data.d2 = 0.066;
			design_data.d1 = 0.0;
			design_data.leader_length = 0.0;
			design_data.cut_off_angle = 0.0;
			printf("test case %d\n",test_id);
			break;
			}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

		case 3: {
			strcpy(design_data.project,"TEST");
			strcpy(design_data.opt,"TWOWAY");
			strcpy(design_data.design,"MTR-101");
			strcpy(design_data.airframe,"TLX-29");

			design_data.mass = 60.0;
			design_data.cd0 = 0.50;
			design_data.g =1.0;
			design_data.s0 = 2400.0;
			design_data.l = 30000.0;
			design_data.airspeed = 300.0;
			design_data.sf = 1.6;
			design_data.seglength = 10;
			design_data.d2 = 0.0;
			design_data.d1 = 0.050;
			design_data.leader_length = 30000.0;

			if((design_data.leader_length > 100) | (design_data.leader_length == 0))
				design_data.cut_off_angle = 0.0;
			else
				design_data.cut_off_angle = design_data.leader_length / RAD_TO_DEG;
			printf("test case %d\n",test_id);
			break;
			}
/****************************************************************************
 *
 *
 *
 ***************************************************************************/

		case 4: {
			strcpy(design_data.project,"TEST");
			strcpy(design_data.opt,"TWOWAY");
			strcpy(design_data.design,"MTR-101");
			strcpy(design_data.airframe,"TLX-IR");

			design_data.mass = 60.0;
			design_data.cd0 = 0.50;
			design_data.g =1.0;
			design_data.s0 = 2500.0;
			design_data.l = 30000.0;
			design_data.airspeed = 300.0;
			design_data.sf = 1.6;
			design_data.seglength = 10;
			design_data.d2 = 0.0;
			design_data.d1 = 0.050;
			design_data.leader_length = 30000.0;

			if((design_data.leader_length > 100) | (design_data.leader_length == 0))
				design_data.cut_off_angle = 0.0;
			else
				design_data.cut_off_angle = design_data.leader_length / RAD_TO_DEG;
			printf("test case %d\n",test_id);
			break;
			}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

		default: {
			strcpy(design_data.project,"TEST");
			strcpy(design_data.opt,"ONEWAY");
			strcpy(design_data.design,"LTC-2");
			strcpy(design_data.airframe,"TGT-12");

			design_data.mass = 60.0;
			design_data.cd0 = 2.10;
			design_data.g =1.0;
			design_data.s0 = 171.0;
			design_data.l = 3000.0;
			design_data.airspeed = 300.0;
			design_data.sf = 1.7;
			design_data.seglength = 10;
			design_data.d2 = 0.066;
			design_data.d1 = 0.0;
			design_data.leader_length = 0.0;
			design_data.cut_off_angle = 0.0;
			printf("test case %d\n",test_id);
			break;
			}
		}
 }
