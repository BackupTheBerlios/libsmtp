/*
  libsmtp is a library to send mail via SMTP
     This is the MIME handling part
   
Copyright © 2001 Kevin Read <obsidian@berlios.de>

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

Kevin Read <obsidian@berlios.de>
Thu Aug 16 2001 */

/* This will only be included when MIME is enabled */

#ifdef LIBSMTP_USE_MIME

/* #ifndef __G_LIB_H__ */
  #include <glib.h>
/* #endif */

#include "libsmtp.h"
#include "libsmtp_mime.h"

/* This function creates a new body part, checks for conformance to RFC822
   and RFC 2045 and maybe attaches it to the session. */

struct libsmtp_part_struct *libsmtp_part_new \
    (struct libsmtp_part_struct *libsmtp_int_parent_part, int libsmtp_int_type,\
    int libsmtp_int_subtype, int libsmtp_int_encoding, char *libsmtp_int_desc,\
    struct libsmtp_session_struct *libsmtp_session)
{
  struct libsmtp_part_struct *libsmtp_int_part;
  GNode *libsmtp_int_temp_node;
  
  /* First we have to check if the session already has a main type */
  
  if (libsmtp_session->Parts)
  {
    /* Yes, there already is a main part. Now lets see if he has passed
       us a non-NULL parent pointer */
    
    if (libsmtp_int_parent_part)
    {
      /* Ok, it is non-null. Now the parent part this pointer points to has
         to be some kind of multipart */
      
      if ((libsmtp_int_parent_part->type!=LIBSMTP_MIME_MULTIPART) &&
          (libsmtp_int_parent_part->type!=LIBSMTP_MIME_MESSAGE))
      {
        /* No, it isn't multipart. We can't append new parts to it. */
        libsmtp_session->ErrorCode=LIBSMTP_NOMULTIPART;
        return NULL;
      }
    }
    else
    {
      /* We already have a parent part but he passed a NULL pointer;
         This won't do!! */
      libsmtp_session->ErrorCode = LIBSMTP_PART_EXISTS;
      return NULL;
    }
  }
  else
  {
    /* This session hasn't got a main type yet. Lets see if he wants to
       define it. */
    if (libsmtp_int_parent_part)
    {
      /* He doesn't want to define the main part!! */
      libsmtp_session->ErrorCode=LIBSMTP_NOPARENT;
      return NULL;
    }
  }
      
  /* Ok. If we got so far the parent argument should be ok. */
         
  /* We use calloc here to clear the memory. GLists are initialized when
     they point to NULL, so it must be cleared. */

  libsmtp_int_part = (struct libsmtp_part_struct *)calloc (1, sizeof(struct libsmtp_part_struct));
           
  if (libsmtp_int_part == NULL)
    return NULL;
                 
  /* The GStrings must be initialized */
  libsmtp_int_part->custom_type = g_string_new (NULL);
  libsmtp_int_part->custom_subtype = g_string_new (NULL);
  libsmtp_int_part->description = g_string_new (NULL);
  
  /* Now we check if any invalid MIME arguments have been given.*/
  
