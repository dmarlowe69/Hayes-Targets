;******************************************************************************
;
;   peek.asm - C interfaced procedure to examine memory
;
;      usage: _peek(int segment, int offset)
;
;******************************************************************************
;
;******************************************************************************
;
;       This Software was Designed using the following development tools:
;
;       MS-DOS Verison 3.3
;       Microsoft C Version 5.1
;       MASM Version 5.1
;       Datalight C_Thru_ROM Version 5.1
;
;               This Software is dedicated to the memory of
;
;                       NYSSA ANN MARLOWE
;                   17 OCT 1988 - 03 NOV 1988
;
;******************************************************************************
;
;*@H***************************************************************************
;
;    Name:   ... _new_funct
;
;    Entry:  ...
;
;    Exit:   ...
;
;    Notes:  ...
;
;    Desc:   ...
;
;***************************************************************************@H*
;
;*@R***************************************************************************
;
;    Created:
;
;    Last update: ..
;
;***************************************************************************@R*

        include        mmacros.inc
               
CbegCode

c_func          _peek

                push     bp
                mov      bp,sp
                push     es
                mov      ax,word ptr [bp+4]   ;segment of address
                mov      bx,word ptr [bp+6]   ;offset
                mov      es,ax
                mov      al,es:byte ptr [bx]
                xor      ah,ah
                pop      es
                pop      bp
                ret

c_endp          _peek

CEndCode

;******************************************************************************

              end

