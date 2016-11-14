// file: ground.c
//
// #define _WATCH_DOG
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

 #include	<hayes.h>
 #include   <gnd.h>

 #include   "pilot.h"
 #include   "cmd_tm.h"
 #include   "utility.h"

/****************************************************************************
 *
 *	end of include files
 *
 ***************************************************************************/

 void reset_boot(void);								/* cold boot */
 void reset_kernel(void);

 void (interrupt far *funptr_reset)(void);			/* pointer to function */

 void _power_on_reset(void);
 void _kernel_reset(void);

/****************************************************************************
 *
 *	Ground function prototypes
 *
 ***************************************************************************/

 void comm_port(void);                  /* test comm ports for access */
 void ground_menu(void);

 #pragma page(1)

/***************************************************************************
 *
 *	terminal break in
 *
 *  written by denton marlowe
 *  (c)1990 Hayes Targets
 *
 **************************************************************************/

 #pragma page(1)

 void comm_port(void)
 {
    if(com_break()==ERR) {

 #ifdef _WATCH_DOG
 #pragma message("!!! WATCH-DOG !!! INSTALL JUMPER J12 TO ENABLE !!!")

    	deinit_the_dog();

 #endif
        autopilot_keys();

 #ifdef _WATCH_DOG
 #pragma message("!!! WATCH-DOG !!! INSTALL JUMPER J12 TO ENABLE !!!")

    init_the_dog();
    kick_the_dog();

 #endif
        }

 }

 #pragma page(1)

/****************************************************************************
 *
 *  serial port command processor
 *
 *  written by denton marlowe
 *  (c)1990 Hayes Targets
 *
 ***************************************************************************/
 void ground_menu(void)
 {
 #ifndef _EPROM
 #pragma message("run time debugging included")

	int c;

	while(TRUE)
	{
		com_puts("\r\n\r\nHAYES TARGETS BIRD BRAIN Version 2.00\r\n");

        com_puts("1. Terminal Loopback mode\r\n");
        com_puts("2. Utility Functions Menu\r\n");
        com_puts("3. Analog Functions Menu\r\n");
        com_puts("4. CMD/TM Functions Menu\r\n");
        com_puts("5. \r\n");
        com_puts("6. Communications Debugger \r\n");
        com_puts("7. \r\n");
        com_puts("8. Reset Menu\r\n");
        com_puts("9. exit\r\n");
        com_puts("\r\n    Make Selection ");

		c = com_getc();        /* get a character from the serial port */

        switch(toupper(c))
        {
            case '1':
                	term();         /* Run the terminal loopback mode */
                    break;

            case '2':
                    utility_menu(); /* functions menu */
                    break;

            case '3':
                    analog_menu();
					break;

            case '4':
                    cmd_tm_menu();
					break;

            case '5':
                    break;

            case '6':
                    decoder_application();
                    break;

            case '7':
                    break;

            case '8':
                    reset_menu();
                    break;

            case '9':
                    return;

            default:
					break;
		}
	}

 #endif
 }
 #pragma page(1)

/****************************************************************************
 *
 *      COLD RESET
 *
 *      DELCARATION:    void reset_boot(void)
 *
 *      PARAMETERS:     None
 *
 *      RETURNS:        Nothing
 *
 *      ENTRY
 *       Conditions:    None
 *
 *      EXIT
 *       Conditions:    CPU reset
 *
 *      LOCALS:         None
 *
 *      EXTERNALS
 *       Referenced:   Memory Location ffff:0000
 *
 *      HEADERS:        None
 *
 *      WARNINGS:       None
 *
 *      SOURCE:         hardware.c
 *
 *      DESCRIPTION:
 *
 *      This function resets the processor thru its powerup cold boot
 *      vector. This functions does not return as the processor, once
 *      reset, will return to the original cold start up code.
 *
 *      written by denton marlowe
 *		(c)1990-1993 Hayes Targets
 *
 ***************************************************************************/

 #pragma page(1)

 void reset_boot(void)
 {
    scu_close();
    int_disable(TCT0_IRQ);  /* disable specific interrupt */
    int_disable(AD_IRQ);    /* disable specific interrupt */
    int_disable(TM_IRQ);    /* disable specific interrupt */
    int_disable(CMD_IRQ);   /* disable specific interrupt */

	_disable(); 					/* disable interrutps at CPU

	outp(ICU_BASE+1,0xff);			/* mask all interruputs (disable) */

    int_disable_all();            /* disable interrutps at ICU */

    delay(32000);
    delay(32000);
    delay(32000);
	delay(32000);

    funptr_reset = (void (interrupt far *)(void)) 0xffff0000;
									/* install pointer to application */
    (*funptr_reset)();              /* call application code */
 }

 #pragma page(1)

 void reset_kernel(void)
 {
	_disable(); 					/* disable interrutps at CPU

	outp(ICU_BASE+1,0xff);			/* mask all interruputs (disable) */

    funptr_reset = (void (interrupt far *)(void)) 0xfffe0000;
                                    /* install pointer to application */

	(*funptr_reset)();				/* call application code */
 }

 void _power_on_reset(void)
 {

    _asm {

        ;set long jump

        mov     ax, 0002h   ; flags - 0x0002
        push    ax
        mov     ax, 0ffffh  ; cs    - 0xffff
        push    ax
        mov     ax, 0000h   ; ip    - 0x0000
        push    ax

        iret                ; return to 0xffff:0000 with flags set 0x0002

        }
 }

 void _kernel_reset(void)
 {

    _asm {

        ;set long jump

        mov     ax, 0002h   ; flags - 0x0002
        push    ax
        mov     ax, 0fffeh  ; cs    - 0xfffe
        push    ax
        mov     ax, 0000h   ; ip    - 0x0000
        push    ax

        iret                ; return to 0xfffe:0000 with flags set 0x0002

        }
 }
