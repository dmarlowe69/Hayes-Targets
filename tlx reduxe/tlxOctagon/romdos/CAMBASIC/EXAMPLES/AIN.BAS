1 'Example program using CONFIG AIN statement and AIN function
3 '
4 '
5 '
10 CONFIG AIN 0,0: 'Config A/D channel 0 to 0 to 5V input range
20 CONFIG AIN 1,1: 'Config A/D channel 1 to 0 to 10V input range
30 CONFIG AIN 2,2: 'Config A/D channel 2 to -5 to 5V input range
40 CONFIG AIN 3,3: 'Config A/D channel 3 to -10 to 10V input range
50 CONFIG AIN 4,0: 'Config A/D channel 4 to 0 to 5V input range
60 CONFIG AIN 5,0: 'Config A/D channel 5 to 0 to 5V input range
70 CONFIG AIN 6,0: 'Config A/D channel 6 to 0 to 5V input range
80 CONFIG AIN 7,0: 'Config A/D channel 7 to 0 to 5V input range
100 FOR X=0 TO 5
110 C(X) = AIN(0): 'Assign analog input readings from channel 0 to array C
120 NEXT X

