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
  mailsession = calloc (1, sizeof (struct libsmtp_session_struct));
  if (libsmtp_connect ("container",0,0,mailsession))
    printf ("Ha!\n");
  
  free (mailsession);
  return 0;
}