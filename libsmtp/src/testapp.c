/*
  just a small test app for libsmtp
   
Copyright © 2001 Kevin Read <myself@obsidian.de>

This software is available under the GNU Lesser Public License as described
in the COPYING file.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Kevin Read <myself@obsidian.de>
Thu Aug 16 2001 */

#include <glib.h>

#include "libsmtp.h"

int main(void)
{

  struct libsmtp_session_struct *mailsession;
  mailsession = libsmtp_session_initialize();

  libsmtp_set_environment ("kread@newnet-marketing.de","Test", 0, mailsession);
  
  libsmtp_add_recipient (LIBSMTP_REC_TO, "kread@newnet-marketing.de", mailsession);
  libsmtp_add_recipient (LIBSMTP_REC_TO, "kread@mconsole.com", mailsession);
  libsmtp_add_recipient (LIBSMTP_REC_CC, "obsidian@unverbraucht.de", mailsession);
  libsmtp_add_recipient (LIBSMTP_REC_CC, "obsidian@obsidian.de", mailsession);
  libsmtp_add_recipient (LIBSMTP_REC_BCC, "obsidian@panokratie.de", mailsession);
  libsmtp_add_recipient (LIBSMTP_REC_BCC, "obsidian@gmx.net", mailsession);

  if (libsmtp_connect ("container",0,0,mailsession))
    printf ("Ha!\n");
  
  if (libsmtp_dialogue (mailsession))
  {
    printf ("Ha 2!!\n");
    printf ("Error: %s, %s\n", libsmtp_strerr(mailsession), mailsession->LastResponse->str);
  }
  
  if (libsmtp_headers (mailsession))
  {
    printf ("Ha 3!!!\n");
    printf ("Error: %s\nLastMsg: %s\n", libsmtp_strerr(mailsession), mailsession->LastResponse->str);
  }

  if (libsmtp_body_send ("Bla!!", mailsession))
  {
    printf ("Ha 4!!!!\n");
    printf ("Error: %s\nLastMsg: %s\n", libsmtp_strerr(mailsession), mailsession->LastResponse->str);
  }

  if (libsmtp_body_end (mailsession))
  {
    printf ("Ha 5!!!!!\n");
    printf ("Error: %s\nLastMsg: %s\n", libsmtp_strerr(mailsession), mailsession->LastResponse->str);
  }
  
  if (libsmtp_quit (mailsession))
  {
    printf ("Ha 6!!!!!!\n");
    printf ("Error: %s\nLastMsg: %s\n", libsmtp_strerr(mailsession), mailsession->LastResponse->str);
  }

  free (mailsession);
  return 0;
}