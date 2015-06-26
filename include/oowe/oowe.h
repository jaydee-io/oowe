#ifndef __OOWE__
#define __OOWE__

#include <oowe/oowe_types.h>
#include <oowe/Session.h>
#include <oowe/MultiSession.h>
#include <oowe/HttpSession.h>
#include <oowe/HttpMultiSession.h>
#include <oowe/HttpPost.h>
#include <oowe/StdOutputStream.h>
#include <oowe/StdInputStream.h>

namespace oowe {

void init(long flags);
void init(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c);
void cleanup(void);

curl_version_info_data & version(CURLversion type);
char *                   version(void);

}; /* Namespace oowe */

std::ostream & operator <<(std::ostream & os, curl_version_info_data & info);

#endif /* __OOWE__ */
