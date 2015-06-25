#ifndef __OOWE_SESSION__
#define __OOWE_SESSION__
#include <curl/curl.h>
#include <chrono>
#include "oowe/oowe_types.h"
#include "oowe/oowe_macros.h"
#include "oowe/String.h"
#include "oowe/OutputStream.h"
#include "oowe/InputStream.h"
#include "oowe/Error.h"

#define GET(method, info, methodType, curlType) \
    inline methodType get ## method(void) \
    { \
        return get<curlType>(CURLINFO_ ## info); \
    }
#define GET_BOOL(      method, info) GET(method, info, bool,         long               )
#define GET_LONG(      method, info) GET(method, info, long,         long               )
#define GET_DOUBLE(    method, info) GET(method, info, double,       double             )
#define GET_VOID(      method, info) GET(method, info, void *,       void *             )
#define GET_STRING(    method, info) GET(method, info, const char *, const char *       )
#define GET_STRING_LST(method, info) GET(method, info, StringList,   struct curl_slist *)
#define GET_DURATION(  method, info) \
    inline Duration get ## method(void) \
    { \
        return Duration(get<double>(CURLINFO_ ## info)); \
    }
#define GET_TIME(      method, info) \
    inline Time get ## method(void) \
    { \
        return Time(std::chrono::seconds(get<long>(CURLINFO_ ## info))); \
    }

#define SET(method, opt, methodType, curlType) \
    inline void set ## method(methodType arg) \
    { \
        set<curlType>(CURLOPT_ ## opt, arg); \
    }
#define SET_BOOL(      method, info) SET(method, info, bool,         long               )
#define SET_LONG(      method, info) SET(method, info, long,         long               )
#define SET_VOID(      method, info) SET(method, info, void *,       void *             )
#define SET_FILE(      method, info) SET(method, info, FILE *,       FILE *             )
#define SET_OFFSET(    method, info) SET(method, info, offset_t,     curl_off_t         )
#define SET_STRING(    method, info) SET(method, info, const char *, const char *       )
#define SET_STRING_LST(method, info) SET(method, info, StringList &, struct curl_slist *)

namespace oowe {

class Session
{
    public:
        Session(void);
        Session(const char * url);
        Session(const Session & rhs);
        Session(Session && rhs);
        ~Session(void);

        Session & operator =(const Session & rhs);
        Session & operator =(Session && rhs);

        // Native type
        operator CURL *(void);

        // URL encoding / decoding
        String escape  (const char * str, int len);
        String escape  (const std::string & str);
        String escape  (const String & str);
        String unescape(const char * str, int len);
        String unescape(const std::string & str);
        String unescape(const String & str);

        // Actions
        void   pause  (int action);
        void   perform(void);
        size_t recv   (      void * buffer, size_t len);
        size_t send   (const void * buffer, size_t len);

        // Streams
        void setInputStream (InputStream  * stream);
        void setOutputStream(OutputStream * stream);

        // Informations
        template<typename Ret> Ret get(CURLINFO info);

        GET_STRING    (EffectiveUrl,          EFFECTIVE_URL          ) // Get the last used effective URL
        GET_LONG      (ResponseCode,          RESPONSE_CODE          ) // Get the last received HTTP, FTP or SMTP response code
        GET_LONG      (HttpConnectCode,       HTTP_CONNECTCODE       ) // Get the last received proxy response code to a CONNECT request
        GET_TIME      (FileTime,              FILETIME               ) // Get the remote time of the retrieved document
        GET_DURATION  (TotalTime,             TOTAL_TIME             ) // Get the total time in seconds for the previous transfer, including name resolving, TCP connect etc
        GET_DURATION  (NameLookupTime,        NAMELOOKUP_TIME        ) // Get the time, in seconds, it took from the start until the name resolving was completed
        GET_DURATION  (ConnectTime,           CONNECT_TIME           ) // Get the time, in seconds, it took from the start until the connect to the remote host (or proxy) was completed
        GET_DURATION  (AppConnectTime,        APPCONNECT_TIME        ) // Get the time, in seconds, it took from the start until the SSL/SSH connect/handshake to the remote host was completed
        GET_DURATION  (PreTransferTime,       PRETRANSFER_TIME       ) // Get the time, in seconds, it took from the start until the file transfer is just about to begin
        GET_DURATION  (StartTransferTime,     STARTTRANSFER_TIME     ) // Get the time, in seconds, it took from the start until the first byte is received by libcurl
        GET_DURATION  (RedirectTime,          REDIRECT_TIME          ) // Get the total time, in seconds, it took for all redirection steps include name lookup, connect, pretransfer and transfer before final transaction was started
        GET_LONG      (RedirectCount,         REDIRECT_COUNT         ) // Get the total number of redirections that were actually followed
        GET_STRING    (RedirectUrl,           REDIRECT_URL           ) // Get the URL a redirect would take you to if you would enable FOLLOWLOCATION
        GET_DOUBLE    (SizeUpload,            SIZE_UPLOAD            ) // Get the total amount of bytes that were uploaded
        GET_DOUBLE    (SizeDownload,          SIZE_DOWNLOAD          ) // Get the total amount of bytes that were downloaded
        GET_DOUBLE    (SpeedDownload,         SPEED_DOWNLOAD         ) // Get the average download speed that curl measured for the complete download
        GET_DOUBLE    (SpeedUpload,           SPEED_UPLOAD           ) // Get the average upload speed that curl measured for the complete upload
        GET_LONG      (HeaderSize,            HEADER_SIZE            ) // Get the total size of all the headers received
        GET_LONG      (RequestSize,           REQUEST_SIZE           ) // Get the total size of the issued requests
        GET_BOOL      (SslVerifyResult,       SSL_VERIFYRESULT       ) // Get the result of the certification verification that was requested (using the SSL_VERIFYPEER option)
        GET_STRING_LST(SslEngines,            SSL_ENGINES            ) // Get a linked-list of OpenSSL crypto-engines supported
        GET_DOUBLE    (ContentLengthDownload, CONTENT_LENGTH_DOWNLOAD) // Get the content-length of the download
        GET_DOUBLE    (ContentLengthUpload,   CONTENT_LENGTH_UPLOAD  ) // Get the specified size of the upload
        GET_STRING    (ContentType,           CONTENT_TYPE           ) // Get the content-type of the downloaded object
        GET_VOID      (Private,               PRIVATE                ) // Get the pointer to the private data associated with the session
        GET_LONG      (ProxyAuthAvail,        PROXYAUTH_AVAIL        ) // Get a bitmask indicating the authentication method(s) available for your proxy authentication
        GET_LONG      (OsErrno,               OS_ERRNO               ) // Get the errno variable from a connect failure
        GET_LONG      (NumConnects,           NUM_CONNECTS           ) // Get how many new connections the session had to create to achieve the previous transfer (only the successful connects are counted)
        GET_STRING    (PrimaryIp,             PRIMARY_IP             ) // Get the string holding the IP address of the most recent connection done with this session
        GET_LONG      (PrimaryPort,           PRIMARY_PORT           ) // Get the destination port of the most recent connection done with this session
        GET_STRING    (LocalIp,               LOCAL_IP               ) // Get the pointer to a zero-terminated string holding the local (source) IP address of the most recent connection done with this session
        GET_LONG      (LocalPort,             LOCAL_PORT             ) // Get the local (source) port of the most recent connection done with this session
        GET_STRING_LST(CookieList,            COOKIELIST             ) // Get a linked-list of all cookies the session knows (expired ones, too)
        GET_LONG      (LastSocket,            LASTSOCKET             ) // Get the last socket used by this session
        GET_STRING    (FtpEntryPath,          FTP_ENTRY_PATH         ) // Get a string holding the path of the entry path
//        GET           (CertInfo,              CERTINFO               ) // Get a number of linked lists with info about the certificate chain, assuming you had CERTINFO enabled when the previous request was done
//        GET           (TlsSession,            TLS_SESSION            ) // Get an enum indicating the SSL library used for the handshake and the respective internal TLS session structure of this underlying SSL library
        GET_BOOL      (ConditionUnmet,        CONDITION_UNMET        ) // Get the number 1 if the condition provided in the previous request didn't match (see TIMECONDITION)
        GET_STRING    (RtspSessionId,         RTSP_SESSION_ID        ) // Get a string holding the most recent RTSP Session ID
        GET_LONG      (RtspClientCseq,        RTSP_CLIENT_CSEQ       ) // Get the next CSeq that will be used by the application
        GET_LONG      (RtspServerCseq,        RTSP_SERVER_CSEQ       ) // Get the next server CSeq that will be expected by the application
        GET_LONG      (RtspCseqRecv,          RTSP_CSEQ_RECV         ) // Get the most recently received CSeq from the server

        // Options
        template<typename Arg> void set(CURLoption option, Arg arg);
        void reset(void);

        // Behaviour options
        SET_BOOL      (Verbose,                       VERBOSE              ) // Enable / disable display verbose information
        SET_BOOL      (Header,                        HEADER               ) // Enable / disable include the header in the body output
        SET_BOOL      (NoProgress,                    NOPROGRESS           ) // Enable / disable shut off the progress meter
        SET_BOOL      (NoSignal,                      NOSIGNAL             ) // Enable / disable do not install signal handlers
        SET_BOOL      (WildcardMatch,                 WILDCARDMATCH        ) // Enable / disable transfer multiple files according to a file name pattern

        // Error options
// TODO Handle error buffer
        //SET_STRING    (ErrorBuffer,                   ERRORBUFFER          ) // Set error message buffer
        SET_FILE      (Stderr,                        STDERR               ) // Set stderr replacement stream

        // Network options
        SET_STRING    (Url,                           URL                  ) // Set URL to work on
#if CURL_VERSION_GREATER(7, 42, 0)
        SET_BOOL      (PathAsIs,                      PATH_AS_IS           ) // Enable / disable squashing /../ and /./ sequences in the path
#endif
        SET_LONG      (Protocols,                     PROTOCOLS            ) // Set allowed protocols
        SET_LONG      (RedirProtocols,                REDIR_PROTOCOLS      ) // Set protocols to allow redirects to
        SET_STRING    (Proxy,                         PROXY                ) // Set proxy to use
        SET_LONG      (ProxyPort,                     PROXYPORT            ) // Set proxy port to use
        SET_LONG      (ProxyType,                     PROXYTYPE            ) // Set proxy type
        SET_STRING    (NoProxy,                       NOPROXY              ) // Set filter out hosts from proxy use
        SET_BOOL      (HttpProxyTunnel,               HTTPPROXYTUNNEL      ) // Enable / disable tunnel through the HTTP proxy
        SET_STRING    (Socks5GssApiService,           SOCKS5_GSSAPI_SERVICE) // Set Socks5 GSSAPI service name
        SET_BOOL      (Socks5GssApiNec,               SOCKS5_GSSAPI_NEC    ) // Enable / disable Socks5 GSSAPI NEC mode
        SET_STRING    (Interface,                     INTERFACE            ) // Set bind connection locally to this
        SET_LONG      (LocalPort,                     LOCALPORT            ) // Set bind connection locally to this port
        SET_LONG      (LocalPortRange,                LOCALPORTRANGE       ) // Set bind connection locally to port range
        SET_LONG      (DnsCacheTimeout,               DNS_CACHE_TIMEOUT    ) // Set timeout for DNS cache
        SET_BOOL      (DnsUseGlobalCache,             DNS_USE_GLOBAL_CACHE ) // [OBSOLETE] Enable / disable global DNS cache
        SET_LONG      (BufferSize,                    BUFFERSIZE           ) // Set ask for smaller buffer size
        SET_LONG      (Port,                          PORT                 ) // Set port number to connect to
        SET_BOOL      (TcpNoDelay,                    TCP_NODELAY          ) // Enable / disable the Nagle algorithm
        SET_LONG      (AddressScope,                  ADDRESS_SCOPE        ) // Set IPv6 scope for local addresses
        SET_BOOL      (TcpKeepAlive,                  TCP_KEEPALIVE        ) // Enable / disable TCP keep-alive
        SET_LONG      (TcpKeepIdle,                   TCP_KEEPIDLE         ) // Set idle time before sending keep-alive
        SET_LONG      (TcpKeepInterval,               TCP_KEEPINTVL        ) // Set interval between keep-alive probes
#if CURL_VERSION_GREATER(7, 40, 0)
        SET_STRING    (UnixSocketPath,                UNIX_SOCKET_PATH     ) // Set path to a Unix domain socket
#endif

        // Authentication
        SET_LONG      (NetRc,                         NETRC                ) // Set enable .netrc parsing
        SET_STRING    (NetRcFile,                     NETRC_FILE           ) // Set .netrc file name
        SET_STRING    (UserPassword,                  USERPWD              ) // Set user name and password
        SET_STRING    (ProxyUserPassword,             PROXYUSERPWD         ) // Set proxy user name and password
        SET_STRING    (Username,                      USERNAME             ) // Set user name
        SET_STRING    (Password,                      PASSWORD             ) // Set password
        SET_STRING    (LoginOptions,                  LOGIN_OPTIONS        ) // Set login options
        SET_STRING    (ProxyUsername,                 PROXYUSERNAME        ) // Set proxy user name
        SET_STRING    (ProxyPassword,                 PROXYPASSWORD        ) // Set proxy password
        SET_STRING    (TlsAuthUsername,               TLSAUTH_USERNAME     ) // Set TLS authentication user name
        SET_STRING    (TlsAuthPassword,               TLSAUTH_PASSWORD     ) // Set TLS authentication password
        SET_LONG      (TlsAuthType,                   TLSAUTH_TYPE         ) // Set TLS authentication methods
        SET_BOOL      (SaslInitialResponse,           SASL_IR              ) // Enable / disable SASL initial response
        SET_STRING    (OAuth2BearerToken,             XOAUTH2_BEARER       ) // Set OAuth2 bearer token

        // Protocol options
        SET_BOOL      (TransferText,                  TRANSFERTEXT         ) // Use text transfer [FTP]
        SET_BOOL      (ProxyTransferMode,             PROXY_TRANSFER_MODE  ) // Add transfer mode to URL over proxy [FTP]
        SET_BOOL      (CrLf,                          CRLF                 ) // Convert newlines
        SET_STRING    (Range,                         RANGE                ) // Set range requests
        SET_LONG      (ResumeFrom,                    RESUME_FROM          ) // Set resume a transfer
        SET_OFFSET    (ResumeFromLarge,               RESUME_FROM_LARGE    ) // Set resume a transfer
        SET_STRING    (CustomRequest,                 CUSTOMREQUEST        ) // Set custom request/method
        SET_BOOL      (FileTime,                      FILETIME             ) // Enable / disable request file modification date and time
        SET_BOOL      (DirListOnly,                   DIRLISTONLY          ) // List only
        SET_BOOL      (NoBody,                        NOBODY               ) // Do not get the body contents
        SET_LONG      (InFileSize,                    INFILESIZE           ) // Set size of file to send
        SET_OFFSET    (InFileSizeLarge,               INFILESIZE_LARGE     ) // Set size of file to send
        SET_BOOL      (Upload,                        UPLOAD               ) // Enable / disable upload data
        SET_LONG      (MaxFileSize,                   MAXFILESIZE          ) // Set maximum file size to get
        SET_OFFSET    (MaxFileSizeLarge,              MAXFILESIZE_LARGE    ) // Set maximum file size to get
        SET_LONG      (TimeCondition,                 TIMECONDITION        ) // Set make a time conditional request
        SET_LONG      (TimeValue,                     TIMEVALUE            ) // Set time value for the time conditional request

        // Connection options
        SET_LONG      (Timeout,                       TIMEOUT              ) // Set timeout for the entire request
        SET_LONG      (TimeoutMs,                     TIMEOUT_MS           ) // Set millisecond timeout for the entire request
        SET_LONG      (LowSpeedLimit,                 LOW_SPEED_LIMIT      ) // Set low speed limit to abort transfer
        SET_LONG      (LowSpeedTime,                  LOW_SPEED_TIME       ) // Set time to be below the speed to trigger low speed abort
        SET_OFFSET    (MaxSendSpeedLarge,             MAX_SEND_SPEED_LARGE ) // Set cap the upload speed to this
        SET_OFFSET    (MaxRecvSpeedLarge,             MAX_RECV_SPEED_LARGE ) // Set cap the download speed to this
        SET_LONG      (MaxConnects,                   MAXCONNECTS          ) // Set maximum number of connections in the connection pool
        SET_BOOL      (FreshConnect,                  FRESH_CONNECT        ) // Use a new connection
        SET_BOOL      (ForbidReuse,                   FORBID_REUSE         ) // Prevent subsequent connections from re-using this
        SET_LONG      (ConnectTimeout,                CONNECTTIMEOUT       ) // Set timeout for the connection phase
        SET_LONG      (ConnectTimeoutMs,              CONNECTTIMEOUT_MS    ) // Set millisecond timeout for the connection phase
        SET_LONG      (IpResolve,                     IPRESOLVE            ) // Set IP version to resolve to
        SET_BOOL      (ConnectOnly,                   CONNECT_ONLY         ) // Only connect, nothing else
        SET_LONG      (UseSsl,                        USE_SSL              ) // Set use TLS/SSL
        SET_STRING_LST(Resolve,                       RESOLVE              ) // Set provide fixed/fake name resolves
        SET_STRING    (DnsInterface,                  DNS_INTERFACE        ) // Set bind name resolves to this interface
        SET_STRING    (DnsLocalIp4,                   DNS_LOCAL_IP4        ) // Set bind name resolves to this IP4 address
        SET_STRING    (DnsLocalIp6,                   DNS_LOCAL_IP6        ) // Set bind name resolves to this IP6 address
        SET_STRING    (DnsServers,                    DNS_SERVERS          ) // Set preferred DNS servers
        SET_LONG      (AcceptTimeoutMs,               ACCEPTTIMEOUT_MS     ) // Set timeout for waiting for the server's connect back to be accepted

        // SSL & Security options
        SET_STRING    (SslCertificate,                SSLCERT              ) // Set client cert
        SET_STRING    (SslCertificateType,            SSLCERTTYPE          ) // Set client cert type
        SET_STRING    (SslKey,                        SSLKEY               ) // Set client key
        SET_STRING    (SslKeyType,                    SSLKEYTYPE           ) // Set client key type
        SET_STRING    (KeyPassword,                   KEYPASSWD            ) // Set client key password
        SET_STRING    (SslEngine,                     SSLENGINE            ) // Set use identifier with SSL engine
        SET_BOOL      (SslEngineDefault,              SSLENGINE_DEFAULT    ) // Set default SSL engine
#if CURL_VERSION_GREATER(7, 42, 0)
        SET_BOOL      (SslFalseStart,                 SSL_FALSESTART       ) // Set enable TLS False Start
#endif
        SET_LONG      (SslVersion,                    SSLVERSION           ) // Set SSL version to use
        SET_LONG      (SslVerifyHost,                 SSL_VERIFYHOST       ) // Set verify the host name in the SSL certificate
        SET_BOOL      (SslVerifyPeer,                 SSL_VERIFYPEER       ) // Set verify the peer's SSL certificate
#if CURL_VERSION_GREATER(7, 41, 0)
        SET_BOOL      (SslVerifyStatus,               SSL_VERIFYSTATUS     ) // Set verify the SSL certificate's status
#endif
        SET_STRING    (CaInfo,                        CAINFO               ) // Set CA cert bundle
        SET_STRING    (SslIssuerCertificate,          ISSUERCERT           ) // Set issuer certificate
        SET_STRING    (CaPath,                        CAPATH               ) // Set path to CA cert bundle
        SET_STRING    (CertificateRevocationListFile, CRLFILE              ) // Set certificate Revocation List
        SET_BOOL      (CertificateInfo,               CERTINFO             ) // Set extract certificate info
#if CURL_VERSION_GREATER(7, 39, 0)
        SET_STRING    (PinnedPublicKey,               PINNEDPUBLICKEY      ) // Set set pinned SSL public key
#endif
        SET_STRING    (RandomFile,                    RANDOM_FILE          ) // Set provide source for entropy random data
        SET_STRING    (EgdSocket,                     EGDSOCKET            ) // Set identify EGD socket for entropy
        SET_STRING    (SslCipherList,                 SSL_CIPHER_LIST      ) // Set ciphers to use
        SET_BOOL      (SslSessionIdCache,             SSL_SESSIONID_CACHE  ) // Set disable SSL session-id cache
        SET_LONG      (SslOptions,                    SSL_OPTIONS          ) // Set control SSL behavior
        SET_STRING    (KerberosLevel,                 KRBLEVEL             ) // Set kerberos security level [FTP]

        // Other options
        SET_VOID      (Private,                       PRIVATE              ) // Set private pointer to store
// TODO Handle share
        //SET           (Share,                         SHARE, Share &, CURLSH *) // Set share object to use
        SET_LONG      (NewFilePerms,                  NEW_FILE_PERMS       ) // Set mode for creating new remote files
        SET_LONG      (NewDirectoryPerms,             NEW_DIRECTORY_PERMS  ) // Set mode for creating new remote directories

    private:
        static size_t writeCallback(char * buffer, size_t size, size_t nitems, void * user);
        static size_t readCallback (char * buffer, size_t size, size_t nitems, void * user);
        static int    seekCallback (void * user, curl_off_t offset, int origin);

    private:
        CURL *         curl;
        InputStream *  inputStream;
        OutputStream * outputStream;
};

// Templates methods
template<typename Arg>
void Session::set(CURLoption option, Arg arg)
{
    OOWE_CHECK_ERROR_THROW(curl_easy_setopt(curl, option, arg));
}

template<typename Ret>
Ret Session::get(CURLINFO info)
{
    Ret val;

    OOWE_CHECK_ERROR_THROW(curl_easy_getinfo(curl, info, &val));
    return val;
}

}; /* Namespace oowe */

#endif /* __OOWE_SESSION__ */
