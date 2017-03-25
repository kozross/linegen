CC=gcc
CFLAGS=-c -Wextra -Wall -Wextra

linegen: main.o bsafe.o bstrlib.o buniutil.o utf8util.o
	$(CC) main.o bsafe.o bstrlib.o buniutil.o utf8util.o -o linegen
	
main.o: main.c
	$(CC) $(CFLAGS) main.c
	
bsafe.o: bstrlib/bsafe.c
	$(CC) $(CFLAGS) bstrlib/bsafe.c
	
bstrlib.o: bstrlib/bstrlib.c
	$(CC) $(CFLAGS) bstrlib/bstrlib.c
	
buniutil.o: bstrlib/buniutil.c
	$(CC) $(CFLAGS) bstrlib/buniutil.c
	
utf8util.o: bstrlib/utf8util.c
	$(CC) $(CFLAGS) bstrlib/utf8util.c
	
clean:
	rm *o linegen