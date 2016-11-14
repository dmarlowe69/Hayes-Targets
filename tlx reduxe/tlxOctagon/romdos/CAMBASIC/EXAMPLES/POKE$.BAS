1 'Example of Poke$ statement
10 POKE$ &0,"ABCD",&6000:'Write a string to &6000:0000
20 PRINT PEEK$(&0,&6000):'Return a string from memory at &6000:0000
