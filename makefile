CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui gnome` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui gnome`
EXTRAS	= 

POPT	= popt/popt.o popt/poptconfig.o popt/popthelp.o popt/poptparse.o popt/findme.o

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

all:	gd tags libtags
#all:	canvas tags libtags

gd:	args.o compare.o derived.o diff.o file.o gd.o global.o mdi.o menu.o node.o options.o progress.o spawn.o tree.o
	$(CC) $(EXTRAS) $(LIBS) $^ -o $@
	#$@ &

libtags:
	ctags -f libtags -i+p						\
		/usr/src/gnome-libs-1.0.14/libgnome/*.[ch]		\
		/usr/src/gnome-libs-1.0.14/libgnomeui/*.[ch]		\
		/usr/src/gnome-libs-1.0.14/libart_lgpl/*.[ch]		\
		/usr/src/gtk+-1.2.3/gdk/*.[ch]				\
		/usr/src/gtk+-1.2.3/gtk/*.[ch]				\
		/usr/src/glib-1.2.3/*.[ch]

tags:	phony
	ctags -i+p *.[ch]

clean:
	rm -f *.o
	rm -f `find . -type f -perm 755 -maxdepth 1`

veryclean: clean
	rm -f tags libtags

phony:

args.o: args.c args.h options.h diff.h
compare.o: compare.c compare.h diff.h spawn.h
derived.o: derived.c derived.h diff.h spawn.h progress.h global.h node.h tree.h menu.h mdi.h options.h
diff.o: diff.c diff.h
file.o: file.c file.h
gd.o: gd.c config.h args.h options.h diff.h mdi.h global.h
global.o: global.c global.h icons/open.xpm icons/closed.xpm icons/leaf.xpm
mdi.o: mdi.c mdi.h options.h diff.h menu.h derived.h compare.h global.h
menu.o: menu.c config.h menu.h derived.h diff.h node.h tree.h compare.h mdi.h options.h file.h
node.o: node.c node.h derived.h diff.h
options.o: options.c options.h
progress.o: progress.c progress.h
spawn.o: spawn.c spawn.h
tree.o: tree.c tree.h node.h derived.h diff.h global.h

################################################################################

richard: richard.o derived.o menu.o progress.o global.o #spawn.o
	$(CC) $(LIBS) $^ -o $@
exclude: exclude.o
	$(CC) $(LIBS) $^ -o $@
	#$@ &
spawn:	spawn.o
	$(CC) $(LIBS) $^ -o $@
	#$@ &
canvas:	canvas.o
	$(CC) $(LIBS) $^ -o $@
	#$@ &
p:	popt.o
	$(CC) $(LIBS) $^ -o $@
hi:	hi.o
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
testgtk.o:
	$(CC) -I. -I.. $(CFLAGS) -o $@ ../junk/testgtk.c
testgtk: testgtk.o
	$(CC) $(LIBS) $^ -o $@

################################################################################

SOURCE	= args.c compare.c derived.c diff.c file.c gd.c global.c mdi.c menu.c node.c options.c preferences.c progress.c spawn.c tree.c
HEADER	= args.h compare.h config.h derived.h diff.h file.h global.h mdi.h menu.h node.h options.h progress.h spawn.h tree.h
EXTRA	= makefile todo
FILES	= $(SOURCE) $(HEADER) $(EXTRA)

co:
	co -q -l  $(FILES)
ci:
	ci -q -m. $(FILES)
diff:
	@rcsdiff -q --brief $(FILES) | cut -d' ' -f4-
deps:
	gcc -MM `gnome-config --cflags gnomeui gnome gtk glib` $(SOURCE)
	

