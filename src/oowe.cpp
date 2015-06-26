#include "oowe/oowe.h"
#include <iostream>
#include <iomanip>

namespace oowe {

void init(long flags)
{
    OOWE_CHECK_ERROR_THROW(curl_global_init(flags));
}

void init(long flags, curl_malloc_callback m, curl_free_callback f, curl_realloc_callback r, curl_strdup_callback s, curl_calloc_callback c)
{
    OOWE_CHECK_ERROR_THROW(curl_global_init_mem(flags, m, f, r, s, c));
}

void cleanup(void)
{
    curl_global_cleanup();
}

curl_version_info_data & version(CURLversion type)
{
    return *curl_version_info(type);
}

char * version(void)
{
    return curl_version();
}

}; /* Namespace oowe */

#define COLOR(col, msg) "\033[" #col "m" msg "\033[0m"

#define CHECK_FEATURES(feature, msg) \
    if(info.features & CURL_VERSION_ ## feature) os << PREFIX "   " << std::left << std::setw(41) << msg << " = " << COLOR(32, "yes") << std::endl; \
    else                                         os << PREFIX "   " << std::left << std::setw(41) << msg << " = " << COLOR(31, "no" ) << std::endl;

#define PREFIX "#   "
std::ostream & operator <<(std::ostream & os, curl_version_info_data & info)
{
    int age = (int) info.age;

    os << PREFIX "Age = " << age << std::endl;

    if(age >= CURLVERSION_FIRST)
    {
        os << PREFIX "Version  = \"" << info.version << '"' << std::endl;
        os << PREFIX "Version  =  "  << CURL_VERSION_MAJOR(info.version_num) <<  '.' << CURL_VERSION_MINOR(info.version_num) << '.' << CURL_VERSION_PATCH(info.version_num) << std::endl;
        os << PREFIX "Host     = \"" << info.host << '"' << std::endl;
        
        os << PREFIX "Features =" << std::endl;
        CHECK_FEATURES(IPV6        , "Support IPv6"                             )
        CHECK_FEATURES(KERBEROS4   , "Support Kerberos V4 auth"                 )
        CHECK_FEATURES(KERBEROS5   , "Support Kerberos V5 auth"                 )
        CHECK_FEATURES(SSL         , "Support SSL"                              )
        CHECK_FEATURES(LIBZ        , "Support libz"                             )
        CHECK_FEATURES(NTLM        , "Support NTLM auth"                        )
        CHECK_FEATURES(GSSNEGOTIATE, "Support GSS Negotiate"                    )
        CHECK_FEATURES(DEBUG       , "Debug built"                              )
        CHECK_FEATURES(ASYNCHDNS   , "Asynchronous DNS resolves"                )
        CHECK_FEATURES(SPNEGO      , "Support SPNEGO auth"                      )
        CHECK_FEATURES(LARGEFILE   , "Support files larger than 2GB"            )
        CHECK_FEATURES(IDN         , "Support Internationized Domain Names"     )
        CHECK_FEATURES(SSPI        , "Support Windows SSPI"                     )
        CHECK_FEATURES(CONV        , "Support character conversions"            )
        CHECK_FEATURES(CURLDEBUG   , "Support debug memory tracking"            )
        CHECK_FEATURES(TLSAUTH_SRP , "Support TLS-SRP auth"                     )
        CHECK_FEATURES(NTLM_WB     , "Support NTLM delegation to winbind helper")
        CHECK_FEATURES(HTTP2       , "Support HTTP2"                            )
        CHECK_FEATURES(GSSAPI      , "GSS-API built"                            )
        CHECK_FEATURES(UNIX_SOCKETS, "Support Unix domain sockets"              )

        os << PREFIX "SSL  Version  = \"" << info.ssl_version  << "\" / " << info.ssl_version_num << std::endl;
        os << PREFIX "libz Version  = \"" << info.libz_version << '"' << std::endl;

        os << PREFIX "Protocols = \"";
        for(int i = 0; info.protocols[i]; i++)
        {
            os << info.protocols[i];
            if(info.protocols[i+1])
                os << ' ' ;
        }
        os << '"' << std::endl;
    }

    if(age >= CURLVERSION_SECOND)
        os << PREFIX "Ares   Version  = \"" << (info.ares ? info.ares : "[" COLOR(31 ,"NONE") "]") << "\" / " << info.ares_num << std::endl;

    if(age >= CURLVERSION_THIRD)
        os << PREFIX "libidn Version  = \"" << (info.libidn ? info.libidn : "[" COLOR(31 ,"NONE") "]") << '"'  << std::endl;

    if(age >= CURLVERSION_FOURTH)
    {
        os << PREFIX "iconv  Version  =  "  << info.iconv_ver_num  << std::endl;
        os << PREFIX "libssh Version  = \"" << (info.libssh_version ? info.libssh_version : "[" COLOR(31 ,"NONE") "]") << '"' << std::endl;
    }

    return os;
}
