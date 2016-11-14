/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	<graph.h>
 #include	<math.h>
 #include	<stdlib.h>
 #include	<stdio.h>
 #include	<conio.h>

/****************************************************************************
 *
 *      Include Microsoft definitions
 *
 ***************************************************************************/

 #include	"menu.h"
 #include	"mouse.h"
 #include	"cable.h"
 #include	"editor.h"

 #include	"swap.h"

/****************************************************************************
 *
 *	Array and enum for main menu
 *
 ***************************************************************************/

 ITEM mnuMain[] =
 {							     /* Highlight Char	Pos */
    { 0, "0-Quit"						   },	/* 0	 0  */
    { 0, "1-trim analysis"					   },	/* 1	 0  */
    { 0, "2-tow reel parameters"				   },	/* 2	 0  */
    { 0, "3-max cone angle"					   },	/* 3	 0  */
    { 0, "4-complete analysis"					   },	/* 4	 0  */
    { 0, "5-trim analysis with user inputs for alpha and beta"	   },	/* 5	 0  */
    { 0, "6-trim analysis with user inputs for canard positions"   },	/* 6	 0  */
    { 0, "7-alpha and beta sweep"				   },	/* 7	 0  */
	{ 0, "8-canard sweep"					},	/* 8	 0	*/
	{ 0, "9-actuator accuracy"				},	/* 9	 0	*/
	{ 0, "A-envelop sweep"					},	/* A	 0	*/
	{ 0, "B-Minimum Drag"					},	/* B	 0	*/
	{ 0, "C-print menu" 					},	/* C	 0	*/
	{ 0, "D-exit"							},	/* D	 0	*/
	{ 0, "F-Initial Cable Loads"			},	/* E	 0	*/
	{ 0, ""							   }
 };

/****************************************************************************
 *
 *	Array and enum for print menu
 *
 ***************************************************************************/

 ITEM mnuPrint[] =
 {							     /* Highlight Char	Pos */
	{ 0, "0-Quit"							   },	/* 0	 0	*/
	{ 0, "1-print cable.out"				   },	/* 1	 0	*/
    { 0, "2-print cone.out"					   },	/* 2	 0  */
    { 0, "3-print reel.out"					   },	/* 3	 0  */
    { 0, "4-print trim.out"					   },	/* 4	 0  */
	{ 0, "5-set print options"				   },	/* 5	 0	*/
    { 0, "6-DOS Shell"						   },	/* 6	 0  */
	{ 0, "7-Editor"							   },	/* 6	 0	*/
	{ 0, ""									   }
 };

/****************************************************************************
 * Arrays of video mode menu items and of corresponding mode numbers.
 * Each has a temporary array containing all items, and a pointer version
 * including all except Olivetti.
 ***************************************************************************/

 ITEM mnuModesT[] =
 {		      /* Highlight Char  Pos */
    { 0, "ORESCOLOR "   },      /* O     0  */
    { 4, "MRES4COLOR "  },      /* 4     4  */
    { 4, "MRESNOCOLOR"  },      /* N     4  */
    { 4, "HRESBW"       },      /* B     4  */
    { 0, "MRES16COLOR"  },      /* M     0  */
    { 0, "HRES16COLOR"  },      /* H     0  */
    { 0, "ERESCOLOR"    },      /* E     0  */
    { 4, "VRES2COLOR"   },      /* 2     4  */
    { 0, "VRES16COLOR"  },      /* V     0  */
    { 1, "MRES256COLOR" },      /* R     4  */
    { 0, ""             }
 };
 ITEM *mnuModes = &mnuModesT[1];  /* Default is no Olivetti mode */

 int aModesT[] =
 {
    _MRES4COLOR,
    _MRES4COLOR,
    _MRESNOCOLOR,
    _HRESBW,
    _MRES16COLOR,
    _HRES16COLOR,
    _ERESCOLOR,
    _VRES2COLOR,
    _VRES16COLOR,
    _MRES256COLOR,
    _TEXTMONO,
    _ERESNOCOLOR,
    _HERCMONO
 };

 int *aModes = &aModesT[1];		 /* Default is no Olivetti mode */

/****************************************************************************
 *
 *	display colors
 *
 ***************************************************************************/

 #define CHAR_BLACK 0x00
 #define CHAR_BLUE 0x01
 #define CHAR_GREEN 0x02
 #define CHAR_CYAN 0x03
 #define CHAR_RED 0x04
 #define CHAR_PURPLE 0x05
 #define CHAR_BROWN 0x06
 #define CHAR_GRAY 0x07
 #define CHAR_DARK_GRAY 0x08
 #define CHAR_BRIGHT_BLUE 0x09
 #define CHAR_BRIGHT_GREEN 0x0a
 #define CHAR_BRIGHT_CYAN 0x0b
 #define CHAR_BRIGHT_RED 0x0c
 #define CHAR_BRIGHT_PURPLE 0x0d
 #define CHAR_BRIGHT_YELLOW 0x0e
 #define CHAR_WHITE 0x0f
 #define CHAR_BACK_BLACK 0x00
 #define CHAR_BACK_BLUE 0x10
 #define CHAR_BACK_GREEN 0x20
 #define CHAR_BACK_CYAN 0x30
 #define CHAR_BACK_RED 0x40
 #define CHAR_BACK_PURPLE 0x50
 #define CHAR_BACK_BROWN 0x60
 #define CHAR_BACK_GRAY 0x70

