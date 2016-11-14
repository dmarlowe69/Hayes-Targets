1 'Example of PR USING, PRINT USING statement
40 A = 1.2345: PRINT USING "##.##";A
50 'Screen should show 1.23
60 B = 1.2345: PRINT USING ".##";B
70 'Screen should show %1.23;  The '%' sign indicates that digit(s) present outside the specified field
80 C = 1234.67: PRINT USING "#.#^^^^";C
90 'Screen should show 0.1E+04;  The 1st # specifies the leading 0s to the left of the decimal point
100 D = 1234.67: PRINT USING "##.##^^^^";D
110 'Screen should show 1.23E+03; leading 0s to the left of the decimal point are suppressed
120 E = 25.555: PRINT USING "##.##";E
130 'Screen should show 25.56; note that true rounding took place to the second decimal point.
