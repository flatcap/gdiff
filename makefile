CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui gnome gtk glib` -c -g -Wall
EXTRAS	= /usr/src/gtk+-1.2.3/gtk/gtkctree.o
LIBS	= $(EXTRAS) `gnome-config --libs   gnomeui gnome gtk glib`

SOURCE	= args.c canvas.c chunk.c color.c compare.c config.c derived.c exclude.c file.c font.c fork.c gd.c global.c gnome.c lang.c main.c menu.c node.c options.c positional.c progress.c richard.c spawn.c tree.c window.c
HEADERS	= color.h confdefs.h config.h derived.h file.h font.h global.h main.h menu.h node.h options.h progress.h spawn.h tree.h
PROGS	= main g testgtk richard file compare fork exclude lang auto window mdi canvas spawn args p gd
POPT	= popt/popt.o popt/poptconfig.o popt/popthelp.o popt/poptparse.o popt/findme.o

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

all:	gd tags libtags

gd:	gd.o args.o mdi.o options.o diff.o menu.o derived.o global.o spawn.o progress.o node.o tree.o
	$(CC) $(LIBS) $^ -o $@

richard: richard.o derived.o menu.o progress.o global.o #spawn.o
	$(CC) $(LIBS) $^ -o $@

exclude: exclude.o
	$(CC) $(LIBS) $^ -o $@
	$@ &

spawn:	spawn.o
	$(CC) $(LIBS) $^ -o $@
	#$@ &

canvas:	canvas.o
	$(CC) $(LIBS) $^ -o $@
	$@ &

p:	popt.o
	$(CC) $(LIBS) $^ -o $@

args:	args.o
	$(CC) $(POPT) $(LIBS) $^ -o $@

auto:	auto.o
	$(CC) $(LIBS) $^ -o $@

window:	window.o
	$(CC) $(LIBS) $^ -o $@

mdi:	mdi.o
	$(CC) $(LIBS) $^ -o $@

lang:	lang.o
	$(CC) $(LIBS) $^ -o $@

fork:	fork.o
	$(CC) $(LIBS) $^ -o $@

compare: compare.o
	$(CC) $(LIBS) $^ -o $@
	cat data/y | $@ &

file:	file.o menu.o
	$(CC) $(LIBS) $^ -o $@

main.o:	global.h tree.h node.h
main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) $^ -o $@

g:	gnome.o
	$(CC) $(LIBS) $^ -o $@

testgtk: testgtk.o
	$(CC) $(LIBS) $^ -o $@

libtags:
	ctags -f libtags -i+p /usr/src/gnome-libs-1.0.14/libgnome/*.[ch] /usr/src/gnome-libs-1.0.14/libgnomeui/*.[ch] /usr/src/gtk+-1.2.3/gdk/*.[ch] /usr/src/gtk+-1.2.3/gtk/*.[ch] /usr/src/glib-1.2.3/*.[ch]

tags:	phony
	ctags -i+p *.[ch]

clean:
	rm -f *.o $(PROGS)

veryclean: clean
	rm tags libtags

phony:

