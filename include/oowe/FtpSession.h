#ifndef __OOWE_FTP_SESSION__
#define __OOWE_FTP_SESSION__
#include "oowe/Session.h"

namespace oowe {

class FtpSession : public Session
{
    public:
        using Session::Session;

        // Protocol options
        EASY_SET_BOOL  (TransferText,      TRANSFERTEXT       ) // Use text transfer
        EASY_SET_BOOL  (ProxyTransferMode, PROXY_TRANSFER_MODE) // Add transfer mode to URL over proxy

        // SSL & Security options
        EASY_SET_STRING(KerberosLevel,     KRBLEVEL           ) // Set kerberos security level
};

}; /* Namespace oowe */

#endif /* __OOWE_FTP_SESSION__ */
