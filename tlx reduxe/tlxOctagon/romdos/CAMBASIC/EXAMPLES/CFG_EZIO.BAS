1 'Example program using CONFIG EZIO statement
3 ' The following example configures port B with an initial value of all 1's
4 ' and the lower 4 bits of port C of the Octagon EZ I/O at base address
5 ' &140 as outputs. The example also configures port A and the upper 4 bits
6 ' of port C as inputs.  All the bits of the output ports are set as 1's.
7'
10 CONFIG EZIO &140,&FF,0,0,&FF,&F0,&0F


