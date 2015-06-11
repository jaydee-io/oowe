#ifndef __OOWE_SESSION__
#define __OOWE_SESSION__
#include <curl/curl.h>
#include <chrono>
#include "oowe/String.h"
#include "oowe/Error.h"

namespace oowe {

class Session
{
    public:
        typedef curl_off_t                                                               offset_t;
        typedef std::chrono::duration<double>                                            Duration; // In seconds
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> Time;     // In seconds

    public:
        Session(void);
        Session(const char * url);
        Session(const Session & rhs);
        Session(Session && rhs);
        ~Session(void);

        Session & operator =(const Session & rhs);
        Session & operator =(Session && rhs);

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

        // Informations
        template<typename Ret> Ret get(CURLINFO info);

        const char * getEffectiveUrl         (void); // Get the last used effective URL
        long         getResponseCode         (void); // Get the last received HTTP, FTP or SMTP response code
        long         getHttpConnectCode      (void); // Get the last received proxy response code to a CONNECT request
        Time         getFileTime             (void); // Get the remote time of the retrieved document
        Duration     getTotalTime            (void); // Get the total time in seconds for the previous transfer, including name resolving, TCP connect etc
        Duration     getNameLookupTime       (void); // Get the time, in seconds, it took from the start until the name resolving was completed
        Duration     getConnectTime          (void); // Get the time, in seconds, it took from the start until the connect to the remote host (or proxy) was completed
        Duration     getAppConnectTime       (void); // Get the time, in seconds, it took from the start until the SSL/SSH connect/handshake to the remote host was completed
        Duration     getPretransferTime      (void); // Get the time, in seconds, it took from the start until the file transfer is just about to begin
        Duration     getStarttransferTime    (void); // Get the time, in seconds, it took from the start until the first byte is received by libcurl
        Duration     getRedirectTime         (void); // Get the total time, in seconds, it took for all redirection steps include name lookup, connect, pretransfer and transfer before final transaction was started
        long         getRedirectCount        (void); // Get the total number of redirections that were actually followed
        const char * getRedirectUrl          (void); // Get the URL a redirect would take you to if you would enable FOLLOWLOCATION
        double       getSizeUpload           (void); // Get the total amount of bytes that were uploaded
        double       getSizeDownload         (void); // Get the total amount of bytes that were downloaded
        double       getSpeedDownload        (void); // Get the average download speed that curl measured for the complete download
        double       getSpeedUpload          (void); // Get the average upload speed that curl measured for the complete upload
        long         getHeaderSize           (void); // Get the total size of all the headers received
        long         getRequestSize          (void); // Get the total size of the issued requests
        bool         getSslVerifyResult      (void); // Get the result of the certification verification that was requested (using the SSL_VERIFYPEER option)
        StringList   getSslEngines           (void); // Get a linked-list of OpenSSL crypto-engines supported
        double       getContentLengthDownload(void); // Get the content-length of the download
        double       getContentLengthUpload  (void); // Get the specified size of the upload
        const char * getContentType          (void); // Get the content-type of the downloaded object
        void *       getPrivate              (void); // Get the pointer to the private data associated with the session
        long         getHttpAuthAvail        (void); // Get a bitmask indicating the authentication method(s) available
        long         getProxyAuthAvail       (void); // Get a bitmask indicating the authentication method(s) available for your proxy authentication
        long         getOsErrno              (void); // Get the errno variable from a connect failure
        long         getNumConnects          (void); // Get how many new connections the session had to create to achieve the previous transfer (only the successful connects are counted)
        const char * getPrimaryIp            (void); // Get the string holding the IP address of the most recent connection done with this session
        long         getPrimaryPort          (void); // Get the destination port of the most recent connection done with this session
        const char * getLocalIp              (void); // Get the pointer to a zero-terminated string holding the local (source) IP address of the most recent connection done with this session
        long         getLocalPort            (void); // Get the local (source) port of the most recent connection done with this session
        StringList   getCookieList           (void); // Get a linked-list of all cookies the session knows (expired ones, too)
        long         getLastSocket           (void); // Get the last socket used by this session
        const char * getFtpEntryPath         (void); // Get a string holding the path of the entry path
//        CERTINFO     getCertInfo             (void); // Get a number of linked lists with info about the certificate chain, assuming you had CERTINFO enabled when the previous request was done
//        TLSSESSION   getTlsSession           (void); // Get an enum indicating the SSL library used for the handshake and the respective internal TLS session structure of this underlying SSL library
        bool         getConditionUnmet       (void); // Get the number 1 if the condition provided in the previous request didn't match (see TIMECONDITION)
        const char * getRtspSessionId        (void); // Get a string holding the most recent RTSP Session ID
        long         getRtspClientCseq       (void); // Get the next CSeq that will be used by the application
        long         getRtspServerCseq       (void); // Get the next server CSeq that will be expected by the application
        long         getRtspCseqRecv         (void); // Get the most recently received CSeq from the server

