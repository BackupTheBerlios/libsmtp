/*
  libsmtp is a library to send mail via SMTP
     This is the communication part
   
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

/* #ifndef __G_LIB_H__ */
  #include <glib.h>
/* #endif */

#include "libsmtp.h"

/* internal communication functions */

/* Type is one of:
	0 normal body mesg
	1 normal header mesg
	2 normal dialogue mesg

  This function won't return correct libsmtp_gstring_read for dialogue mesgs */

int libsmtp_int_read (GString *libsmtp_gstring_read, struct libsmtp_session_struct *libsmtp_session, int type)
{
  int libsmtp_int_bytes;
  char *libsmtp_int_temp_buffer;
  char libsmtp_int_rec_buffer[4096];
  
  bzero (libsmtp_int_rec_buffer, sizeof(libsmtp_int_rec_buffer));
  
  libsmtp_int_bytes=recv (libsmtp_session->socket, libsmtp_int_rec_buffer, sizeof(libsmtp_int_rec_buffer), 0);
  if (libsmtp_int_bytes<=0)
  {
    libsmtp_session->ErrorCode=LIBSMTP_ERRORREADFATAL;
    libsmtp_session->Stage=type;
    close (libsmtp_session->socket);
    libsmtp_session->socket=0;
    return LIBSMTP_ERRORREAD;
  }

  #ifdef DEBUG_LIBSMTP
    printf ("DEBUG in read: %s\n", libsmtp_int_rec_buffer);
  #endif
  
  /* Update statistics */
  switch (type)
  {
    case (0):
      libsmtp_session->BodyBytes+=libsmtp_int_bytes;
      break;
    
    case (1):
      libsmtp_session->HeaderBytes+=libsmtp_int_bytes;
      libsmtp_session->HeadersSent++;
      break;

    case (2):
      libsmtp_session->DialogueBytes+=libsmtp_int_bytes;
      libsmtp_session->DialogueSent++;

      g_string_assign (libsmtp_gstring_read, libsmtp_int_rec_buffer);

      /* Ok, take the first part of the response ... */
      libsmtp_int_temp_buffer = strtok ((char *)libsmtp_int_rec_buffer, " ");
  
      /* and extract the response code */
      libsmtp_session->LastResponseCode = atoi(libsmtp_int_temp_buffer);
  
      /* Then fetch the rest of the string and save it */
      libsmtp_int_temp_buffer = strtok (NULL, "\0");
      libsmtp_session->LastResponse = g_string_new (libsmtp_int_temp_buffer);
      break;
  }
  return LIBSMTP_NOERR;
}    

/* Type is one of:
	0 normal body mesg
	1 normal header mesg
	2 normal dialogue mesg */

int libsmtp_int_send (GString *libsmtp_send_gstring, struct libsmtp_session_struct *libsmtp_session, int type)
{
  int libsmtp_int_bytes;

  #ifdef DEBUG_LIBSMPT
    printf ("DEBUG in send: %s\n", libsmtp_send_gstring->str);
  #endif
  
  libsmtp_int_bytes=send (libsmtp_session->socket, libsmtp_send_gstring->str, libsmtp_send_gstring->len, 0);
  if (libsmtp_int_bytes<0)
  {
    libsmtp_session->ErrorCode=LIBSMTP_ERRORSENDFATAL;
    libsmtp_session->Stage=type;
    close (libsmtp_session->socket);
    libsmtp_session->socket=0;
    return LIBSMTP_ERRORSEND;
  }
  /* Update statistics */
  switch (type)
  {
    case (0):
      libsmtp_session->BodyBytes+=libsmtp_int_bytes;
      break;
    case (1):
      libsmtp_session->HeaderBytes+=libsmtp_int_bytes;
      libsmtp_session->HeadersSent++;
      break;
    case (2):
      libsmtp_session->DialogueBytes+=libsmtp_int_bytes;
      libsmtp_session->DialogueSent++;
      break;
  }
    
  return LIBSMTP_NOERR;
}

