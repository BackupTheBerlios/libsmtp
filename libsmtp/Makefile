
all:	libsmtp.a testapp

libsmtp.a:	libsmtp.o libsmtp_commands.o libsmtp_error.o
	ar -r libsmtp.a libsmtp.o libsmtp_commands.o libsmtp_error.o

libsmtp.o:	libsmtp.c libsmtp_int.h
	gcc -c -O2 -o libsmtp.o libsmtp.c

libsmtp_commands.o:	libsmtp_commands.c libsmtp_commands.h
	gcc -c -O2 -o libsmtp_commands.o libsmtp_commands.c

libsmtp_error.o:	libsmtp_error.c
	gcc -c -O2 -o libsmtp_error.o libsmtp_error.c

testapp.o:	testapp.c
	gcc -c -O2 -o testapp.o testapp.c

testapp:	testapp.o libsmtp.a
	gcc -o testapp -O2 -L. testapp.o -lsmtp

clean:
	rm testapp *.o *.a