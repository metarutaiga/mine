#pragma once

//------------------------------------------------------------------------------
#define END_OPERATION_MMX }; \
        format.operation = specialize<mmx_register::register_t, mmx_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_REG_MMX }; \
        format.operation = specialize<x86_register::register_t, mmx_register::register_t>(operation); \
    }
#define END_OPERATION_MMX_REG }; \
        format.operation = specialize<mmx_register::register_t, x86_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
