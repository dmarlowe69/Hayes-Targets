/* file: tlx_plot.h */

/****************************************************************************
 *
 *	hud_plot function header file
 *
 *	written by denton marlowe
 *	(c)1989-1992 Hayes Targets
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
 *	function prototypes
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
