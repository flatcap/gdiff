CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui gnome` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui gnome`

POPT	= popt/popt.o popt/poptconfig.o popt/popthelp.o popt/poptparse.o popt/findme.o

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

all:	gd tags libtags

gd:	gd.o args.o mdi.o options.o diff.o menu.o derived.o global.o spawn.o progress.o node.o tree.o compare.o
	$(CC) $(LIBS) $^ -o $@
	#$@ &

libtags:
	ctags -f libtags -i+p						\
		/usr/src/gnome-libs-1.0.14/libgnome/*.[ch]		\
		/usr/src/gnome-libs-1.0.14/libgnomeui/*.[ch]		\
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

deps:
	gcc -MM `gnome-config --cflags gnomeui gnome gtk glib` *.c
	
args.o: args.c options.h diff.h
compare.o: compare.c compare.h spawn.h
derived.o: derived.c derived.h spawn.h progress.h global.h node.h tree.h
diff.o: diff.c diff.h
gd.o: gd.c config.h args.h options.h diff.h mdi.h global.h
global.o: global.c global.h icons/open.xpm icons/closed.xpm icons/leaf.xpm
mdi.o: mdi.c options.h mdi.h diff.h menu.h derived.h
menu.o: menu.c menu.h derived.h node.h tree.h compare.h
node.o: node.c node.h derived.h
options.o: options.c options.h
progress.o: progress.c progress.h
spawn.o: spawn.c spawn.h
tree.o: tree.c tree.h node.h derived.h global.h

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
	$@ &
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

FILES	= args.c args.h compare.c config.h derived.c derived.h diff.c diff.h gd.c global.c global.h mdi.c mdi.h menu.c menu.h node.c node.h options.c options.h progress.c progress.h spawn.c spawn.h tree.c tree.h makefile todo compare.h
co:
	co -l -q $(FILES)
ci:
	ci -q -m. $(FILES)
diff:
	@rcsdiff -q --brief $(FILES) | cut -d' ' -f4-

