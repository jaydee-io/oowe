#ifndef __OOWE_INPUT_STREAM__
#define __OOWE_INPUT_STREAM__
#include <cstddef>

namespace oowe {

class InputStream
{
    public:
        virtual size_t read(char * ptr, size_t size) = 0;
        virtual int    seek(long long offset, int origin) = 0;
};

}; /* Namespace oowe */

#endif /* __OOWE_INPUT_STREAM__ */

