/* file: tlxcode.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Autopilot application")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *  Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<stdlib.h>
 #include   <dos.h>
 #include   <conio.h>

/****************************************************************************
 *
 *	Include definitions
 *
 ***************************************************************************/

 #include   <hayes.h>
 #include   <gnd.h>

/****************************************************************************
 *
 *	Include Microsoft definitions
 *
 ***************************************************************************/

 #include       <stdio.h>
 #include       <math.h>
 #include       <float.h>


 #if defined(OUTPUT)

	FILE *f_tlx;

 #endif


 #pragma page(1)

/***************************************************************************
 *
 *		TLX autopilot test program
 *
 *      written by denton marlowe
 *      (c)1990 Hayes Targets
 *
 **************************************************************************/
 void tlx_test(void)
 {
 double t;						/* time in seconds */
 float f_rad;					/* user input sin wave frequency */
 float f_hz;                    /* user input sin wave frequency */
 float amplitude;               /* amplitude of sine wave */
 double temp,temp1;             /* temporary variables */
 float step;                    /* user input step magnitude */
 unsigned char option;          /* user input option */
 float run_time;                /* user input for run time in seconds */
 unsigned long int run_count;   /* number of time slices */
 unsigned int plot_count;       /* user input for plot frequency */
 unsigned int p_count;          /* counter for plot frequency */
 unsigned long int i;			/* counter */

/****************************************************************************
 *
 *      get user inputs
 *
 ***************************************************************************/

 #if defined(OUTPUT)

	if ((f_tlx=fopen("tlx.out","w")) == NULL)
		{
			printf("error opening output file\n");
			exit(0);
		}
 #endif


	printf("enter run time :");
    //scanf("%f",&run_time);
	run_count = (long)(run_time/0.0625) + 1;

	printf("enter plot frequency :");
    //scanf("%d",&plot_count);
	p_count = 1;

	printf("enter preset :");
    //scanf("%f",&fpreset);

	printf("enter altitude :");
    //scanf("%f",&falt);

	printf("enter accel :");
    //scanf("%f",&faccel);

/****************************************************************************
 *
 *      write header info to file
 *
 ***************************************************************************/

 #if defined(OUTPUT)

	fprintf(f_tlx,"T\t");
	fprintf(f_tlx,"AVG ALT\t");
	fprintf(f_tlx,"ALT ERR\t");
	fprintf(f_tlx,"ALT RTE\t");
	fprintf(f_tlx,"ACC\t");
	fprintf(f_tlx,"WNG\t");
	fprintf(f_tlx,"WNG");
	fprintf(f_tlx,"\n");

 #endif

/******************************************************************************
 *
 *      start run
 *
 *****************************************************************************/

	tlx_init();

	for(i=0;i<run_count;i++) {

		t = (double)(i)/16.0;

		tlx_pilot();

		p_count--;
		if(p_count == 0) {
			printf("%6.4f\t",t);
			printf("%d\t",average_alt);
			printf("%d\t",alt_error);
			printf("%d\t",height_rate);
			printf("%d\t",integrate_accel);
			printf("%d\t",wing);
			printf("%6.4f\t",dwing);
			printf("\n");

 #if defined(OUTPUT)

			fprintf(f_tlx,"%6.4f\t",t);
			fprintf(f_tlx,"%d\t",average_alt);
			fprintf(f_tlx,"%d\t",alt_error);
			fprintf(f_tlx,"%d\t",height_rate);
			fprintf(f_tlx,"%d\t",integrate_accel);
			fprintf(f_tlx,"%d\t",wing);
			fprintf(f_tlx,"%6.4f\t",dwing);
			fprintf(f_tlx,"\n");

 #endif

			/*alt_stack_out();*/

			/*accel_stack_out();*/

			p_count = plot_count;

			} /* if */

		} /* for */

/****************************************************************************
 *
 *      close output files
 *
 ***************************************************************************/

 #if defined(OUTPUT)

	fclose(f_tlx);

 #endif

 }
