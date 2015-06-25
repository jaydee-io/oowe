#include "oowe/MultiSession.h"
#include <utility>

namespace oowe {

MultiSession::MultiSession(void)
: curl(curl_multi_init()), nbRunning(0)
{
    if(!curl)
    {
        std::cerr << "Erreur dans curl_multi_init()" << std::endl;
        // TODO Throw error
    }
}

MultiSession::MultiSession(MultiSession && rhs)
: curl(nullptr), nbRunning(0)
{
    std::swap(curl,      rhs.curl);
    std::swap(nbRunning, rhs.nbRunning);
}

MultiSession::~MultiSession(void)
{
    if(curl)
        if(curl_multi_cleanup(curl) != CURLM_OK)
        {
            std::cerr << "Erreur dans curl_multi_cleanup()" << std::endl;
            // TODO Throw error
        }
}

MultiSession & MultiSession::operator =(MultiSession && rhs)
{
    if(this != &rhs)
    {
        if(curl)
        {
            if(curl_multi_cleanup(curl) != CURLM_OK)
            {
                std::cerr << "Erreur dans curl_multi_cleanup()" << std::endl;
                // TODO Throw error
            }
            curl      = nullptr;
            nbRunning = 0;
        }
        std::swap(curl,      rhs.curl);
        std::swap(nbRunning, rhs.nbRunning);
    }

    return *this;
}

void MultiSession::operator += (Session & session)
{
    OOWE_CHECK_MULTI_ERROR_THROW(curl_multi_add_handle(curl, session));
}

void MultiSession::operator -= (Session & session)
{
    OOWE_CHECK_MULTI_ERROR_THROW(curl_multi_remove_handle(curl, session));
}

void MultiSession::perform(void)
{
    OOWE_CHECK_MULTI_ERROR_THROW(curl_multi_perform(curl, &nbRunning));
}

void MultiSession::getFileDescriptorsSet(int * maxFd, fd_set * readSet, fd_set * writeSet, fd_set * excepSet)
{
    OOWE_CHECK_MULTI_ERROR_THROW(curl_multi_fdset(curl, readSet, writeSet, excepSet, maxFd));
}

struct timeval MultiSession::getTimeout(void)
{
    struct timeval timeout;
    long           curlTimeout;

    OOWE_CHECK_MULTI_ERROR_THROW(curl_multi_timeout(curl, &curlTimeout));

    if(curlTimeout < 0)
        curlTimeout = 1000;

    timeout.tv_sec  = curlTimeout / 1000;
    timeout.tv_usec = (curlTimeout % 1000) * 1000;
    return timeout;
}

}; /* Namespace oowe */
