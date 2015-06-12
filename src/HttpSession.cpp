#include "oowe/HttpSession.h"
#include <utility>

namespace oowe {

// Session informations
#define GET_INFO(method, info, methodType, curlType) \
    methodType HttpSession::method(void) \
    { \
        return get<curlType>(CURLINFO_ ## info); \
    }

GET_INFO(getHttpAuthAvail, HTTPAUTH_AVAIL, long, long)

// Session options
#define SET_OPT(method, opt, methodType, curlType) \
    void HttpSession::method(methodType arg) \
    { \
        set<curlType>(CURLOPT_ ## opt, arg); \
    }

// Error options
SET_OPT(setFailOnError, FAILONERROR, bool, long)

// Authentication
SET_OPT(setHttpAuth,  HTTPAUTH , long, long)
SET_OPT(setProxyAuth, PROXYAUTH, long, long)

// SSL & Security options
SET_OPT(setSslEnableAlpn,    SSL_ENABLE_ALPN  , bool, long)
SET_OPT(setSslEnableNpn,     SSL_ENABLE_NPN   , bool, long)
SET_OPT(setGssApiDelegation, GSSAPI_DELEGATION, long, long)

// Specific HTTP options
SET_OPT(setAutoReferer,          AUTOREFERER           , bool        , long                  )
SET_OPT(setAcceptEncoding,       ACCEPT_ENCODING       , const char *, const char *          )
SET_OPT(setTransferEncoding,     TRANSFER_ENCODING     , bool        , long                  )
SET_OPT(setFollowLocation,       FOLLOWLOCATION        , bool        , long                  )
SET_OPT(setUnrestrictedAuth,     UNRESTRICTED_AUTH     , bool        , long                  )
SET_OPT(setMaxRedirs,            MAXREDIRS             , long        , long                  )
SET_OPT(setPostRedir,            POSTREDIR             , long        , long                  )
SET_OPT(setPut,                  PUT                   , bool        , long                  )
SET_OPT(setPost,                 POST                  , bool        , long                  )
SET_OPT(setPostFields,           POSTFIELDS            , const char *, const char *          )
SET_OPT(setPostFieldSize,        POSTFIELDSIZE         , long        , long                  )
SET_OPT(setPostFieldSizeLarge,   POSTFIELDSIZE_LARGE   , offset_t    , curl_off_t            )
SET_OPT(setCopyPostFields,       COPYPOSTFIELDS        , const char *, const char *          )
SET_OPT(setReferer,              REFERER               , const char *, const char *          )
SET_OPT(setUserAgent,            USERAGENT             , const char *, const char *          )
SET_OPT(setHttpHeader,           HTTPHEADER            , StringList &, struct curl_slist *   )
SET_OPT(setHeaderOpt,            HEADEROPT             , long        , long                  )
SET_OPT(setProxyHeader,          PROXYHEADER           , StringList &, struct curl_slist *   )
SET_OPT(setHttp200Aliases,       HTTP200ALIASES        , StringList &, struct curl_slist *   )
SET_OPT(setCookie,               COOKIE                , const char *, const char *          )
SET_OPT(setCookieFile,           COOKIEFILE            , const char *, const char *          )
SET_OPT(setCookieJar,            COOKIEJAR             , const char *, const char *          )
SET_OPT(setCookieSession,        COOKIESESSION         , bool        , long                  )
SET_OPT(setCookieList,           COOKIELIST            , const char *, const char *          )
SET_OPT(setHttpGet,              HTTPGET               , bool        , long                  )
SET_OPT(setHttpVersion,          HTTP_VERSION          , long        , long                  )
SET_OPT(setIgnoreContentLength,  IGNORE_CONTENT_LENGTH , bool        , long                  )
SET_OPT(setHttpContentDecoding,  HTTP_CONTENT_DECODING , bool        , long                  )
SET_OPT(setHttpTransferDecoding, HTTP_TRANSFER_DECODING, bool        , long                  )
SET_OPT(setExpect100TimeoutMs,   EXPECT_100_TIMEOUT_MS , long        , long                  )

}; /* Namespace oowe */
