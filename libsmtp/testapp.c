#include "libsmtp.h"

int main(void)
{
  char tracelog[200];
  int errornum;
  
  libsmtp_init ();

/*  libsmtp_set_options (LIBSMTP_COMMAND_USE8BIT, 1); */
  libsmtp_set_options (LIBSMTP_COMMAND_CHECK_FOR_8BIT, 1);

  libsmtp_set_debug (1, tracelog, sizeof(tracelog));
  
  if ((errornum=libsmtp_sendmail ("libsmtp-testapp@obsidian.de", "myself@obsidian.de", "mail.obsidian.de", "testmail", "Just a lousy testmail!:дцья")))
  {
    printf ("Error while sending mail: %s!\n", libsmtp_strerr(errornum));
    printf ("Tracelog: %s\n", tracelog);
    return errornum;
  }
  
  printf ("Mail sent successfully. Tracelog: %s\n", tracelog);
  
  return 0;
}


