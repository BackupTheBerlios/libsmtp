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

#include <glib.h>
#include "libsmtp.h"

/* old libsmtp_sendmail func_

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
  
const char *libsmtp_strerr_strings_fatal[] = {
  "No error",   /* 0 */
  "Unable to create local socket",
  "Mailserver unknown",
  "Connection to mailserver failed",
  "Unable to read from socket, fatal", /* 4 */
  "Mailserver didn't greet correctly conforming to RFC, we might not be welcome",
  "Can't find our hostname",
  "Unable to send to socket" /* 7 */
};

const char *libsmtp_strerr_strings_nonfatal[] = {
  "Error reading from socket",   /* 1024 */
  "Error sending to socket"
};

const char *libsmtp_undef_errstr = "Undefined error";  

#define MAX_FATAL_ERRNO 7
#define MIN_NONFATAL_ERRNO 1024
#define MAX_NONFATAL_ERRNO 1024+1

const char *libsmtp_strerr (struct libsmtp_session_struct *libsmtp_session)
{
  /* This shouldn't really happen, but this is not C++, we can't prevent
     non-libsmtp functions from writing to these ...
     There are no higher error codes than the nonfatal ones */
  if (libsmtp_session->ErrorCode > MAX_NONFATAL_ERRNO)
    return libsmtp_undef_errstr;
  
  /* And there are no valid error codes betwenn fatal and nonfatal */
  if ((libsmtp_session->ErrorCode > MAX_FATAL_ERRNO) && \
      (libsmtp_session->ErrorCode < MIN_NONFATAL_ERRNO))
    return libsmtp_undef_errstr;
  
  /* Now send back the pointer - we have two tables */
  if (libsmtp_session->ErrorCode > MAX_FATAL_ERRNO)
    return libsmtp_strerr_strings_nonfatal [libsmtp_session->ErrorCode];
  else
    return libsmtp_strerr_strings_fatal [libsmtp_session->ErrorCode];
}

int libsmtp_errno (struct libsmtp_session_struct *libsmtp_session)
{
  /* This shouldn't really happen, but this is not C++, we can't prevent
     non-libsmtp functions from writing to these ...
     There are no higher error codes than the nonfatal ones */
  if (libsmtp_session->ErrorCode > MAX_NONFATAL_ERRNO)
    return LIBSMTP_UNDEFERR;
  
  /* And there are no valid error codes betwenn fatal and nonfatal */
  if ((libsmtp_session->ErrorCode > MAX_FATAL_ERRNO) && \
      (libsmtp_session->ErrorCode < MIN_NONFATAL_ERRNO))
    return LIBSMTP_UNDEFERR;

  return libsmtp_session->ErrorCode;
}