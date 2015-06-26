#ifndef __OOWE_MULTI_SESSION__
#define __OOWE_MULTI_SESSION__
#include <curl/curl.h>
#include <oowe/Session.h>
#include "oowe/oowe_macros.h"

namespace oowe {

class MultiSession
{
    public:
        MultiSession(void);
        MultiSession(const MultiSession & rhs) = delete;
        MultiSession(MultiSession && rhs);
        ~MultiSession(void);

        MultiSession & operator =(const MultiSession & rhs) = delete;
        MultiSession & operator =(MultiSession && rhs);

        // Add /remove simple session
        void operator += (Session & session);
        void operator -= (Session & session);

        // Select-style version
        void perform(void);
        void getFileDescriptorsSet(int * maxFd, fd_set * readSet = nullptr, fd_set * writeSet = nullptr, fd_set * excepSet = nullptr);
        struct timeval getTimeout(void);
        inline int getSessionsRunning(void) { return nbRunning; }

        // Event-style version (a.k.a. multi_socket API)
        // curl_multi_socket_action
        // CURLMOPT_SOCKETFUNCTION / CURLMOPT_TIMERFUNCTION

        // curl_multi_info_read

        // Informations
        template<typename Arg> void set(CURLMoption option, Arg arg);

        MULTI_SET_LONG(MaxConnects        , MAXCONNECTS          ) // Set size of connections cache 
        MULTI_SET_LONG(MaxTotalConnections, MAX_TOTAL_CONNECTIONS) // Set max simultaneously open connections

    private:
        CURLM * curl;
        int     nbRunning;
};

// Templates methods
template<typename Arg>
void MultiSession::set(CURLMoption option, Arg arg)
{
    OOWE_CHECK_MULTI_ERROR_THROW(curl_multi_setopt(curl, option, arg));
}

}; /* Namespace oowe */

#endif /* __OOWE_MULTI_SESSION__ */
