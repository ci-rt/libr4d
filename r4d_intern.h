#ifndef __R4D_INTERN_H__
#define __R4D_INTERN_H__

#define GETTEXT_PACKAGE "libr4d"
#include <glib.h>
#include <glib/gi18n-lib.h>

#define UUID_BUFLEN	16

struct testsystems
{
	gchar		**location;
	gchar		**name;
	int		*port;
	gchar		**rack;
	guchar		**uuid;
	gsize		*uuid_len;
	gchar **serial_host;
	int *serial_port;
	int		entries;
};

struct r4d_systemid {
	gchar *name;
	guint8 uuid [UUID_BUFLEN];
	gchar *serial_host;
	guint32 serial_port;
};

#endif /* __R4D_H__ */