int libsmtp_int_send_body (char *libsmtp_send_string, struct libsmtp_session_struct *libsmtp_session)
{
  int libsmtp_int_bytes;

  #ifdef DEBUG_LIBSMTP
    printf ("DEBUG in bsend : %s\n", libsmtp_send_string);
  #endif
  
  libsmtp_int_bytes=send (libsmtp_session->socket, libsmtp_send_string, strlen(libsmtp_send_string), 0);
  if (libsmtp_int_bytes<0)
  {
    libsmtp_session->ErrorCode=LIBSMTP_ERRORSENDFATAL;
    close (libsmtp_session->socket);
    libsmtp_session->socket=0;
    return LIBSMTP_ERRORSEND;
  }
  /* Update statistics */
  libsmtp_session->BodyBytes+=libsmtp_int_bytes;
    
  return LIBSMTP_NOERR;
}

/* Use this function to make libsmtp run the SMTP dialogue */

int libsmtp_dialogue (struct libsmtp_session_struct *libsmtp_session)
{
  int libsmtp_temp;
  GString *libsmtp_temp_gstring;
  
  libsmtp_temp_gstring = g_string_new(NULL);
  
  /* First we check if sender, subject and at least one recipient has
     been set */
  
  #ifdef DEBUG_LIBSMTP
    printf ("DEBUG:List length: %d!\n", g_list_length (libsmtp_session->To));
  #endif
  
  if ((libsmtp_session->From->len < 1) || (libsmtp_session->Subject->len < 1) \
       || (g_list_length (libsmtp_session->To) <1 ))
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADARGS;
    return LIBSMTP_BADARGS;
  }
  
  /* We enter the sender stage now */
  libsmtp_session->Stage = LIBSMTP_SENDER_STAGE;

  /* Ok, now lets give him the sender address */
  
  g_string_sprintf (libsmtp_temp_gstring, "MAIL FROM: %s\r\n", \
                      libsmtp_session->From->str);
  
  if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 2))
  {
    libsmtp_session->ErrorCode = LIBSMTP_ERRORSENDFATAL;
    return LIBSMTP_ERRORSENDFATAL;
  }
  
  /* Now we have to see if he likes it */
  
  if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
  {
    libsmtp_session->ErrorCode = LIBSMTP_ERRORREADFATAL;
    return LIBSMTP_ERRORREADFATAL;
  }

  if (libsmtp_session->LastResponseCode > 299)
  {
    libsmtp_session->ErrorCode = LIBSMTP_WONTACCEPTSENDER;
    close(libsmtp_session->socket);
    libsmtp_session->socket=0;
    return LIBSMTP_WONTACCEPTSENDER;
  }

  /* We enter the recipient stage now */
  libsmtp_session->Stage = LIBSMTP_RECIPIENT_STAGE;

  /* Now we go through all recipients, To: first */

  for (libsmtp_temp=0; libsmtp_temp < g_list_length(libsmtp_session->To);\
         libsmtp_temp++)
  {
    libsmtp_session->To=g_list_nth (libsmtp_session->To, libsmtp_temp);
    g_string_sprintf (libsmtp_temp_gstring, "RCPT TO: %s\r\n", \
        libsmtp_session->To->data);
    

    /* Every recipient gets sent to the server */

    if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 2))
    {
      libsmtp_session->ErrorCode = LIBSMTP_ERRORSENDFATAL;
      return LIBSMTP_ERRORSENDFATAL;
    }

    /* We have to read the servers response of course */
    if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
    {
      libsmtp_session->ErrorCode = LIBSMTP_ERRORREADFATAL;
      return LIBSMTP_ERRORREADFATAL;
    }

    /* Did he like this one? */
    if (libsmtp_session->LastResponseCode > 299)
    {
      libsmtp_session->ErrorCode = LIBSMTP_WONTACCEPTREC;
      
      /* No, he didn't. We write the response code into the failed linked
         list so that the denial reason can be seen later on */
      
      libsmtp_session->ToResponse=g_list_insert (libsmtp_session->ToResponse,
         libsmtp_temp_gstring->str, libsmtp_temp);

      libsmtp_session->NumFailedTo++;
    }
    else
    {
      /* Ok, he liked it. We write his response in the string anyway */
      libsmtp_session->ToResponse=g_list_insert (libsmtp_session->ToResponse,
         libsmtp_temp_gstring->str, libsmtp_temp);
      
    }

  }

  /* Now we go through all CC recipients */

  for (libsmtp_temp=0; libsmtp_temp < g_list_length(libsmtp_session->CC);\
         libsmtp_temp++)
  {
    libsmtp_session->CC=g_list_nth (libsmtp_session->CC, libsmtp_temp);
    g_string_sprintf (libsmtp_temp_gstring, "RCPT TO: %s\r\n", \
        libsmtp_session->CC->data);
    

    /* Every recipient gets sent to the server */

    if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 2))
    {
      libsmtp_session->ErrorCode = LIBSMTP_ERRORSENDFATAL;
      return LIBSMTP_ERRORSENDFATAL;
    }

    /* We have to read the servers response of course */
    if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
    {
      libsmtp_session->ErrorCode = LIBSMTP_ERRORREADFATAL;
      return LIBSMTP_ERRORREADFATAL;
    }

    /* Did he like this one? */
    if (libsmtp_session->LastResponseCode > 299)
    {
      libsmtp_session->ErrorCode = LIBSMTP_WONTACCEPTREC;
      
      /* No, he didn't. We write the response code into the failed linked
         list so that the denial reason can be seen later on */
      
      libsmtp_session->CCResponse=g_list_insert (libsmtp_session->CCResponse,
         libsmtp_temp_gstring->str, libsmtp_temp);

      libsmtp_session->NumFailedCC++;
    }
    else
    {
      /* Ok, he liked it. We write his response in the string anyway */
      libsmtp_session->CCResponse=g_list_insert (libsmtp_session->CCResponse,
         libsmtp_temp_gstring->str, libsmtp_temp);
      
    }
  }

  /* Now we go through all BCC recipients */

  for (libsmtp_temp=0; libsmtp_temp < g_list_length(libsmtp_session->BCC);\
         libsmtp_temp++)
  {
    libsmtp_session->BCC=g_list_nth (libsmtp_session->BCC, libsmtp_temp);
    g_string_sprintf (libsmtp_temp_gstring, "RCPT TO: %s\r\n", \
        libsmtp_session->BCC->data);
    

    /* Every recipient gets sent to the server */

    if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 2))
    {
      libsmtp_session->ErrorCode = LIBSMTP_ERRORSENDFATAL;
      return LIBSMTP_ERRORSENDFATAL;
    }

    /* We have to read the servers response of course */
    if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
    {
      libsmtp_session->ErrorCode = LIBSMTP_ERRORREADFATAL;
      return LIBSMTP_ERRORREADFATAL;
    }

    /* Did he like this one? */
    if (libsmtp_session->LastResponseCode > 299)
    {
      libsmtp_session->ErrorCode = LIBSMTP_WONTACCEPTREC;
      
      /* No, he didn't. We write the response code into the failed linked
         list so that the denial reason can be seen later on */
      
      libsmtp_session->BCCResponse=g_list_insert (libsmtp_session->BCCResponse,
         libsmtp_temp_gstring->str, libsmtp_temp);

      libsmtp_session->NumFailedBCC++;
    }
    else
    {
      /* Ok, he liked it. We write his response in the string anyway */
      libsmtp_session->BCCResponse=g_list_insert (libsmtp_session->BCCResponse,
         libsmtp_temp_gstring->str, libsmtp_temp);
      
    }
  }
}


