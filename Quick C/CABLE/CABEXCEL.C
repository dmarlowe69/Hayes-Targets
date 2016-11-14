/* file: cabexcel.c */

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

 #include	 "float.h"
 #include        "math.h"
 #include        "stdio.h"
 #include	 "cable.h"

/****************************************************************************
 *
 *      print cable analysis header
 *
 ***************************************************************************/
 void print_legend_excel(FILE *stream)
 {
	if(stream == stdprn )
		fprintf(stream,"\f");

	fprintf(stream,"\n");
	fprintf(stream,"xtg\tytg\tztg");
	fprintf(stream,"\ttx\tty\ttz\ttension");
	fprintf(stream,"\tthetss\tsi\tphi");
	fprintf(stream,"\n\n");

	return;
 }
/****************************************************************************
 *
 *      print cable analysis function
 *
 ***************************************************************************/
 void print_analysis_excel(FILE *stream)
 {
	fprintf(stream,"% 7.2f",xtg[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",ytg[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 7.2f",ztg[i]);
	fprintf(stream,"\t");

	fprintf(stream,"% 8.2f",tx[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 8.2f",ty[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 8.2f",tz[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 8.2f",tension[i]);
	fprintf(stream,"\t");

	fprintf(stream,"% 6.2f",thetsd[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 6.2f",sid[i]);
	fprintf(stream,"\t");
	fprintf(stream,"% 6.2f",phid[i]);

	fprintf(stream,"\n");

	return;
 }
/****************************************************************************
 *
 *      print cable analysis
 *
 ***************************************************************************/
 void print_all_excel(FILE *stream)
 {
/****************************************************************************
 *
 *      printheader
 *
 ***************************************************************************/

	/*print_head(stream);*/
	/*print_aero(stream);*/

/****************************************************************************
 *
 *      print parameters
 *
 ***************************************************************************/

	/*print_para(stream);*/

/****************************************************************************
 *
 *      print trim conditions for the target
 *
 ***************************************************************************/

	/*print_trim(stream);*/

/****************************************************************************
 *
 *      print conditions at the target
 *
 ***************************************************************************/

	i = 0;

	/*print_tgt(stream);*/

/****************************************************************************
 *
 *      cable analysis function
 *
 ***************************************************************************/

	print_legend_excel(stream);

	i = 0;

	while( i <= num_segments )
		{
		print_analysis_excel(stream);
		i += 1;
		}

/****************************************************************************
 *
 *      print cone angle
 *
 ***************************************************************************/

	/*print_cone(stream);*/

/****************************************************************************
 *
 *      return to caller
 *
 ***************************************************************************/

	return;
 }
/****************************************************************************
 *
 *
 *
 ***************************************************************************/
