/* file: platform.c */

 #include   <hayes.h>
 #include	<gnd.h>

 #include "pilot.h"
 #include "utility.h"
 #include "ground.h"

 #pragma message("PLATFORM.C")

 #pragma page(1)

/***************************************************************************
 *
 * application hooks
 *
 ***************************************************************************/

 int autopilot_run_flag = 0;
 int autopilot_debug_flag = 1;

 void init_brain_loop(void);
 void brain_loop(void);
 void init_brain(void);
 void far brain(void);

 int main_loop_watch_dog(unsigned int *main_loop_count);

 unsigned int brain_loop_count=0;
 int brain_loop_run   = 0;
 int brain_loop_crash = 0;

 #pragma page(1)

/****************************************************************************
 *
 *	main loop initialization
 *
 ***************************************************************************/
 void init_brain_loop(void)
 {
    brain_loop_run   = 0;
    brain_loop_count = 0;
	brain_loop_crash = 0;

    init_systems();

    autopilot_init();                       /* initialize autopilot */
 }

 #pragma page(1)

/****************************************************************************
 *
 *	main loop
 *
 ***************************************************************************/
 void brain_loop(void)
 {
    brain_loop_run = 1;
    brain_loop_count++;

    cmd_processor();                        /* command processor */

    if(autopilot_run_flag == 1) {
        autopilot_run_flag = 0;
		autopilot();
        }
 }

 #pragma page(1)

/****************************************************************************
 *
 *      initialize timer hardware
 *
 ***************************************************************************/
 void init_brain(void)
 {
    init_timer(SAMPLE_RATE);                /* initialize timer */
	install_application(brain);             /* activate autopilot */
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Hardware Sample Rate Function
 *
 *      written by denton marlowe
 *      (c)1991-1993 Hayes Targets
 *
 ***************************************************************************/
 void far brain(void)
 {
 static int brain_count = 0;

    autopilot_run_flag = 1;

    if(tcu_table.ticks > 100) {
		tcu_table.ticks = 0;
		toggle_LED(0,0);

		if(brain_count==0) {
			brain_count = 1;
            }
		else {
			brain_count = 0;
            }

    	if(brain_loop_run==1) {
        	if(main_loop_watch_dog(&brain_loop_count)!=0) {
            	if(brain_loop_crash == 0) {
                	com_puts("Brain Crash");
                	brain_loop_crash = 1;
                    }
                }
            }

        }
 }

 #define MAIN_LOOP_WATCH_COUNT      5U

 #pragma page(1)

/****************************************************************************
 *
 *  Main Loop Watch Dog
 *
 *  written by denton marlowe
 *  (c)1994 Hayes Targets
 *
 ***************************************************************************/
 int main_loop_watch_dog(unsigned int *main_loop_count)
 {
 static unsigned int main_loop_status = 0;

    if (*main_loop_count < 100) {     /* check command counter */
        main_loop_status += 1;        /* increment command check counter */

        if (main_loop_status > MAIN_LOOP_WATCH_COUNT) {
            main_loop_status = MAIN_LOOP_WATCH_COUNT;
            return(1);                      /* return ERR */
			}
        return(0);                          /* return OK */
        }

    *main_loop_count = 0;                   /* reset command counter to zero */
    main_loop_status = 0;                   /* reset to zero */
    return(0);                              /* return OK */
 }