/* This function starts the DATA part. No more dialogue stuff can be sent
   from this time on. */

int libsmtp_headers (struct libsmtp_session_struct *libsmtp_session)
{
  GString *libsmtp_temp_gstring;
  int libsmtp_temp;
  
  /* Are we at the end of the dialogue stage */
  if (libsmtp_session->Stage < LIBSMTP_RECIPIENT_STAGE)
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADSTAGE;
    return LIBSMTP_BADSTAGE;
  }

  /* Great finality. After this no more dialogue can go on */
  libsmtp_temp_gstring = g_string_new ("DATA\r\n");
  
  if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 2))
    return LIBSMTP_ERRORSENDFATAL;
  
  /* What has he say to a little bit of DATA? */
  
  if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
  {
    libsmtp_session->ErrorCode = LIBSMTP_ERRORREADFATAL;
    return LIBSMTP_ERRORREADFATAL;
  }

  if (libsmtp_session->LastResponseCode != 354)
  {
    libsmtp_session->ErrorCode = LIBSMTP_WONTACCEPTDATA;
    close(libsmtp_session->socket);
    libsmtp_session->socket=0;
    return LIBSMTP_WONTACCEPTDATA;
  }

  /* We enter the data stage now */
  libsmtp_session->Stage = LIBSMTP_HEADERS_STAGE;

  /* Now we send through all the headers. No more responses will come from
     the mailserver until we end the DATA part. */
  
  /* First the From: header */
  
  g_string_sprintf (libsmtp_temp_gstring, "From: %s\n", \
                      libsmtp_session->From->str);
  
  if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 1))
    return LIBSMTP_ERRORSENDFATAL;

  /* Then the Subject: header */

  g_string_sprintf (libsmtp_temp_gstring, "Subject: %s\n", \
                      libsmtp_session->From->str);
  
  if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 1))
    return LIBSMTP_ERRORSENDFATAL;
  
  /* Then we send all the To: addresses */

  g_string_assign (libsmtp_temp_gstring, "To: ");

  for (libsmtp_temp=0; libsmtp_temp < g_list_length(libsmtp_session->To);\
       libsmtp_temp++)
  {
    /* Select the respective node of the linked list */
    libsmtp_session->To=g_list_nth (libsmtp_session->To, libsmtp_temp);

    /* If this is the last entry of the list, don't append a comma */
    if (libsmtp_temp==g_list_length (libsmtp_session->To))
    {
      g_string_append (libsmtp_temp_gstring, libsmtp_session->To->data);
      g_string_append (libsmtp_temp_gstring, "\r\n");
    }
    else
    {
      g_string_append (libsmtp_temp_gstring, libsmtp_session->To->data);
      g_string_append (libsmtp_temp_gstring, ", ");
    }
  } 
  /* Send the line to the server */

  if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 1))
    return LIBSMTP_ERRORSENDFATAL;

  /* Then we iterate through all CC recipients */
  
  if (g_list_length (libsmtp_session->CC))
  {
    g_string_assign (libsmtp_temp_gstring, "CC: ");

    for (libsmtp_temp=0; libsmtp_temp < g_list_length(libsmtp_session->CC);\
         libsmtp_temp++)
    {
      /* Select the respective node of the linked list */
      libsmtp_session->To=g_list_nth (libsmtp_session->CC, libsmtp_temp);

      /* If this is the last entry of the list, don't append a comma */
      if (libsmtp_temp==g_list_length (libsmtp_session->CC))
      {
        g_string_append (libsmtp_temp_gstring, libsmtp_session->To->data);
        g_string_append (libsmtp_temp_gstring, "\r\n");
      }
      else
      {
        g_string_append (libsmtp_temp_gstring, libsmtp_session->To->data);
        g_string_append (libsmtp_temp_gstring, ", ");
      }
    } 
    /* Send the line to the server */

    if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 1))
      return LIBSMTP_ERRORSENDFATAL;
  }
  
  /* Now let there be a blank line */
  g_string_assign (libsmtp_temp_gstring, "\n");
  
  if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 1))
    return LIBSMTP_ERRORSENDFATAL;  
}


