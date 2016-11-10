

*=======================================================*
*							*
*     This do nothing program is the simplest provided	*
*   to demonstrate the simplest use of the assembler,	*
*   linker, and debugger programs.			*
*							*
*=======================================================*

*---------	Functions Defined	     -----------*
;		Main		only program

*/////////	       PROCEDURES	     \\\\\\\\\\\*

		Section C,?
MAIN
	 NOP
Main_1	 NOP			;Do forever
	 NOP
	 NOP
Main_2	 NOP
	 NOP
Main_3	 NOP
	 NOP
Main_4	 NOP
	 NOP
Main_5	 NOP
	 NOP
Main_X	 LBR  Main_1

	 db  32 DUP (0c4h)		;more nops

	 END
