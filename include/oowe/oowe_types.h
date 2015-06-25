#ifndef __OOWE_TYPES__
#define __OOWE_TYPES__

#include <curl/curl.h>
#include <chrono>

namespace oowe
{

    typedef curl_off_t                                                               offset_t;
    typedef std::chrono::duration<double>                                            Duration; // In seconds
    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> Time;     // In seconds

}; /* Namespace oowe */

#endif /* __OOWE_TYPES__ */
