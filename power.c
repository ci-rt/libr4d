#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <uuid.h>

#include "auto-gen/soapH.h"

#include "r4d_intern.h"
#include "r4d.h"

/**
 * @defgroup libr4d_power libr4d testsystem power functions
 * @ingroup libr4d_public
 * @brief public power functions
 * @{
 */

/**
 * change powerstate of a testsystem
 * @param endpoint URL of r4d daemon SOAP endpoint
 * @param name testsystem name
 * @param state new powerstate
 * @returns 0 or -1 on failure
 */
int r4d_power_change (const gchar *endpoint, const gchar *name, gboolean state)
{
	struct soap *soap = soap_new ();
	struct _ns1__power_change req;
	struct _ns1__power_changeResponse resp;
	int ret = -1;

	if (!name)
		return -1;

	req.system = g_strdup (name);
	req.state = state;

	ret = soap_call___ns1__power_change (soap, endpoint, NULL,
					     &req, &resp);
	if (ret == SOAP_OK)
		ret = 0;
	else
		soap_print_fault (soap, stderr);

	g_free (req.system);
	soap_destroy (soap);
	soap_end (soap);
	soap_free (soap);

	return ret;
}

/**
 * get powerstate of a testsystem
 * @param endpoint URL of r4d daemon SOAP endpoint
 * @param name testsystem name
 * @returns powerstate of a testsystem or -1 on failure
 */
int r4d_power_status (const gchar *endpoint, const gchar *name)
{
	struct soap *soap = soap_new ();
	struct _ns1__power_status req;
	struct _ns1__power_statusResponse resp;
	int ret, status = -1;

	if (!name)
		return -1;

	req.system = g_strdup (name);

	ret = soap_call___ns1__power_status (soap, endpoint, NULL,
					     &req, &resp);
	if (ret == SOAP_OK)
		status = resp.state;
	else
		soap_print_fault (soap, stderr);

	g_free (req.system);
	soap_destroy (soap);
	soap_end (soap);
	soap_free (soap);

	return status;
}

/**
 * graceful shutdown of a testsystem
 * @param endpoint URL of r4d daemon SOAP endpoint
 * @param name testsystem name
 * @returns 0 or -1 on failure
 */
int r4d_shutdown (const gchar *endpoint, const gchar *name)
{
	struct soap *soap = soap_new ();
	struct _ns1__shutdown req;
	struct _ns1__shutdownResponse resp;
	int ret = -1;

	if (!name)
		return -1;

	req.system = g_strdup (name);

	ret = soap_call___ns1__shutdown (soap, endpoint, NULL,
					 &req, &resp);
	if (ret == SOAP_OK)
		ret = 0;
	else
		soap_print_fault (soap, stderr);

	g_free (req.system);
	soap_destroy (soap);
	soap_end (soap);
	soap_free (soap);

	return ret;
}

/** @} */
