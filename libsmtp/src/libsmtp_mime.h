
#ifdef LIBSMTP_USE_MIME

  #ifndef __G_LIB_H__
    #include <glib.h>
  #endif

  #ifndef LIB_SMTP_MIME_H

    #define LIB_SMTP_MIME_H

    /* MIME types */

    #define LIBSMTP_MIME_TEXT	0
    #define LIBSMTP_MIME_MESSAGE	1
    #define LIBSMTP_MIME_IMAGE	2
    #define LIBSMTP_MIME_AUDIO	3
    #define LIBSMTP_MIME_VIDEO	4
    #define LIBSMTP_MIME_APPLICATION	5
    #define LIBSMTP_MIME_MULTIPART	6
    #define LIBSMTP_MIME_CUSTOM	7

    #define LIBSMTP_MAX_MIME	7
    
    /* MIME subtypes */

    /* 0 to 999 are TEXT subtypes */

    #define LIBSMTP_MIME_SUB_PLAIN	0
    #define LIBSMTP_MIME_SUB_HTML	1
    #define LIBSMTP_MIME_SUB_ENGLISH	2
    #define LIBSMTP_MIME_SUB_RICHTEXT	3
    
    #define LIBSMTP_MAX_MIME_SUB0	3

    /* 1000 to 1999 are MESSAGE subtypes */

    #define LIBSMTP_MIME_SUB_RFC822	1000
    #define LIBSMTP_MIME_SUB_PARTIAL	1001

    #define LIBSMTP_MAX_MIME_SUB1	1001

    /* 2000 to 2999 are IMAGE subtypes */

    #define LIBSMTP_MIME_SUB_GIF	2000
    #define LIBSMTP_MIME_SUB_JPG	2001
    #define LIBSMTP_MIME_SUB_PNG	2002
    #define LIBSMTP_MIME_SUB_TIFF	2003
    #define LIBSMTP_MIME_SUB_MS-BMP	2004
    #define LIBSMTP_MIME_SUB_XBITMAP	2005
    #define LIBSMTP_MIME_SUB_XPIXMAP	2006
    #define LIBSMTP_MIME_SUB_PORTABLE_ANYMAP	2007
    #define LIBSMTP_MIME_SUB_PORTABLE_BITMAP	2008
    #define LIBSMTP_MIME_SUB_PORTABLE_GRAYMAP	2009
    #define LIBSMTP_MIME_SUB_PORTABLE_PIXMAP	2010

    #define LIBSMTP_MAX_MIME_SUB2	2010

    /* 3000 to 3999 are AUDIO subtypes */

    #define LIBSMTP_MIME_SUB_MPEG	3000
    #define LIBSMTP_MIME_SUB_MIDI	3001
    #define LIBSMTP_MIME_SUB_WAV	3002
    #define LIBSMTP_MIME_SUB_AIFF	3003

    #define LIBSMTP_MAX_MIME_SUB3	3003

    /* 4000 to 4999 are VIDEO subtypes */

    #define LIBSMTP_MIME_SUB_MPEG	4000
    #define LIBSMTP_MIME_SUB_MSVIDEO	4001
    #define LIBSMTP_MIME_SUB_QUICKTIME	4002
    #define LIBSMTP_MIME_SUB_FLI	4003

    #define LIBSMTP_MAX_MIME_SUB4	4003

    /* 5000 to 5999 are APPLICATION subtypes */

    #define LIBSMTP_MIME_SUB_RTF	5000
    #define LIBSMTP_MIME_SUB_POSTSCRIPT	5001
    #define LIBSMTP_MIME_SUB_PDF	5002
    #define LIBSMTP_MIME_SUB_ZIP	5003
    #define LIBSMTP_MIME_SUB_DEBIAN_PACKAGE	5004
    #define LIBSMTP_MIME_SUB_EXECUTABLE	5005
    #define LIBSMTP_MIME_SUB_GTAR	5006
    #define LIBSMTP_MIME_SUB_SHELLSCRIPT	5007
    #define LIBSMTP_MIME_SUB_TAR	5008
    #define LIBSMTP_MIME_SUB_OCTET_STREAM	5009

    #define LIBSMTP_MAX_MIME_SUB5	5008

    /* 6000 to 6999 are MULTIPART subtypes */

    #define LIBSMTP_MIME_SUB_MIXED	6000
    #define LIBSMTP_MIME_SUB_PARALLEL	6001
    #define LIBSMTP_MIME_SUB_DIGEST	6002
    #define LIBSMTP_MIME_SUB_ALTERNATIVE	6003

    #define LIBSMTP_MAX_MIME_SUB6	6003

    /* 30000 (for signed ints!!) is the CUSTOM subtype */

    #define LIBSMTP_MIME_SUB_CUSTOM	30000


    /* Encoding types */

    #define LIBSMTP_ENC_7BIT	0
    #define LIBSMTP_ENC_8BIT	1 /* not really used in current version */
    #define LIBSMTP_ENC_BINARY	2 /* not really used in current version */
    #define LIBSMTP_ENC_BASE64	3
    #define LIBSMTP_ENC_QUOTED	4 /* not really used in current version */

    #define LIBSMTP_MAX_ENC	4

  #endif /* LIB_SMTP_MIME_H */
  
  int libsmtp_mime_new (struct libsmtp_part_struct *, int, int, int,\
        struct libsmtp_session_struct *libsmtp_session);
        
  int libsmtp_mime_type_custom (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);

  int libsmtp_mime_subtype_custom (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);

  int libsmtp_mime_content_desc (char *, struct libsmtp_part_struct *,\
        struct libsmtp_session_struct *);
  
  struct libsmtp_part_struct {
    int internal_id;	/* internal id number */
    int type;	/* MIME type */
    GString *custom_type;	/* optional custom MIME type */
    int subtype;	/* MIME subtype */
    GString *custom_subtype;	/* optional custom MIME subtype */
    int encoding;	/* MIME transfer encoding */
    GString *description;	/* MIME part description */
  }

#endif /* LIBSMTP_USE_MIME */