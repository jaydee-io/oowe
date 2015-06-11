#include "oowe/String.h"
#include <cstring>

namespace oowe {

String::String(const char * s, int l)
: str(s), len((l != -1) ? l : strlen(s))
{
}

String::~String(void)
{
    curl_free(const_cast<char *>(str));
}

int String::size(void) const
{
    return len;
}

const char * String::c_str(void) const
{
    return str;
}

const std::string String::cpp_str(void) const
{
    return std::string(str, len);
}





StringList::Iterator::Iterator(struct curl_slist * l)
: slist(l)
{
}

StringList::Iterator::Iterator(const StringList::Iterator & rhs)
: slist(rhs.slist)
{
}

StringList::Iterator::Iterator(StringList::Iterator && rhs)
: slist(nullptr)
{
    std::swap(slist, rhs.slist);
}

StringList::Iterator & StringList::Iterator::operator ++(void)
{
    if(slist)
        slist = slist->next;

    return *this;
}

StringList::Iterator StringList::Iterator::operator ++(int)
{
    Iterator old(*this);

    operator++();

    return old;
}

bool StringList::Iterator::operator ==(const StringList::Iterator & rhs)
{
    return (slist == rhs.slist);
}

bool StringList::Iterator::operator !=(const StringList::Iterator & rhs)
{
    return !(*this == rhs);
}

struct curl_slist * StringList::Iterator::operator * (void)
{
    return slist;
}

struct curl_slist * StringList::Iterator::operator ->(void)
{
    return slist;
}





StringList::StringList(struct curl_slist * l)
: slist(l)
{
}

StringList::StringList(StringList && rhs)
: slist(nullptr)
{
    std::swap(slist, rhs.slist);
}

StringList::~StringList(void)
{
    curl_slist_free_all(slist);
}

StringList & StringList::operator =(StringList && rhs)
{
    if(this != &rhs)
    {
        if(slist)
        {
            curl_slist_free_all(slist);
            slist = nullptr;
        }
        std::swap(slist, rhs.slist);
    }

    return *this;
}

StringList::operator struct curl_slist *(void)
{
    return slist;
}

StringList & StringList::operator +=(const char * str)
{
    slist = curl_slist_append(slist, str);

    return *this;
}

StringList::Iterator StringList::begin(void) const
{
    return StringList::Iterator(slist);
}

StringList::Iterator StringList::end(void) const
{
    return StringList::Iterator(nullptr);
}

}; /* Namespace oowe */

std::ostream & operator <<(std::ostream & os, const oowe::String & str)
{
    os << str.c_str();
    return os;
}

std::ostream & operator <<(std::ostream & os, const oowe::StringList & list)
{
    os << "{ ";
    for(struct curl_slist * str : list)
        if(str->next)
            os << str->data << ", ";
        else
            os << str->data;
    os << " }";

    return os;
}
