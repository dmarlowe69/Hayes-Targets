/* file: talk.c */

 #pragma linesize(132)
 #pragma pagesize(63)
 #pragma title("Main function")
 #pragma subtitle("(c)1990 Hayes Targets")

/****************************************************************************
 *
 *      BIRD BRAIN SERIAL COMMUNICATIONS
 *
 *      written by Denton Marlowe
 *      (c)1991 HAYES TARGETS
 *
 ***************************************************************************/

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

 #include	<stdio.h>
 #include	<stdlib.h>
 #include	<conio.h>

 #include	<signal.h>
 #include	<dos.h>
 #include	<bios.h>
 #include	<process.h>
 #include   <string.h>
 #include   <search.h>

 #include   <graph.h>

/****************************************************************************
 *
 *      Include Hayes definitions
 *
 ***************************************************************************/

 #include	<hayes.h>

 #include	"screen.h"

 #include   "menu.h"
 #include   "user.h"
 #include	"mouse.h"
 #include	"hud_scan.h"

 #ifdef FONTS

 #include	"fonts.h"

 #endif

 #ifdef COMM

 //#include	"8250.h"
 //#include	"packet.h"

 #pragma message("comm version")

 #endif

/****************************************************************************
 *
 *      end of include files
 *
 ***************************************************************************/

 #pragma page(1)

/****************************************************************************
 *
 *      Defintions
 *
 ***************************************************************************/

 #define TRUE  1
 #define FALSE 0
 #define ERR  -1
 #define OK    0

 /*#define Baud 38400U*/
 #define Baud 19200
 /*#define Baud 9600*/

 #define Port 1

 #define COLOR_ON	1
 #define COLOR_OFF  0

/****************************************************************************
 *
 *  Golbals
 *
 ***************************************************************************/

 int serial_port;						/* serial port */

 int debug_hud; 						/* debug flag */

 int color_flag;						/* color display flag */

 int talk_text_rows;					/* interrupt timer */

 long int timer=0L; 					/* interrupt timer */

 void (interrupt far *old_handler)();   /* pointer to interrupt handler */

/****************************************************************************
 *
 *      Prototypes
 *
 ***************************************************************************/

 void comm_quit(void);
 void monitor(void);
 void monitor_2(void);

 void set_timer_int(void);
 void reset_timer_int(void);
 void interrupt far new_handler(void);  /* interrupt service routine */

 int process_command_line(int argc, char *argv[], char *envp[] );
 int match(char *s1, char *s2, char min);
 int process_envp(char envp_table[][81], ITEM_ENVPFUNC *envpfunc );
 int init_menu_up(void);
 void reset_comm_port(void);

 #pragma page(1)

