CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui` `glib-config --libs gthread`
EXTRA	= -lefence

richard:	richard.o derived.o menu.o
	$(CC) $(LIBS) derived.o richard.o menu.o -o richard

richard.o:	richard.c derived.h
	$(CC) $(CFLAGS) richard.c

spawn.o:	spawn.c spawn.h
	$(CC) $(CFLAGS) spawn.c

derived.o:	derived.c derived.h
	$(CC) $(CFLAGS) derived.c

main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) main.o node.o tree.o global.o -o main

menu.o:	menu.c
	$(CC) $(CFLAGS) menu.c

main.o:	main.c node.h global.h tree.h
	$(CC) $(CFLAGS) main.c
	@echo '^' $^ # main.c node.h global.h tree.h
	@echo '<' $< # main.c
	@echo '@' $@ # main.o

g:	gnome.o
	$(CC) $(LIBS) gnome.o -o g

gnome.o:	gnome.c gnome.h
	$(CC) $(CFLAGS) gnome.c

global.o:	global.c global.h
	$(CC) $(CFLAGS) global.c

tree.o:	tree.c tree.h
	$(CC) $(CFLAGS) tree.c

node.o:	node.c node.h
	$(CC) $(CFLAGS) node.c

testgtk.o:	testgtk.c
	$(CC) $(CFLAGS) testgtk.c

testgtk:	testgtk.o
	$(CC) $(LIBS) testgtk.o -o testgtk

clean:
	rm -f *.o main g testgtk richard
