/* file: cabinput.c */

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
 #include	<stdlib.h>
 #include	<string.h>
 #include	<dos.h>
 #include	"cable.h"
 #include	"msc.h"
 #include	"mouse.h"

/****************************************************************************
 *
 *	input aero data from disk file
 *
 ***************************************************************************/
 void init_aero(void)
 {
	char buffer[81];
	FILE *aero;

	if ((aero=fopen("aero12.dat","r")) == NULL)
		{
			printf("error opening aero data file\n");
			exit(0);
		}

	fgets(aerocode, 80, aero);

	if(strcmp(aerocode,"AERODESIGN\n") == 0) {

		fgets(title12, 80, aero);

		fgets(buffer, 80, aero);
		sscanf(buffer," %f %f %f %f %f %f %f",&crw, &dref, &cna, &cnd, &cma, &cmd, &sigma);

		fgets(buffer, 80, aero);
		sscanf(buffer," %f %f %f %f %f %f %f",&crt, &aref, &bow, &bot, &cnaw, &cnat, &smar);
		}
	else {
		printf("\nAERO DATA OF UNKNOWN TYPE\n");
		fclose(aero);
		exit(0);
		}

	fclose(aero);

	alpad = 0.0;
	betad = 0.0;

	return;
 }
/****************************************************************************
 *
 *	input cable analysis parameters
 *
 ***************************************************************************/
 void init_para(void)
 {
	char buffer[256];
	FILE *tr3;
	double max_angle;

	if ((tr3=fopen("tr3.dat","r")) == NULL)
		{
			printf("error opening parameter data file\n");
			exit(0);
		}

	fgets(title, 255, tr3);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %d", &num_speeds);

	for( k=0; k < num_speeds; ++k)
		{
		fgets(buffer, 255, tr3);
		sscanf(buffer," %f", &kcas);
		airspeed[k] = kcas;
		}

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&alt);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&tgwt);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&cd0);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %d",&drag_index);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&drag_factor);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&xlb);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&dia);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&offset);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&max_tension);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&max_alpha);

	max_angle = cma/cmd;
	max_angle = fabs(max_angle);
	max_alpha =  max_alpha / (1.0 + max_angle);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&scale_area);

/****************************************************************************
 *
 *	cable parameters
 *
 ***************************************************************************/

	fgets(buffer, 255, tr3);
	sscanf(buffer," %d",&cable_index);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&cdc_correct);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&picf_correct);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&cablen);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&seglen);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %f",&seglen_out);

	fgets(buffer, 255, tr3);
	sscanf(buffer," %d", &cab_steps);

	cablen = 0.0;
	for(step=0;step < cab_steps;step++) {

		fgets(buffer, 255, tr3);
		sscanf(buffer," %f", &cab_dia[step]);

		fgets(buffer, 255, tr3);
		sscanf(buffer," %f", &cab_len[step]);

		fgets(buffer, 255, tr3);
		sscanf(buffer," %f", &cab_wt[step]);
		if(cab_wt[step] == 0.0) {
			cable_info(cab_dia[step]);
			cab_wt[step] = xm1;
			}
		cablen = cablen + cab_len[step];
		}

	fclose(tr3);

	return;
 }
