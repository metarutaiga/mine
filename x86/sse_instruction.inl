#pragma once

//------------------------------------------------------------------------------
#define END_OPERATION_SSE }; \
        format.operation = specialize<sse_register::register_t, sse_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_REG_SSE }; \
        format.operation = specialize<x86_register::register_t, sse_register::register_t>(operation); \
    }
#define END_OPERATION_SSE_REG }; \
        format.operation = specialize<sse_register::register_t, x86_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_MMX_SSE }; \
        format.operation = specialize<mmx_register::register_t, sse_register::register_t>(operation); \
    }
#define END_OPERATION_SSE_MMX }; \
        format.operation = specialize<sse_register::register_t, mmx_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
