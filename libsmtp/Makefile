# Generated automatically from Makefile.in by configure.

all:	libsmtp.a testapp

libsmtp.a:	libsmtp_connection.o libsmtp_error.o
	ar -r libsmtp.a libsmtp_connection.o libsmtp_error.o

libsmtp.o:	libsmtp.c libsmtp.h
	gcc `glib-config --cflags` -c -g -o libsmtp.o libsmtp.c

libsmtp_connection.o:	libsmtp_connection.c libsmtp.h
	gcc -c -g -o libsmtp_connection.o libsmtp_connection.c

libsmtp_error.o:	libsmtp_error.c libsmtp.h
	gcc -c -g -o libsmtp_error.o libsmtp_error.c

testapp.o:	testapp.c libsmtp.h
	gcc -c -g -o testapp.o testapp.c `glib-config --cflags`

testapp:	testapp.o libsmtp.a
	gcc -o testapp -g -L. testapp.o -lsmtp `glib-config --libs` -lglib

clean:
	rm testapp *.o *.a