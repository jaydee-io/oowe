#ifndef __OOWE_MACROS__
#define __OOWE_MACROS__

#define CURL_VERSION_GREATER(major, minor, patch) (\
	(LIBCURL_VERSION_MAJOR >  major) || \
	(LIBCURL_VERSION_MAJOR == major && LIBCURL_VERSION_MINOR >  minor) || \
	(LIBCURL_VERSION_MAJOR == major && LIBCURL_VERSION_MINOR == minor && LIBCURL_VERSION_PATCH >= patch))

#endif /* __OOWE_MACROS__ */
