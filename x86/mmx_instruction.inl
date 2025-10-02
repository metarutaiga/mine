#pragma once

//------------------------------------------------------------------------------
#define END_OPERATION_MMX }; \
        format.operation = specialize<mmx_register::register_t, mmx_register::register_t>(operation); \
    }
//------------------------------------------------------------------------------
