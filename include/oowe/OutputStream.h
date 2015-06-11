#ifndef __OOWE_OUTPUT_STREAM__
#define __OOWE_OUTPUT_STREAM__
#include <cstddef>

namespace oowe {

class OutputStream
{
    public:
        virtual size_t write(char * ptr, size_t size) = 0;
};

}; /* Namespace oowe */

#endif /* __OOWE_OUTPUT_STREAM__ */

