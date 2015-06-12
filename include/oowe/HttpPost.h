#ifndef __OOWE_HTTP_POST__
#define __OOWE_HTTP_POST__
#include "oowe/Session.h"
#include <curl/curl.h>
#include <vector>

namespace oowe {

class HttpPost
{
    public:
        HttpPost(void) = default;
        HttpPost(const HttpPost & rhs) = delete;
        HttpPost(HttpPost && rhs);
        ~HttpPost(void);

        HttpPost & operator =(const HttpPost & rhs) = delete;
        HttpPost & operator =(HttpPost && rhs);

        operator struct curl_httppost *(void);

        template<typename... Args> void add(Args... args);

        HttpPost & addName          (const char * name       , long size = 0); // PTRNAME  & [NAMELENGTH]
        HttpPost & addNameCopy      (const char * name       , long size = 0); // COPYNAME & [NAMELENGTH]
        HttpPost & addContent       (const char * contents   , long size = 0); // PTRCONTENTS  & [CONTENTSLENGTH]
        HttpPost & addContentCopy   (const char * contents   , long size = 0); // COPYCONTENTS & [CONTENTSLENGTH]
        HttpPost & addContentType   (const char * contentType               ); // CONTENTTYPE
        HttpPost & addContentHeader (StringList & contentHeader             ); // CONTENTHEADER
        HttpPost & addFile          (const char * filename   , long size = 0); // FILE (= FILECONTENT & FILENAME & CONTENTTYPE) & [CONTENTSLENGTH]
        HttpPost & addFileContent   (const char * filename                  ); // FILECONTENT
        HttpPost & addFileName      (const char * filename                  ); // FILENAME
        HttpPost & addBuffer        (const char * buffer     , long size = 0); // BUFFERPTR & [BUFFERLENGTH]
        HttpPost & addBufferFileName(const char * filename                  ); // BUFFER

        HttpPost & setStream(Session & session); // STREAM
        void       end(void);

    private:
        struct curl_httppost * first = nullptr;
        struct curl_httppost * last  = nullptr;

        std::vector<struct curl_forms> forms;
};

// Template methods
template<typename... Args>
void HttpPost::add(Args... args)
{
    switch(curl_formadd(&first, &last, args...))
    {
        case CURL_FORMADD_OK             : /* Everything OK */ break;
        case CURL_FORMADD_MEMORY         : std::cerr << "Error adding section dur HTTP form : \"Memory allocation fails\""                       << std::endl; break;
        case CURL_FORMADD_OPTION_TWICE   : std::cerr << "Error adding section dur HTTP form : \"One option is given twice for one Form\""        << std::endl; break;
        case CURL_FORMADD_NULL           : std::cerr << "Error adding section dur HTTP form : \"A null pointer was given for a char\""           << std::endl; break;
        case CURL_FORMADD_UNKNOWN_OPTION : std::cerr << "Error adding section dur HTTP form : \"An unknown option was used\""                    << std::endl; break;
        case CURL_FORMADD_INCOMPLETE     : std::cerr << "Error adding section dur HTTP form : \"The some FormInfo is not complete (or error)\""  << std::endl; break;
        case CURL_FORMADD_ILLEGAL_ARRAY  : std::cerr << "Error adding section dur HTTP form : \"An illegal option is used in an array\""         << std::endl; break;
        default                          : std::cerr << "Error adding section dur HTTP form : \"Unkown error\""                                  << std::endl; break;
    }
}

}; /* Namespace oowe */

#endif /* __OOWE_HTTP_POST__ */