  if ((libsmtp_int_type < 0) || (libsmtp_int_type > LIBSMTP_MAX_MIME))
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADARGS;
    return NULL;
  }

  libsmtp_int_part->type = libsmtp_int_type;
  
  /* Now the same for the subtype argument. This must correspond to the
     selected type */

  switch (libsmtp_int_part->type)
  {
    case (LIBSMTP_MIME_TEXT):
      if ((libsmtp_int_subtype < 0) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB0))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }
      /* Text types can have any encoding */
      if ((libsmtp_int_encoding < 0) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }

    break;

    case (LIBSMTP_MIME_MESSAGE):
      if ((libsmtp_int_subtype < 1000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB1))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Message types can have any encoding */
      if ((libsmtp_int_encoding < 0) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;

    case (LIBSMTP_MIME_IMAGE):
      if ((libsmtp_int_subtype < 2000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB2))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Image types must be in a non-text encoding */
      if ((libsmtp_int_encoding < LIBSMTP_ENC_BINARY) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;                         

    case (LIBSMTP_MIME_AUDIO):
      if ((libsmtp_int_subtype < 3000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB3))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Audio types must be in a non-text encoding */
      if ((libsmtp_int_encoding < LIBSMTP_ENC_BINARY) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;

    case (LIBSMTP_MIME_VIDEO):
      if ((libsmtp_int_subtype < 4000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB4))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Video types must be in a non-text encoding */
      if ((libsmtp_int_encoding < LIBSMTP_ENC_BINARY) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;

    case (LIBSMTP_MIME_APPLICATION):
      if ((libsmtp_int_subtype < 5000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB5))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Application types must be in a non-text encoding */
      if ((libsmtp_int_encoding < LIBSMTP_ENC_BINARY) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;

    case (LIBSMTP_MIME_MULTIPART):
      if ((libsmtp_int_subtype < 6000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB6))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Application types must be in a text encoding, and should only be
         7bit */
      if (libsmtp_int_encoding != LIBSMTP_ENC_7BIT)
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;

    case (LIBSMTP_MIME_CUSTOM):
      if (libsmtp_int_subtype != LIBSMTP_MIME_SUB_CUSTOM)
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return NULL;
      }

      /* Custom type can have any encoding, of course */
      if ((libsmtp_int_encoding < 0) || (libsmtp_int_encoding > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return NULL;
      }
    break;
  }
  
  /* Ok, we passed. Filling in the structure */
  libsmtp_int_part->subtype=libsmtp_int_subtype;
  libsmtp_int_part->encoding=libsmtp_int_encoding;
  
  if (libsmtp_session->Parts)
  {
    /* This is a sibling part. We search the N-Tree for the data */
    libsmtp_int_temp_node = g_node_find (libsmtp_session->Parts, \
        G_IN_ORDER, G_TRAVERSE_ALL, libsmtp_int_parent_part);
    g_node_append_data (libsmtp_int_temp_node, libsmtp_int_part);
  }
  else
  {
    /* This is the main part. */
    libsmtp_session->Parts=g_node_new (libsmtp_int_part);
  }

  return libsmtp_int_part;
}
                           
/*        
  int libsmtp_mime_type_custom (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);

  int libsmtp_mime_subtype_custom (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *); */



/* This function returns the current part. It should be used directly after
   sending the headers to find out which part will be transmitted first. It
   can be used at any time to find out what part is currently being sent, of
   course. */
   
struct libsmtp_part_struct *libsmtp_part_query \
     (struct libsmtp_session_struct *libsmtp_session)
{
  /* Are we in data stage? */
  if ((libsmtp_session->Stage < LIBSMTP_HEADER_STAGE) ||
      (libsmtp_session->Stage > LIBSMTP_BODY_STAGE))
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADSTAGE;
    return LIBSMTP_BADSTAGE;
  }

  /* Check to see if we already are in a part */
  if (!libsmtp_session->PartNow)
  {
    /* We are not at the moment working on one part. Lets see if any parts
       are defined at all! */
    if (!libsmtp_session->Parts)
    {
      /* nope. bad mistake! */
      libsmtp_session->ErrorCode=LIBSMTP_NOPARENT;
      return LIBSMTP_NOPARENT;
    }
    
    /* So we try to lookup the first part that might contain data */
    if (libsmtp_int_nextpart (libsmtp_session))
      return LIBSMTP_PARTSERR;
  }
  
  /* Ok, return the current part */
  return libsmtp_session->PartNow;
}


/* This function sends body data. It can only be used in the appropriate
   stage. The data to be sent will to be formatted according to RFC822 and
   the MIME standards. */

int libsmtp_part_send (char *libsmtp_body_data, \
            struct libsmtp_session_struct *libsmtp_session)
{

  /* Headers should have been sent before body data goes out, but we must
     still be in body stage at most */
  if ((libsmtp_session->Stage < LIBSMTP_HEADERS_STAGE) ||
      (libsmtp_session->Stage > LIBSMTP_BODY_STAGE))
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADSTAGE;
    return LIBSMTP_BADSTAGE;
  }

  /* Check to see if we already are in a part */
  if (!libsmtp_session->PartNow)
  {
    /* We are not at the moment working on one part. Lets see if any parts
       are defined at all! */
    if (!libsmtp_session->Parts)
    {
      /* nope. bad mistake! */
      libsmtp_session->ErrorCode=LIBSMTP_NOPARENT;
      return LIBSMTP_NOPARENT;
    }
    
    /* So we try to lookup the first part that might contain data */
    if (libsmtp_int_nextpart (libsmtp_session))
      return LIBSMTP_PARTSERR;
  }
  /* Ok, we are in a part */

  if (libsmtp_session->Stage != LIBSMTP_BODY_STAGE)
    /* We now enter the body stage */
    libsmtp_session->Stage = LIBSMTP_BODY_STAGE;
    
  /* We pass the data on to the encoding and sending function */

  if (libsmtp_int_encode_send (libsmtp_body_data, libsmtp_session))
    return LIBSMTP_ERRORSENDFATAL;  /* FIXME This is not always the case */
    
  return LIBSMTP_NOERR;
}

#endif /* LIBSMTP_USE_MIME */
