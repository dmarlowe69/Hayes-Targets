/* file: tlx_plot.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Hud Support function")
 #pragma subtitle("(c)1989-1992 Hayes Targets")

/****************************************************************************
 *
 *      HUD Support Functions
 *
 *      written by denton marlowe
 *      (c)1989-1992 Hayes Targets
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
 *      MS-DOS Verison 5.0
 *      Microsoft C Version 6.00a
 *      MASM Version 5.1
 *      Datalight C_Thru_ROM Version 1.70c
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

 #include       <dos.h>
 #include       <conio.h>
 #include       <stdio.h>
 #include       <stdlib.h>
 #include       <string.h>

 #include       <math.h>
 #include       <float.h>

 #include       <graph.h>

 #include       <pgchart.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include       <hayes.h>
 #include       "menu.h"
 #include       "talk.h"
 //#include     "packet.h"
 //#include     "8250.h"
 #include       "screen.h"
 #include       "tlx.h"
 #include       "key.h"
 #include       "hud_scan.h"
 #include       "hud.h"
 #include       "hud_plot.h"
 #include       "tlxhud.h"
 #include       "mouse.h"
 #include       "hrtime.h"

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      function prototypes
 *
 ***************************************************************************/

 void hud_data_action(char c);
 void hud_data_help(void);

 void hud_playback_action(char c);
 void hud_playback_help(void);

 void hud_init_tm_data(void);
 int hud_get_tm_data(unsigned long int hrtime);

 void hud_eng_data_tlx(void);

 int hud_load_tm_data(void);

 #pragma page(1)

