CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui`

SOURCE	= chunk.c color.c config.c derived.c font.c global.c gnome.c main.c menu.c node.c progress.c richard.c spawn.c tree.c
HEADERS	= color.h confdefs.h config.h derived.h font.h global.h main.h menu.h node.h progress.h spawn.h tree.h
PROGS	= main g testgtk richard file compare fork
.c.o:
	$(CC) $(CFLAGS) -o $@ $<

all:	fork

richard: richard.o derived.o menu.o progress.o global.o spawn.o
	$(CC) $(LIBS) $^ -o $@

fork: fork.o
	$(CC) $(LIBS) $^ -o $@

compare: compare.o
	$(CC) $(LIBS) $^ -o $@

file:	file.o menu.o
	$(CC) $(LIBS) $^ -o $@

main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) $^ -o $@

g:	gnome.o
	$(CC) $(LIBS) $^ -o $@

testgtk: testgtk.o
	$(CC) $(LIBS) $^ -o $@

tags:	$(SOURCE) $(HEADERS)
	ctags -i+p *.[ch] gdk/*.[ch] glib/*.[ch] gnome/*.[ch] gnomeui/*.[ch] gtk/*.[ch]

clean:
	rm -f *.o tags $(PROG)

