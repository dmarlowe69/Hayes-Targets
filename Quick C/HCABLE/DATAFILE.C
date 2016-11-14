/* file: datafile.c */

 #include	"hcable.h"

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void read_mono(void)
 {
 FILE *mono;
 char *number;
 char *type;
 char *status;
 static char *string;
 static char file_name[] = { "mono.dat" };
 static char buffer[81];
 double dia,rts,mass,cf,cdc,elongation;
 int index;

	if ((mono=fopen(file_name,"r")) == NULL)
		{
			printf("error opening mono data file\n");
			exit(0);
		}

	index = 0;

	while(1) {
		fgets(buffer, 80, mono);

 #ifdef DEBUG

		printf("%s",buffer);

 #endif

		string = &buffer[0];

		number = strtok(string,",");	/* get number */
		dia = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		rts = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		mass = atof(number);			/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		cf = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		cdc = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		elongation = atof(number);		/* convert to double floating point */
		type = strtok('\0',",");		/* get type	*/
		status = strtok('\0',"\n");		/* get status */

//		sscanf(buffer,"%f,%f,%f,%f,%f,%f,%s",&dia, &rts, &mass, &cf, &cdc, &elongation, &string);
//		type = strtok('string',",");	/* get type	*/
//		status = strtok('\0',"\n");		/* get status */

 #ifdef DEBUG

		printf("dia %f\n",dia);
		printf("rts %f\n",rts);
		printf("mass %f\n",mass);
		printf("cf %f\n",cf);
		printf("cdc %f\n",cdc);
		printf("elongation %f\n",elongation);
		printf("type %s\n",type);
		printf("status %s\n",status);

 #endif

		if(stricmp(type,"END")==0) {
			cable_data[TYPE_MONO].number = index;
			break;
			}
		if(stricmp(status,"ACTIVE")==0) {
			cable_data[TYPE_MONO].diameter[index] = dia;
			cable_data[TYPE_MONO].strength[index] = rts;
			cable_data[TYPE_MONO].weight[index] = mass;
			cable_data[TYPE_MONO].cf[index] = cf;
			cable_data[TYPE_MONO].cdc[index] = cdc;
			cable_data[TYPE_MONO].elongation[index] = elongation;
			cable_data[TYPE_MONO].type[index] = TYPE_MONO;
			index++;

 #ifdef DEBUG

			printf("cable parameters loaded\n");

 #endif

			}

 #ifdef DEBUG

 #ifdef _WINDOWS

		getchar();

 #else

		getch();

 #endif

 #endif

		}

	fclose(mono);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void dump_mono(void)
 {
 int index;

	printf("\nMONO-Filament Cable Parameters\n");

	printf("number of entries %d\n",cable_data[TYPE_MONO].number);

	for(index=0;index<cable_data[TYPE_MONO].number;index++) {
		printf("dia %f\n",cable_data[TYPE_MONO].diameter[index]);
		printf("rts %f\n",cable_data[TYPE_MONO].strength[index]);
		printf("mass %f\n",cable_data[TYPE_MONO].weight[index]);
		printf("cf %f\n",cable_data[TYPE_MONO].cf[index]);
		printf("cdc %f\n",cable_data[TYPE_MONO].cdc[index]);
		printf("elongation %f\n",cable_data[TYPE_MONO].elongation[index]);
		printf("type %d\n",cable_data[TYPE_MONO].type[index]);
		}
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void read_strand(void)
 {
 FILE *strand;
 char *number;
 char *type;
 char *status;
 static char *string;
 static char file_name[] = { "strand.dat" };
 static char buffer[81];
 double dia,rts,mass,cf,cdc,elongation;
 int index;

	if ((strand=fopen(file_name,"r")) == NULL)
		{
			printf("error opening strand data file\n");
			exit(0);
		}

	index = 0;

	while(1) {
		fgets(buffer, 80, strand);

 #ifdef DEBUG

		printf("%s",buffer);

 #endif

		string = &buffer[0];

		number = strtok(string,",");	/* get number */
		dia = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		rts = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		mass = atof(number);			/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		cf = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		cdc = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		elongation = atof(number);		/* convert to double floating point */
		type = strtok('\0',",");		/* get type	*/
		status = strtok('\0',"\n");		/* get status */

//		sscanf(buffer,"%f,%f,%f,%f,%f,%f,%s",&dia, &rts, &mass, &cf, &cdc, &elongation, &string);
//		type = strtok('string',",");	/* get type	*/
//		status = strtok('\0',"\n");		/* get status */

 #ifdef DEBUG

		printf("dia %f\n",dia);
		printf("rts %f\n",rts);
		printf("mass %f\n",mass);
		printf("cf %f\n",cf);
		printf("cdc %f\n",cdc);
		printf("elongation %f\n",elongation);
		printf("type %s\n",type);
		printf("status %s\n",status);

 #endif

		if(stricmp(type,"END")==0) {
			cable_data[TYPE_STRAND].number = index;
			break;
			}
		if(stricmp(status,"ACTIVE")==0) {
			cable_data[TYPE_STRAND].diameter[index] = dia;
			cable_data[TYPE_STRAND].strength[index] = rts;
			cable_data[TYPE_STRAND].weight[index] = mass;
			cable_data[TYPE_STRAND].cf[index] = cf;
			cable_data[TYPE_STRAND].cdc[index] = cdc;
			cable_data[TYPE_STRAND].elongation[index] = elongation;
			cable_data[TYPE_STRAND].type[index] = TYPE_STRAND;
			index++;

 #ifdef DEBUG

			printf("cable parameters loaded\n");

 #endif

			}

 #ifdef DEBUG

		getch();

 #endif

		}

	fclose(strand);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void dump_strand(void)
 {
 int index;

	printf("\nSTRANDed Cable Parameters\n");
	printf("number of entries %d\n",cable_data[TYPE_STRAND].number);
	for(index=0;index<cable_data[TYPE_STRAND].number;index++) {
		printf("dia %f\n",cable_data[TYPE_STRAND].diameter[index]);
		printf("rts %f\n",cable_data[TYPE_STRAND].strength[index]);
		printf("mass %f\n",cable_data[TYPE_STRAND].weight[index]);
		printf("cf %f\n",cable_data[TYPE_STRAND].cf[index]);
		printf("cdc %f\n",cable_data[TYPE_STRAND].cdc[index]);
		printf("elongation %f\n",cable_data[TYPE_STRAND].elongation[index]);
		printf("type %d\n",cable_data[TYPE_STRAND].type[index]);
		}
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int getmono(double diameter)
 {
 int status;

	status = getcable(diameter,TYPE_MONO);

	return(status);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int getstrand(double diameter)
 {
 int status;

	status = getcable(diameter,TYPE_STRAND);

	return(status);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int getcable(double diameter, int type)
 {
 int index;
 int flag;
 double temp;

	flag = 0;
	for(index=0;index<cable_data[type].number;index++) {
		temp = diameter - cable_data[type].diameter[index];
		temp = fabs(temp);
		if(temp < 0.0001) {
		//if(diameter == cable_data[type].diameter[index]) {
			flag = 1;
			break;
			}
		}

	if(flag == 0) {
		printf("CABLE DIA %f NOT ON FILE\n",diameter);
		printf("type :%d\n",type);
		cable_look.diameter	  = 999.0;
		cable_look.strength	  = 999.0;
		cable_look.weight	  = 999.0;
		cable_look.cf		  = 999.0;
		cable_look.cdc		  = 999.0;
		cable_look.elongation = 999.0;
		cable_look.type		  = 999;
		return(-1);
	}

	cable_look.diameter	  = cable_data[type].diameter[index];
	cable_look.strength	  = cable_data[type].strength[index];
	cable_look.weight	  = cable_data[type].weight[index];
	cable_look.cf		  = cable_data[type].cf[index];
	cable_look.cdc		  = cable_data[type].cdc[index];
	cable_look.elongation = cable_data[type].elongation[index];
	cable_look.type		  = cable_data[type].type[index];
	return(0);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void lookupmono(double diameter, double sf, double tension)
 {
	lookupcable(diameter,sf,tension,TYPE_MONO);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void lookupstrand(double diameter, double sf, double tension)
 {
	lookupcable(diameter,sf,tension,TYPE_STRAND);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void lookupcable(double diameter, double sf, double tension, int type)
 {
 double strenght_required;
 int index;
 int flag;

	strenght_required = sf * tension;

	flag = 0;
	for(index=0;index<cable_data[type].number;index++) {
		if( (cable_data[type].diameter[index]+0.0005) >= diameter ) {
			if( cable_data[type].strength[index] >= strenght_required ) {
				flag = 1;
				break;
				}
			}
		}

	if(flag == 0) {
		printf("CABLE CAPABILITY EXCEEDED\n");
		cable_look.diameter	  = 999.0;
		cable_look.strength	  = 999.0;
		cable_look.weight	  = 999.0;
		cable_look.cf		  = 999.0;
		cable_look.cdc		  = 999.0;
		cable_look.elongation = 999.0;
		cable_look.type		  = 999;
		return;
		}

	cable_look.diameter	  = cable_data[type].diameter[index];
	cable_look.strength	  = cable_data[type].strength[index];
	cable_look.weight	  = cable_data[type].weight[index];
	cable_look.cf		  = cable_data[type].cf[index];
	cable_look.cdc		  = cable_data[type].cdc[index];
	cable_look.elongation = cable_data[type].elongation[index];
	cable_look.type		  = cable_data[type].type[index];
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void load_cable_data(int type,int index)
 {
	cable_data[type].diameter[index]   = cable_look.diameter;
	cable_data[type].strength[index]   = cable_look.strength;
	cable_data[type].weight[index]	   = cable_look.weight;
	cable_data[type].cf[index]		   = cable_look.cf;
	cable_data[type].cdc[index]		   = cable_look.cdc;
	cable_data[type].elongation[index] = cable_look.elongation;
	cable_data[type].type[index]	   = cable_look.type;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void dump_look(void)
 {
	printf("diameter %f\n",cable_look.diameter);
	printf("strength %f\n",cable_look.strength);
	printf("weight %f\n",cable_look.weight);
	printf("cf %f\n",cable_look.cf);
	printf("cdc %f\n",cable_look.cdc);
	printf("elongation %f\n",cable_look.elongation);
	printf("type %d\n",cable_look.type);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void read_data(void)
 {
 FILE *target;
 int index;
 char *number;
 static char *string;
 static char file_name[] = { "target.cab" };
 static char buffer[256];
 char *project;
 char *design;
 char *airframe;
 double mass;
 double cd0;
 double g;
 double s0;
 double l;
 int j1;
 double seglength;
 double d1;
 double d2;
 char *opt;
 double dia;
 double length;
 char *type;

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	printf("Enter input file name: ");
	scanf("%s",file_name);
	strcat(file_name,".cab");

	if ((target=fopen(file_name,"r")) == NULL)
		{
			printf("error opening target data file %s\n",file_name);
			exit(0);
		}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	fgets(buffer, 256, target);

 #ifdef DEBUG

	printf("%s",buffer);

 #endif

	string = buffer;

	project = strtok(string,",");	/* get number */
	strcpy(design_data.project,project);

	design = strtok('\0',",");		/* get status */
	strcpy(design_data.design,design);

	airframe = strtok('\0',",");		/* get status */
	strcpy(design_data.airframe,airframe);

	number = strtok('\0',",");		/* get number */
	mass = atof(number);			/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	cd0 = atof(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	g = atof(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	s0 = atof(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	l = atof(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	j1 = atoi(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	seglength = atof(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	d1 = atof(number);				/* convert to double floating point */
	number = strtok('\0',",");		/* get number */
	d2 = atof(number);				/* convert to double floating point */

	design_data.mass = mass;
	design_data.cd0 = cd0;
	design_data.g = g;
	design_data.s0 = s0;
	design_data.l = l;
	design_data.j1 = j1;
	design_data.seglength = seglength;
	design_data.d1 = d1;
	design_data.d2 = d2;

	fgets(buffer, 256, target);

 #ifdef DEBUG

	printf("%s",buffer);

 #endif

	string = buffer;
	opt = strtok(string,"\n");	/* get number */
	strcpy(design_data.opt,opt);

	index = 0;

	while(1) {

		fgets(buffer, 256, target);

 #ifdef DEBUG

		printf("%s",buffer);

 #endif

		string = buffer;

		number = strtok(string,",");	/* get number */
		dia = atof(number);				/* convert to double floating point */
		number = strtok('\0',",");		/* get number */
		length = atof(number);			/* convert to double floating point */
		type = strtok('\0',"\n");		/* get type	*/

		design_data.diameter[index] = dia;
		design_data.length[index] = length;

		if(stricmp(type,"MONO")==0) {
			design_data.type[index] = TYPE_MONO;
			}

		if(stricmp(type,"1X7")==0) {
			design_data.type[index] = TYPE_STRAND;
			}

 #ifdef DEBUG

		printf("dia %f\n",dia);
		printf("length %f\n",length);
		printf("type %s\n",type);

 #ifdef _WINDOWS

		getchar();

 #else

		getch();

 #endif

 #endif

		if(dia == 999.0)
			break;

		index++;

		}

	design_data.index = index;

	design_data.total_length = 0.0;
	for(index=0;index<design_data.index;index++) {
		design_data.total_length += design_data.length[index];
		}
	design_data.length_run[0] = design_data.length[0];
	for(index=1;index<design_data.index;index++) {
		design_data.length_run[index] = design_data.length_run[index-1] + design_data.length[index];
		}

	fclose(target);

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void dump_data(void)
 {
 int index;

	printf("\nCable design Parameters\n");

	printf("project %s\n",design_data.project);
	printf("design %s\n",design_data.design);
	printf("airframe %s\n",design_data.airframe);

	printf("mass %f\n",design_data.mass);
	printf("cd0 %f\n",design_data.cd0);
	printf("g %f\n",design_data.g);
	printf("s0 %f\n",design_data.s0);
	printf("l %f\n",design_data.l);
	printf("j1 %f\n",design_data.j1);
	printf("seglength %f\n",design_data.seglength);
	printf("d1 %f\n",design_data.d1);
	printf("d2 %f\n",design_data.d2);

	printf("opt %s\n",design_data.opt);

	printf("number of entries %d\n",design_data.index);
	for(index=0;index<design_data.index;index++) {
		printf("diameter %f\n",design_data.diameter[index]);
		printf("length %f\n",design_data.length[index]);
		printf("length_run %f\n",design_data.length_run[index]);
		printf("type %d\n",design_data.type[index]);
		}
	printf("total length %f\n",design_data.total_length);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void load_design_data(void)
 {
 int index;

	index = 0;
	cable_data[TYPE_INPUT].diameter[index] = design_data.diameter[index];
	cable_data[TYPE_INPUT].length[index] = design_data.length[index];
	cable_data[TYPE_INPUT].strength[index] = 0.0;
	cable_data[TYPE_INPUT].weight[index] = 0.0;
	cable_data[TYPE_INPUT].cf[index] = 0.0;
	cable_data[TYPE_INPUT].cdc[index] = 0.0;
	cable_data[TYPE_INPUT].elongation[index] = 0.0;
	cable_data[TYPE_INPUT].type[index] = design_data.type[index];

	for(index=1;index<design_data.index;index++) {
		cable_data[TYPE_INPUT].diameter[index] = design_data.diameter[index];
		cable_data[TYPE_INPUT].length[index] = design_data.length[index];
		cable_data[TYPE_INPUT].strength[index] = 0.0;
		cable_data[TYPE_INPUT].weight[index] = 0.0;
		cable_data[TYPE_INPUT].cf[index] = 0.0;
		cable_data[TYPE_INPUT].cdc[index] = 0.0;
		cable_data[TYPE_INPUT].elongation[index] = 0.0;
		cable_data[TYPE_INPUT].type[index] = design_data.type[index];

		cable_data[TYPE_INPUT].length[index] += cable_data[TYPE_INPUT].length[index-1];
		}

	cable_data[TYPE_INPUT].number = design_data.index;

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void write_data(void)
 {
 FILE *target;
 int index;
 static char file_name[] = { "output.cab" };
 static char buffer[81];

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	printf("Enter output file name: ");
	scanf("%s",file_name);
	strcat(file_name,".cab");

	if ((target=fopen(file_name,"w")) == NULL)
		{
			printf("error opening target data file %s\n",file_name);
			exit(0);
		}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	fprintf(target,"%s,%s,%s,",design_data.project \
		,design_data.design,design_data.airframe);

	fprintf(target,"%f,%f,%f,%f,%f,%f,%f,%f,%f\n",design_data.mass \
		,design_data.cd0,design_data.g,design_data.s0 \
		,design_data.l,design_data.j1,design_data.seglength \
		,design_data.d1,design_data.d2);

	fprintf(target,"%s\n",design_data.opt);

	for(index=0;index<design_data.index;index++) {
		fprintf(target,"%f,",design_data.diameter[index]);
		fprintf(target,"%f,",design_data.length[index]);
		switch(design_data.type[index]) {
			case TYPE_MONO:
				fprintf(target,"MONO\n");
				break;
			case TYPE_STRAND:
				fprintf(target,"1X7\n");
				break;
			}
		}

	fprintf(target,"999,999,999\n");

	fclose(target);
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void input_design(void)
 {
 char buffer[81];
 char *user;

	printf("PROJECT NAME :");
	scanf("%s",buffer);
	user = strtok(buffer,"\n");
	strcpy(design_data.project,user);

	while(1) {
		printf("LTC-2 , LTC-6 , MTR-101 OR TLX :");
		scanf("%s",buffer);
		user = strtok(buffer,"\n");
		strcpy(design_data.design,user);

		if(_stricmp(user,"TLX") == 0) {
			in_target();
			in_cable();
			break;
			}

		if(_stricmp(user,"LTC-6") == 0) {
			in_target();
			in_cable();
			break;
			}

		if(_stricmp(user,"LTC-2") == 0) {
			in_target();
			in_cable();
			break;
			}

		if(_stricmp(user,"MTR-101") == 0) {
			in_target();
			in_cable();
			break;
			}
		}
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void in_target(void)
 {
 char buffer[81];
 char *user;
 float a,b,c;

	printf("\nTLX-1 OR TLX-IR OR TLX-FUSE OR TLX-19 or TLX-29 :");
	scanf("%s",buffer);
	user = strtok(buffer,"\n");
	strcpy(design_data.airframe,user);

	printf("\nWEIGHT (LBS), CD (DRAG COEFFICIENT) ,G LOADING :");
	scanf("%f,%f,%f",&a,&b,&c);
	design_data.mass = (double)a;
	design_data.cd0 = (double)b;
	design_data.g= (double)c;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void in_cable(void)
 {
 float a,b,c;

	printf("\nTARGET SPOOL VOLUME (CUBIC INCHES) OR DIAMETER (IN) :");
	scanf("%f",&a);
	design_data.s0 = (double)a;
	if(design_data.s0 == 9.0)
		design_data.s0 = 268.0;

	if((design_data.s0 == 7.0) | (design_data.s0 == 7.125))
		design_data.s0	= 171.0;

	printf("\nREQUIRED CABLE LENGHT (FEET) , AIRSPEED (KIAS) , SAFETY FACTOR :");
	scanf("%f,%f,%f",&a,&b,&c);
	design_data.l = (double)a;
	design_data.airspeed = (double)b;
	design_data.sf= (double)c;

	printf("\nCABLE ANALYSIS STEP LENGTH ( 1 - 25 FT) :");
	scanf("%f",&a);
	design_data.seglength = (double)a;
	if((design_data.seglength <= 0.0) | (design_data.seglength > 25.0))
		design_data.seglength = 10.0;

	while(1) {
		printf("\nSTRANDED LEADER CABLE DIAMETER (INCHES) (ZERO IF NONE) ");
		scanf("%f",&a);
		design_data.d1 = (double)a;
		if(design_data.d1 == 0.0)
			break;

		if((getstrand(design_data.d1) == 0))
			break;

		}

	if(design_data.d1 != 0.0) {
		printf("\nCUT OFF ANGLE (DEGREES) OR LEADER LENGTH (FEET): ");
			scanf("%f",&a);
			design_data.leader_length =(double)a;
			if((design_data.leader_length > 100.0) | (design_data.leader_length > 0.0))
				design_data.cut_off_angle = 0.0;
			else
				design_data.cut_off_angle = design_data.leader_length / RAD_TO_DEG;
		}

	while(1) {
		printf("\nSTARTING MONOFILAMENT DIAMETER (INCHES) (ZERO IF NONE) ");
		scanf("%f",&a);
		design_data.d2 = (double)a;
		if(design_data.d2 == 0.0)
			break;

		if((getmono(design_data.d2) == 0))
			break;

		}

 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 int scope_it(void)
 {
 int index;
 //float scope;
 double scope;
 int flag;
 char string[81];

	printf("CABLE DESIGN LENGTH %f\n",design_data.total_length);

	printf("ENTER CABLE FLY LENGTH :");
	fflush( stdin );
	gets( string );
	if(strlen(string) == 0) {
		scope = design_data.total_length;
	}
	else {
		scope = atof(string);
	}
	printf("Cable Scope %f\n",scope);

//	scanf("%f",&scope);
//	if(scope == 0.0) {
//		scope = design_data.total_length;
//	}

	index = 0;
	flag = 0;
	while(1) {
		if(design_data.length_run[index] >= scope) {
			design_data.l = scope;
			design_data.total_length = scope;
			design_data.length_run[index] = scope;
			design_data.length[index] = design_data.length_run[index] - design_data.length_run[index-1];
			design_data.index = index + 1;
			flag = 0;
			break;
			}
		else {
			index = index + 1;
			if(index > design_data.index ) {
				printf("\nCABLE FLY LENGTH TOO LONG\n");
				flag = -1;
				break;
				}
			}
		}

	return(flag);
 }
