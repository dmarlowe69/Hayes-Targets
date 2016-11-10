*=======================================================*
*							*
*	      A SHORT DEMO PROGRAM, DEMO.S		*
*							*
*     This program takes a block of 256 bytes from	*
*     block # 1  and places them into block # 2.	*
*							*
*=======================================================*



*--------	    VARIABLES			   ----------*

	 Section  Var,?,DATA

VarBegin

block_1
	    INCLUDE ASCIITBL
block_1_end

	    db	       16 dup (0)

BLOCK_SIZE  EQU   block_1_end - Block_1

block_2     ds		BLOCK_SIZE
block_2_end

	    ds	       256
STACK				       ; Top of stack

VarEnd				       ; Mark end of variable
				       ; section for reference.


*<<<<<<<<<<<	       PROCEDURES		      >>>>>>>>>>>*

	 SECTION   Prog,?,CODE	       ; Make the labels relocatable.
ProgBegin
	  NOP


*-----------		 MAIN  PROGRAM		   -----------*
;   The register are allocated as follows:

PC	    EQU        R0	 ; program counter.
PCirq	    EQU        R1	 ; reserved for interrupts.
SPptr	    EQU        R2	 ; stack pointer.
from_ptr    EQU        R3	 ; data source pointer.
to_ptr	    EQU        R4	 ; data destination pointer.
count	    EQU        R5	 ; loop counter.

Start
	  NOP
	  LDI  HIGH(STACK)	 ; Init stack.
	  PHI  SPptr
	  LDI  LOW(STACK)
	  PLO  SPptr

Start_1   LDI  HIGH(block_2)	 ; Init destination pointer.
	  PHI  to_ptr
	  LDI  LOW(block_2)
	  PLO  to_ptr

	  LDI  HIGH(ASCII)	 ; Init source pointer.
	  PHI  from_ptr
	  LDI  LOW(ASCII)
	  PLO  from_ptr

	  LDI  HIGH(BLOCK_SIZE)  ; Init counter.
	  PHI  count
	  LDI  LOW(ASCII)
	  PLO  count


Start_2   LDN  from_ptr 	 ; Move byte from source
	  STR  to_ptr		 ;  to destination
	  INC  from_ptr 	 ;  and advance pointers.
	  INC  to_ptr

	  DEC  count		 ; If count <> 0,
	  GLO  count		 ;
	  LBNZ Start_2		 ;   Then move next byte,

TAG	  NOP			 ;   Else block has been moved.
Start_X   LBR  Start_1		 ;   Let's do it again.



	  NOP
	  NOP

ProgEnd 			   ; Mark beginning of patch space.
	  db   32 dup ($C4)	   ; Erase patching area.

	  END