        // Options
        template<typename Arg> void set(CURLoption option, Arg arg);
        void reset(void);

        // Behaviour options
        void setVerbose            (bool enable);
        void setIncludeHeaderInBody(bool enable);
        void setNoProgress         (bool disable);
        void setNoSignal           (bool disable);
        void setWildcardMatch      (bool enable);

        // Callback options

        // Error options
// TODO Handle error buffer
//        void setErrorBuffer(const char * buffer); // Set error message buffer
        void setStderr     (FILE * stream); // Set stderr replacement stream
        void setFailOnError(bool   enable); // Enable / disable fail on HTTP 4xx errors [HTTP]

        // Network options
        void setUrl                (const char * url         ); // Set URL to work on
        void setDisableUrlSquashing(bool         disable     ); // Enable / disable squashing /../ and /./ sequences in the path
        void setProtocols          (long         protos      ); // Set allowed protocols
        void setRedirectProtocols  (long         protos      ); // Set protocols to allow redirects to
        void setProxyUrl           (const char * url         ); // Set proxy to use
        void setProxyPort          (long         port        ); // Set proxy port to use
        void setProxyType          (long         type        ); // Set proxy type
        void setNoProxyUrls        (const char * urls        ); // Set filter out hosts from proxy use
        void setProxyTunnel        (bool         enable      ); // Enable / disable tunnel through the HTTP proxy
        void setiSocks5Name        (const char * name        ); // Set Socks5 GSSAPI service name
        void setiSocks5Unprotected (bool         enable      ); // Enable / disable Socks5 GSSAPI NEC mode
        void setProxyName          (const char * name        ); // Set bind connection locally to this
        void setLocalPort          (long         port        ); // Set bind connection locally to this port
        void setLocalPortRange     (long         range       ); // Set bind connection locally to port range
        void setDnsCacheTimeout    (long         timeout     ); // Set timeout for DNS cache
        void setDnsUseGlobalCache  (bool         enable      ); // [OBSOLETE] Enable / disable global DNS cache
        void setBufferSize         (long         size        ); // Set ask for smaller buffer size
        void setDistantPort        (long         port        ); // Set port number to connect to
        void setTcpNoDelay         (bool         enable      ); // Enable / disable the Nagle algorithm
        void setAddressScope       (long         addressScope); // Set IPv6 scope for local addresses
        void setTcpKeepAlive       (bool         enable      ); // Enable / disable TCP keep-alive
        void setTcpKeepIdle        (long         idleTime    ); // Set idle time before sending keep-alive
        void setTcpKeepInterval    (long         intervalTime); // Set interval between keep-alive probes
        void setUnixSocketPath     (const char * path        ); // Set path to a Unix domain socket

        // Authentication options
        void setNetrc              (long         level   ); // Set enable .netrc parsing
        void setNetrcFile          (const char * file    ); // Set .netrc file name
        void setUserPassword       (const char * userpwd ); // Set user name and password
        void setProxyUserPassword  (const char * userpwd ); // Set proxy user name and password
        void setUsername           (const char * username); // Set user name
        void setPassword           (const char * pwd     ); // Set password
        void setLoginOptions       (const char * options ); // Set login options
        void setProxyUsername      (const char * username); // Set proxy user name
        void setProxyPassword      (const char * pwd     ); // Set proxy password
        void setHttpAuth           (long         auth    ); // Set HTTP server authentication methods [HTTP]
        void setTlsUsername        (const char * username); // Set TLS authentication user name
        void setTlsPassword        (const char * pwd     ); // Set TLS authentication password
        void setTlsAuth            (long         auth    ); // Set TLS authentication methods
        void setProxyAuth          (long         auth    ); // Set HTTP proxy authentication methods [HTTP]
        void setSaslInitialResponse(bool         enable  ); // Enable / disable SASL initial response
        void setOAuth2BearerToken  (const char * token   ); // Set OAuth2 bearer token

        // Protocol options
        void setTransferText     (bool         text    ); // Use text transfer [FTP]
        void setProxyTransferMode(bool         enabled ); // Add transfer mode to URL over proxy [FTP]
        void setCrLf             (bool         enabled ); // Convert newlines
        void setRange            (const char * range   ); // Set range requests
        void setResumeFrom       (long         from    ); // Set resume a transfer
        void setResumeFromLarge  (offset_t     from    ); // Set resume a transfer
        void setCustomRequest    (const char * request ); // Set custom request/method
        void setFileTime         (bool         gettime ); // Enable / disable request file modification date and time
        void setDirListOnly      (bool         listonly); // List only
        void setNoBody           (bool         opt     ); // Do not get the body contents
        void setInFileSize       (long         filesize); // Set size of file to send
        void setInFileSizeLarge  (offset_t     filesize); // Set size of file to send
        void setUpload           (bool         upload  ); // Enable / disable upload data
        void setMaxFileSize      (long         size    ); // Set maximum file size to get
        void setMaxFileSizeLarge (offset_t     size    ); // Set maximum file size to get
        void setTimeCondition    (long         cond    ); // Set make a time conditional request
        void setTimeValue        (long         val     ); // Set time value for the time conditional request

