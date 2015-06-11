#include "oowe/Session.h"
#include <utility>

namespace oowe {

Session::Session(void)
: curl(curl_easy_init())
{
    if(!curl)
    {
        std::cerr << "Erreur dans curl_easy_init()" << std::endl;
        // TODO Throw error
    }
}

Session::Session(const char * url)
: Session()
{
    setUrl(url);
}

Session::Session(const Session & rhs)
: curl(curl_easy_duphandle(rhs.curl))
{
}

Session::Session(Session && rhs)
: curl(nullptr)
{
    std::swap(curl, rhs.curl);
}

Session::~Session(void)
{
    if(curl)
        curl_easy_cleanup(curl);
}

Session & Session::operator =(const Session & rhs)
{
    if(this != &rhs)
    {
        if(curl)
        {
            curl_easy_cleanup(curl);
            curl = nullptr;
        }
        curl = curl_easy_duphandle(rhs.curl);
    }

    return *this;
}

Session & Session::operator =(Session && rhs)
{
    if(this != &rhs)
    {
        if(curl)
        {
            curl_easy_cleanup(curl);
            curl = nullptr;
        }
        std::swap(curl, rhs.curl);
    }

    return *this;
}

String Session::escape(const char * str, int len)
{
    const char * escStr = curl_easy_escape(curl, str, len);

    return String(escStr);
}

String Session::escape(const std::string & str)
{
    const char * escStr = curl_easy_escape(curl, str.c_str(), str.size());

    return String(escStr);
}

String Session::escape(const String & str)
{
    const char * escStr = curl_easy_escape(curl, str.c_str(), str.size());

    return String(escStr);
}

String Session::unescape(const char * str, int len)
{
    int unescLen = 0;
    const char * unescStr = curl_easy_unescape(curl, str, len, &unescLen);

    return String(unescStr, unescLen);
}

String Session::unescape(const std::string & str)
{
    int unescLen = 0;
    const char * unescStr = curl_easy_unescape(curl, str.c_str(), str.size(), &unescLen);

    return String(unescStr, unescLen);
}

String Session::unescape(const String & str)
{
    int unescLen = 0;
    const char * unescStr = curl_easy_unescape(curl, str.c_str(), str.size(), &unescLen);

    return String(unescStr, unescLen);
}

void Session::pause(int action)
{
    OOWE_CHECK_ERROR_THROW(curl_easy_pause(curl, action));
}

void Session::perform(void)
{
    OOWE_CHECK_ERROR_THROW(curl_easy_perform(curl));
}

size_t Session::recv(void * buffer, size_t len)
{
    size_t n;

    OOWE_CHECK_ERROR_THROW(curl_easy_recv(curl, buffer, len, &n));

    return n;
}

size_t Session::send(const void * buffer, size_t len)
{
    size_t n;

    OOWE_CHECK_ERROR_THROW(curl_easy_send(curl, buffer, len, &n));

    return n;
}

void Session::reset(void)
{
    curl_easy_reset(curl);
}

// Session informations
#define GET_INFO(method, info, methodType, curlType) \
    methodType Session::method(void) \
    { \
        return get<curlType>(CURLINFO_ ## info); \
    }

#define GET_DURATION_INFO(method, info, methodType, curlType) \
    methodType Session::method(void) \
    { \
        return Duration(get<curlType>(CURLINFO_ ## info)); \
    }


GET_INFO         (getEffectiveUrl,          EFFECTIVE_URL          , const char *       ,   const char *       )
GET_INFO         (getResponseCode,          RESPONSE_CODE          , long               ,   long               )
GET_INFO         (getHttpConnectCode,       HTTP_CONNECTCODE       , long               ,   long               )
GET_DURATION_INFO(getTotalTime,             TOTAL_TIME             , Session::Duration  ,   double             )
GET_DURATION_INFO(getNameLookupTime,        NAMELOOKUP_TIME        , Session::Duration  ,   double             )
GET_DURATION_INFO(getConnectTime,           CONNECT_TIME           , Session::Duration  ,   double             )
GET_DURATION_INFO(getAppConnectTime,        APPCONNECT_TIME        , Session::Duration  ,   double             )
GET_DURATION_INFO(getPretransferTime,       PRETRANSFER_TIME       , Session::Duration  ,   double             )
GET_DURATION_INFO(getStarttransferTime,     STARTTRANSFER_TIME     , Session::Duration  ,   double             )
GET_DURATION_INFO(getRedirectTime,          REDIRECT_TIME          , Session::Duration  ,   double             )
GET_INFO         (getRedirectCount,         REDIRECT_COUNT         , long               ,   long               )
GET_INFO         (getRedirectUrl,           REDIRECT_URL           , const char *       ,   const char *       )
GET_INFO         (getSizeUpload,            SIZE_UPLOAD            , double             ,   double             )
GET_INFO         (getSizeDownload,          SIZE_DOWNLOAD          , double             ,   double             )
GET_INFO         (getSpeedDownload,         SPEED_DOWNLOAD         , double             ,   double             )
GET_INFO         (getSpeedUpload,           SPEED_UPLOAD           , double             ,   double             )
GET_INFO         (getHeaderSize,            HEADER_SIZE            , long               ,   long               )
GET_INFO         (getRequestSize,           REQUEST_SIZE           , long               ,   long               )
GET_INFO         (getSslVerifyResult,       SSL_VERIFYRESULT       , bool               ,   bool               )
GET_INFO         (getSslEngines,            SSL_ENGINES            , StringList         ,   struct curl_slist *)
GET_INFO         (getContentLengthDownload, CONTENT_LENGTH_DOWNLOAD, double             ,   double             )
GET_INFO         (getContentLengthUpload,   CONTENT_LENGTH_UPLOAD  , double             ,   double             )
GET_INFO         (getContentType,           CONTENT_TYPE           , const char *       ,   const char *       )
GET_INFO         (getPrivate,               PRIVATE                , void *             ,   void *             )
GET_INFO         (getHttpAuthAvail,         HTTPAUTH_AVAIL         , long               ,   long               )
GET_INFO         (getProxyAuthAvail,        PROXYAUTH_AVAIL        , long               ,   long               )
GET_INFO         (getOsErrno,               OS_ERRNO               , long               ,   long               )
GET_INFO         (getNumConnects,           NUM_CONNECTS           , long               ,   long               )
GET_INFO         (getPrimaryIp,             PRIMARY_IP             , const char *       ,   const char *       )
GET_INFO         (getPrimaryPort,           PRIMARY_PORT           , long               ,   long               )
GET_INFO         (getLocalIp,               LOCAL_IP               , const char *       ,   const char *       )
GET_INFO         (getLocalPort,             LOCAL_PORT             , long               ,   long               )
GET_INFO         (getCookieList,            COOKIELIST             , StringList         ,   struct curl_slist *)
GET_INFO         (getLastSocket,            LASTSOCKET             , long               ,   long               )
GET_INFO         (getFtpEntryPath,          FTP_ENTRY_PATH         , const char *       ,   const char *       )
//GET_INFO         (getCertInfo,              CERTINFO               , CERTINFO           ,   CERTINFO  )
//GET_INFO         (getTlsSession,            TLS_SESSION            , TLSSESSION         ,   TLSSESSION)
GET_INFO         (getConditionUnmet,        CONDITION_UNMET        , bool               ,   bool               )
GET_INFO         (getRtspSessionId,         RTSP_SESSION_ID        , const char *       ,   const char *       )
GET_INFO         (getRtspClientCseq,        RTSP_CLIENT_CSEQ       , long               ,   long               )
GET_INFO         (getRtspServerCseq,        RTSP_SERVER_CSEQ       , long               ,   long               )
GET_INFO         (getRtspCseqRecv,          RTSP_CSEQ_RECV         , long               ,   long               )

Session::Time Session::getFileTime(void)
{
    return Time(std::chrono::seconds(get<long>(CURLINFO_FILETIME)));
}

// Session options
#define SET_OPT(method, opt, methodType, curlType) \
    void Session::method(methodType arg) \
    { \
        set<curlType>(CURLOPT_ ## opt, arg); \
    }

// Behaviour options
SET_OPT(setVerbose,             VERBOSE,       bool, long)
SET_OPT(setIncludeHeaderInBody, HEADER,        bool, long)
SET_OPT(setNoProgress,          NOPROGRESS,    bool, long)
SET_OPT(setNoSignal,            NOSIGNAL,      bool, long)
SET_OPT(setWildcardMatch,       WILDCARDMATCH, bool, long)

// Error options
SET_OPT(setStderr,      STDERR     , FILE *, FILE *)
SET_OPT(setFailOnError, FAILONERROR, bool,   bool  ) // [HTTP]

// Network options
SET_OPT(setUrl,                 URL,                  const char *, const char *)
SET_OPT(setDisableUrlSquashing, PATH_AS_IS,           bool        , long        )
SET_OPT(setProtocols,           PROTOCOLS,            long        , long        )
SET_OPT(setRedirectProtocols,   REDIR_PROTOCOLS,      long        , long        )
SET_OPT(setProxyUrl,            PROXY,                const char *, const char *)
SET_OPT(setProxyPort,           PROXYPORT,            long        , long        )
SET_OPT(setProxyType,           PROXYTYPE,            long        , long        )
SET_OPT(setNoProxyUrls,         NOPROXY,              const char *, const char *)
SET_OPT(setProxyTunnel,         HTTPPROXYTUNNEL,      bool        , long        )
SET_OPT(setiSocks5Name,         SOCKS5_GSSAPI_SERVICE,const char *, const char *)
SET_OPT(setiSocks5Unprotected,  SOCKS5_GSSAPI_NEC,    bool        , long        )
SET_OPT(setProxyName,           INTERFACE,            const char *, const char *)
SET_OPT(setLocalPort,           LOCALPORT,            long        , long        )
SET_OPT(setLocalPortRange,      LOCALPORTRANGE,       long        , long        )
SET_OPT(setDnsCacheTimeout,     DNS_CACHE_TIMEOUT,    long        , long        )
SET_OPT(setDnsUseGlobalCache,   DNS_USE_GLOBAL_CACHE, bool        , long        )
SET_OPT(setBufferSize,          BUFFERSIZE,           long        , long        )
SET_OPT(setDistantPort,         PORT,                 long        , long        )
SET_OPT(setTcpNoDelay,          TCP_NODELAY,          bool        , long        )
SET_OPT(setAddressScope,        ADDRESS_SCOPE,        long        , long        )
SET_OPT(setTcpKeepAlive,        TCP_KEEPALIVE,        bool        , long        )
SET_OPT(setTcpKeepIdle,         TCP_KEEPIDLE,         long        , long        )
SET_OPT(setTcpKeepInterval,     TCP_KEEPINTVL,        long        , long        )
SET_OPT(setUnixSocketPath,      UNIX_SOCKET_PATH,     const char *, const char *)

// Authentication
SET_OPT(setNetrc,               NETRC           , long        , long        )
SET_OPT(setNetrcFile,           NETRC_FILE      , const char *, const char *)
SET_OPT(setUserPassword,        USERPWD         , const char *, const char *)
SET_OPT(setProxyUserPassword,   PROXYUSERPWD    , const char *, const char *)
SET_OPT(setUsername,            USERNAME        , const char *, const char *)
SET_OPT(setPassword,            PASSWORD        , const char *, const char *)
SET_OPT(setLoginOptions,        LOGIN_OPTIONS   , const char *, const char *)
SET_OPT(setProxyUsername,       PROXYUSERNAME   , const char *, const char *)
SET_OPT(setProxyPassword,       PROXYPASSWORD   , const char *, const char *)
SET_OPT(setHttpAuth,            HTTPAUTH        , long        , long        ) // [HTTP]
SET_OPT(setTlsUsername,         TLSAUTH_USERNAME, const char *, const char *)
SET_OPT(setTlsPassword,         TLSAUTH_PASSWORD, const char *, const char *)
SET_OPT(setTlsAuth,             TLSAUTH_TYPE    , long        , long        )
SET_OPT(setProxyAuth,           PROXYAUTH       , long        , long        ) // [HTTP]
SET_OPT(setSaslInitialResponse, SASL_IR         , bool        , long        )
SET_OPT(setOAuth2BearerToken,   XOAUTH2_BEARER  , const char *, const char *)

// Protocol options
SET_OPT(setTransferText,      TRANSFERTEXT,        bool        , long        ) // [FTP]
SET_OPT(setProxyTransferMode, PROXY_TRANSFER_MODE, bool        , long        ) // [FTP]
SET_OPT(setCrLf,              CRLF,                bool        , long        )
SET_OPT(setRange,             RANGE,               const char *, const char *)
SET_OPT(setResumeFrom,        RESUME_FROM,         long        , long        )
SET_OPT(setResumeFromLarge,   RESUME_FROM_LARGE,   curl_off_t  , curl_off_t  )
SET_OPT(setCustomRequest,     CUSTOMREQUEST,       const char *, const char *)
SET_OPT(setFileTime,          FILETIME,            bool        , long        )
SET_OPT(setDirListOnly,       DIRLISTONLY,         bool        , long        )
SET_OPT(setNoBody,            NOBODY,              bool        , long        )
SET_OPT(setInFileSize,        INFILESIZE,          long        , long        )
SET_OPT(setInFileSizeLarge,   INFILESIZE_LARGE,    curl_off_t  , curl_off_t  )
SET_OPT(setUpload,            UPLOAD,              bool        , long        )
SET_OPT(setMaxFileSize,       MAXFILESIZE,         long        , long        )
SET_OPT(setMaxFileSizeLarge,  MAXFILESIZE_LARGE,   curl_off_t  , curl_off_t  )
SET_OPT(setTimeCondition,     TIMECONDITION,       long        , long        )
SET_OPT(setTimeValue,         TIMEVALUE,           long        , long        )

// Connection options
SET_OPT(setTimeout,              TIMEOUT,              long        , long               )
SET_OPT(setTimeoutMillis,        TIMEOUT_MS,           long        , long               )
SET_OPT(setLowSpeedLimit,        LOW_SPEED_LIMIT,      long        , long               )
SET_OPT(setLowSpeedTime,         LOW_SPEED_TIME,       long        , long               )
SET_OPT(setMaxSendSpeedLarge,    MAX_SEND_SPEED_LARGE, curl_off_t  , curl_off_t         )
SET_OPT(setMaxRecvSpeedLarge,    MAX_RECV_SPEED_LARGE, curl_off_t  , curl_off_t         )
SET_OPT(setMaxConnects,          MAXCONNECTS,          long        , long               )
SET_OPT(setFreshConnect,         FRESH_CONNECT,        bool        , long               )
SET_OPT(setForbidReuse,          FORBID_REUSE,         bool        , long               )
SET_OPT(setConnectTimeout,       CONNECTTIMEOUT,       long        , long               )
SET_OPT(setConnectTimeoutMillis, CONNECTTIMEOUT_MS,    long        , long               )
SET_OPT(setIpResolve,            IPRESOLVE,            long        , long               )
SET_OPT(setConnectOnly,          CONNECT_ONLY,         bool        , long               )
SET_OPT(setUseLevel,             USE_SSL,              long        , long               )
SET_OPT(setResolve,              RESOLVE,              StringList &, struct curl_slist *)
SET_OPT(setDnsInterface,         DNS_INTERFACE,        const char *, const char *       )
SET_OPT(setDnsLocalIp4,          DNS_LOCAL_IP4,        const char *, const char *       )
SET_OPT(setDnsLocalIp6,          DNS_LOCAL_IP6,        const char *, const char *       )
SET_OPT(setDnsServers,           DNS_SERVERS,          const char *, const char *       )
SET_OPT(setAcceptTimeoutMillis,  ACCEPTTIMEOUT_MS,     long        , long               )

// SSL & Security options
SET_OPT(setSslCertificate,                    SSLCERT            , const char *, const char *)
SET_OPT(setSslCertificateType,                SSLCERTTYPE        , const char *, const char *)
SET_OPT(setSslPrivateKeyFilename,             SSLKEY             , const char *, const char *)
SET_OPT(setSslPrivateKeyType,                 SSLKEYTYPE         , const char *, const char *)
SET_OPT(setKeyPassword,                       KEYPASSWD          , const char *, const char *)
SET_OPT(setSslEnableAlpn,                     SSL_ENABLE_ALPN    , bool        , long        ) // [HTTP]
SET_OPT(setSslEnableNpn,                      SSL_ENABLE_NPN     , bool        , long        ) // [HTTP]
SET_OPT(setSslEngine,                         SSLENGINE          , const char *, const char *)
SET_OPT(setSslEngineDefault,                  SSLENGINE_DEFAULT  , bool        , long        )
SET_OPT(setSslFalseStart,                     SSL_FALSESTART     , bool        , long        )
SET_OPT(setSslVersion,                        SSLVERSION         , long        , long        )
SET_OPT(setSslVerifyHost,                     SSL_VERIFYHOST     , long        , long        )
SET_OPT(setSslVerifyPeer,                     SSL_VERIFYPEER     , bool        , long        )
SET_OPT(setSslVerifyStatus,                   SSL_VERIFYSTATUS   , bool        , long        )
SET_OPT(setCaInfo,                            CAINFO             , const char *, const char *)
SET_OPT(setSslIssuerCertificateFilename,      ISSUERCERT         , const char *, const char *)
SET_OPT(setCaPath,                            CAPATH             , const char *, const char *)
SET_OPT(setCertificateRevocationListFilename, CRLFILE            , const char *, const char *)
SET_OPT(setCertificateInfo,                   CERTINFO           , bool        , long        )
SET_OPT(setPinnedPublicKey,                   PINNEDPUBLICKEY    , const char *, const char *)
SET_OPT(setRandomFile,                        RANDOM_FILE        , const char *, const char *)
SET_OPT(setEgdSocketPath,                     EGDSOCKET          , const char *, const char *)
SET_OPT(setSslCipherList,                     SSL_CIPHER_LIST    , const char *, const char *)
SET_OPT(setSslSessionIdCache,                 SSL_SESSIONID_CACHE, bool        , long        )
SET_OPT(setSslOptions,                        SSL_OPTIONS        , long        , long        )
SET_OPT(setKerberosSecurityLevel,             KRBLEVEL           , const char *, const char *) // [FTP]
SET_OPT(setGssApiDelegation,                  GSSAPI_DELEGATION  , long        , long        ) // HTTP

// Other options
SET_OPT(setPrivateData,       PRIVATE            , void *, void *)
SET_OPT(setNewFilePerms,      NEW_FILE_PERMS     , long  , long  )
SET_OPT(setNewDirectoryPerms, NEW_DIRECTORY_PERMS, long  , long  )

}; /* Namespace oowe */
