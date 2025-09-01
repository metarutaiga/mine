#pragma once

//------------------------------------------------------------------------------
#define END_OPERATION_FLOAT }; \
        switch (format.width) { \
        case 32: format.operation = specialize<float>(operation);       break; \
        case 64: format.operation = specialize<double>(operation);      break; \
        case 80: format.operation = specialize<long double>(operation); break; \
        } \
    }
//------------------------------------------------------------------------------
