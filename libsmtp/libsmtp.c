/*
  libsmtp is a library to send mail via SMTP
   
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

#include "libsmtp.h"
#include <glib.h>

struct libsmtp_session_struct *libsmtp_connect (char *libsmtp_server, unsigned int libsmtp_port, unsigned int libsmtp_flags)
{
  int libsmtp_socket; 		/* The temporary socket handle */
  int libsmtp_bytes_read=0;     /* How many bytes read? */
  struct libsmtp_session_struct *libsmtp_session; /* Our session struct */
  struct hostent *libsmtp_mailhost;		/* We need this to convert the hostname to an IP */
  struct sockaddr_in libsmtp_sock;		/* We need this for the connection */
  char libsmtp_temp_buffer[4098];	/* Temp string for reads and writes */
  char libsmtp_search_buffer[4098];	/* Used for searching in strings */
  GString *libsmtp_response;		/* Response save */

  /* First we zero out the session struct so that everything is NULL'ed.
     This is especially important to keep Glibs GList functions happy. */

  bzero (libsmtp_session, sizeof (libsmtp_session));

  /* We need a socket anyway :) */
  libsmtp_socket = socket (PF_INET, SOCK_STREAM, 0)
  
  /* Socket ok?
  if (libsmtp_socket < 0)
  {
    libsmtp_session.ErrorCode = LIBSMTP_SOCKETNOCREATE;
    return NULL;
  }
  
  /* Now we need to get the IP from the hostname... */
  if ((hostname=gethostbyname((const char *)libsmtp_server))==NULL)
  {
    libsmtp_session.ErrorCode = LIBSMTP_HOSTNOTFOUND;
    close (libsmtp_socket);
    return NULL;
  }
  
  /* This struct is needed for the connect call */
  libsmtp_sock.sin_family = AF_INET;
  libsmtp_sock.sin_addr = *(struct in_addr *)hostname->h_addr;
  if (!libsmtp_port)
    libsmtp_sock.sin_port = htons (25);
  else
    libsmtp_sock.sin_port = htons (libsmtp_port);

  /* Now we make the connection to the smart host on the specified port */

  if (connect (libsmtp_socket, (struct sockaddr *) &libsmtp_sock, sizeof (libsmtp_sock) ) < 0)
  {
    libsmtp_session.ErrorCode = LIBSMTP_CONNECTERR;
    close (libsmtp_socket);
    return NULL;
  }
  
  /* Ok, lets set the session socket to the right handler */
  libsmtp_session.socket = libsmtp_socket;
  
  /* Now we should read the mail servers greeting */

  if ((libsmtp_bytes_read=recv (libsmtp_session.socket, libsmtp_buffer, 4096, 0))<0)
  {
    libsmtp_session.ErrorCode=LIBSMTP_ERRORREADFATAL;
    close (libsmtp_session.socket)
    return NULL;
  }
  
  /* Ok, take the first part of the response ... */
  libsmtp_search_string = strtok (libsmtp_buffer, " ");

  /* and extract the response code */
  libsmtp_session.LastResponseCode = atoi(libsmtp_search_string);
  
  /* Then fetch the rest of the string and save it */
  libsmtp_search_string = strtok (NULL, " ");
  libsmtp_session.Lastresponse = GString_new (libsmtp_search_string);

  if (libsmtp_session.LastResponseCode != 220)
  {
    libsmtp_session.ErrorC = LIBSMTP_NOTWELCOME;
    close (libsmtp_session.socket);
    return NULL;
  }
}
  