/****************************************************************************
 *
 *      process keyboard inputs
 *
 ***************************************************************************/
 void hud_data_action(char c)
 {
 unsigned fCursor;
 long bgColor;
 short fgColor;
 struct rccoord rc;
 short left, top, right, bottom;
 char buffer[81];

	fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
	_gettextwindow( &top, &left, &bottom, &right );

	switch(c) {
		case F1_KEY:
					if(capdata.capture_flag == ON) {
						capdata.capture_flag = OFF;
						capdata.data_points = capdata.data_index;
						hud_resume_update();
						}
					else {
						capdata.capture_flag = ON;
						hud_inhibit_update();
						}
					(*helpfunptr)();
					break;

		case F2_KEY:
					if(capdata.capture_flag == OFF) {
						hud_init_tm_data();
						hud_resume_update();
					}
					break;

		case F3_KEY:
					hud_write_plot_file();
					(*helpfunptr)();
					break;

		case F4_KEY:
					hud_plot();
					hud_display();
					(*helpfunptr)();
					break;

		case F5_KEY:
					hud_plot_all();
					hud_display();
					(*helpfunptr)();
					break;

		case F6_KEY:
					if(capdata.capture_flag == OFF) {
						hud_inhibit_update();
						_settextposition(2,2);
						_outtext("PLAY BACK     ");
						capdata.data_index = 0;
						_settextposition(19,2);
						_outtext("                          ");
						_settextposition(19,2);
						sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
						_outtext(buffer);
						actionfunptr = hud_playback_action;
						helpfunptr = hud_playback_help;
						(*helpfunptr)();

						hud_player();

						return;
						}
					else {
						putchar('\a');  /* ring bell */
						}
					break;

		case F7_KEY:
					hud_read_plot_file();
					(*helpfunptr)();
					break;

		case F8_KEY:
					actionfunptr = hud_key_action;
					helpfunptr = hud_help;
					(*helpfunptr)();
					return;
					break;

		default:
					break;
	} /* switch */

	_settextwindow( top, left, bottom, right );
	_settextposition( rc.row, rc.col );
	_settextcolor( fgColor );
	_setbkcolor( bgColor );
	_displaycursor( fCursor );
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Hud Help
 *
 ***************************************************************************/
 void hud_data_help(void)
 {
 unsigned fCursor;
 long bgColor;
 short fgColor;
 struct rccoord rc;
 short left, top, right, bottom;

 char on[] = "ON";
 char off[] = "OFF";
 char buffer[81];

	fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
	_gettextwindow( &top, &left, &bottom, &right );

	_settextwindow(HELP_TOP,HELP_LEFT,HELP_BOTTOM,HELP_RIGHT);
	_clearscreen(_GWINDOW);

	_settextcolor(_TBRIGHTWHITE);

	buffer[0] = '\0';
	strcat(buffer,"F1-Capture ");
	if(capdata.capture_flag == ON)
	strcat(buffer,off);
    else
	strcat(buffer,on);

	_outtext(buffer);
	_outtext("\n");

	_settextcolor(_TBRIGHTWHITE);

	_outtext("F2-RESET CAPTURE\n");

	_settextcolor(_TBRIGHTWHITE);

	_outtext("F3-WRITE DATA\n");

	_settextcolor(_TLIGHTYELLOW);

	_outtext("F4-PLOT DATA");

	_settextwindow(HELP_TOP,HELP_CENTER,HELP_BOTTOM,HELP_RIGHT);

	_settextcolor(_TGREEN);

	_outtext("F5-PLOT ALL DATA\n");

	_settextcolor(_TCYAN);

	_outtext("F6-PLAY BACK\n");

	_settextcolor(_TGREEN);

	_outtext("F7-READ DATA\n");

	_settextcolor(_TRED);

	_outtext("F8-EXIT");

	_settextwindow( top, left, bottom, right );
	_settextposition( rc.row, rc.col );
	_settextcolor( fgColor );
	_setbkcolor( bgColor );
	_displaycursor( fCursor );
 }

 #pragma page(1)

/****************************************************************************
 *
 *      process keyboard inputs
 *
 ***************************************************************************/
 void hud_playback_action(char c)
 {
 unsigned fCursor;
 long bgColor;
 short fgColor;
 struct rccoord rc;
 short left, top, right, bottom;
 char buffer[81];

	fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
	_gettextwindow( &top, &left, &bottom, &right );

	switch(c) {
		case F1_KEY:
					if(capdata.play_back_flag == ON) {
						capdata.play_back_flag = OFF;
						}
					else {
						capdata.play_back_flag = ON;
						if(capdata.data_points == 0) {
							capdata.play_back_flag = OFF;
							_settextposition(18,2);
							_outtext("No data points to playback");
							getch();
							_settextposition(18,2);
							_outtext("                          ");
							capdata.play_back_flag = OFF;
							putchar('\a');  /* ring bell */
							}
						}
					tlx_hud.hud_play_back_flag = capdata.play_back_flag;
					(*helpfunptr)();
					break;

		case F2_KEY:
					if(capdata.free_run_flag == 1) {
						capdata.free_run_flag = 0;
						}
					else {
						capdata.free_run_flag = 1;
						}
					(*helpfunptr)();
					break;

		case F3_KEY:
					break;

		case F4_KEY:
					break;

		case F5_KEY:
					break;

		case F6_KEY:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index = 0;
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;

		case F7_KEY:
					if(capdata.play_back_flag == OFF) {
						hud_read_plot_file();
						(*helpfunptr)();
						}
					else {
						putchar('\a');  /* ring bell */
						}
						break;

		case F8_KEY:
					if(capdata.play_back_flag == OFF) {
						if(capdata.capture_flag == OFF) {
							hud_resume_update();
							}
						_settextposition(2,2);
						_outtext("              ");
						actionfunptr = hud_data_action;
						helpfunptr = hud_data_help;
						(*helpfunptr)();
						tlx_hud.hud_player_exit_flag = 1;
						return;
						}
					else {
						putchar('\a');  /* ring bell */
						}
					break;

		case KEY_HOME:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index = 0;
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;
					break;

		case KEY_END:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index = capdata.data_points;
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;

		case KEY_PGUP:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index += 10;
					if(capdata.data_index > capdata.data_points) {
						capdata.data_index = capdata.data_points;
						}
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;

		case KEY_PGDN:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index -= 10;
					if(capdata.data_index < 0) {
						capdata.data_index = 0;
						}
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;

		case KEY_UP_ARROW:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index += 1;
					if(capdata.data_index > capdata.data_points) {
						capdata.data_index = capdata.data_points;
						}
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;

		case KEY_DOWN_ARROW:
					_settextposition(18,2);
					_outtext("                             ");
					capdata.data_index -= 1;
					if(capdata.data_index < 0) {
						capdata.data_index = 0;
						}
					_settextposition(19,2);
					_outtext("                             ");
					_settextposition(19,2);
					sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
					_outtext(buffer);
					break;

		default:
					break;
	} /* switch */

	_settextwindow( top, left, bottom, right );
	_settextposition( rc.row, rc.col );
	_settextcolor( fgColor );
	_setbkcolor( bgColor );
	_displaycursor( fCursor );
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Hud Help
 *
 ***************************************************************************/
 void hud_playback_help(void)
 {
 unsigned fCursor;
 long bgColor;
 short fgColor;
 struct rccoord rc;
 short left, top, right, bottom;

 char on[] = "ON";
 char off[] = "OFF";
 char buffer[81];

	fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
	_gettextwindow( &top, &left, &bottom, &right );

	_settextwindow(HELP_TOP,HELP_LEFT,HELP_BOTTOM,HELP_RIGHT);
	_clearscreen(_GWINDOW);

	_settextcolor(_TBRIGHTWHITE);

	buffer[0] = '\0';
	strcat(buffer,"F1-PLAY BACK ");
	if(capdata.play_back_flag == ON)
	strcat(buffer,off);
    else
	strcat(buffer,on);

	_outtext(buffer);
	_outtext("\n");

	_settextcolor(_TBRIGHTWHITE);

	buffer[0] = '\0';
	strcat(buffer,"F2-FREE RUN ");
	if(capdata.free_run_flag == ON)
	strcat(buffer,off);
    else
	strcat(buffer,on);

	_outtext(buffer);
	_outtext("\n");

	_settextcolor(_TBRIGHTWHITE);

	_outtext("F3-\n");

	_settextcolor(_TLIGHTYELLOW);

	_outtext("F4-");

	_settextwindow(HELP_TOP,HELP_CENTER,HELP_BOTTOM,HELP_RIGHT);

	_settextcolor(_TGREEN);

	_outtext("F5-\n");

	_settextcolor(_TGREEN);

	_outtext("F6-RESET PLAYER\n");

	_settextcolor(_TGREEN);

	_outtext("F7-READ DATA\n");

	_settextcolor(_TRED);

	_outtext("F8-EXIT");

	_settextwindow( top, left, bottom, right );
	_settextposition( rc.row, rc.col );
	_settextcolor( fgColor );
	_setbkcolor( bgColor );
	_displaycursor( fCursor );
 }

 #pragma page(1)

/****************************************************************************
 *
 *      initialize telemetry data capture
 *
 ***************************************************************************/
 void hud_init_tm_data(void)
 {
 unsigned fCursor;
 long bgColor;
 short fgColor;
 struct rccoord rc;
 short left, top, right, bottom;
 int i,j;
 char buffer[81];

	fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
	_gettextwindow( &top, &left, &bottom, &right );

	engfunptr = hud_eng_data_tlx;

	capdata.data_points = 0;

	capdata.data_index = capdata.data_points;

	capdata.play_back_flag = 0;

	capdata.free_run_flag = 0;

	capdata.num_charts = 9;

	capdata.chart_ch[0] = 1;
	capdata.chart_ch[1] = 2;
	capdata.chart_ch[2] = 3;
	capdata.chart_ch[3] = 4;
	capdata.chart_ch[4] = 5;
	capdata.chart_ch[5] = 6;
	capdata.chart_ch[6] = 7;
	capdata.chart_ch[7] = 8;
	capdata.chart_ch[8] = 9;
	capdata.chart_ch[9] = -1;

	for(j=0;j<SERIES;j++)
		for(i=0;i<NUM_DATA;i++)
			hud_data[j][i]=0;

	_disable();
	timer = 0L;
	_enable();

	_settextwindow(1,1,25,80);

	_settextposition(18,2);
	_outtext("                           ");

	_settextposition(19,2);
	sprintf(buffer,"index % 4d <% 4d @ % 4ld   ",capdata.data_index,NUM_DATA,timer);
	_outtext(buffer);

	_settextwindow( top, left, bottom, right );
	_settextposition( rc.row, rc.col );
	_settextcolor( fgColor );
	_setbkcolor( bgColor );
	_displaycursor( fCursor );
 }

 #pragma page(1)

/****************************************************************************
 *
 *      capture telemetry data
 *
 ***************************************************************************/
 int hud_get_tm_data(unsigned long int hrtime)
 {
 int ch;
 int ch_index;
 char buffer[81];

		if(capdata.capture_flag == ON) {
			if(capdata.data_index < NUM_DATA) {

				for(ch=0;ch<capdata.num_charts;ch++) {
					ch_index = capdata.chart_ch[ch];
					if(ch_index == -1)
						break;
					hud_data[ch][capdata.data_index] = cmd_buffer.data[ch_index];
				}

				if(capdata.data_index == 0) {
					capdata.time_offset = (short)timer;
					capdata.hrtime_offset = hrtime;
				}

				hud_time[capdata.data_index] = (short)(timer);
				hud_hrtime[capdata.data_index] = hrtime;
				capdata.data_index++;

				_settextposition(19,2);
				sprintf(buffer,"index % 4d <% 4d @ % 4ld",capdata.data_index,NUM_DATA,timer);
				_outtext(buffer);
				return(capdata.data_index);
			}
			else {
				capdata.data_points = capdata.data_index;
				_settextposition(18,2);
				_outtext("TM DATA CAPTURE BUFFER FULL");
				capdata.capture_flag = OFF;
				hud_resume_update();
				(*helpfunptr)();
				return(-1);
			}
		}
 }

 #pragma page(1)

/****************************************************************************
 *
 *      scale data to enginerring units
 *
 ***************************************************************************/
 void hud_eng_data_tlx(void)
 {
 FILE *outfile;
 int i;
 float t;
 float hrt;

 int radar;                     // ch 1
 float alt_dev;         // ch 2
 float fs_alt;          // ch 3
 float ex_alt;          // ch 4
 float wing;            // ch 5
 float aux;                     // ch 6
 float accel;           // ch 7
 float batt;            // ch 8
 int rcvy_fly;          // ch 9 bit 0
 int prog;                      // ch 9 bit 1 and 2
 int ok;                        // ch 9 bit 7

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	if(capdata.data_points == 0) {
		_outtext("No data points to output");
		getch();
		return;
		}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	_outtext("Enter ENG output file name: \n");
	_getstring(capdata.filename);

	if ((outfile = fopen(capdata.filename,"w")) == NULL) {
		_outtext("error opening output file\n");
		_outtext("press any key\n");
		getch();
		return;
	}

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	fprintf(outfile,"time,hrtime,rebl,alt_dev,fs_alt,ex_alt,wing,aux,accel,batt,rcvy/fly,prog,ok\n");

	for(i=0;i<capdata.data_points;i++) {

		t = (float)(hud_time[i] - capdata.time_offset)/18.2;
		hrt = (float)(hud_hrtime[i] - capdata.hrtime_offset)/HRTIME_TCKS_PER_SECOND;

		if(hud_data[0][i] > 64)
			radar = 1;
		else
			radar = 0;

		alt_dev = (float)hud_data[1][i] * (100.0/256.0) - 50.0;

		if(hud_data[2][i] < 160) {
			fs_alt = (float)hud_data[2][i] * 500.0/160.0;
			}
		else {
			fs_alt = ((float)(hud_data[2][i]-160) * 2000.0/96.0) + 500.0;
			}

		ex_alt = (float)hud_data[3][i] * (100.0/256.0);

		wing = (float)hud_data[4][i] * (10.0/256.0) - 9.0;

		aux = (float)hud_data[5][i] * (5.0/256.0);

		accel = (float)hud_data[6][i] * (2.0/256.0) + 0.5; // rev 12/24/1993

		batt = (float)hud_data[7][i] * (35.0/256.0);

		rcvy_fly = hud_data[8][i] & 0x01;

		prog = (hud_data[8][i] & 0x06) >> 1;

		ok = (hud_data[8][i] & 0x80) >> 7;

		fprintf(outfile,"%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d\n",\
			t,hrt,radar,alt_dev,fs_alt,ex_alt,wing,aux,accel,batt,rcvy_fly,prog,ok);

		_outtext(".");

		}

/****************************************************************************
 *
 *         close output file
 *
 ***************************************************************************/

	fclose(outfile);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      capture telemetry data
 *
 ***************************************************************************/
 int hud_load_tm_data(void)
 {
 int ch;
 int ch_index;
 char buffer[81];

	//if(capdata.data_index < NUM_DATA) {
	if(capdata.data_index < capdata.data_points) {

		for(ch=0;ch<capdata.num_charts;ch++) {
			ch_index = capdata.chart_ch[ch];
				if(ch_index == -1)
					break;
			cmd_buffer.data[ch_index] = hud_data[ch][capdata.data_index];
			}

		if(capdata.free_run_flag == 1) {
			capdata.data_index++;
			}

		_settextposition(19,2);
		sprintf(buffer,"index % 4d <% 4d",capdata.data_index,NUM_DATA);
		_outtext(buffer);
		for(ch=0;ch<NUM_DATA;ch++); /* delay */
		return(capdata.data_index);
	}
	else {
		_settextposition(18,2);
		_outtext("END OF TM DATA CAPTURE BUFFER");
		capdata.capture_flag = OFF;
		capdata.play_back_flag = 0;
		hud_resume_update();
		(*helpfunptr)();
		return(-1);
			}
 }
