CC	= gcc
CFLAGS	= `gnome-config --cflags gnomeui` -c -g -Wall
LIBS	= `gnome-config --libs   gnomeui`

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

richard:	richard.o derived.o menu.o
	$(CC) $(LIBS) $^ -o $@

main:	main.o node.o tree.o global.o
	$(CC) $(LIBS) $^ -o $@

g:	gnome.o
	$(CC) $(LIBS) $^ -o $@

testgtk:	testgtk.o
	$(CC) $(LIBS) $^ -o $@

clean:
	rm -f *.o main g testgtk richard
