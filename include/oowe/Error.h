#ifndef __OOWE_ERROR__
#define __OOWE_ERROR__
#include <curl/curl.h>
#include <iostream>

#define OOWE_CHECK_ERROR_THROW(test) do { CURLcode error = test; if(error != CURLE_OK) { std::cerr << "ERROR " << error << " : " << curl_easy_strerror(error) << std::endl; } } while(false)

namespace oowe
{

}; /* Namespace oowe */

#endif /* __OOWE_ERROR__ */

