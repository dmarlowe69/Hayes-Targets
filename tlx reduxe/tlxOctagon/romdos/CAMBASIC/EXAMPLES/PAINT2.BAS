1 'Example 2 of PAINT statement
10 SCREEN 8
20 DRAW "C5":'Set color to magenta
30 DRAW "F60 L120 E60":'Draw a triangle in magenta
40 DRAW "BD30":'Move down to inside of triangle
50 TEXTURE$ = CHR$(&55) + CHR$(&55) + CHR$(&55) + CHR$(&55)  
60 TEXTURE$ = TEXTURE$ + TEXTURE$:'Create texture pattern
70 PAINT STEP (+0, +5), TEXTURE$, 5:'Move down 5 pixels, paint the triangle with texture