/****************************************************************************
 *
 *      BIRD BRAIN SERIAL COMMUNICATIONS
 *
 *      written by Denton Marlowe
 *      (c)1990 HAYES TARGETS
 *
 ***************************************************************************/
 void main( int argc, char *argv[], char *envp[] )
 {
 char buffer[80];
 int i;
 char c,key,ex_key;

/****************************************************************************
 *
 *      initialize variables
 *
 ***************************************************************************/

	//serial_port = 0;				 /* set serial port to comm 1*/

	serial_port = 1;				/* set serial port to comm 1*/

	debug_hud = 0;					/* debugging flag off */

	color_flag = COLOR_ON;  /* color flag on */

/****************************************************************************
 *
 *  print greetings
 *
 ***************************************************************************/

    dos_welcome();

/****************************************************************************
 *
 *  read command line arguments
 *
 ***************************************************************************/

    if( process_command_line( argc, argv, envp ) == -1) {
	    printf("error in command line\n");
	    exit(1);
	}

	if(debug_hud !=0) {
		printf("\npress any key\n");
		getch();
		}

 #ifdef ALTERNATE_CMD_LINE

    if(argc > 1) {
	for(i=0; i<argc; i++) {
	    if( (argv[i][0] == '/') || (argv[i][0] == '-') ) {
		switch(argv[i][1]) {
		    case '?':   read_help();
				exit(0);
				break;

		} /* switch */

	    } /* if */

	} /* for */

    } /* if */

 #endif

/****************************************************************************
 *
 *  check environment variables
 *
 ***************************************************************************/

    process_envp(envp_table, &envpfunc );

	if(debug_hud !=0) {
		printf("\npress any key\n");
		getch();
		}

/****************************************************************************
 *
 *      setup terminal display
 *
 *  Select best graphics mode, adjust menus, set color flag. Note
 *  that this requires checking both the adapter and the mode.
 *
 ***************************************************************************/

    _getvideoconfig( &vc );

    switch( vc.adapter )
    {
	case _CGA:
			if( ( talk_text_rows = _settextrows(25) ) != 25) {
		puts("Text rows required not available. \n");
		exit(1);
	    }
	    break;

	case _EGA:
			if( ( talk_text_rows = _settextrows(43) ) != 43) {
		puts("Text rows required not available. \n");
		exit(1);
	    }
	    break;

	case _VGA:
			if( ( talk_text_rows = _settextrows(50) ) != 50) {
		puts("Text rows required not available. \n");
		exit(1);
	    }
	    break;

	case _SVGA:
			if( ( talk_text_rows = _settextrows(50) ) != 50) {
		puts("Text rows required not available. \n");
		exit(1);
	    }
	    break;

	default:
	    puts( "No graphics mode available.\n" );
	    exit(1);
    }

    if( _setvideomode(_TEXTC80)==0 ) {
		printf("Can't set _TEXTC80 mode");
		exit(1);
		}

	_setactivepage(1);
    _setbkcolor((long)_TBLACK);
	_clearscreen(_GCLEARSCREEN);

    if(vc.adapter == _CGA) {
	_setactivepage(2);
	_setbkcolor((long)_TBLACK);
	_clearscreen(_GCLEARSCREEN);
    }

/****************************************************************************
 *
 *	initialize fonts
 *
 ***************************************************************************/

 #ifdef FONTS

	init_font();

 #endif

/****************************************************************************
 *
 *      setup terminal display
 *
 ***************************************************************************/

	main_display();

/****************************************************************************
 *
 *      setup mouse
 *
 ***************************************************************************/

    if(mouse_reset()==MOUSE_NOT_FOUND) {
	_outtext("No mouse - press any key\n");
	getch();
    }

    if(mouse_get_crt_page() != 1) {
		mouse_set_crt_page(1);
    }

    _setactivepage(1);
    _setvisualpage(1);

	set_graphics_cursor_block();

	mouse_show_cursor();

	mouse_box( 1, 1, talk_text_rows, 80 );

	mouse_show_cursor();

/****************************************************************************
 *
 *      open serial port
 *
 ***************************************************************************/

	c = 0x00;
	key = 0x00;
	buffer[1]='\0';

 #ifdef COMM

	comm_open(Baud,0,serial_port);  /* open the serial port */

	comm_flush();

 #endif

/****************************************************************************
 *
 *      setup interrupts
 *
 ***************************************************************************/

    set_timer_int();

/****************************************************************************
 *
 *      establish comunications
 *
 ***************************************************************************/

 #ifdef COMM

	comm_putc(0x1b);                        /* output ESC */

	i = 0;

	while(comm_check()==ERR) {
	    i++;
	    if(i>32000) {
			_outtext("serial time out\n");
			_outtext("Remote not responding\n");
			getch();
	    break;
		}
	}

	if(comm_getc() != '?') {                /* check response */
		_outtext("Remote not responding correctly\n");
		getch();
	}

 #endif

/****************************************************************************
 *
 *      user initilaiztion
 *
 ***************************************************************************/

	user_init();

/****************************************************************************
 *
 *      options menu
 *
 ***************************************************************************/

	init_menu_up();

	main_display();

/****************************************************************************
 *
 *
 *
 ***************************************************************************/

	while(TRUE) {

 #ifdef COMM
		while(comm_check()!=ERR) {
			buffer[0]=comm_getc();
			_outtext(buffer);
			}
 #endif

/****************************************************************************
 *
 *	check mouse state
 *
 ***************************************************************************/

//	if( mouse_get_button_press(LEFT_BUTTON) != 0 )
//		if( mouse.vertical < 8 ) {
//				work_main_mouse();
//				}

/****************************************************************************
 *
 *	check for user input keyboard/mouse
 *
 ***************************************************************************/

 //#define MENU_OLD

 #ifndef MENU_OLD

		WorkALTKey();

 #endif

 #ifdef MENU_OLD

		if(kbhit()) {
			key = getch();
			if(key == 0x00) {
				ex_key = getch();
				check_main_menu_hotkey(ex_key);
				}
			else
				{
 #ifdef COMM
				comm_putc(key);
 #else
				buffer[0]=key;
				_outtext(buffer);

				if(key == '\r')
					_outtext("\n");
 #endif
				} /* else */

			} /* outer if */
 #endif

		} /* while */
 }

 #pragma page(1)

