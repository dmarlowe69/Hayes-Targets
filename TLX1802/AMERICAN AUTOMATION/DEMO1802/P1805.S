
;			A JOURNAL FILE FOR PATCHES
;		  (It starts out empty, like this.)
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;
;	  (PAGE DOWN to review preparation of the P.S journal file.)
;

*=======================================================*
*							*
*      A PATCH FOR THE DEMO.S PROGRAM			*
*							*
*  DEMO.S program reverses the bytes when transferring	*
*  information from block to block.  This patch makes	*
*  the program NOT reverse the bytes.			*
*							*
*=======================================================*

	   INCLUDE LABELS	   ; Bring in labels from DEMO.S
				   ; for use in patching.

*   The register are allocated as follows:
from_ptr    EQU        R3	   ; data source pointer.
to_ptr	    EQU        R4	   ; data destination pointer.
count	    EQU        R5	   ; loop counter.

       SECTION Pch,Start_1,CODE    ; Origin of the first
				   ; instruction to change.

	    LDI  HIGH(block_2_end) ; Point at the end of
	    PHI  to_ptr 	   ; the block, not the beginning.
	    LDI  LOW(block_2_end)
	    PLO  to_ptr


	    ORG  Start_2+2	   ; Position patch call.

PATCH	    LBR      FIX	   ; Jump to the patch routine.   [STEP 1]
				   ; Realign to instruction boundary.
Patch_X


* Place this patch into the patch space after the file

	    ORG       ProgEnd	   ; The patch is placed at	  [STEP 2]
	    NOP 		   ; the end of the program and
	   CORG       $10	   ; aligned to the next nibble FRAME.



FIX	    INC  from_ptr	   ; Replace snuffed instruction. [STEP 4]
	    DEC  to_ptr 	   ; Now use decreasing pointer.  [STEP 3]
	    DEC  count
FixEnd	    LBR  Patch_X	   ; Return to point of insertion.[STEP 5]


*------------		   END OF PATCH 	    -------------------*


	    db	 32 dup ($C4)	    ; Erase patching area.

ProgEnd2			    ; Put a label here in case
	  END			    ; we want to make another patch.


* NOTE: Patch is fully documented and may be edited into DEMO.S *

