graph: Dmenu.o uimenu.o fegraph.o begraph.o
	gcc -Wall -ggdb -o graph Dmenu.o uimenu.o fegraph.o begraph.o
Dmenu.o: Dmenu.c
	gcc -c -ggdb -Wall Dmenu.c
uimenu.o: uimenu.c uimenu.h
	gcc -c -ggdb -Wall uimenu.c
fegraph.o: fegraph.c fegraph.h
	gcc -c -ggdb -Wall fegraph.c
begraph.o: begraph.c begraph.h
	gcc -c -ggdb -Wall begraph.c

clean:
	rm *.o
	
