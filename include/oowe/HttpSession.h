#ifndef __OOWE_HTTP_SESSION__
#define __OOWE_HTTP_SESSION__
#include "oowe/Session.h"

namespace oowe {

class HttpSession : public Session
{
    public:
        using Session::Session;

        // Informations
        long getHttpAuthAvail(void); // Get a bitmask indicating the authentication method(s) available

        // Error options
        void setFailOnError(bool enable); // Enable / disable fail on HTTP 4xx errors

        // Authentication options
        void setHttpAuth (long auth); // Set HTTP server authentication methods
        void setProxyAuth(long auth); // Set HTTP proxy authentication methods

        // SSL & Security options
        void setSslEnableAlpn   (bool enable); // Set enable use of ALPN
        void setSslEnableNpn    (bool enable); // Set enable use of NPN
        void setGssApiDelegation(long level ); // Set disable GSS-API delegation

        // Specific HTTP options
        void setAutoReferer         (bool         enable      ); // Set automatically set Referer: header
        void setAcceptEncoding      (const char * enc         ); // Set accept-Encoding and automatic decompressing data
        void setTransferEncoding    (bool         enable      ); // Set request Transfer-Encoding
        void setFollowLocation      (bool         enable      ); // Set follow HTTP redirects
        void setUnrestrictedAuth    (bool         enable      ); // Set do not restrict authentication to original host
        void setMaxRedirs           (long         amount      ); // Set maximum number of redirects to follow
        void setPostRedir           (long         redir       ); // Set how to act on redirects after POST
        void setPut                 (bool         enable      ); // Set issue a HTTP PUT request
        void setPost                (bool         enable      ); // Set issue a HTTP POST request
        void setPostFields          (const char * postdata    ); // Set send a POST with this data
        void setPostFieldSize       (long         size        ); // Set the POST data is this big
        void setPostFieldSizeLarge  (offset_t     size        ); // Set the POST data is this big
        void setCopyPostFields      (const char * data        ); // Set send a POST with this data - and copy it
        void setReferer             (const char * referer     ); // Set referer: header
        void setUserAgent           (const char * ua          ); // Set user-Agent: header
        void setHttpHeader          (StringList & headers     ); // Set custom HTTP headers
        void setHeaderOpt           (long         headeropt   ); // Set control custom headers
        void setProxyHeader         (StringList & headers     ); // Set custom HTTP headers sent to proxy
        void setHttp200Aliases      (StringList & alias       ); // Set alternative versions of 200 OK
        void setCookie              (const char * cookie      ); // Set cookie(s) to send
        void setCookieFile          (const char * filename    ); // Set file to read cookies from
        void setCookieJar           (const char * filename    ); // Set file to write cookies to
        void setCookieSession       (bool         enable      ); // Set start a new cookie session
        void setCookieList          (const char * cookie      ); // Set add or control cookies
        void setHttpGet             (bool         enable      ); // Set do a HTTP GET request
        void setHttpVersion         (long         version     ); // Set hTTP version to use
        void setIgnoreContentLength (bool         enable      ); // Set ignore Content-Length
        void setHttpContentDecoding (bool         enable      ); // Set disable Content decoding
        void setHttpTransferDecoding(bool         enable      ); // Set disable Transfer decoding
        void setExpect100TimeoutMs  (long         milliseconds); // Set 100-continue timeout
};

}; /* Namespace oowe */

#endif /* __OOWE_HTTP_SESSION__ */
