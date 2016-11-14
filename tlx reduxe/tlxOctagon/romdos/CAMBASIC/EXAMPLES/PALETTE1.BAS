1 'Example 1 of PALETTE statement
10 SCREEN 8
20 DRAW "C5":'Set color to magenta
30 DRAW "F60 L120 E60":'Draw a triangle in magenta
40 DRAW "BD30":'Move down to inside of triangle
50 DRAW "P14,5":'Paint the interior of triangle to yellow
60 PALETTE 14,4:'Map color 14 to 4 (yellow to red)
