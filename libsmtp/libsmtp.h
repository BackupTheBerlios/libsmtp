
#ifndef LIB_SMTP_H

#define LIB_SMTP_H

/* These flags show what the server can do */

#define LIBSMTP_HAS_TLS	1
#define LIBSMTP_HAS_8BIT	2
#define LIBSMTP_HAS_AUTH	4
#define LIBSMTP_HAS_PIPELINIG	8
#define LIBSMTP_HAS_SIZE	16
#define LIBSMTP_HAS_DSN		32

/* These are the error definitions */

/* Error codes below 1024 are fatal errors - the socket will be closed */
#define LIBSMTP_SOCKETNOCREATE	1
#define LIBSMTP_HOSTNOTFOUND	2
#define LIBSMTP_CONNECTERR	3
#define LIBSMTP_ERRORREADFATAL	4
#define LIBSMTP_NOTWELCOME	5

/* This structure defines one libsmtp session */

struct libsmtp_session_struct {
  int connection;	/* internal connection number */
  int socket;		/* socket handle */

  GString *From;	/* From address */
  GList *To;		/* All recipients addresses */
  GList *CC;		/* All Carbon Copy recipients addresses */
  GList *BCC;		/* All Blind Carbon Copy recipients addresses */
  int numfailedTo;	/* number of rejected recipients */
  int numfailedCC;	/* number of rejected CC recipients */
  int numfailedBCC;	/* number of rejected BCC recipients */
  GList *failedTo;	/* List of failed recipients containing the response for
  			   each failure */
  GList *failedCC;	/* The same for CC recipients */
  GList *failedBCC;	/* And for BCC recipients */

  GString *Subject;	/* Mail subject */
  GString *LastResponse;	/* Last SMTP response string from server */
  int LastResponseCode;	/* Last SMTP response code from server */
  int ErrorCode;	/* Internal libsmtp error code from last error */

  int DialogueSent;	/* Number of SMTP dialogue lines sent */
  int DialogueBytesSent;	/* Bytes of SMTP dialogue data sent */
  int HeadersSent;  	/* Number of header lines sent */
  int HeadersBytesSent;	/* Bytes of header data sent */
  int BodyBytesSent;	/* Bytes of body data sent */
};

struct libsmtp_session_struct *libsmtp_connect (char *, unsigned int, unsigned int);

int libsmtp_errno(struct libsmtp_session_struct *);

const char *libsmtp_strerr (struct libsmtp_session_struct *);

int libsmtp_add_recipient (int, char *, struct libsmtp_session_struct *);

int libsmtp_set_environment (char *, char *, struct libsmtp_session_struct *);

int libsmtp_dialogue_send (char *, struct libsmtp_session_struct *);

int libsmtp_dialogue (struct libsmtp_session_struct *);

int libsmtp_header_send (char *, struct libsmtp_session_struct *);

int libsmtp_body_send (char *, struct libsmtp_session_struct *);

int libsmtp_close (struct libsmtp_session_struct *);

int libsmtp_free (struct libsmtp_session_struct *);

#endif