        // Connection options
        void setTimeout             (long         timeout   ); // Set timeout for the entire request
        void setTimeoutMillis       (long         timeout   ); // Set millisecond timeout for the entire request
        void setLowSpeedLimit       (long         speedlimit); // Set low speed limit to abort transfer
        void setLowSpeedTime        (long         speedtime ); // Set time to be below the speed to trigger low speed abort
        void setMaxSendSpeedLarge   (offset_t     maxspeed  ); // Set cap the upload speed to this
        void setMaxRecvSpeedLarge   (offset_t     speed     ); // Set cap the download speed to this
        void setMaxConnects         (long         amount    ); // Set maximum number of connections in the connection pool
        void setFreshConnect        (bool         fresh     ); // Use a new connection
        void setForbidReuse         (bool         close     ); // Prevent subsequent connections from re-using this
        void setConnectTimeout      (long         timeout   ); // Set timeout for the connection phase
        void setConnectTimeoutMillis(long         timeout   ); // Set millisecond timeout for the connection phase
        void setIpResolve           (long         resolve   ); // Set IP version to resolve to
        void setConnectOnly         (bool         only      ); // Only connect, nothing else
        void setUseLevel            (long         level     ); // Set use TLS/SSL
        void setResolve             (StringList & hosts     ); // Set provide fixed/fake name resolves
        void setDnsInterface        (const char * ifname    ); // Set bind name resolves to this interface
        void setDnsLocalIp4         (const char * address   ); // Set bind name resolves to this IP4 address
        void setDnsLocalIp6         (const char * address   ); // Set bind name resolves to this IP6 address
        void setDnsServers          (const char * servers   ); // Set preferred DNS servers
        void setAcceptTimeoutMillis (long         ms        ); // Set timeout for waiting for the server's connect back to be accepted

        // SSL & Security options
        void setSslCertificate                   (const char * cert   ); // Set client cert
        void setSslCertificateType               (const char * type   ); // Set client cert type
        void setSslPrivateKeyFilename            (const char * keyfile); // Set client key
        void setSslPrivateKeyType                (const char * type   ); // Set client key type
        void setKeyPassword                      (const char * pwd    ); // Set client key password
        void setSslEnableAlpn                    (bool         enable ); // Set enable use of ALPN [HTTP]
        void setSslEnableNpn                     (bool         enable ); // Set enable use of NPN [HTTP]
        void setSslEngine                        (const char * id     ); // Set use identifier with SSL engine
        void setSslEngineDefault                 (bool         enable ); // Set default SSL engine
        void setSslFalseStart                    (bool         enable ); // Set enable TLS False Start
        void setSslVersion                       (long         version); // Set SSL version to use
        void setSslVerifyHost                    (long         verify ); // Set verify the host name in the SSL certificate
        void setSslVerifyPeer                    (bool         verify ); // Set verify the peer's SSL certificate
        void setSslVerifyStatus                  (bool         verify ); // Set verify the SSL certificate's status
        void setCaInfo                           (const char * path   ); // Set CA cert bundle
        void setSslIssuerCertificateFilename     (const char * file   ); // Set issuer certificate
        void setCaPath                           (const char * path   ); // Set path to CA cert bundle
        void setCertificateRevocationListFilename(const char * file   ); // Set certificate Revocation List
        void setCertificateInfo                  (bool         enable ); // Set extract certificate info
        void setPinnedPublicKey                  (const char * key    ); // Set set pinned SSL public key
        void setRandomFile                       (const char * path   ); // Set provide source for entropy random data
        void setEgdSocketPath                    (const char * path   ); // Set identify EGD socket for entropy
        void setSslCipherList                    (const char * list   ); // Set ciphers to use
        void setSslSessionIdCache                (bool         enable ); // Set disable SSL session-id cache
        void setSslOptions                       (long         options); // Set control SSL behavior
        void setKerberosSecurityLevel            (const char * level  ); // Set kerberos security level [FTP]
        void setGssApiDelegation                 (long   level        ); // Set disable GSS-API delegation [HTTP]
        
        // SSH options

        // Other options
        void setPrivateData      (void *  pointer); // Set private pointer to store
// TODO Handle share
//        void setShareObject      (Share & share  ); // Set share object to use
        void setNewFilePerms     (long    mode   ); // Set mode for creating new remote files
        void setNewDirectoryPerms(long    mode   ); // Set mode for creating new remote directories

        // Telnet options

    private:
        CURL * curl;
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
