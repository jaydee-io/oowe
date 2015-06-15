#include "oowe/Session.h"
#include <utility>

namespace oowe {

Session::Session(void)
: curl(curl_easy_init()), inputStream(nullptr), outputStream(nullptr)
{
    if(!curl)
    {
        std::cerr << "Erreur dans curl_easy_init()" << std::endl;
        // TODO Throw error
    }
}

Session::Session(const char * url)
: Session()
{
    setUrl(url);
}

Session::Session(const Session & rhs)
: curl(curl_easy_duphandle(rhs.curl)), inputStream(rhs.inputStream), outputStream(rhs.outputStream)
{
}

Session::Session(Session && rhs)
: curl(nullptr), inputStream(nullptr), outputStream(nullptr)
{
    std::swap(curl,         rhs.curl);
    std::swap(inputStream,  rhs.inputStream );
    std::swap(outputStream, rhs.outputStream);
}

Session::~Session(void)
{
    if(curl)
        curl_easy_cleanup(curl);
}

Session & Session::operator =(const Session & rhs)
{
    if(this != &rhs)
    {
        if(curl)
        {
            curl_easy_cleanup(curl);
            curl         = nullptr;
            inputStream  = nullptr;
            outputStream = nullptr;
        }
        curl = curl_easy_duphandle(rhs.curl);
        inputStream  = rhs.inputStream;
        outputStream = rhs.outputStream;
    }

    return *this;
}

Session & Session::operator =(Session && rhs)
{
    if(this != &rhs)
    {
        if(curl)
        {
            curl_easy_cleanup(curl);
            curl         = nullptr;
            inputStream  = nullptr;
            outputStream = nullptr;
        }
        std::swap(curl,         rhs.curl);
        std::swap(inputStream,  rhs.inputStream);
        std::swap(outputStream, rhs.outputStream);
    }

    return *this;
}

String Session::escape(const char * str, int len)
{
    const char * escStr = curl_easy_escape(curl, str, len);

    return String(escStr);
}

String Session::escape(const std::string & str)
{
    const char * escStr = curl_easy_escape(curl, str.c_str(), str.size());

    return String(escStr);
}

String Session::escape(const String & str)
{
    const char * escStr = curl_easy_escape(curl, str.c_str(), str.size());

    return String(escStr);
}

String Session::unescape(const char * str, int len)
{
    int unescLen = 0;
    const char * unescStr = curl_easy_unescape(curl, str, len, &unescLen);

    return String(unescStr, unescLen);
}

String Session::unescape(const std::string & str)
{
    int unescLen = 0;
    const char * unescStr = curl_easy_unescape(curl, str.c_str(), str.size(), &unescLen);

    return String(unescStr, unescLen);
}

String Session::unescape(const String & str)
{
    int unescLen = 0;
    const char * unescStr = curl_easy_unescape(curl, str.c_str(), str.size(), &unescLen);

    return String(unescStr, unescLen);
}

void Session::pause(int action)
{
    OOWE_CHECK_ERROR_THROW(curl_easy_pause(curl, action));
}

void Session::perform(void)
{
    OOWE_CHECK_ERROR_THROW(curl_easy_perform(curl));
}

size_t Session::recv(void * buffer, size_t len)
{
    size_t n;

    OOWE_CHECK_ERROR_THROW(curl_easy_recv(curl, buffer, len, &n));

    return n;
}

size_t Session::send(const void * buffer, size_t len)
{
    size_t n;

    OOWE_CHECK_ERROR_THROW(curl_easy_send(curl, buffer, len, &n));

    return n;
}

void Session::setInputStream(InputStream * stream)
{
    curl_read_callback cbRead = stream ? Session::readCallback : nullptr;
    curl_seek_callback cbSeek = stream ? Session::seekCallback : nullptr;

    inputStream = stream;
    set<curl_read_callback>(CURLOPT_READFUNCTION, cbRead);
    set<void *            >(CURLOPT_READDATA,     this);
    set<curl_seek_callback>(CURLOPT_SEEKFUNCTION, cbSeek);
    set<void *            >(CURLOPT_SEEKDATA,     this);
}

void Session::setOutputStream(OutputStream * stream)
{
    curl_write_callback cb = stream ? Session::writeCallback : nullptr;

    outputStream = stream;
    set<curl_write_callback>(CURLOPT_WRITEFUNCTION, cb);
    set<void *             >(CURLOPT_WRITEDATA,     this);
}

void Session::reset(void)
{
    curl_easy_reset(curl);
}

size_t Session::writeCallback(char * buffer, size_t size, size_t nitems, void * user)
{
    if(!user)
        return 0;

    Session * session = reinterpret_cast<Session *>(user);

    if(!session->outputStream)
        return 0;

    return session->outputStream->write(buffer, size * nitems);
}

size_t Session::readCallback(char * buffer, size_t size, size_t nitems, void * user)
{
    if(!user)
        return 0;

    Session * session = reinterpret_cast<Session *>(user);

    if(!session->inputStream)
        return 0;

    return session->inputStream->read(buffer, size * nitems);
}

int Session::seekCallback(void * user, curl_off_t offset, int origin)
{
    if(!user)
        return 0;

    Session * session = reinterpret_cast<Session *>(user);

    if(!session->inputStream)
        return 0;

    return session->inputStream->seek(offset, origin);
}

}; /* Namespace oowe */
