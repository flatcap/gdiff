CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui` `glib-config --libs gthread`
EXTRA	= -lefence

richard:	richard.o derived.o
	$(CC) $(LIBS) derived.o richard.o -o richard

richard.o:	richard.c
	$(CC) $(CFLAGS) richard.c -o richard.o

derived.o:	derived.c derived.h
	$(CC) $(CFLAGS) derived.c -o derived.o

main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) main.o node.o tree.o global.o -o main

main.o:	main.c node.h global.h tree.h
	$(CC) $(CFLAGS) main.c -o main.o

g:	gnome.o
	$(CC) $(LIBS) gnome.o -o g

gnome.o:	gnome.c gnome.h
	$(CC) $(CFLAGS) gnome.c -o gnome.o

global.o:	global.c global.h
	$(CC) $(CFLAGS) global.c -o global.o

tree.o:	tree.c tree.h
	$(CC) $(CFLAGS) tree.c -o tree.o

node.o:	node.c node.h
	$(CC) $(CFLAGS) node.c -o node.o

testgtk.o:	testgtk.c
	$(CC) $(CFLAGS) testgtk.c -o testgtk.o

testgtk:	testgtk.o
	$(CC) $(LIBS) testgtk.o -o testgtk

clean:
	rm -f *.o main g testgtk richard
