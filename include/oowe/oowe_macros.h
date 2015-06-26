#ifndef __OOWE_MACROS__
#define __OOWE_MACROS__

#define CURL_VERSION_GREATER(major, minor, patch) (\
    (LIBCURL_VERSION_MAJOR >  major) || \
    (LIBCURL_VERSION_MAJOR == major && LIBCURL_VERSION_MINOR >  minor) || \
    (LIBCURL_VERSION_MAJOR == major && LIBCURL_VERSION_MINOR == minor && LIBCURL_VERSION_PATCH >= patch))

#define GET(method, info, methodType, curlType) \
    inline methodType get ## method(void) \
    { \
        return get<curlType>(info); \
    }
#define EASY_GET_BOOL(      method, info) GET(method, CURLINFO_ ## info, bool,         long               )
#define EASY_GET_LONG(      method, info) GET(method, CURLINFO_ ## info, long,         long               )
#define EASY_GET_DOUBLE(    method, info) GET(method, CURLINFO_ ## info, double,       double             )
#define EASY_GET_VOID(      method, info) GET(method, CURLINFO_ ## info, void *,       void *             )
#define EASY_GET_STRING(    method, info) GET(method, CURLINFO_ ## info, const char *, const char *       )
#define EASY_GET_STRING_LST(method, info) GET(method, CURLINFO_ ## info, StringList,   struct curl_slist *)
#define EASY_GET_DURATION(  method, info) \
    inline Duration get ## method(void) \
    { \
        return Duration(get<double>(CURLINFO_ ## info)); \
    }
#define EASY_GET_TIME(      method, info) \
    inline Time get ## method(void) \
    { \
        return Time(std::chrono::seconds(get<long>(CURLINFO_ ## info))); \
    }




#define SET(method, opt, methodType, curlType) \
    inline void set ## method(methodType arg) \
    { \
        set<curlType>(opt, arg); \
    }
#define EASY_SET_BOOL(      method, info) SET(method, CURLOPT_ ## info, bool,         long                  )
#define EASY_SET_LONG(      method, info) SET(method, CURLOPT_ ## info, long,         long                  )
#define EASY_SET_VOID(      method, info) SET(method, CURLOPT_ ## info, void *,       void *                )
#define EASY_SET_FILE(      method, info) SET(method, CURLOPT_ ## info, FILE *,       FILE *                )
#define EASY_SET_OFFSET(    method, info) SET(method, CURLOPT_ ## info, offset_t,     curl_off_t            )
#define EASY_SET_STRING(    method, info) SET(method, CURLOPT_ ## info, const char *, const char *          )
#define EASY_SET_STRING_LST(method, info) SET(method, CURLOPT_ ## info, StringList &, struct curl_slist *   )
#define EASY_SET_HTTP_POST( method, info) SET(method, CURLOPT_ ## info, HttpPost &  , struct curl_httppost *)

#define MULTI_SET_LONG(method, info) SET(method, CURLMOPT_ ## info, long, long)

#endif /* __OOWE_MACROS__ */
