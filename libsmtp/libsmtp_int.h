
#ifndef LIB_SMTP_H

#define LIB_SMTP_H

#ifndef LIBSMTP_C

extern int libsmtp_int_errno;
extern int libsmtp_debug;
#endif

char *libsmtp_tracelog;
int libsmtp_size_tracelog;
int libsmtp_written_tracelog;

int libsmtp_errno();

void libsmtp_set_debug(int, char *, unsigned int);

int libsmtp_sendmail (const char *, const char *, const char *, const char *, const char *);

struct libsmtp_required_struct {
  int initialized;
  int eightbit;
/*  int length_constraint;
  int sancheck_addrs; */
  int check_8bit;
/*  int constraint_sender;
  int constraint_recipient;
  int constraint_subject; */
  int exit_on_8bit;
  int thissession_8bit;
  int charset;
} libsmtp_required;

#endif