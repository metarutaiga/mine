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
#define END_OPERATION_RANGE_FLOAT_REVERSE(low, high) }; \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<float, double>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, double>(operation); \
        if (format.width == 80 && format.width >= low && format.width <= high) \
            format.operation = specialize<long double, double>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE_FLOAT_SIGNED(low, high) }; \
        if (format.width == 16 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, int16_t>(operation); \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, int32_t>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<double, int64_t>(operation); \
    }
//------------------------------------------------------------------------------
#define END_OPERATION_RANGE_SIGNED_FLOAT(low, high) }; \
        if (format.width == 16 && format.width >= low && format.width <= high) \
            format.operation = specialize<int16_t, double>(operation); \
        if (format.width == 32 && format.width >= low && format.width <= high) \
            format.operation = specialize<int32_t, double>(operation); \
        if (format.width == 64 && format.width >= low && format.width <= high) \
            format.operation = specialize<int64_t, double>(operation); \
    }
//------------------------------------------------------------------------------
