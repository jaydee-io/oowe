#include "oowe/HttpPost.h"
#include <utility>

namespace oowe {

HttpPost::HttpPost(HttpPost && rhs)
: first(nullptr), last(nullptr)
{
    std::swap(first, rhs.first);
    std::swap(last,  rhs.last);
}

HttpPost::~HttpPost(void)
{
    curl_formfree(first);
}

HttpPost & HttpPost::operator =(HttpPost && rhs)
{
    if(this != &rhs)
    {
        if(first)
        {
            curl_formfree(first);
            first = nullptr;
            last  = nullptr;
        }
        std::swap(first, rhs.first);
        std::swap(last,  rhs.last);
    }

    return *this;
}

HttpPost::operator struct curl_httppost *(void)
{
    return first;
}

#define ADD_STR(method, optStr) \
    HttpPost & HttpPost::method(const char * str) \
    { \
        forms.push_back( { CURLFORM_ ## optStr, str } ); \
        return *this; \
    }

#define ADD_STR_OPT_SIZE(method, optStr, optSize) \
    HttpPost & HttpPost::method(const char * str, long size) \
    { \
        forms.push_back( { CURLFORM_ ## optStr, str } ); \
        if(size) \
            forms.push_back( { CURLFORM_ ## optSize, reinterpret_cast<const char *>(size) } ); \
        return *this; \
    }

ADD_STR_OPT_SIZE(addName          , PTRNAME     , NAMELENGTH)
ADD_STR_OPT_SIZE(addNameCopy      , COPYNAME    , NAMELENGTH)
ADD_STR_OPT_SIZE(addContent       , PTRCONTENTS , CONTENTSLENGTH)
ADD_STR_OPT_SIZE(addContentCopy   , COPYCONTENTS, CONTENTSLENGTH)
ADD_STR         (addContentType   , CONTENTTYPE)
ADD_STR_OPT_SIZE(addFile          , FILE        , CONTENTSLENGTH)
ADD_STR         (addFileContent   , FILECONTENT)
ADD_STR         (addFileName      , FILENAME)
ADD_STR_OPT_SIZE(addBuffer        , BUFFERPTR   , BUFFERLENGTH)
ADD_STR         (addBufferFileName, BUFFER)

HttpPost & HttpPost::addContentHeader(StringList & contentHeader)
{
    forms.push_back( { CURLFORM_CONTENTHEADER, reinterpret_cast<const char *>(static_cast<struct curl_slist *>(contentHeader)) } );
    return *this;
}

HttpPost & HttpPost::setStream(Session & session)
{
    forms.push_back( { CURLFORM_STREAM, reinterpret_cast<const char *>(&session) } );
    return *this;
}

void HttpPost::end(void)
{
    forms.push_back( { CURLFORM_END, nullptr } );
    add(CURLFORM_ARRAY, forms.data(), CURLFORM_END);
    forms.clear();
}

}; /* Namespace oowe */
