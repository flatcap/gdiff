CC	= g++
CFLAGS	= `gnome-config --cflags gnomeui` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui`

main:	main.o node.o
	$(CC) $(LIBS) main.o node.o -o main

main.o:	main.c node.h
	$(CC) $(CFLAGS) main.c -o main.o

node.o:	node.c
	$(CC) $(CFLAGS) node.c -o node.o

clean:
	rm -f *.o main
