1 'Example 3 of COLOR statement
10 SCREEN 8
20 COLOR 1, 2:'Forground color blue, background color green
30 CIRCLE (25,25),10: 'Blue circle on green background 
40 DELAY 1:'Delay for effect
50 COLOR ,3:'Forground color unchanged, background cyan
60 DELAY 1:'Delay for effect
70 CIRCLE (50,50),25:'Blue circle on cyan
80 DELAY 1:'Delay for effect
90 COLOR 4:'Forground color red, background unchanged
100 CIRCLE (100,100),50:'Red circle on cyan backgrond
110 DELAY 1:'Delay for effect
120 COLOR ,0:'Forground unchanged, background blue
