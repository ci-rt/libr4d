#ifndef __R4D_H__
#define __R4D_H__

#include <glib.h>

#define UUID_BUFLEN	16

struct r4d_systemid;

gchar *r4d_systemid_name (struct r4d_systemid *systemid);
guint8 *r4d_systemid_uuid (struct r4d_systemid *systemid);
gchar *r4d_systemid_serial_host (struct r4d_systemid *systemid);
guint32 *r4d_systemid_serial_port (struct r4d_systemid *systemid);

void r4d_systemid_free (struct r4d_systemid *systemid);

int r4d_power_change (const gchar *endpoint, const gchar *name, gboolean state);
int r4d_power_status (const gchar *endpoint, const gchar *name);

gchar **r4d_list_systems (const gchar *endpoint);
struct r4d_systemid **r4d_list_systemids (const gchar *endpoint, const gchar *name);
gchar **r4d_list_uuids (const gchar *endpoint);

int r4d_shutdown (const gchar *endpoint, const gchar *name);

#endif /* __R4D_H__ */
