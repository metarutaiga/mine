#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#include "x86/x86_i386.h"

constexpr uint64_t operator ""_CC(char const* text, size_t length) {
    uint64_t value = 0;
    for (size_t i = 0; i < length; ++i)
        value += uint64_t(uint8_t(text[i])) << (i % 8 * 8);
    return value;
};

#ifdef __cplusplus
extern "C" {
#endif

size_t syscall_i386(void* data, size_t index)
{
    [[maybe_unused]] auto* cpu = (x86_i386*)data;
    [[maybe_unused]] auto& x86 = cpu->x86;
    [[maybe_unused]] auto& x87 = cpu->x87;
    [[maybe_unused]] auto* allocator = cpu->Allocator();
    [[maybe_unused]] auto* memory = cpu->Memory();
    [[maybe_unused]] auto* stack = memory + cpu->Stack();

    auto EAX = [&](long value) {
        x86.regs[0].d = uint32_t(value);
        return 0;
    };

    auto EDXEAX = [&](long long value) {
        x86.regs[0].d = uint32_t(value);
        x86.regs[2].d = uint32_t(value >> 32);
        return 0;
    };

    auto FLD = [&](double value) {
        x87.sts[x87.status._TOP -= 1].d = value;
        return 0;
    };

    switch (index) {

    // assert
    case (uint32_t)-1:      return EAX(syscall_assert(stack));

    // ctype
    case (uint32_t)-11:     return EAX(syscall_isalnum(stack));
    case (uint32_t)-12:     return EAX(syscall_isalpha(stack));
    case (uint32_t)-13:     return EAX(syscall_iscntrl(stack));
    case (uint32_t)-14:     return EAX(syscall_isdigit(stack));
    case (uint32_t)-15:     return EAX(syscall_isgraph(stack));
    case (uint32_t)-16:     return EAX(syscall_islower(stack));
    case (uint32_t)-17:     return EAX(syscall_isprint(stack));
    case (uint32_t)-18:     return EAX(syscall_ispunct(stack));
    case (uint32_t)-19:     return EAX(syscall_isspace(stack));
    case (uint32_t)-20:     return EAX(syscall_isupper(stack));
    case (uint32_t)-21:     return EAX(syscall_isxdigit(stack));
    case (uint32_t)-22:     return EAX(syscall_tolower(stack));
    case (uint32_t)-23:     return EAX(syscall_toupper(stack));

    // locale
    case (uint32_t)-31:     return EAX(syscall_localeconv(memory));
    case (uint32_t)-32:     return EAX(syscall_setlocale(memory, stack));

    // math
    case (uint32_t)-41:     return FLD(syscall_acos(stack));
    case (uint32_t)-42:     return FLD(syscall_acosh(stack));
    case (uint32_t)-43:     return FLD(syscall_asin(stack));
    case (uint32_t)-44:     return FLD(syscall_asinh(stack));
    case (uint32_t)-45:     return FLD(syscall_atan(stack));
    case (uint32_t)-46:     return FLD(syscall_atan2(stack));
    case (uint32_t)-47:     return FLD(syscall_atanh(stack));
    case (uint32_t)-48:     return FLD(syscall_cbrt(stack));
    case (uint32_t)-49:     return FLD(syscall_ceil(stack));
    case (uint32_t)-50:     return FLD(syscall_copysign(stack));
    case (uint32_t)-51:     return FLD(syscall_cos(stack));
    case (uint32_t)-52:     return FLD(syscall_cosh(stack));
    case (uint32_t)-53:     return FLD(syscall_erf(stack));
    case (uint32_t)-54:     return FLD(syscall_erfc(stack));
    case (uint32_t)-55:     return FLD(syscall_exp(stack));
    case (uint32_t)-56:     return FLD(syscall_exp2(stack));
    case (uint32_t)-57:     return FLD(syscall_expm1(stack));
    case (uint32_t)-58:     return FLD(syscall_fabs(stack));
    case (uint32_t)-59:     return FLD(syscall_fdim(stack));
    case (uint32_t)-60:     return FLD(syscall_floor(stack));
    case (uint32_t)-61:     return FLD(syscall_fma(stack));
    case (uint32_t)-62:     return FLD(syscall_fmax(stack));
    case (uint32_t)-63:     return FLD(syscall_fmin(stack));
    case (uint32_t)-64:     return FLD(syscall_fmod(stack));
    case (uint32_t)-65:     return EAX(syscall_fpclassify(stack));
    case (uint32_t)-66:     return FLD(syscall_frexp(memory, stack));
    case (uint32_t)-67:     return FLD(syscall_hypot(stack));
    case (uint32_t)-68:     return EAX(syscall_ilogb(stack));
    case (uint32_t)-69:     return EAX(syscall_isfinite(stack));
    case (uint32_t)-70:     return EAX(syscall_isgreater(stack));
    case (uint32_t)-71:     return EAX(syscall_isgreaterequal(stack));
    case (uint32_t)-72:     return EAX(syscall_isinf(stack));
    case (uint32_t)-73:     return EAX(syscall_isless(stack));
    case (uint32_t)-74:     return EAX(syscall_islessequal(stack));
    case (uint32_t)-75:     return EAX(syscall_islessgreater(stack));
    case (uint32_t)-76:     return EAX(syscall_isnan(stack));
    case (uint32_t)-77:     return EAX(syscall_isnormal(stack));
    case (uint32_t)-78:     return EAX(syscall_isunordered(stack));
    case (uint32_t)-79:     return FLD(syscall_ldexp(stack));
    case (uint32_t)-80:     return FLD(syscall_lgamma(stack));
    case (uint32_t)-81:     return EDXEAX(syscall_llrint(stack));
    case (uint32_t)-82:     return EDXEAX(syscall_llround(stack));
    case (uint32_t)-83:     return FLD(syscall_log(stack));
    case (uint32_t)-84:     return FLD(syscall_log10(stack));
    case (uint32_t)-85:     return FLD(syscall_log1p(stack));
    case (uint32_t)-86:     return FLD(syscall_log2(stack));
    case (uint32_t)-87:     return FLD(syscall_logb(stack));
    case (uint32_t)-88:     return EAX(syscall_lrint(stack));
    case (uint32_t)-89:     return EAX(syscall_lround(stack));
    case (uint32_t)-90:     return FLD(syscall_modf(memory, stack));
    case (uint32_t)-91:     return FLD(syscall_nan(memory, stack));
    case (uint32_t)-92:     return FLD(syscall_nanf(memory, stack));
    case (uint32_t)-93:     return FLD(syscall_nanl(memory, stack));
    case (uint32_t)-94:     return FLD(syscall_nearbyint(stack));
    case (uint32_t)-95:     return FLD(syscall_nextafter(stack));
    case (uint32_t)-96:     return FLD(syscall_nexttoward(stack));
    case (uint32_t)-97:     return FLD(syscall_pow(stack));
    case (uint32_t)-98:     return FLD(syscall_remainder(stack));
    case (uint32_t)-99:     return FLD(syscall_remquo(memory, stack));
    case (uint32_t)-100:    return FLD(syscall_rint(stack));
    case (uint32_t)-101:    return FLD(syscall_round(stack));
    case (uint32_t)-102:    return FLD(syscall_scalbln(stack));
    case (uint32_t)-103:    return FLD(syscall_scalbn(stack));
    case (uint32_t)-104:    return EAX(syscall_signbit(stack));
    case (uint32_t)-105:    return FLD(syscall_sin(stack));
    case (uint32_t)-106:    return FLD(syscall_sinh(stack));
    case (uint32_t)-107:    return FLD(syscall_sqrt(stack));
    case (uint32_t)-108:    return FLD(syscall_tan(stack));
    case (uint32_t)-109:    return FLD(syscall_tanh(stack));
    case (uint32_t)-110:    return FLD(syscall_tgamma(stack));
    case (uint32_t)-111:    return FLD(syscall_trunc(stack));

    // setjmp
    case (uint32_t)-121:    return EAX(syscall_longjmp(memory, stack));
    case (uint32_t)-122:    return EAX(syscall_setjmp(memory, stack));

    // signal
    case (uint32_t)-131:    return EAX(syscall_raise(stack));

    // stdio
    case (uint32_t)-141:    return EAX(syscall_clearerr(memory, stack));
    case (uint32_t)-142:    return EAX(syscall_fclose(memory, stack, allocator));
    case (uint32_t)-143:    return EAX(syscall_feof(memory, stack));
    case (uint32_t)-144:    return EAX(syscall_ferror(memory, stack));
    case (uint32_t)-145:    return EAX(syscall_fflush(memory, stack));
    case (uint32_t)-146:    return EAX(syscall_fgetc(memory, stack));
    case (uint32_t)-147:    return EAX(syscall_fgetpos(memory, stack));
    case (uint32_t)-148:    return EAX(syscall_fgets(memory, stack));
    case (uint32_t)-149:    return EAX(syscall_fopen(memory, stack, allocator));
    case (uint32_t)-150:    return EAX(syscall_fprintf(memory, stack));
    case (uint32_t)-151:    return EAX(syscall_fputc(memory, stack));
    case (uint32_t)-152:    return EAX(syscall_fputs(memory, stack));
    case (uint32_t)-153:    return EAX(syscall_fread(memory, stack));
    case (uint32_t)-154:    return EAX(syscall_freopen(memory, stack));
    case (uint32_t)-155:    return EAX(syscall_fscanf(memory, stack));
    case (uint32_t)-156:    return EAX(syscall_fseek(memory, stack));
    case (uint32_t)-157:    return EAX(syscall_fsetpos(memory, stack));
    case (uint32_t)-158:    return EAX(syscall_ftell(memory, stack));
    case (uint32_t)-159:    return EAX(syscall_fwrite(memory, stack));
    case (uint32_t)-160:    return EAX(syscall_getc(memory, stack));
    case (uint32_t)-161:    return EAX(syscall_getchar());
    case (uint32_t)-162:    return EAX(syscall_gets(memory, stack));
    case (uint32_t)-163:    return EAX(syscall_perror(memory, stack));
    case (uint32_t)-164:    return EAX(syscall_printf(memory, stack, vprintf));
    case (uint32_t)-165:    return EAX(syscall_putc(memory, stack));
    case (uint32_t)-166:    return EAX(syscall_putchar(stack));
    case (uint32_t)-167:    return EAX(syscall_puts(memory, stack));
    case (uint32_t)-168:    return EAX(syscall_remove(memory, stack));
    case (uint32_t)-169:    return EAX(syscall_rename(memory, stack));
    case (uint32_t)-170:    return EAX(syscall_rewind(memory, stack));
    case (uint32_t)-171:    return EAX(syscall_scanf(memory, stack));
    case (uint32_t)-172:    return EAX(syscall_setbuf(memory, stack));
    case (uint32_t)-173:    return EAX(syscall_setvbuf(memory, stack));
    case (uint32_t)-174:    return EAX(syscall_snprintf(memory, stack));
    case (uint32_t)-175:    return EAX(syscall_sprintf(memory, stack));
    case (uint32_t)-176:    return EAX(syscall_sscanf(memory, stack));
    case (uint32_t)-177:    return EAX(syscall_tmpfile(memory, allocator));
    case (uint32_t)-178:    return EAX(syscall_tmpnam(memory, stack));
    case (uint32_t)-179:    return EAX(syscall_ungetc(memory, stack));
    case (uint32_t)-180:    return EAX(syscall_vfprintf(memory, stack));
    case (uint32_t)-181:    return EAX(syscall_vfscanf(memory, stack));
    case (uint32_t)-182:    return EAX(syscall_vprintf(memory, stack, vprintf));
    case (uint32_t)-183:    return EAX(syscall_vscanf(memory, stack));
    case (uint32_t)-184:    return EAX(syscall_vsnprintf(memory, stack));
    case (uint32_t)-185:    return EAX(syscall_vsprintf(memory, stack));

    // stdlib
    case (uint32_t)-191:    return EAX(syscall__Exit(stack));
    case (uint32_t)-192:    return EAX(syscall_abort());
    case (uint32_t)-193:    return EAX(syscall_abs(stack));
    case (uint32_t)-194:    return EAX(syscall_at_quick_exit(memory, stack));
    case (uint32_t)-195:    return EAX(syscall_atexit(memory, stack));
    case (uint32_t)-196:    return FLD(syscall_atof(memory, stack));
    case (uint32_t)-197:    return EAX(syscall_atoi(memory, stack));
    case (uint32_t)-198:    return EAX(syscall_atol(memory, stack));
    case (uint32_t)-199:    return EDXEAX(syscall_atoll(memory, stack));
    case (uint32_t)-200:    return EAX(syscall_bsearch(memory, stack));
    case (uint32_t)-201:    return EAX(syscall_calloc(stack, allocator));
    case (uint32_t)-202:    return EAX(syscall_div(stack).quot);
    case (uint32_t)-203:    return EAX(syscall_exit(stack));
    case (uint32_t)-204:    return EAX(syscall_expand(stack, allocator));
    case (uint32_t)-205:    return EAX(syscall_free(stack, allocator));
    case (uint32_t)-206:    return EAX(syscall_getenv(memory, stack));
    case (uint32_t)-207:    return EAX(syscall_labs(stack));
    case (uint32_t)-208:    return EAX(syscall_ldiv(stack).quot);
    case (uint32_t)-209:    return EDXEAX(syscall_llabs(stack));
    case (uint32_t)-210:    return EDXEAX(syscall_lldiv(stack).quot);
    case (uint32_t)-211:    return EAX(syscall_malloc(stack, allocator));
    case (uint32_t)-212:    return EAX(syscall_mblen(memory, stack));
    case (uint32_t)-213:    return EAX(syscall_mbstowcs(memory, stack));
    case (uint32_t)-214:    return EAX(syscall_mbtowc(memory, stack));
    case (uint32_t)-215:    return EAX(syscall_msize(stack, allocator));
    case (uint32_t)-216:    return EAX(syscall_qsort(memory, stack));
    case (uint32_t)-217:    return EAX(syscall_quick_exit(stack));
    case (uint32_t)-218:    return EAX(syscall_rand());
    case (uint32_t)-219:    return EAX(syscall_realloc(stack, allocator));
    case (uint32_t)-220:    return EAX(syscall_recalloc(stack, allocator));
    case (uint32_t)-221:    return EAX(syscall_srand(stack));
    case (uint32_t)-222:    return FLD(syscall_strtod(memory, stack));
    case (uint32_t)-223:    return FLD(syscall_strtof(memory, stack));
    case (uint32_t)-224:    return EAX(syscall_strtol(memory, stack));
    case (uint32_t)-225:    return FLD(syscall_strtold(memory, stack));
    case (uint32_t)-226:    return EDXEAX(syscall_strtoll(memory, stack));
    case (uint32_t)-227:    return EAX(syscall_strtoul(memory, stack));
    case (uint32_t)-228:    return EDXEAX(syscall_strtoull(memory, stack));
    case (uint32_t)-229:    return EAX(syscall_system(memory, stack));
    case (uint32_t)-230:    return EAX(syscall_wcstombs(memory, stack));
    case (uint32_t)-231:    return EAX(syscall_wctomb(memory, stack));

    // string
    case (uint32_t)-241:    return EAX(syscall_memchr(memory, stack));
    case (uint32_t)-242:    return EAX(syscall_memcmp(memory, stack));
    case (uint32_t)-243:    return EAX(syscall_memcpy(memory, stack));
    case (uint32_t)-244:    return EAX(syscall_memmove(memory, stack));
    case (uint32_t)-245:    return EAX(syscall_memset(memory, stack));
    case (uint32_t)-246:    return EAX(syscall_strcat(memory, stack));
    case (uint32_t)-247:    return EAX(syscall_strchr(memory, stack));
    case (uint32_t)-248:    return EAX(syscall_strcmp(memory, stack));
    case (uint32_t)-249:    return EAX(syscall_strcoll(memory, stack));
    case (uint32_t)-240:    return EAX(syscall_strcpy(memory, stack));
    case (uint32_t)-251:    return EAX(syscall_strcspn(memory, stack));
    case (uint32_t)-252:    return EAX(syscall_strerror(memory, stack));
    case (uint32_t)-253:    return EAX(syscall_strlen(memory, stack));
    case (uint32_t)-254:    return EAX(syscall_strncat(memory, stack));
    case (uint32_t)-255:    return EAX(syscall_strncmp(memory, stack));
    case (uint32_t)-256:    return EAX(syscall_strncpy(memory, stack));
    case (uint32_t)-257:    return EAX(syscall_strpbrk(memory, stack));
    case (uint32_t)-258:    return EAX(syscall_strrchr(memory, stack));
    case (uint32_t)-259:    return EAX(syscall_strspn(memory, stack));
    case (uint32_t)-260:    return EAX(syscall_strstr(memory, stack));
    case (uint32_t)-261:    return EAX(syscall_strtok(memory, stack));
    case (uint32_t)-262:    return EAX(syscall_strxfrm(memory, stack));

    // time
    case (uint32_t)-271:    return EAX(syscall_asctime(memory, stack));
    case (uint32_t)-272:    return EAX(syscall_clock());
    case (uint32_t)-273:    return EAX(syscall_ctime(memory, stack));
    case (uint32_t)-274:    return FLD(syscall_difftime(stack));
    case (uint32_t)-275:    return EAX(syscall_gmtime(memory, stack));
    case (uint32_t)-276:    return EAX(syscall_localtime(memory, stack));
    case (uint32_t)-277:    return EAX(syscall_mktime(memory, stack));
    case (uint32_t)-278:    return EAX(syscall_strftime(memory, stack));
    case (uint32_t)-279:    return EAX(syscall_time(memory, stack));
    }
    return 0;
}

size_t syscall_symbol_i386(const char* file, const char* name, size_t address, void* sym_data)
{
    if (file == nullptr)
        return 0;

    switch (operator""_CC(name, strlen(name))) {

    // assert
    case "assert"_CC:           return (uint32_t)-1;

    // ctype
    case "isalnum"_CC:          return (uint32_t)-11;
    case "isalpha"_CC:          return (uint32_t)-12;
    case "iscntrl"_CC:          return (uint32_t)-13;
    case "isdigit"_CC:          return (uint32_t)-14;
    case "isgraph"_CC:          return (uint32_t)-15;
    case "islower"_CC:          return (uint32_t)-16;
    case "isprint"_CC:          return (uint32_t)-17;
    case "ispunct"_CC:          return (uint32_t)-18;
    case "isspace"_CC:          return (uint32_t)-19;
    case "isupper"_CC:          return (uint32_t)-20;
    case "isxdigit"_CC:         return (uint32_t)-21;
    case "tolower"_CC:          return (uint32_t)-22;
    case "toupper"_CC:          return (uint32_t)-23;

    // locale
    case "localeconv"_CC:       return (uint32_t)-31;
    case "setlocale"_CC:        return (uint32_t)-32;

    // math
    case "acos"_CC:             return (uint32_t)-41;
    case "acosh"_CC:            return (uint32_t)-42;
    case "asin"_CC:             return (uint32_t)-43;
    case "asinh"_CC:            return (uint32_t)-44;
    case "atan"_CC:             return (uint32_t)-45;
    case "atan2"_CC:            return (uint32_t)-46;
    case "atanh"_CC:            return (uint32_t)-47;
    case "cbrt"_CC:             return (uint32_t)-48;
    case "ceil"_CC:             return (uint32_t)-49;
    case "copysign"_CC:         return (uint32_t)-50;
    case "cos"_CC:              return (uint32_t)-51;
    case "cosh"_CC:             return (uint32_t)-52;
    case "erf"_CC:              return (uint32_t)-53;
    case "erfc"_CC:             return (uint32_t)-54;
    case "exp"_CC:              return (uint32_t)-55;
    case "exp2"_CC:             return (uint32_t)-56;
    case "expm1"_CC:            return (uint32_t)-57;
    case "fabs"_CC:             return (uint32_t)-58;
    case "fdim"_CC:             return (uint32_t)-59;
    case "floor"_CC:            return (uint32_t)-60;
    case "fma"_CC:              return (uint32_t)-61;
    case "fmax"_CC:             return (uint32_t)-62;
    case "fmin"_CC:             return (uint32_t)-63;
    case "fmod"_CC:             return (uint32_t)-64;
    case "fpclassify"_CC:       return (uint32_t)-65;
    case "frexp"_CC:            return (uint32_t)-66;
    case "hypot"_CC:            return (uint32_t)-67;
    case "ilogb"_CC:            return (uint32_t)-68;
    case "isfinite"_CC:         return (uint32_t)-69;
    case "isgreater"_CC:        return (uint32_t)-70;
    case "isgreaterequal"_CC:   return (uint32_t)-71;
    case "isinf"_CC:            return (uint32_t)-72;
    case "isless"_CC:           return (uint32_t)-73;
    case "islessequal"_CC:      return (uint32_t)-74;
    case "islessgreater"_CC:    return (uint32_t)-75;
    case "isnan"_CC:            return (uint32_t)-76;
    case "isnormal"_CC:         return (uint32_t)-77;
    case "isunordered"_CC:      return (uint32_t)-78;
    case "ldexp"_CC:            return (uint32_t)-79;
    case "lgamma"_CC:           return (uint32_t)-80;
    case "llrint"_CC:           return (uint32_t)-81;
    case "llround"_CC:          return (uint32_t)-82;
    case "log"_CC:              return (uint32_t)-83;
    case "log10"_CC:            return (uint32_t)-84;
    case "log1p"_CC:            return (uint32_t)-85;
    case "log2"_CC:             return (uint32_t)-86;
    case "logb"_CC:             return (uint32_t)-87;
    case "lrint"_CC:            return (uint32_t)-88;
    case "lround"_CC:           return (uint32_t)-89;
    case "modf"_CC:             return (uint32_t)-90;
    case "nan"_CC:              return (uint32_t)-91;
    case "nanf"_CC:             return (uint32_t)-92;
    case "nanl"_CC:             return (uint32_t)-93;
    case "nearbyint"_CC:        return (uint32_t)-94;
    case "nextafter"_CC:        return (uint32_t)-95;
    case "nexttoward"_CC:       return (uint32_t)-96;
    case "pow"_CC:              return (uint32_t)-97;
    case "remainder"_CC:        return (uint32_t)-98;
    case "remquo"_CC:           return (uint32_t)-99;
    case "rint"_CC:             return (uint32_t)-100;
    case "round"_CC:            return (uint32_t)-101;
    case "scalbln"_CC:          return (uint32_t)-102;
    case "scalbn"_CC:           return (uint32_t)-103;
    case "signbit"_CC:          return (uint32_t)-104;
    case "sin"_CC:              return (uint32_t)-105;
    case "sinh"_CC:             return (uint32_t)-106;
    case "sqrt"_CC:             return (uint32_t)-107;
    case "tan"_CC:              return (uint32_t)-108;
    case "tanh"_CC:             return (uint32_t)-109;
    case "tgamma"_CC:           return (uint32_t)-110;
    case "trunc"_CC:            return (uint32_t)-111;

    // setjmp
    case "longjmp"_CC:          return (uint32_t)-121;
    case "setjmp"_CC:           return (uint32_t)-122;

    // signal
    case "raise"_CC:            return (uint32_t)-131;

    // stdio
    case "clearerr"_CC:         return (uint32_t)-141;
    case "fclose"_CC:           return (uint32_t)-142;
    case "feof"_CC:             return (uint32_t)-143;
    case "ferror"_CC:           return (uint32_t)-144;
    case "fflush"_CC:           return (uint32_t)-145;
    case "fgetc"_CC:            return (uint32_t)-146;
    case "fgetpos"_CC:          return (uint32_t)-147;
    case "fgets"_CC:            return (uint32_t)-148;
    case "fopen"_CC:            return (uint32_t)-149;
    case "fprintf"_CC:          return (uint32_t)-150;
    case "fputc"_CC:            return (uint32_t)-151;
    case "fputs"_CC:            return (uint32_t)-152;
    case "fread"_CC:            return (uint32_t)-153;
    case "freopen"_CC:          return (uint32_t)-154;
    case "fscanf"_CC:           return (uint32_t)-155;
    case "fseek"_CC:            return (uint32_t)-156;
    case "fsetpos"_CC:          return (uint32_t)-157;
    case "ftell"_CC:            return (uint32_t)-158;
    case "fwrite"_CC:           return (uint32_t)-159;
    case "getc"_CC:             return (uint32_t)-160;
    case "getchar"_CC:          return (uint32_t)-161;
    case "gets"_CC:             return (uint32_t)-162;
    case "perror"_CC:           return (uint32_t)-163;
    case "printf"_CC:           return (uint32_t)-164;
    case "putc"_CC:             return (uint32_t)-165;
    case "putchar"_CC:          return (uint32_t)-166;
    case "puts"_CC:             return (uint32_t)-167;
    case "remove"_CC:           return (uint32_t)-168;
    case "rename"_CC:           return (uint32_t)-169;
    case "rewind"_CC:           return (uint32_t)-170;
    case "scanf"_CC:            return (uint32_t)-171;
    case "setbuf"_CC:           return (uint32_t)-172;
    case "setvbuf"_CC:          return (uint32_t)-173;
    case "snprintf"_CC:         return (uint32_t)-174;
    case "sprintf"_CC:          return (uint32_t)-175;
    case "sscanf"_CC:           return (uint32_t)-176;
    case "tmpfile"_CC:          return (uint32_t)-177;
    case "tmpnam"_CC:           return (uint32_t)-178;
    case "ungetc"_CC:           return (uint32_t)-179;
    case "vfprintf"_CC:         return (uint32_t)-180;
    case "vfscanf"_CC:          return (uint32_t)-181;
    case "vprintf"_CC:          return (uint32_t)-182;
    case "vscanf"_CC:           return (uint32_t)-183;
    case "vsnprintf"_CC:        return (uint32_t)-184;
    case "vsprintf"_CC:         return (uint32_t)-185;

    // stdlib
    case "_Exit"_CC:            return (uint32_t)-191;
    case "abort"_CC:            return (uint32_t)-192;
    case "abs"_CC:              return (uint32_t)-193;
    case "at_quick_exit"_CC:    return (uint32_t)-194;
    case "atexit"_CC:           return (uint32_t)-195;
    case "atof"_CC:             return (uint32_t)-196;
    case "atoi"_CC:             return (uint32_t)-197;
    case "atol"_CC:             return (uint32_t)-198;
    case "atoll"_CC:            return (uint32_t)-199;
    case "bsearch"_CC:          return (uint32_t)-200;
    case "calloc"_CC:           return (uint32_t)-201;
    case "div"_CC:              return (uint32_t)-202;
    case "exit"_CC:             return (uint32_t)-203;
    case "_expand"_CC:          return (uint32_t)-204;
    case "free"_CC:             return (uint32_t)-205;
    case "getenv"_CC:           return (uint32_t)-206;
    case "labs"_CC:             return (uint32_t)-207;
    case "ldiv"_CC:             return (uint32_t)-208;
    case "llabs"_CC:            return (uint32_t)-209;
    case "lldiv"_CC:            return (uint32_t)-210;
    case "malloc"_CC:           return (uint32_t)-211;
    case "mblen"_CC:            return (uint32_t)-212;
    case "mbstowcs"_CC:         return (uint32_t)-213;
    case "mbtowc"_CC:           return (uint32_t)-214;
    case "_msize"_CC:           return (uint32_t)-215;
    case "qsort"_CC:            return (uint32_t)-216;
    case "quick_exit"_CC:       return (uint32_t)-217;
    case "rand"_CC:             return (uint32_t)-218;
    case "realloc"_CC:          return (uint32_t)-219;
    case "recalloc"_CC:         return (uint32_t)-220;
    case "srand"_CC:            return (uint32_t)-221;
    case "strtod"_CC:           return (uint32_t)-222;
    case "strtof"_CC:           return (uint32_t)-223;
    case "strtol"_CC:           return (uint32_t)-224;
    case "strtold"_CC:          return (uint32_t)-225;
    case "strtoll"_CC:          return (uint32_t)-226;
    case "strtoul"_CC:          return (uint32_t)-227;
    case "strtoull"_CC:         return (uint32_t)-228;
    case "system"_CC:           return (uint32_t)-229;
    case "wcstombs"_CC:         return (uint32_t)-230;
    case "wctomb"_CC:           return (uint32_t)-231;

    // string
    case "memchr"_CC:           return (uint32_t)-241;
    case "memcmp"_CC:           return (uint32_t)-242;
    case "memcpy"_CC:           return (uint32_t)-243;
    case "memmove"_CC:          return (uint32_t)-244;
    case "memset"_CC:           return (uint32_t)-245;
    case "strcat"_CC:           return (uint32_t)-246;
    case "strchr"_CC:           return (uint32_t)-247;
    case "strcmp"_CC:           return (uint32_t)-248;
    case "strcoll"_CC:          return (uint32_t)-249;
    case "strcpy"_CC:           return (uint32_t)-240;
    case "strcspn"_CC:          return (uint32_t)-251;
    case "strerror"_CC:         return (uint32_t)-252;
    case "strlen"_CC:           return (uint32_t)-253;
    case "strncat"_CC:          return (uint32_t)-254;
    case "strncmp"_CC:          return (uint32_t)-255;
    case "strncpy"_CC:          return (uint32_t)-256;
    case "strpbrk"_CC:          return (uint32_t)-257;
    case "strrchr"_CC:          return (uint32_t)-258;
    case "strspn"_CC:           return (uint32_t)-259;
    case "strstr"_CC:           return (uint32_t)-260;
    case "strtok"_CC:           return (uint32_t)-261;
    case "strxfrm"_CC:          return (uint32_t)-262;

    // time
    case "asctime"_CC:          return (uint32_t)-271;
    case "clock"_CC:            return (uint32_t)-272;
    case "ctime"_CC:            return (uint32_t)-273;
    case "difftime"_CC:         return (uint32_t)-274;
    case "gmtime"_CC:           return (uint32_t)-275;
    case "localtime"_CC:        return (uint32_t)-276;
    case "mktime"_CC:           return (uint32_t)-277;
    case "strftime"_CC:         return (uint32_t)-278;
    case "time"_CC:             return (uint32_t)-279;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
