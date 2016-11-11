unsigned char peek_byte(int segment, int offset)
{
	_asm {
		push	es
		mov		ax,word ptr [bp+4]	 ;segment of address
		mov		bx,word ptr [bp+6]	 ;offset
		mov		es,ax
		mov		al,es:byte ptr [bx]
		xor		ah,ah
		pop		es
		}
}

unsigned int peek_word(int segment, int offset)
{
	_asm {
		push	es
		mov		ax,word ptr [bp+4]	 ;segment of address
		mov		bx,word ptr [bp+6]	 ;offset
		mov		es,ax
		mov		ax,es:word ptr [bx]
		pop		es
		}
}
