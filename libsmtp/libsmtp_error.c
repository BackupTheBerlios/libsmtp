/*
  This is a library to send mail via SMTP
   
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

#include "libsmtp_int.h"

/* libsmtp_sendmail

  return values:
  
  0 ok, mail sent to server
  1 socket couldn't be created
  2 mailserver unknown
  3 connection to mailserver failed
  4 unable to read from socket
  5 mailserver didn't greet correctly (ie its no rfc-conform smtp)
  6 unable to send to socket
  7 mailserver didn't accept our HELO
  8 mailserver didn't accept our MAIL FROM
  9 mailserver didn't accept our RCPT TO
  10 mailserver didn't like us to DATA
  11 mailserver rejected mail after .
  12 Can't get local hostname
  13 sender address incomplete
  14 recipient address incomplete
  15 no 8bit support on server side
  16 8bit check said 8bit chars in body
  17 sendmail called without libsmtp_init
  18 malloc error
  19 unbalanced < in address
  */
  
const char *libsmtp_strerr_strings[] = {
  "No error, mail sent",
  "Unable to create local socket",
  "Mailserver unknown",
  "Connection to mailserver failed",
  "Unable to read from socket",
  "Mailserver didn't greet correctly conforming to RFC",
  "Unable to send to socket",
  "Mailserver didn't accept our HELO command",
  "Mailserver didn't accept our MAIL FROM command",
  "Mailserver didn't accept our RCPT TO command",
  "Mailserver didn't accept our DATA command",
  "Mailserver rejected mail after DATA",
  "Unable to get local hostname",
  "Incomplete From-Address",
  "Incomplete Recipient Address",
  "No 8 bit support",
  "8bit check failed!",
  "libsmtp_sendmail called without calling libsmtp_init first",
  "malloc error",
  "unbalanced '<' in address"
};

const char *libsmtp_undef_errstr = "Undefined error";  

#define MIN_ERRNO 0
#define MAX_ERRNO 19


const char *libsmtp_strerr (int libsmtp_temp_errno)
{
  if ((libsmtp_temp_errno > MAX_ERRNO) || (libsmtp_temp_errno < MIN_ERRNO))
    return libsmtp_undef_errstr;
  
  return libsmtp_strerr_strings [libsmtp_temp_errno];
}

int libsmtp_errno ()
{
  return libsmtp_int_errno;
}