/****************************************************************************
 *
 *	user input
 *
 ***************************************************************************/
 int init_user(void)
 {
	char comma;
	float delta_13,delta_24;

/****************************************************************************
 *
 *	setup mouse
 *
 ***************************************************************************/

	if(mousecheck()==0) {	/* see if mouse exists, & # of buttons */
		printf("No mouse\n");
		/*exit(1);*/
		}
	mousecursor(1);		/* turn mouse cursor on */
	wait_mouseclick();	/* wait for mouse buttons were clicked */
	mousecursor(0);		/* turn mouse cursor off */

/****************************************************************************
 *
 *	setup screen
 *
 ***************************************************************************/

	option = msc();

/*
	printf("\n");
	printf("trx-12/uav performance\n\n");
	printf("analysis option:\n\n");
	printf("(1) trim analysis\n");
	printf("(2) tow reel parameters\n");
	printf("(3) max cone angle\n");
	printf("(4) complete analysis\n");
	printf("(5) trim analysis with user inputs for alpha and beta\n");
	printf("(6) trim analysis with user inputs for canard positions\n");
	printf("(7) alpha and beta sweep\n");
	printf("(8) canard sweep\n");
	printf("(9) actuator accuracy\n");
	printf("(10) envelop sweep\n");
	printf("(11) exit\n");
	printf("\n");
	printf("enter option :");
	scanf("%d",&option);
*/

/*
	switch(option)
	{
	case 1:exit(0);
	case 4:exit(0);
	case 7:exit(0);
	case 8:exit(0);
	case 9:exit(0);
	}
*/
	if( ( (option == 5) || (option == 6) ) )
		{
		printf("\n");
		printf("\nEnter airspeed (kias) > ");
		scanf("%f",&kcas);
		if( kcas == 0.0 )
			exit(0);
		}
	if( option == 5 )
		{
		printf("\n");
		printf("\nEnter alpha and beta trim (ex:3.5,0) > ");
		scanf("%f %c %f",&alpad,&comma,&betad);
		}

	if( option == 6 )
		{
		printf("\n");
		printf("\nEnter delta 2/4 and delta 1/3 (ex:3.5,0) > ");
		scanf("%f %c %f",&delta_24,&comma,&delta_13);
		dlpichd = delta_24;
		dlyawd = delta_13;
		}

	if( ( (option == 7) || (option == 8) ) )
		{
		printf("\n");
		printf("\nEnter airspeed (kias) > ");
		scanf("%f",&kcas);
		if( kcas == 0.0 )
			exit(0);
		}

	if(option == 9)
		{
		printf("\n");
		printf("\nEnter airspeed (kias) > ");
		scanf("%f",&kcas);
		if( kcas == 0.0 )
			exit(0);
		}

	if(option == 10)
		{
		printf("\n");
		printf("\nEnter delta 2/4 and delta 1/3 (ex:3.5,0) > ");
		scanf("%f %c %f",&delta_24,&comma,&delta_13);
		dlpichd = delta_24;
		dlyawd = delta_13;
		printf("\n");
		printf("\nEnter airspeed (kias) > ");
		scanf("%f",&kcas);
		if( kcas == 0.0 )
			exit(0);
		}

	if( option == 12 )
		return(option);

	if( option == 13 )
		exit(0);

	if( option == 0 )
		exit(0);

	if( option == 14 ) {
		printf("\n");
		printf("\nEnter initial tx > ");
		scanf("%f",&initial_tx);
		printf("\nEnter initial ty > ");
		scanf("%f",&initial_ty);
		printf("\nEnter initial tz > ");
		scanf("%f",&initial_tz);
		return(option);
		}


	printf("\n");
	printf("\nEnter station number > ");
	scanf("%d",&position);

	return(option);
 }
/****************************************************************************
 *
 *	find cable parameters given diameter
 *
 ***************************************************************************/
 int cable_info(float diameter)
 {
 static int index;
 static float acco[19][3] =	{	{0.040, 380.0, 3.85},
					{0.044, 440.0, 4.52},
					{0.050, 560.0, 5.76},
					{0.055, 680.0, 7.01},
					{0.059, 750.0, 7.91},
					{0.064, 880.0, 9.39},
					{0.070,1040.0,11.5},
					{0.076,1210.0,13.4},
					{0.082,1390.0,15.4},
					{0.087,1570.0,17.7},
					{0.092,1770.0,19.6},
					{0.097,1980.0,22.8},
					{0.104,2200.0,25.2},
					{0.108,2390.0,28.2},
					{0.114,2660.0,30.9},
					{0.131,2850.0,34.3},
					{0.141,3300.0,40.1},
					{0.151,3720.0,46.2},
					{0.161,4290.0,53.5}
				};
/*
	for(index=0;index<19;index++) {
	    printf(" %f %f %f\n",acco[index][0],acco[index][1],acco[index][2]);
		}
*/
	for(index=0;index<19;index++) {
		if(diameter==acco[index][0]) {
		    rts = acco[index][1];
		    xm1 = acco[index][2];
		    return(0);
		}
	}
	return(1);
 }
