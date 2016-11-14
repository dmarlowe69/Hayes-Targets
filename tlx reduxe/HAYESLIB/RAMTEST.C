 int ramtest(unsigned int segment, unsigned int offset_start, unsigned int offset_stop)
 {
 _asm
 {
;****************************************************************************
;
; EDN Design Ideas
; RAM test program prevents crashes
; by C.M.Petersen
; EDN Oct 25,1990 page 204-205
;
;****************************************************************************
;       .MODEL SMALL
;       .CODE
; The following procedure performs a test of memory
; Calling format from C is:
; int ramtest(unsigned int segment, unsigned int offset_start,
;               unsigned int offset_stop);
; Return codes: 0-no error, 1-error occured
; Notes:
;       The first memory location is used to detect addressing problems.
;       The last memory location tested is offset_stop -1.
;       This routine is limited to testing on 64k segment at a time. Use
;       multiple calls to test larger memory arrays.
;       All interrupts must be turned off before running this procedure.
;       offset_stop - offset_start must be greater than 1.
;
; Registers usage:
;       ds      Segment of memory to be tested
;       bx      Current location of test
;       dx      Ending location of test
;       ax      Used to move and compare data
;       ch      Save data under test for later restoration
;       cl      Save first byte (used for detection of addressing errors)
;       di      Starting location of test
;       bp      pointer to stack
;
;       PUBLIC _ramtest
;_ramtest        PROC
		push    di              ; Program begins here
		push    bp              ; Save registers used by module
		push    cx
		push    ds
		push    dx
		mov     bp,sp           ; Get stack pointer
		mov     ax,[bp+12]      ; Get segment off of stack
		mov     ds,ax           ; Set segment register
		mov     ax,[bp+14]      ; get starting offset
		mov     bx,ax           ; save in bx
		mov     ax,[bp+16]      ; get ending offset
		mov     dx,ax           ; save in dx
; Test begins here
		mov     cl,ds:[bx]      ; Save first memory location
		mov     al,5ah          ; use 0x5a in first loc. to id
					; addressing problems
		mov     ds:[bx],al      ; Save in first location
		mov     di,bx           ; Remember location
		inc     bx              ; Bump to first location to test
ramloop:        mov     ch,ds:[bx]      ; Save data at byte to be tested
		mov     al,55h          ; Test pattern in 0x55
		mov     ds:[bx],al      ; Write out to test location
		cmp     ds:[bx],al      ; Same?
		jne     ramerror        ; No, report error and exit
		mov     al,0aah         ; Test pattern is now 0xaa
		mov     ds:[bx],al      ; Write
		cmp     ds:[bx],al      ; Same:
		jne     ramerror
; Now perform a "walking ones" test on this location
		mov     ah,1            ; Start at d0 = 1
walking_ones:   mov     ds:[bx],ah      ; Write test pattern
		cmp     ds:[bx],ah      ; Same?
		jne     ramerror        ; No, report error and exit
		cmp     ah,80h          ; Done?
		je      test_done
		rol     ah,1            ; rotate one to next position
		jmp     walking_ones    ; Loop to continue with test
; Test successfully completed at byte
test_done:      mov     ds:[bx],ch      ; Restore original data byte
		inc     bx              ; Bump pointer to next test byte
		cmp     bx,dx           ; Is this the last location?
		jne     ramloop         ; No, test this byte
; Main test is completed, now go back and verify that the first byte was
; not changed. A changed byte would indicate addressing problems
		cmp     BYTE PTR ds:[di],5ah      ; Has byte changed?
		jne     ramerror1       ; Yes, report error
; Test is finished - no problems
		sub     ax,ax           ; Return code of 0 in ax
		jmp     exit_test       ; Restore registers
ramerror:       mov     ds:[bx],ch      ; Error - Restore test data byte
ramerror1:      mov     ax,1            ; Set return code to 1
exit_test:      mov     ds:[di],cl        ; Restore 1st byte
		pop     dx              ; Restore registers
		pop     ds
		pop     cx
		pop     bp
		pop     di
		ret                     ; return with status
;_ramtest        ENDP                    ; End of procedure
;               END
 }
 }
