
#ifndef LIB_SMTP_H

#define LIB_SMTP_H

/* These flags show what the server can do */

#define LIBSMTP_HAS_TLS	1
#define LIBSMTP_HAS_8BIT	2
#define LIBSMTP_HAS_AUTH	4
#define LIBSMTP_HAS_PIPELINING	8
#define LIBSMTP_HAS_SIZE	16
#define LIBSMTP_HAS_DSN		32
#define LIBSMTP_HAS_ETRN	64
#define LIBSMTP_HAS_ENHANCEDSTATUSCODES	128

/* Recipient types for libsmtp_add_recipient */

#define LIBSMTP_REC_MAX	2

#define LIBSMTP_REC_TO	0
#define LIBSMTP_REC_CC	1
#define LIBSMTP_REC_BCC	2

/* These are the error definitions */

/* Error codes below 1024 are fatal errors - the socket will be closed */
#define LIBSMTP_NOERR		0
#define LIBSMTP_SOCKETNOCREATE	1
#define LIBSMTP_HOSTNOTFOUND	2
#define LIBSMTP_CONNECTERR	3
#define LIBSMTP_ERRORREADFATAL	4
#define LIBSMTP_NOTWELCOME	5
#define LIBSMTP_WHATSMYHOSTNAME	6
#define LIBSMTP_ERRORSENDFATAL	7

/* Codes >= 1024 are errors that are not fatal to the whole SMTP session */
#define LIBSMTP_ERRORREAD	1024
#define LIBSMTP_ERRORSEND	1025
#define LIBSMTP_BADARGS		1026

#define LIBSMTP_UNDEFERR	10000 /* ErrorCode was undefined!! */
/* This structure defines one libsmtp session */

struct libsmtp_session_struct {
  int serverflags;	/* Server capability flags */
  int socket;		/* socket handle */

  GString *From;	/* From address */
  GList *To;		/* All recipients addresses */
  GList *CC;		/* All Carbon Copy recipients addresses */
  GList *BCC;		/* All Blind Carbon Copy recipients addresses */
  int NumFailedTo;	/* number of rejected recipients */
  int NumFailedCC;	/* number of rejected CC recipients */
  int NumFailedBCC;	/* number of rejected BCC recipients */
  GList *FailedTo;	/* List of failed recipients containing the response for
  			   each failure */
  GList *FailedCC;	/* The same for CC recipients */
  GList *FailedBCC;	/* And for BCC recipients */

  GString *Subject;	/* Mail subject */
  GString *LastResponse;	/* Last SMTP response string from server */
  int LastResponseCode;	/* Last SMTP response code from server */
  int ErrorCode;	/* Internal libsmtp error code from last error */

  int DialogueSent;	/* Number of SMTP dialogue lines sent */
  int DialogueBytes;	/* Bytes of SMTP dialogue data sent */
  int HeadersSent;  	/* Number of header lines sent */
  int HeaderBytes;	/* Bytes of header data sent */
  int BodyBytes;	/* Bytes of body data sent */
};

struct libsmtp_session_struct *libsmtp_session_initialize (void);

int libsmtp_connect (char *, unsigned int, unsigned int, struct libsmtp_session_struct *);

int libsmtp_errno(struct libsmtp_session_struct *);

const char *libsmtp_strerr (struct libsmtp_session_struct *);

int libsmtp_add_recipient (int, char *, struct libsmtp_session_struct *);

int libsmtp_set_environment (char *, char *, unsigned int, struct libsmtp_session_struct *);

int libsmtp_dialogue_send (char *, struct libsmtp_session_struct *);

int libsmtp_dialogue (struct libsmtp_session_struct *);

int libsmtp_header_send (char *, struct libsmtp_session_struct *);

int libsmtp_body_send (char *, struct libsmtp_session_struct *);

int libsmtp_close (struct libsmtp_session_struct *);

int libsmtp_free (struct libsmtp_session_struct *);

#endif
