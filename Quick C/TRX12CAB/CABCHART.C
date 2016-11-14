 /* file: cabchart.c */

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
 *		 This Software is dedicated to the memory of
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

 #include	<conio.h>
 #include	<graph.h>
 #include	<string.h>
 #include	<stdio.h>
 #include	<stdlib.h>
 #include	<pgchart.h>
 #include	"cable.h"

/****************************************************************************
 *
 *	Plotting Arrays
 *
 ***************************************************************************/

 float _far x_series[NUM_SEGMENTS];
 float _far y_series[NUM_SEGMENTS];

/****************************************************************************
 *
 *	EGA palette colors
 *
 ***************************************************************************/

 #define _EGA_BLACK		1
 #define _EGA_BLUE		2
 #define _EGA_GREEN		3
 #define _EGA_CYAN		4
 #define _EGA_RED		5
 #define _EGA_MAG		6
 #define _EGA_BROWN		7
 #define _EGA_WHITE		8
 #define _EGA_DRAKGRAY		9
 #define _EGA_LIGHTBLUE 	10
 #define _EGA_LIGHTGREEN	11
 #define _EGA_LIGHTCYAN 	12
 #define _EGA_LIGHTRED		13
 #define _EGA_LIGHTMAG		14
 #define _EGA_YELLOW		15
 #define _EGA_INTENSEWHITE	16

/****************************************************************************
 *
 *	EGA Viewport defintions
 *
 ***************************************************************************/

 #define _EGA_VIEWPORT1_X1	0
 #define _EGA_VIEWPORT1_Y1	0
 #define _EGA_VIEWPORT1_X2	320
 #define _EGA_VIEWPORT1_Y2	175

 #define _EGA_VIEWPORT2_X1	321
 #define _EGA_VIEWPORT2_Y1	0
 #define _EGA_VIEWPORT2_X2	639
 #define _EGA_VIEWPORT2_Y2	175

 #define _EGA_VIEWPORT3_X1	0
 #define _EGA_VIEWPORT3_Y1	176
 #define _EGA_VIEWPORT3_X2	320
 #define _EGA_VIEWPORT3_Y2	349

 #define _EGA_VIEWPORT4_X1	321
 #define _EGA_VIEWPORT4_Y1	176
 #define _EGA_VIEWPORT4_X2	639
 #define _EGA_VIEWPORT4_Y2	349

/****************************************************************************
 *
 *	plot a cable shape
 *
 ***************************************************************************/
 void cable_plot(void)
 {
	int i;
	struct videoconfig vc;
	chartenv env;

	if( !_setvideomode( _MAXRESMODE ) ) /* Find a valid graphics mode */
		exit( 1 );

	_getvideoconfig( &vc );

	_displaycursor( _GCURSOROFF );

	_pg_initchart();			/* Initialize chart system    */

/* Show single-series scatter chart. */
	_pg_defaultchart (&env, _PG_SCATTERCHART, _PG_POINTANDLINE );

	env.chartwindow.bordercolor = _EGA_CYAN;
	env.datawindow.bordercolor = _EGA_CYAN;
	env.xaxis.axiscolor = _EGA_CYAN;
	env.yaxis.axiscolor = _EGA_CYAN;

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	i = 0;
	while( i <= num_segments )
		{
		x_series[i] = (float)(xtg[i]);
		y_series[i] = (float)(ztg[i]);
		i += 1;
		}
	env.chartwindow.x1 = 0;
	env.chartwindow.y1 = 0;
	env.chartwindow.x2 = vc.numxpixels/2;
	env.chartwindow.y2 = vc.numypixels/2;

	env.datawindow.x1 = 0;
	env.datawindow.y1 = 0;
	env.datawindow.x2 = vc.numxpixels/2;
	env.datawindow.y2 = vc.numypixels/2;

	strcpy( env.maintitle.title, "CABLE" );
	strcpy( env.xaxis.axistitle.title, "x" );
	strcpy( env.yaxis.axistitle.title, "z" );

	_pg_chartscatter( &env, x_series, y_series, num_segments );

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	i = 0;
	while( i <= num_segments )
		{
		x_series[i] = (float)(xtg[i]);
		y_series[i] = (float)(ytg[i]);
		i += 1;
		}
	env.chartwindow.x1 = vc.numxpixels/2 + 1;
	env.chartwindow.y1 = 0;
	env.chartwindow.x2 = vc.numxpixels - 1;
	env.chartwindow.y2 = vc.numypixels/2;

	env.datawindow.x1 = vc.numxpixels/2 + 1;
	env.datawindow.y1 = 0;
	env.datawindow.x2 = vc.numxpixels - 1;
	env.datawindow.y2 = vc.numypixels/2;

	strcpy( env.maintitle.title, "CABLE" );
	strcpy( env.xaxis.axistitle.title, "x" );
	strcpy( env.yaxis.axistitle.title, "y" );

	_pg_chartscatter( &env, x_series, y_series, num_segments );

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	i = 0;
	while( i <= num_segments )
		{
		x_series[i] = (float)(ytg[i]);
		y_series[i] = (float)(ztg[i]);
		i += 1;
		}
	env.chartwindow.x1 = 0;
	env.chartwindow.y1 = vc.numypixels/2 + 1;
	env.chartwindow.x2 = vc.numxpixels/2;
	env.chartwindow.y2 = vc.numypixels - 1;

	env.datawindow.x1 = 0;
	env.datawindow.y1 = vc.numypixels/2 + 1;
	env.datawindow.x2 = vc.numxpixels/2;
	env.datawindow.y2 = vc.numypixels - 1;

	strcpy( env.maintitle.title, "CABLE" );
	strcpy( env.xaxis.axistitle.title, "y" );
	strcpy( env.yaxis.axistitle.title, "z" );

	_pg_chartscatter( &env, x_series, y_series, num_segments );

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	i = 0;
	while( i <= num_segments )
		{
		x_series[i] = (float)(xtg[i]);
		y_series[i] = (float)(tension[i]);
		i += 1;
		}
	env.chartwindow.x1 = vc.numxpixels/2 + 1;
	env.chartwindow.y1 = vc.numypixels/2 + 1;
	env.chartwindow.x2 = vc.numxpixels - 1;
	env.chartwindow.y2 = vc.numypixels - 1;

	env.datawindow.x1 = vc.numxpixels/2 + 1;
	env.datawindow.y1 = vc.numypixels/2 + 1;
	env.datawindow.x2 = vc.numxpixels - 1;
	env.datawindow.y2 = vc.numypixels - 1;

	strcpy( env.maintitle.title, "CABLE" );
	strcpy( env.xaxis.axistitle.title, "x" );
	strcpy( env.yaxis.axistitle.title, "tension" );

	_pg_chartscatter( &env, x_series, y_series, num_segments );

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	getch();

	_setvideomode( _DEFAULTMODE );
 }