/* This function sends body data. It can only be used in the appropriate
   stage. The data to be sent has to be formatted according to RFC822 and
   the MIME standards. */

int libsmtp_body_send (char *libsmtp_body_data, \
            struct libsmtp_session_struct *libsmtp_session)
{

  /* Headers should have been sent before body data goes out */
  if (libsmtp_session->Stage < LIBSMTP_HEADERS_STAGE)
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADSTAGE;
    return LIBSMTP_BADSTAGE;
  }
  
  /* We now enter the body stage */
  libsmtp_session->Stage = LIBSMTP_BODY_STAGE;

  if (libsmtp_int_send_body (libsmtp_body_data, libsmtp_session))
    return LIBSMTP_ERRORSENDFATAL;
  
  return LIBSMTP_NOERR;
}


/* This function ends the body part. It can only be used in certain stages */

int libsmtp_body_end (struct libsmtp_session_struct *libsmtp_session)
{
  GString *libsmtp_temp_gstring;
  
  libsmtp_temp_gstring = g_string_new (NULL);

  /* We need to be in body stage to leave it :) */
  if (libsmtp_session->Stage < LIBSMTP_BODY_STAGE)
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADSTAGE;
    return LIBSMTP_BADSTAGE;
  }
  
  /* We now enter the finished stage */
  libsmtp_session->Stage = LIBSMTP_FINISHED_STAGE;  

  /* Now let there be a line with only a dot on it */
  
  if (libsmtp_int_send_body ("\r\n", libsmtp_session))
    return LIBSMTP_ERRORSENDFATAL;

  if (libsmtp_int_send_body (".\r\n", libsmtp_session))
    return LIBSMTP_ERRORSENDFATAL;

  /* Did you like that body, connisseur? */
  
  sleep (2);
  if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
    return LIBSMTP_ERRORREADFATAL;

  #ifdef DEBUG_LIBSMTP
    printf ("DEBUG: %s\n", libsmtp_session->LastResponse);
  #endif
 
  if (libsmtp_session->LastResponseCode > 299)
  {
    /* Aaaw no, he didn't. Don't ask me how that can happen... */

    libsmtp_session->ErrorCode = LIBSMTP_REJECTBODY;
    close (libsmtp_session->socket);
    libsmtp_session->socket=0;
    return LIBSMTP_REJECTBODY;
  }
  
  return LIBSMTP_NOERR;
}

