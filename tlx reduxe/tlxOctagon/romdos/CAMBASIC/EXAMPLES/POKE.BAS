1 'Example of POKE statement
10 POKE &0,&55,&6000:'Write a byte to &6000:0000
20 PRINT HEX$(PEEK(&0,&6000)):'Print a byte content at &6000:0000
