
all:	libsmtp.a testapp

libsmtp.a:	libsmtp.o
	ar -r libsmtp.a libsmtp.o

libsmtp.o:	libsmtp.c libsmtp.h
	gcc `glib-config --cflags` -c -g -o libsmtp.o libsmtp.c

libsmtp_commands.o:	libsmtp_commands.c libsmtp_commands.h
	gcc -c -O2 -o libsmtp_commands.o libsmtp_commands.c

libsmtp_error.o:	libsmtp_error.c
	gcc -c -O2 -o libsmtp_error.o libsmtp_error.c

testapp.o:	testapp.c
	gcc -c -g -o testapp.o testapp.c `glib-config --cflags`

testapp:	testapp.o libsmtp.a
	gcc -o testapp -g -L. testapp.o -lsmtp `glib-config --libs` -lglib

clean:
	rm testapp *.o *.a