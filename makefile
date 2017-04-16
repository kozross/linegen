CC=gcc
OTHER_CFLAGS =-c -std=gnu11 -I./bstrlib -Wall -Wextra
MY_CFLAGS=$(OTHER_CFLAGS) -Werror -Wfatal-errors

OBJECTS=main.o line.o scr.o bstrlib.o buniutil.o utf8util.o

linegen: $(OBJECTS)
	$(CC) -Wall -Wextra -o linegen $(OBJECTS)
	
main.o: main.c
	$(CC) $(MY_CFLAGS) main.c

line.o: line.c
	$(CC) $(MY_CFLAGS) line.c

scr.o: scr.c
	$(CC) $(MY_CFLAGS) scr.c
	
bstrlib.o: bstrlib/bstrlib.c
	$(CC) $(OTHER_CFLAGS) bstrlib/bstrlib.c
	
buniutil.o: bstrlib/buniutil.c
	$(CC) $(OTHER_CFLAGS) bstrlib/buniutil.c
	
utf8util.o: bstrlib/utf8util.c
	$(CC) $(OTHER_CFLAGS) bstrlib/utf8util.c
	
clean:
	rm linegen $(OBJECTS)