1 'Example of CLEAR DISPLAY, CONFIG DISPLAY and DISPLAY statements
10 CONFIG PIO &100,0,0,0,0,0:'Configure an 8255 at &100 to be output ports
20 CONFIG DISPLAY &100,0,0:'Use 2x20 flourescent display with a hidden cursor at I/O address &100
30 CLEAR DISPLAY
40 DISPLAY (2,0)"OCTAGON SYSTEMS";
50 DISPLAY (2,1)"CORPORATION";
