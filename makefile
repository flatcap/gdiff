CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui`

SOURCE	= chunk.c color.c config.c derived.c font.c global.c gnome.c main.c menu.c node.c progress.c richard.c spawn.c tree.c
HEADERS	= color.h confdefs.h config.h derived.h font.h global.h main.h menu.h node.h progress.h spawn.h tree.h

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

all:	tags richard

richard: richard.o derived.o menu.o progress.o global.o spawn.o
	$(CC) $(LIBS) $^ -o $@

main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) $^ -o $@

g:	gnome.o
	$(CC) $(LIBS) $^ -o $@

testgtk: testgtk.o
	$(CC) $(LIBS) $^ -o $@

tags:	$(SOURCE) $(HEADERS)
	ctags -i+p *.[ch]

clean:
	rm -f *.o main g testgtk richard tags