/****************************************************************************
 *
 *	Global video configuration
 *
 ***************************************************************************/

 struct videoconfig vc;

/****************************************************************************
 *
 *	Main Menu
 *
 ***************************************************************************/
 int msc(void)
 {
    int rowMid, colMid;
    int fColor, fFirstTime = TRUE;
    int iMode, iMainCur = 0, iModesCur = 0;

    _displaycursor( _GCURSOROFF );
    _getvideoconfig( &vc );
    rowMid = vc.numtextrows / 2;
    colMid = vc.numtextcols / 2;

/****************************************************************************
 *
 * Select best graphics mode, adjust menus, set color flag. Note
 * that this requires checking both the adapter and the mode.
 *
 ***************************************************************************/

	switch( vc.adapter )
	{
        case _CGA:
            mnuModesT[4].achItem[0] = '\0';     /* Turn off EGA modes    */
            iMode = _MRES4COLOR;
            break;
        case _HGC:
	    mnuModesT[7].achItem[0] = '\0';     /* Turn off VGA modes    */
            iMode = _HERCMONO;
            break;
        case _EGA:
            mnuModesT[7].achItem[0] = '\0';     /* Turn off VGA modes    */
            if( vc.memory > 64 )
                iMode = _ERESCOLOR;
            else
                iMode = _HRES16COLOR;
            break;
        case _VGA:
            iMode = _VRES16COLOR;
            break;
        case _SVGA:
            iMode = _VRES16COLOR;
            break;
        case _MCGA:
            iMode = _MRES256COLOR;
            break;
        case _MDPA:
        default:
            puts( "No graphics mode available.\n" );
            iMode = _VRES16COLOR;
            ;return TRUE;
    }

    switch( vc.mode )
    {
        case _TEXTBW80:
        case _TEXTBW40:
            fColor = FALSE;
            break;
        case _TEXTMONO:
        case _ERESNOCOLOR:
        case _HERCMONO:
            fColor = FALSE;
            if( iMode != _HERCMONO )
                iMode = _ERESNOCOLOR;
            mnuMain[8].achItem[0] = '\0';       /* Turn off mode change */
            break;
        default:
            fColor = TRUE;
            break;
    }
/****************************************************************************
 *
 *	Find current mode in mode array
 *
 ***************************************************************************/

    for( iModesCur = 0; aModes[iModesCur] != iMode; iModesCur++ );

/****************************************************************************
 *
 *	Set text mode and clear the screen to cyan
 *
 ***************************************************************************/

	_setvideomode(_DEFAULTMODE );
        if( fColor )
	    _setbkcolor( (long)_TCYAN );
        _clearscreen( _GCLEARSCREEN );

/****************************************************************************
 *
 *	Main display
 *
 ***************************************************************************/

	mousecursor(1);		/* turn mouse cursor on */

	_settextcolor(CHAR_BLACK);
	_settextposition(2,24);
	_outtext("3-DIMENSIONAL CABLE ANALYSIS");
	_settextposition(3,24);
	_outtext(" written by Denton Marlowe");
	_settextposition(4,24);
	_outtext("   (c)1990 HAYES TARGETS");

/****************************************************************************
 *
 *	Select from menu
 *
 ***************************************************************************/

	iMainCur = Menu( rowMid, colMid, mnuMain, iMainCur );

	/*_setvideomode( _DEFAULTMODE );*/

	if( fColor ) {
		_setbkcolor( (long)_TBLACK );
		_settextcolor(CHAR_CYAN);
		}

	_clearscreen( _GCLEARSCREEN );

	 return(iMainCur);
 }
/****************************************************************************
 *
 *	Print Menu
 *
 ***************************************************************************/
 void msc_print(void)
 {
    int rowMid, colMid;
    int fColor, fFirstTime = TRUE;
    int iMode, iMainCur = 0, iModesCur = 0;

	_getvideoconfig( &vc );
	rowMid = vc.numtextrows / 2;
	colMid = vc.numtextcols / 2;

	iMainCur = Menu( rowMid, colMid, mnuPrint, iMainCur );

	_clearscreen( _GCLEARSCREEN );

	switch(iMainCur)
	{
	case 01:{
		print_cable_out();
		break;
		}
	case 02:{
		print_cone_out();
		break;
		}
	case 03:{
		print_reel_out();
		break;
		}
	case 04:{
		print_trim_out();
		break;
		}
	case 05:{
		print_option();
		break;
		}
	case 06:{
		//system("command.com");
		shell();
		break;
		}
	case 07:{
		_setvideomode( _DEFAULTMODE );
		_clearscreen( _GCLEARSCREEN );
		edit("edit.dat");
		_setvideomode( _DEFAULTMODE );
		_clearscreen( _GCLEARSCREEN );
		break;
		}
	}

	_clearscreen( _GCLEARSCREEN );
 }
