1 'Example of DISPLAY USING statement
10 CONFIG PIO &100,0,0,0,0,0
20 CONFIG DISPLAY &100,2,0:'Use 4x20 LCD display with a hidden cursor at I/O address &100
30 CLEAR DISPLAY
40 A = 1.2345:DISPLAY USING (0,0) "##.##";A
50 'Display should show 1.23
60 B = 1.2345:DISPLAY USING (6,0) ".##";B
70 'Display should show %1.23;  The '%' sign indicates that digit(s) present outside the specified field
80 C = 1234.67:DISPLAY USING (12,0) "#.#^^^^";C
90 'Display should show 0.1E+04;  The 1st # specifies the leading 0s to the left of the decimal point
100 D = 1234.67:DISPLAY USING (0,1) "##.##^^^^";D
110 'Display should show 1.23E+03;  leading 0s to the left of the decimal point are suppressed
120 E = 25.555: DISPLAY USING (12,1) "##.##";E
130 'Display should show 25.56; note that true rounding took place to the second decimal point.
