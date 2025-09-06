#pragma once

//------------------------------------------------------------------------------
#define END_OPERATION_RANGE_FLOAT(low, high) }; \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, float>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, double>(operation); \
        if (format.width == 80 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, long double>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE_FLOAT_STORE(low, high) }; \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<float, float>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, double>(operation); \
        if (format.width == 80 && format.width >= low && format.width <= high) \
            format.operation = specialize<long double, long double>(operation); \
    }
//------------------------------------------------------------------------------
