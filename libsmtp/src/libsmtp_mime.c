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

#ifdef #LIBSMTP_USE_MIME

/* #ifndef __G_LIB_H__ */
  #include <glib.h>
/* #endif */

#include "libsmtp.h"
#include "libsmtp_mime.h"

/* This function creates a new body part, checks for conformance to RFC822
   and RFC 2045 and maybe attaches it to the session (if this is the main
   part) */

int libsmtp_mime_new (int libsmtp_int_type, int libsmtp_int_subtype, \
       int libsmtp_int_enc, struct libsmtp_session_struct *libsmtp_session)
{
  
  struct libsmtp_part_struct *libsmtp_int_part;
  
  /* First we have to check if the session already has a main type */
  
  if (libsmtp_session->mainpart_id)
  {
    /* Yes, there already is a main part. Lets see if its multipart */
    
    libsmtp_int_part = (struct libsmtp_part_struct *)libsmtp_session->mainpart_ptr;
    
    if (libsmtp_int_part /* FIXME*/
  
  /* We use calloc here to clear the memory. GLists are initialized when
     they point to NULL, so it must be cleared. */
  libsmtp_int_part = (struct libsmtp_part_struct *)calloc (1, sizeof(struct libsmtp_part_struct));
           
  if (libsmtp_int_part == NULL)
    return NULL;
                 
  /* The GStrings must be initialized */
  libsmtp_int_part->custom_type = g_string_new (NULL);
  libsmtp_int_part->custom_subtype = g_string_new (NULL);
  libsmtp_int_part->description = g_string_new (NULL);
  
  /* Now we check if any invalid arguments have been given.*/
  

  if ((libsmtp_int_type < 0) || (libsmtp_int_type > LIBSMTP_MAX_MIME))
  {
    libsmtp_session->ErrorCode = LIBSMTP_BADARGS;
    return LIBSMTP_BADARGS;
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
        return LIBSMTP_BADMIME;
      }
      /* Text types can have any encoding */
      if ((libsmtp_int_enc < 0) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }

      libsmtp_int_part->subtype=libsmtp_int_subtype;
    break;

    case (LIBSMTP_MIME_MESSAGE):
      if ((libsmtp_int_subtype < 1000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB1))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      }
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Message types can have any encoding */
      if ((libsmtp_int_enc < 0) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;

    case (LIBSMTP_MIME_IMAGE):
      if ((libsmtp_int_subtype < 2000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB2))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      }
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Image types must be in a non-text encoding */
      if ((libsmtp_int_enc < LIBSMTP_ENC_BINARY) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;                         

    case (LIBSMTP_MIME_AUDIO):
      if ((libsmtp_int_subtype < 3000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB3))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      }
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Audio types must be in a non-text encoding */
      if ((libsmtp_int_enc < LIBSMTP_ENC_BINARY) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;

    case (LIBSMTP_MIME_VIDEO):
      if ((libsmtp_int_subtype < 4000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB4))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      }
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Video types must be in a non-text encoding */
      if ((libsmtp_int_enc < LIBSMTP_ENC_BINARY) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;

    case (LIBSMTP_MIME_APPLICATION):
      if ((libsmtp_int_subtype < 5000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB5))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      }
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Application types must be in a non-text encoding */
      if ((libsmtp_int_enc < LIBSMTP_ENC_BINARY) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;

    case (LIBSMTP_MIME_MULTIPART):
      if ((libsmtp_int_subtype < 6000) || (libsmtp_int_subtype > LIBSMTP_MAX_MIME_SUB6))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      }
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Application types must be in a text encoding, and should only be
         7bit */
      if (libsmtp_int_enc != LIBSMTP_ENC_7BIT)
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;

    case (LIBSMTP_MIME_CUSTOM):
      if (libsmtp_int_subtype != LIBSMTP_MIME_SUB_CUSTOM)
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADMIME;
        return LIBSMTP_BADMIME;
      libsmtp_int_part->subtype=libsmtp_int_subtype;

      /* Custom type can have any encoding, of course */
      if ((libsmtp_int_enc < 0) || (libsmtp_int_enc > LIBSMTP_MAX_ENC))
      {
        libsmtp_session->ErrorCode = LIBSMTP_BADENCODING;
        return LIBSMTP_BADENCODING;
      }
    break;
  }
  return libsmtp_int_part;
}
                           
        
  int libsmtp_mime_type_custom (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);

  int libsmtp_mime_subtype_custom (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);

  int libsmtp_mime_content_desc (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);

#endif /* LIBSMTP_USE_MIME */
