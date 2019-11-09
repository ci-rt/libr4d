#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <glib.h>
#include <uuid.h>

#include "auto-gen/soapH.h"

#include "r4d_intern.h"
#include "r4d.h"

/**
 * @defgroup libr4d_systems libr4d testsystem related functions
 * @ingroup libr4d_public
 * @brief public testsystem functions
 * @{
 */

/**
 * get testsystem name from struct r4d_systemid
 * @param systemid testsystem info struct
 * @returns testsystem name or NULL on failure
 */
gchar *r4d_systemid_name (struct r4d_systemid *systemid)
{
	return (!systemid) ? NULL : systemid->name;
}

/**
 * get testsystem binary UUID from struct r4d_systemid
 * @param systemid testsystem info struct
 * @returns testsystem binary UUID or NULL on failure
 */
guint8 *r4d_systemid_uuid (struct r4d_systemid *systemid)
{
	return (!systemid) ? NULL : systemid->uuid;
}

gchar *r4d_systemid_serial_host (struct r4d_systemid *systemid)
{
	return (!systemid) ? NULL : systemid->serial_host;
}

guint32 *r4d_systemid_serial_port (struct r4d_systemid *systemid)
{
	return (!systemid) ? NULL : &systemid->serial_port;
}

/**
 * free struct r4d_systemid
 * @param systemid testsystem info struct
 */
void r4d_systemid_free (struct r4d_systemid *systemid)
{
	if (!systemid)
		return;

	free (systemid->name);
	free (systemid);
}

static void free_testsystems (struct testsystems *systems)
{
	int i;

	if (systems)
	{
		if (systems->location)
			for (i = 0; i < systems->entries; i++)
				free (systems->location [i]);
		if (systems->name)
			for (i = 0; i < systems->entries; i++)
				free (systems->name [i]);
		if (systems->rack)
			for (i = 0; i < systems->entries; i++)
				free (systems->rack [i]);
		if (systems->uuid)
			for (i = 0; i < systems->entries; i++)
				free (systems->location [i]);
		if (systems->serial_host)
			for (i = 0; i < systems->entries; i++)
				free (systems->serial_host [i]);

		free (systems->location);
		free (systems->name);
		free (systems->port);
		free (systems->rack);
		free (systems->uuid);
		free (systems->uuid_len);
		free (systems->serial_port);
	}
	free (systems);
}

static struct testsystems *alloc_testsystems (int entries)
{
	struct testsystems *systems;

	systems = malloc (sizeof (*systems));
	if (!systems)
		return NULL;

	systems->entries = entries;
	/* add guard */
	entries++;
	systems->location = calloc (entries, sizeof (*systems->location));
	systems->name = calloc (entries, sizeof (*systems->name));
	systems->port = calloc (entries, sizeof (*systems->port));
	systems->rack = calloc (entries, sizeof (*systems->rack));
	systems->uuid = calloc (entries, sizeof (*systems->uuid));
	systems->uuid_len = calloc (entries, sizeof (*systems->uuid_len));
	systems->serial_host = calloc (entries, sizeof (*systems->serial_host));
	systems->serial_port = calloc (entries, sizeof (*systems->serial_port));
	if (!systems->location || !systems->name || !systems->port ||
	    !systems->rack || !systems->uuid || !systems->serial_host ||
	    !systems->serial_port)
	{
		free_testsystems (systems);
		systems = NULL;
	}

	return systems;
}

static struct testsystems *list_sys (const gchar *endpoint, const gchar *name)
{
	struct soap *soap = soap_new ();
	struct _ns1__list_systems req;
	struct _ns1__list_systemsResponse response;
	struct ns1__ArrayOflist_systemsResponselist *list;
	struct testsystems *systems = NULL;
	int i, ret;

	req.name = g_strdup (name);
	ret = soap_call___ns1__list_systems (soap, endpoint, NULL,
					     &req, &response);
	g_free (req.name);
	if (ret == SOAP_OK)
	{
		list = response.list;

		systems = alloc_testsystems (response.__sizelist);
		if (!systems)
			goto out;

		for (i = 0; i < response.__sizelist; i++)
		{
			gsize len;
			struct _ns1__get_serialResponse serialResponse;
			struct _ns1__get_serial get_serial;
			if (!list[i].name)
				break;
			systems->name [i] = g_strdup (list[i].name);
			systems->uuid [i] = g_base64_decode (list[i].uuid,
							     &len);
			systems->uuid_len [i] = len;
			/* retrieve serial connection */
			get_serial.system = g_strdup (list[i].name);
			ret = soap_call___ns1__get_serial (soap,
							   endpoint,
							   NULL,
							   &get_serial,
							   &serialResponse);
			g_free (get_serial.system);
			if (ret == SOAP_OK) {
				systems->serial_port [i] = serialResponse.port;
				systems->serial_host [i] = g_strdup(serialResponse.host);
			} else
				fprintf(stderr, "get serial console details failed\n");
		}
	}
	else
		soap_print_fault (soap, stderr);

out:
	soap_destroy (soap);
	soap_end (soap);
	soap_free (soap);

	return systems;
}

/**
 * list testsystem names
 * @param endpoint URL of r4d daemon SOAP endpoint
 * @returns testsystem names or NULL on failure
 */
gchar **r4d_list_systems (const gchar *endpoint)
{
	struct testsystems *systems = list_sys (endpoint, NULL);
	gchar **list;

	if (!systems)
		return NULL;

	list = systems->name;
	systems->name = NULL;
	free_testsystems (systems);

	return list;
}

/**
 * get array of testsystem info struct
 * @param endpoint URL of r4d daemon SOAP endpoint
 * @param name system name filter or NULL to list all systems
 * @returns array of testsystem info struct or NULL on failure
 */
struct r4d_systemid **r4d_list_systemids (const gchar *endpoint, const gchar *name)
{
	struct testsystems *systems = list_sys (endpoint, name);
	struct r4d_systemid **list;
	int i;

	if (!systems)
		return NULL;

	list = calloc (systems->entries + 1, sizeof (*list));

	for (i = 0; i < systems->entries; i++)
	{
		if (!systems->name [i])
			break;
		gsize len = UUID_BUFLEN;
		list [i] = calloc (1, sizeof (**list));
		if (!list [i])
			break;

		list [i]->name = g_strdup (systems->name [i]);
		list [i]->serial_host = g_strdup (systems->serial_host [i]);
		list [i]->serial_port = systems->serial_port [i];

		if (systems->uuid_len [i] < len)
			len = systems->uuid_len [i];
		memcpy (&list [i]->uuid, systems->uuid [i], len);
	}
	free_testsystems (systems);

	return list;
}

/**
 * get testsystem UUIDs
 * @param endpoint URL of r4d daemon SOAP endpoint
 * @returns testsystem UUIDs or NULL on failure
 */
gchar **r4d_list_uuids (const gchar *endpoint)
{
	struct testsystems *systems = list_sys (endpoint, NULL);
	gchar **list;
	int i;

	if (!systems)
		return NULL;

	list = calloc (systems->entries + 1, sizeof (*list));

	for (i = 0; i < systems->entries; i++)
	{
		gsize len = UUID_BUFLEN;
		uuid_t uu;
		gchar *out;

		out = calloc (1, 37);
		if (!out)
			break;
		if (systems->uuid_len [i] < len)
			len = systems->uuid_len [i];
		memcpy (uu, systems->uuid [i], len);
		uuid_unparse (uu, out);
		list [i] = out;
	}

	free_testsystems (systems);

	return list;
}

/** @} */
