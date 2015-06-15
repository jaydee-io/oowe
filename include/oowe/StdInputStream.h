#ifndef __OOWE_STD_INPUT_STREAM__
#define __OOWE_STD_INPUT_STREAM__
#include <oowe/InputStream.h>
#include <type_traits>
#include <istream>

namespace oowe {

template<class BaseStream>
class StdInputStream : public InputStream, public BaseStream
{
    static_assert(std::is_base_of<std::istream, BaseStream>::value, "StdInputStream need a template derived from 'std::istream'");

    public:
        using BaseStream::BaseStream;

        size_t read(char * ptr, size_t size) override;
        int    seek(long long offset, int origin) override;
};

// Templates method
template<class BaseStream>
size_t StdInputStream<BaseStream>::read(char * ptr, size_t size)
{
    std::streamsize readSize = BaseStream::readsome(ptr, size);

    if(BaseStream::fail())
        return 0;
    else
        return readSize;
}
template<class BaseStream>
int StdInputStream<BaseStream>::seek(long long offset, int origin)
{
    switch(origin)
    {
        case SEEK_SET : BaseStream::seekg(offset, BaseStream::beg); break;
        case SEEK_CUR : BaseStream::seekg(offset, BaseStream::cur); break;
        case SEEK_END : BaseStream::seekg(offset, BaseStream::end); break;
    }

    if(BaseStream::fail())
        return 2;
    else
        return 0;
}

}; /* Namespace oowe */

#endif /* __OOWE_STD_INPUT_STREAM__ */

