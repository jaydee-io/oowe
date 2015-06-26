#ifndef __OOWE_HTTP_MULTI_SESSION__
#define __OOWE_HTTP_MULTI_SESSION__
#include <oowe/MultiSession.h>
#include "oowe/oowe_macros.h"

namespace oowe {

class HttpMultiSession : public MultiSession
{
    public:
        using MultiSession::MultiSession;

        MULTI_SET_LONG      (Pipelining              , PIPELINING                 ) // Enable / disable HTTP pipelining 
        MULTI_SET_LONG      (MaxHostConnections      , MAX_HOST_CONNECTIONS       ) // Set max number of connections to a single host 
        MULTI_SET_LONG      (MaxPipelineLength       , MAX_PIPELINE_LENGTH        ) // Set maximum number of requests in a pipeline
        MULTI_SET_LONG      (ContentLengthPenaltySize, CONTENT_LENGTH_PENALTY_SIZE) // Set size threshold for pipelining penalty
        MULTI_SET_LONG      (ChunkLengthPenaltySize  , CHUNK_LENGTH_PENALTY_SIZE  ) // Set chunk length threshold for pipelining
        MULTI_SET_STRING_LST(PipeliningSiteBL        , PIPELINING_SITE_BL         ) // Set pipelining host blacklist
        MULTI_SET_STRING_LST(PipeliningServerBL      , PIPELINING_SERVER_BL       ) // Set pipelining server blacklist
};

}; /* Namespace oowe */

#endif /* __OOWE_HTTP_MULTI_SESSION__ */