/****************************************************************************
 *
 *      comm_quit function ( MS-DOS kernel only )
 *
 ***************************************************************************/
 void comm_quit(void)
 {
 #ifdef COMM

	comm_close();

 #endif

	off_display();

 #ifdef FONTS

	exit_font();

 #endif

	reset_timer_int();

	exit(0);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      Output comm port incomming to screen
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void monitor(void)
 {
 char buffer[81];

	buffer[1]='\0';

 #ifdef COMM

	while(comm_check()!=ERR) {
		buffer[0]=comm_getc();
		_outtext(buffer);
		}
 #endif

 }

 #pragma page(1)

/****************************************************************************
 *
 *      Output comm port incomming to screen
 *
 *      written by denton marlowe
 *      (c)1991 Hayes Targets
 *
 ***************************************************************************/
 void monitor_2(void)
 {
 char buffer[81];
 char c;

	buffer[1]='\0';

 #ifdef COMM

	while(comm_check()!=ERR) {
		c = comm_getc();
		buffer[0]=c;
		if(c!=0x0a)
			_outtext(buffer);
		if(c==0x0d)
			_outtext("\n");
			}
 #endif

 }

 #pragma page(1)

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void set_timer_int(void)
 {
 union REGS inregs, outregs;

    old_handler = _dos_getvect(0x08);

    _disable();

    _dos_setvect(0x08,new_handler);

    _enable();
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void reset_timer_int(void)
 {
 union REGS inregs, outregs;

    _disable();

    _dos_setvect(0x08,old_handler);

    _enable();
 }

 #pragma page(1)

/****************************************************************************
 *
 *
 *
 ***************************************************************************/
 void interrupt far new_handler(void)
 {
	timer++;

	_chain_intr(old_handler);
 }

 #pragma page(1)

/*****************************************************************************
 *
 *  process command line arguments
 *
 ****************************************************************************/
 int process_command_line(int argc, char *argv[], char *envp[] )
 {
 int i;
 int j;
 void (*cmdfunptr)(char *argv);                         /* function pointer */

     flag1 = 0;
/*
     if (argc==1) {
	  printf("Format is: TALK [options] filesname\n");
	  printf("  options:\n");
	  printf("     /?     - Breif Help.\n");
	  printf("     /HELP  - Extended Help.\n");
	  printf("     /COM=x - Set commport.\n");
	  return(-1);
	  }
*/
     for (i=1; i<argc; i++) {
	  for (j=0; optable[j].optname; j++) {
	       if ( match(argv[i],optable[j].optname,optable[j].min) ) {

		    cmdfunptr = optable[j].funptrs;
		    if(cmdfunptr == NULL) {
			printf("error in talk.c : NULL FUNCTION POINTER IN CMD CALL\n");
			return(-1);
		    }
					(*cmdfunptr)(argv[i]);

		    if ( optable[j].onoff == ON )
			 *optable[j].flag |= optable[j].bit;
			 else *optable[j].flag &= ~optable[j].bit;
		    break;
		    }
	       }
	  if ( optable[j].optname == NULL ) {
	       printf("Invalid parameter: '%s'.\n",argv[i]);
	       return(-1);
	       }
	  }
 #ifdef FILE_OPT

     file1 = fopen(argv[1],"r");
     if (file1==0) {
	  printf("Unable to open file '%s'.\n",argv[1]);
	  return(-1);
	  }
     file2 = fopen(argv[2],"r");
     if (file2==0) {
	  printf("Unable to open file '%s'.\n",argv[2]);
	  return(-1);
	  }
 #endif

    return(0);
 }

 #pragma page(1)

/*****************************************************************************
 *
 *  match strings with specified minimum
 *
 ****************************************************************************/
 int match(char *s1, char *s2, char min)
 {
 unsigned int i;

	for (i=0; i<min; i++) {
		if (toupper(*s1++) != *s2++)
			return(0);
	}
	return(i>=min);
 /*
     for (i=0; *s1 && *s2; i++) if (toupper(*s1++) != *s2++) return(0);
     if (*s1==0) return(i>=min);
	 return(0);
 */
 }

 #pragma page(1)

/*****************************************************************************
 *
 *  process environment
 *
 ****************************************************************************/
 int process_envp(char envp_table[][81], ITEM_ENVPFUNC *envpfunc )
 {
 unsigned int i;                            /* counter */
 char *envp_var;                            /* pointer to variable found */
 void (*envpfunptr)(char *envp_var);        /* function pointer */

    for(i=0; (char *)envp_table[i][0] != NULL; i++) {
	if( ( envp_var = getenv(envp_table[i] ) ) != NULL) {
	    envpfunptr = envpfunc->funptrs[i];
	    if(envpfunptr == NULL) {
		printf("error in talk.c : NULL FUNCTION POINTER IN ENVP CALL\n");
		return(-1);
	    }
	    (*envpfunptr)(envp_var);         /* call function */
	}
    }

    return(0);
 }

 #pragma page(1)

/****************************************************************************
 *
 *  invoke initialization Menu
 *
 ***************************************************************************/
 int init_menu_up(void)
 {
 int menu_item;

    _settextwindow(1,1,25,80);

    mnuAtrib = set_subAtrib();
    menu_item = Menu( itemlocHayes.row, itemlocHayes.col, itemHayes, 0);
    if(menu_item < 0) {
	_outtext("ERROR IN MENU CALL RETURN\n");
	return(-1);
    }

    if(menu_item < MAXFUNC) {
	funptr = itemfuncHayes.funptrs[menu_item];
	if(funptr == NULL) {
	    _outtext("NULL FUNCTION POINTER IN MENU CALL\n");
	    return(-1);
	}
	(*funptr)();        /* call function */
	menu_item = ENTER;
    }

    return(menu_item);
 }

 #pragma page(1)

/****************************************************************************
 *
 *      reset comm port
 *
 ***************************************************************************/
 void reset_comm_port(void)
 {
 #ifdef COMM

	comm_close();

	comm_open(Baud,0,serial_port);  /* open the serial port */

	comm_flush();

 #endif
 }
