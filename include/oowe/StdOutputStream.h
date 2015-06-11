#ifndef __OOWE_STD_OUTPUT_STREAM__
#define __OOWE_STD_OUTPUT_STREAM__
#include <oowe/OutputStream.h>
#include <type_traits>
#include <ostream>

namespace oowe {

template<class BaseStream>
class StdOutputStream : public OutputStream, public BaseStream
{
    static_assert(std::is_base_of<std::ostream, BaseStream>::value, "StdOutputStream need a template derived from 'std::ostream'");

    public:
        using BaseStream::BaseStream;

        size_t write(char * ptr, size_t size) override;

};

// Templates method
template<class BaseStream>
size_t StdOutputStream<BaseStream>::write(char * ptr, size_t size)
{
    BaseStream::write(ptr, size);

    if(BaseStream::fail())
        return 0;
    else
        return size;
}

}; /* Namespace oowe */

#endif /* __OOWE_STD_OUTPUT_STREAM__ */

