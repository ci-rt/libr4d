#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <uuid.h>

#include "auto-gen/r4dBinding.nsmap"
#include "auto-gen/soapH.h"

#include "r4d_intern.h"
#include "r4d.h"

/**
 * @mainpage libr4d public functions
 * @defgroup libr4d
 *
 * This manual documents the public libr4d C API.
 */

/**
 * @defgroup libr4d_public public available libr4d functions
 * @ingroup libr4d
 * @brief public functions
 */

/**
 * @defgroup libr4d_base libr4d base functions
 * @ingroup libr4d_public
 * @brief public base functions
 * @{
 */

/**
 * get library communication protocol version
 * @returns library communication protocol version
 */
int version (void)
{
	return 1;
}

/** @} */