/* This function ends the SMTP session. It can only be used in certain stages,
   notably in all dialogue modes. */

int libsmtp_quit (struct libsmtp_session_struct *libsmtp_session)
{
  GString *libsmtp_temp_gstring;

  /* We need to be in body stage to leave it :) */
  if ((libsmtp_session->Stage = LIBSMTP_FINISHED_STAGE) || \
      (libsmtp_session->Stage < LIBSMTP_DATA_STAGE))
  {

    /* We now enter the quit stage */
    libsmtp_session->Stage = LIBSMTP_QUIT_STAGE;  

    /* Lets tell him we are quitting. */
    libsmtp_temp_gstring = g_string_new ("QUIT\n");
  
    if (libsmtp_int_send (libsmtp_temp_gstring, libsmtp_session, 1))
      return LIBSMTP_ERRORSENDFATAL;
  
    /* I hope thats okay with him :) */
  
    if (libsmtp_int_read (libsmtp_temp_gstring, libsmtp_session, 2))
      return LIBSMTP_ERRORREADFATAL;

    if (libsmtp_session->LastResponseCode > 299)
    {
      /* He says it isn't, but who cares... */

      libsmtp_session->ErrorCode = LIBSMTP_REJECTQUIT;
      close (libsmtp_session->socket);
      libsmtp_session->socket=0;
      libsmtp_session->Stage=LIBSMTP_NOCONNECT_STAGE;
      return LIBSMTP_REJECTQUIT;
    }
    else
    {
      /* Babe, I'm gonne leave you... */

      libsmtp_session->ErrorCode = LIBSMTP_NOERR;
      close (libsmtp_session->socket);
      libsmtp_session->socket=0;
      libsmtp_session->Stage=LIBSMTP_NOCONNECT_STAGE;
      return LIBSMTP_NOERR;
    }
  }

  /* Wrong stage, dude ! */
  libsmtp_session->ErrorCode = LIBSMTP_BADSTAGE;
  return LIBSMTP_BADSTAGE;
}