/****************************************************************************
 *
 *	input aero data from disk file
 *
 *	written by denton marlowe
 *	(c)1991 Hayes Targets
 *
 ***************************************************************************/
 void init_aero_name(void)
 {
	char *name,*number;
	char buffer[256];
	FILE *aero;
	double data;
	int i,check,line_number;
	int error;
/****************************************************************************
 *
 *	Array of pointers to data item names
 *
 ***************************************************************************/

 #define NUM_AERO_NAMES 14
 static char file_name[] = { "aeroname.dat" };
 static char NAME[14][81] = {	"crw",
				"dref",
				"cna",
				"cnd",
				"cma",
				"cmd",
				"sigma",
				"crt",
				"aref",
				"bow",
				"bot",
				"cnaw",
				"cnat",
				"smar"
				};

/****************************************************************************
 *
 *	Array of pointers to data items
 *
 ***************************************************************************/

 static float *DATA[14] = {	&crw,
				&dref,
				&cna,
				&cnd,
				&cma,
				&cmd,
				&sigma,
				&crt,
				&aref,
				&bow,
				&bot,
				&cnaw,
				&cnat,
				&smar
			};
/****************************************************************************
 *
 *	Data structure for file information
 *
 ***************************************************************************/

 static struct find_t fileinfo;

/****************************************************************************
 *
 *	search for aero data file
 *
 ***************************************************************************/

	if(_dos_findfirst(file_name,_A_NORMAL,&fileinfo) == 0) {
		printf("%s %ld bytes\n",fileinfo.name,fileinfo.size);

		while(_dos_findnext(&fileinfo) == 0)
			printf("%s %ld bytes\n",fileinfo.name,fileinfo.size);
		}
/****************************************************************************
 *
 *	open aero data file
 *
 ***************************************************************************/

	if ((aero=fopen(file_name,"r")) == NULL)
		{
			printf("error opening aero data file\n");
			exit(0);
		}

	line_number = 0;
	error = 0;

	fgets(aerocode, 80, aero);
	line_number++;

	fgets(title12, 80, aero);
	line_number++;

	/*printf("%s\n",title12);*/

	while( fgets(buffer, 255, aero) != NULL) {
		line_number++;			/* increment line number */
		name = strtok(buffer," \t");	/* get name */
		number = strtok('\0'," \t");	/* get number */
		data = atof(number);		/* convert to double floating point */

		/*printf("%s %s %f\n",name,number,data);*/

		check = 0;
		for(i=0;i<NUM_AERO_NAMES;i++) {
		    if(stricmp(name,NAME[i])==0) {
			*DATA[i] = (float)(data);
			check = 1;
			/*printf("%f %f\n",*DATA[i],data);*/
			} /* if */
		    } /* for */
		if(check==0) {				/* check for match */
		    if(strcmp(name,"\n")!=0) {		/* check for blank line */
			printf("%s(%d) : error H0000: name %s for %f data not recognised\n",file_name,line_number,name,data);
			error++;
			    }
			}
		if((name=strtok('\0'," \t")) != NULL) { /* check for extra stuff */
		    if(strcmp(name,";")!=0) {		/* check for comments */
			printf("%s(%d) : error H0001: extra characters ignored\n",file_name,line_number);
			error++;
			    }
			}
		} /* while */

	for(i=0;i<NUM_AERO_NAMES;i++) {
	    printf("%s %f\n",NAME[i],*DATA[i]);
	    }

	if(error != 0)
		exit(0);
 }
