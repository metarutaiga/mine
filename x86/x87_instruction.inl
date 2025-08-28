#pragma once

//------------------------------------------------------------------------------
#undef END_OPERATION
#define END_OPERATION }; \
        switch (format.width) { \
        case 8:     format.operation = specialize<uint8_t>(operation);  break; \
        case 16:    format.operation = specialize<uint16_t>(operation); break; \
        case 32:    format.operation = specialize<uint32_t>(operation); break; \
        case 64:    format.operation = specialize<uint64_t>(operation); break; \
        case 80:    format.operation = specialize<uint64_t>(operation); break; \
        } \
    }
//------------------------------------------------------------------------------
namespace std {
template<class T> struct make_float { using type = T; };
template<class T> using make_float_t = make_float<T>::type;
template<> struct make_float<uint32_t> { using type = float; };
template<> struct make_float<uint64_t> { using type = double; };
};
//------------------------------------------------------------------------------
