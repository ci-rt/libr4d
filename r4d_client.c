#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <locale.h>
#include <r4d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid.h>

#include <glib/gi18n.h>

static gboolean verbose = FALSE;
static gchar *endpoint = "http://localhost:8008/";

static GOptionEntry entries[] =
{
	{"endpoint", 'e', 0, G_OPTION_ARG_STRING, &endpoint, "r4d endpoint",
	 "URI of r4d endpoint"},
	{"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "be verbose", NULL},
	{NULL}
};

int list_sysids (gchar *endpoint, gchar *name)
{
	struct r4d_systemid **sysids, **tmpid;
	guint32 *port;

	g_info ("list_systemids(%s):", name);

	sysids = r4d_list_systemids (endpoint, name);
	if (!sysids)
	{
		g_warning ("list_systemids() failed.");
		return -1;
	}

	for (tmpid = sysids; *tmpid; tmpid++)
	{
		uuid_t uu;
		gchar out [37];

		g_print ("name  : %s\n", r4d_systemid_name (*tmpid));
		memcpy (uu, r4d_systemid_uuid (*tmpid), UUID_BUFLEN);

		uuid_unparse (uu, out);
		g_print ("uuid  : %s\n", out);

		port = r4d_systemid_serial_port (*tmpid);
		if (port)
			g_print ("serial: %s:%d\n",
				 r4d_systemid_serial_host (*tmpid), *port);

		g_print ("power: %d\n", r4d_power_status (endpoint, r4d_systemid_name (*tmpid)));
		r4d_power_change (endpoint, r4d_systemid_name (*tmpid), 1);
	}
	free (sysids);

	return 0;
}

int main (int argc, char *argv [])
{
	GOptionContext *context;
	GError *error = NULL;

	setlocale (LC_ALL, "");
	bindtextdomain (GETTEXT_PACKAGE, DATADIR "/locale");
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	context = g_option_context_new ("- r4d client");
	g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);

	if (!g_option_context_parse (context, &argc, &argv, &error))
	{
		g_warning ("option parsing failed: %s\n", error->message);
		return 1;
	}

	g_info ("endpoint: %s\n", endpoint);
	g_info ("verbose : %s\n", (verbose) ? _("true") : _("false"));

	list_sysids (endpoint, (argc > 1) ? argv [1] : NULL);

	return 0;
}
