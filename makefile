CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui gnome gtk glib` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui gnome gtk glib`

POPT	= popt/popt.o popt/poptconfig.o popt/popthelp.o popt/poptparse.o popt/findme.o

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

all:	gd tags libtags

gd:	gd.o args.o mdi.o options.o diff.o menu.o derived.o global.o spawn.o progress.o node.o tree.o compare.o
	$(CC) $(LIBS) $^ -o $@

libtags:
	ctags -f libtags -i+p /usr/src/gnome-libs-1.0.14/libgnome/*.[ch] /usr/src/gnome-libs-1.0.14/libgnomeui/*.[ch] /usr/src/gtk+-1.2.3/gdk/*.[ch] /usr/src/gtk+-1.2.3/gtk/*.[ch] /usr/src/glib-1.2.3/*.[ch]

tags:	phony
	ctags -i+p *.[ch]

clean:
	rm -f *.o
	rm -f `find . -type f -perm 755 -maxdepth 1`

veryclean: clean
	rm tags libtags

phony:

################################################################################

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
main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) $^ -o $@
g:	gnome.o
	$(CC) $(LIBS) $^ -o $@
testgtk: testgtk.o
	$(CC) $(LIBS) $^ -o $@

co:
	co -l args.c args.h compare.c config.h derived.c derived.h diff.c diff.h gd.c global.c global.h mdi.c mdi.h menu.c menu.h node.c node.h options.c options.h progress.c progress.h spawn.c spawn.h tree.c tree.h
