#ifndef __OOWE_HTTP_SESSION__
#define __OOWE_HTTP_SESSION__
#include "oowe/Session.h"
#include "oowe/HttpPost.h"

namespace oowe {

class HttpSession : public Session
{
    public:
        using Session::Session;

        // Informations
        EASY_GET_LONG      (HttpAuthAvail,        HTTPAUTH_AVAIL        ) // Get a bitmask indicating the authentication method(s) available

        // Error options
        EASY_SET_BOOL      (FailOnError,          FAILONERROR           )    // Enable / disable fail on HTTP 4xx errors

        // Authentication
        EASY_SET_LONG      (HttpAuth,             HTTPAUTH              ) // Set HTTP server authentication methods
        EASY_SET_LONG      (ProxyAuth,            PROXYAUTH             ) // Set HTTP proxy authentication methods

        // SSL & Security options
        EASY_SET_BOOL      (SslEnableAlpn,        SSL_ENABLE_ALPN       ) // Set enable use of ALPN
        EASY_SET_BOOL      (SslEnableNpn,         SSL_ENABLE_NPN        ) // Set enable use of NPN
        EASY_SET_LONG      (GssApiDelegation,     GSSAPI_DELEGATION     ) // Set disable GSS-API delegation

        // Specific HTTP options
        EASY_SET_BOOL      (AutoReferer,          AUTOREFERER           ) // Set automatically set Referer: header
        EASY_SET_STRING    (AcceptEncoding,       ACCEPT_ENCODING       ) // Set accept-Encoding and automatic decompressing data
        EASY_SET_BOOL      (TransferEncoding,     TRANSFER_ENCODING     ) // Set request Transfer-Encoding
        EASY_SET_BOOL      (FollowLocation,       FOLLOWLOCATION        ) // Set follow HTTP redirects
        EASY_SET_BOOL      (UnrestrictedAuth,     UNRESTRICTED_AUTH     ) // Set do not restrict authentication to original host
        EASY_SET_LONG      (MaxRedirs,            MAXREDIRS             ) // Set maximum number of redirects to follow
        EASY_SET_LONG      (PostRedir,            POSTREDIR             ) // Set how to act on redirects after POST
        EASY_SET_BOOL      (Put,                  PUT                   ) // Set issue a HTTP PUT request
        EASY_SET_BOOL      (Post,                 POST                  ) // Set issue a HTTP POST request
        EASY_SET_STRING    (PostFields,           POSTFIELDS            ) // Set send a POST with this data
        EASY_SET_LONG      (PostFieldSize,        POSTFIELDSIZE         ) // Set the POST data is this big
        EASY_SET_OFFSET    (PostFieldSizeLarge,   POSTFIELDSIZE_LARGE   ) // Set the POST data is this big
        EASY_SET_STRING    (CopyPostFields,       COPYPOSTFIELDS        ) // Set send a POST with this data - and copy it
        EASY_SET_HTTP_POST (HttpPost,             HTTPPOST              ) // Set multipart formpost HTTP POST
        EASY_SET_STRING    (Referer,              REFERER               ) // Set referer: header
        EASY_SET_STRING    (UserAgent,            USERAGENT             ) // Set user-Agent: header
        EASY_SET_STRING_LST(HttpHeader,           HTTPHEADER            ) // Set custom HTTP headers
        EASY_SET_LONG      (HeaderOpt,            HEADEROPT             ) // Set control custom headers
        EASY_SET_STRING_LST(ProxyHeader,          PROXYHEADER           ) // Set custom HTTP headers sent to proxy
        EASY_SET_STRING_LST(Http200Aliases,       HTTP200ALIASES        ) // Set alternative versions of 200 OK
        EASY_SET_STRING    (Cookie,               COOKIE                ) // Set cookie(s) to send
        EASY_SET_STRING    (CookieFile,           COOKIEFILE            ) // Set file to read cookies from
        EASY_SET_STRING    (CookieJar,            COOKIEJAR             ) // Set file to write cookies to
        EASY_SET_BOOL      (CookieSession,        COOKIESESSION         ) // Set start a new cookie session
        EASY_SET_STRING    (CookieList,           COOKIELIST            ) // Set add or control cookies
        EASY_SET_BOOL      (HttpGet,              HTTPGET               ) // Set do a HTTP GET request
        EASY_SET_BOOL      (HttpVersion,          HTTP_VERSION          ) // Set hTTP version to use
        EASY_SET_BOOL      (IgnoreContentLength,  IGNORE_CONTENT_LENGTH ) // Set ignore Content-Length
        EASY_SET_BOOL      (HttpContentDecoding,  HTTP_CONTENT_DECODING ) // Set disable Content decoding
        EASY_SET_BOOL      (HttpTransferDecoding, HTTP_TRANSFER_DECODING) // Set disable Transfer decoding
        EASY_SET_BOOL      (Expect100TimeoutMs,   EXPECT_100_TIMEOUT_MS ) // Set 100-continue timeout
};

}; /* Namespace oowe */

#endif /* __OOWE_HTTP_SESSION__ */
