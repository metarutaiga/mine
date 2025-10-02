#pragma once

//------------------------------------------------------------------------------
#define END_OPERATION_SSE }; \
        format.operation = specialize<sse_register::register_t, sse_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
