#ifdef __TURBOC__
/* Borland defines these with a different label */
#define inp(port) 	inportb(port)
#define inpw(port)	inport(port)
#define outp(port,val)	outportb(port,val)
#define outpw(port,val)	outport(port,val)
#endif
