1 'Example of DPEEK function and DPOKE statement
10 DPOKE &0,&55AA,&6000:'Write a word at 6000:0000
20 PRINT HEX$(DPEEK(&0,&6000)):'Get 1 word at 6000:0000 and print the hex representation
