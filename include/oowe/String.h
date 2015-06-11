#ifndef __OOWE_STRING__
#define __OOWE_STRING__
#include <curl/curl.h>
#include <string>
#include <ostream>

namespace oowe {

// Wrap a string allocated by cURL
class String
{
    public :
        String(const char * str, int len = -1);
        ~String(void);

        int size(void) const;

        const char *      c_str(void) const;
        const std::string cpp_str(void) const;

    private :
        const char * str;
        int len;
};

// Wrap a string list allocated by cURL
class StringList
{
    public :
        class Iterator : public std::iterator<std::input_iterator_tag, struct curl_slist>
        {
            public :
                Iterator(struct curl_slist * slist);
                Iterator(const Iterator & rhs);
                Iterator(Iterator && rhs);

                Iterator & operator ++(void);
                Iterator   operator ++(int);

                bool operator ==(const Iterator & rhs);
                bool operator !=(const Iterator & rhs);

                struct curl_slist * operator * (void);
                struct curl_slist * operator ->(void);

            private:
                struct curl_slist * slist = nullptr;
        };

    public:
        StringList(struct curl_slist * slist);
        StringList(const StringList & rhs) = delete;
        StringList(StringList && rhs);
        ~StringList(void);

        StringList & operator =(const StringList & rhs) = delete;
        StringList & operator =(StringList && rhs);

        operator struct curl_slist *(void);

        StringList & operator +=(const char * str);

        Iterator begin(void) const;
        Iterator end(void) const;

    private:
        struct curl_slist * slist = nullptr;
};

}; /* Namespace oowe */

std::ostream & operator <<(std::ostream & os, const oowe::String & str);
std::ostream & operator <<(std::ostream & os, const oowe::StringList & list);

#endif /* __OOWE_STRING__ */

