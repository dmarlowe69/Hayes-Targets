; The _cflgcvt_tab is used to map a floating point conversion 
; to a call to fptrap when Float point is not loaded.
;

include mmacros.inc

c_extrnP	_fptrap

CBegData

	PUBLIC	__cfltcvt_tab
__cfltcvt_tab	LABEL	 WORD

if	largeCODE
	DD	5 dup (_fptrap)
else
	DW	5 dup (_fptrap)
endif


CEndData
	end
