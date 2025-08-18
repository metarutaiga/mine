//==============================================================================
// miCPU : Debugger Source
//
// Copyright (c) 2025 TAiGA
// https://github.com/metarutaiga/miCPU
//==============================================================================
#include "EmulatorPCH.h"
#include <IconFontCppHeaders/IconsFontAwesome4.h>
#include "format/coff/pe.h"
#include "syscall/simple_allocator.h"
#include "syscall/syscall.h"
#include "x86/x86_i386.h"
#include "Debugger.h"

//------------------------------------------------------------------------------
static miCPU* cpu;
static std::string status;
static std::string disasm;
static std::string logs;
static int stackIndex;
static int symbolIndex;
static std::vector<std::pair<std::string, size_t>> symbols;
static bool refresh;
//------------------------------------------------------------------------------
static int LoggerV(const char* format, va_list va)
{
    int length = vsnprintf(nullptr, 0, format, va) + 1;
    size_t offset = logs.size();
    logs.resize(offset + length);
    vsnprintf(logs.data() + offset, length, format, va);
    logs.pop_back();
    if (logs.empty() == false && logs.back() != '\n')
        logs += '\n';
    return length;
}
//------------------------------------------------------------------------------
static int Logger(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int length = LoggerV(format, va);
    va_end(va);
    return length;
}
//------------------------------------------------------------------------------
static size_t Exception(miCPU* cpu, size_t index)
{
    [[maybe_unused]] auto x86 = ((x86_i386*)cpu)->x86;
    [[maybe_unused]] auto x87 = ((x86_i386*)cpu)->x87;
    [[maybe_unused]] auto allocator = cpu->Allocator();
    [[maybe_unused]] auto memory = cpu->Memory();
    [[maybe_unused]] auto stack = memory + cpu->Stack();

    auto FLD = [&](double value) {
        x87.sts[x87.status._TOP -= 1].d = value;
        return 0;
    };

    switch (index) {

    // assert
    case (uint32_t)-1:      return syscall_assert(stack);

    // ctype
    case (uint32_t)-11:     return syscall_isalnum(stack);
    case (uint32_t)-12:     return syscall_isalpha(stack);
    case (uint32_t)-13:     return syscall_iscntrl(stack);
    case (uint32_t)-14:     return syscall_isdigit(stack);
    case (uint32_t)-15:     return syscall_isgraph(stack);
    case (uint32_t)-16:     return syscall_islower(stack);
    case (uint32_t)-17:     return syscall_isprint(stack);
    case (uint32_t)-18:     return syscall_ispunct(stack);
    case (uint32_t)-19:     return syscall_isspace(stack);
    case (uint32_t)-20:     return syscall_isupper(stack);
    case (uint32_t)-21:     return syscall_isxdigit(stack);
    case (uint32_t)-22:     return syscall_tolower(stack);
    case (uint32_t)-23:     return syscall_toupper(stack);

    // locale
    case (uint32_t)-31:     return syscall_localeconv(memory);
    case (uint32_t)-32:     return syscall_setlocale(memory, stack);

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
    case (uint32_t)-65:     return syscall_fpclassify(stack);
    case (uint32_t)-66:     return FLD(syscall_frexp(memory, stack));
    case (uint32_t)-67:     return FLD(syscall_hypot(stack));
    case (uint32_t)-68:     return syscall_ilogb(stack);
    case (uint32_t)-69:     return syscall_isfinite(stack);
    case (uint32_t)-70:     return syscall_isgreater(stack);
    case (uint32_t)-71:     return syscall_isgreaterequal(stack);
    case (uint32_t)-72:     return syscall_isinf(stack);
    case (uint32_t)-73:     return syscall_isless(stack);
    case (uint32_t)-74:     return syscall_islessequal(stack);
    case (uint32_t)-75:     return syscall_islessgreater(stack);
    case (uint32_t)-76:     return syscall_isnan(stack);
    case (uint32_t)-77:     return syscall_isnormal(stack);
    case (uint32_t)-78:     return syscall_isunordered(stack);
    case (uint32_t)-79:     return FLD(syscall_ldexp(stack));
    case (uint32_t)-80:     return FLD(syscall_lgamma(stack));
    case (uint32_t)-81:     return syscall_llrint(stack);
    case (uint32_t)-82:     return syscall_llround(stack);
    case (uint32_t)-83:     return FLD(syscall_log(stack));
    case (uint32_t)-84:     return FLD(syscall_log10(stack));
    case (uint32_t)-85:     return FLD(syscall_log1p(stack));
    case (uint32_t)-86:     return FLD(syscall_log2(stack));
    case (uint32_t)-87:     return FLD(syscall_logb(stack));
    case (uint32_t)-88:     return syscall_lrint(stack);
    case (uint32_t)-89:     return syscall_lround(stack);
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
    case (uint32_t)-104:    return syscall_signbit(stack);
    case (uint32_t)-105:    return FLD(syscall_sin(stack));
    case (uint32_t)-106:    return FLD(syscall_sinh(stack));
    case (uint32_t)-107:    return FLD(syscall_sqrt(stack));
    case (uint32_t)-108:    return FLD(syscall_tan(stack));
    case (uint32_t)-109:    return FLD(syscall_tanh(stack));
    case (uint32_t)-110:    return FLD(syscall_tgamma(stack));
    case (uint32_t)-111:    return FLD(syscall_trunc(stack));

    // setjmp
    case (uint32_t)-121:    return syscall_longjmp(memory, stack);
    case (uint32_t)-122:    return syscall_setjmp(memory, stack);

    // signal
    case (uint32_t)-131:    return syscall_raise(stack);

    // stdio
    case (uint32_t)-141:    return syscall_clearerr(memory, stack);
    case (uint32_t)-142:    return syscall_fclose(memory, stack, allocator);
    case (uint32_t)-143:    return syscall_feof(memory, stack);
    case (uint32_t)-144:    return syscall_ferror(memory, stack);
    case (uint32_t)-145:    return syscall_fflush(memory, stack);
    case (uint32_t)-146:    return syscall_fgetc(memory, stack);
    case (uint32_t)-147:    return syscall_fgetpos(memory, stack);
    case (uint32_t)-148:    return syscall_fgets(memory, stack);
    case (uint32_t)-149:    return syscall_fopen(memory, stack, allocator);
    case (uint32_t)-150:    return syscall_fprintf(memory, stack);
    case (uint32_t)-151:    return syscall_fputc(memory, stack);
    case (uint32_t)-152:    return syscall_fputs(memory, stack);
    case (uint32_t)-153:    return syscall_fread(memory, stack);
    case (uint32_t)-154:    return syscall_freopen(memory, stack);
    case (uint32_t)-155:    return syscall_fscanf(memory, stack);
    case (uint32_t)-156:    return syscall_fseek(memory, stack);
    case (uint32_t)-157:    return syscall_fsetpos(memory, stack);
    case (uint32_t)-158:    return syscall_ftell(memory, stack);
    case (uint32_t)-159:    return syscall_fwrite(memory, stack);
    case (uint32_t)-160:    return syscall_getc(memory, stack);
    case (uint32_t)-161:    return syscall_getchar();
    case (uint32_t)-162:    return syscall_gets(memory, stack);
    case (uint32_t)-163:    return syscall_perror(memory, stack);
    case (uint32_t)-164:    return syscall_printf(memory, stack, LoggerV);
    case (uint32_t)-165:    return syscall_putc(memory, stack);
    case (uint32_t)-166:    return syscall_putchar(stack);
    case (uint32_t)-167:    return syscall_puts(memory, stack);
    case (uint32_t)-168:    return syscall_remove(memory, stack);
    case (uint32_t)-169:    return syscall_rename(memory, stack);
    case (uint32_t)-170:    return syscall_rewind(memory, stack);
    case (uint32_t)-171:    return syscall_scanf(memory, stack);
    case (uint32_t)-172:    return syscall_setbuf(memory, stack);
    case (uint32_t)-173:    return syscall_setvbuf(memory, stack);
    case (uint32_t)-174:    return syscall_snprintf(memory, stack);
    case (uint32_t)-175:    return syscall_sprintf(memory, stack);
    case (uint32_t)-176:    return syscall_sscanf(memory, stack);
    case (uint32_t)-177:    return syscall_tmpfile(memory, allocator);
    case (uint32_t)-178:    return syscall_tmpnam(memory, stack);
    case (uint32_t)-179:    return syscall_ungetc(memory, stack);
    case (uint32_t)-180:    return syscall_vfprintf(memory, stack);
    case (uint32_t)-181:    return syscall_vfscanf(memory, stack);
    case (uint32_t)-182:    return syscall_vprintf(memory, stack, LoggerV);
    case (uint32_t)-183:    return syscall_vscanf(memory, stack);
    case (uint32_t)-184:    return syscall_vsnprintf(memory, stack);
    case (uint32_t)-185:    return syscall_vsprintf(memory, stack);

    // stdlib
    case (uint32_t)-191:    return syscall__Exit(stack);
    case (uint32_t)-192:    return syscall_abort();
    case (uint32_t)-193:    return syscall_abs(stack);
    case (uint32_t)-194:    return syscall_at_quick_exit(memory, stack);
    case (uint32_t)-195:    return syscall_atexit(memory, stack);
    case (uint32_t)-196:    return FLD(syscall_atof(memory, stack));
    case (uint32_t)-197:    return syscall_atoi(memory, stack);
    case (uint32_t)-198:    return syscall_atol(memory, stack);
    case (uint32_t)-199:    return syscall_atoll(memory, stack);
    case (uint32_t)-200:    return syscall_bsearch(memory, stack);
    case (uint32_t)-201:    return syscall_calloc(stack, allocator);
    case (uint32_t)-202:    return syscall_div(stack).quot;
    case (uint32_t)-203:    return syscall_exit(stack);
    case (uint32_t)-204:    return syscall_expand(stack, allocator);
    case (uint32_t)-205:    return syscall_free(stack, allocator);
    case (uint32_t)-206:    return syscall_getenv(memory, stack);
    case (uint32_t)-207:    return syscall_labs(stack);
    case (uint32_t)-208:    return syscall_ldiv(stack).quot;
    case (uint32_t)-209:    return syscall_llabs(stack);
    case (uint32_t)-210:    return syscall_lldiv(stack).quot;
    case (uint32_t)-211:    return syscall_malloc(stack, allocator);
    case (uint32_t)-212:    return syscall_mblen(memory, stack);
    case (uint32_t)-213:    return syscall_mbstowcs(memory, stack);
    case (uint32_t)-214:    return syscall_mbtowc(memory, stack);
    case (uint32_t)-215:    return syscall_msize(stack, allocator);
    case (uint32_t)-216:    return syscall_qsort(memory, stack);
    case (uint32_t)-217:    return syscall_quick_exit(stack);
    case (uint32_t)-218:    return syscall_rand();
    case (uint32_t)-219:    return syscall_realloc(stack, allocator);
    case (uint32_t)-220:    return syscall_recalloc(stack, allocator);
    case (uint32_t)-221:    return syscall_srand(stack);
    case (uint32_t)-222:    return FLD(syscall_strtod(memory, stack));
    case (uint32_t)-223:    return FLD(syscall_strtof(memory, stack));
    case (uint32_t)-224:    return syscall_strtol(memory, stack);
    case (uint32_t)-225:    return FLD(syscall_strtold(memory, stack));
    case (uint32_t)-226:    return syscall_strtoll(memory, stack);
    case (uint32_t)-227:    return syscall_strtoul(memory, stack);
    case (uint32_t)-228:    return syscall_strtoull(memory, stack);
    case (uint32_t)-229:    return syscall_system(memory, stack);
    case (uint32_t)-230:    return syscall_wcstombs(memory, stack);
    case (uint32_t)-231:    return syscall_wctomb(memory, stack);

    // string
    case (uint32_t)-241:    return syscall_memchr(memory, stack);
    case (uint32_t)-242:    return syscall_memcmp(memory, stack);
    case (uint32_t)-243:    return syscall_memcpy(memory, stack);
    case (uint32_t)-244:    return syscall_memmove(memory, stack);
    case (uint32_t)-245:    return syscall_memset(memory, stack);
    case (uint32_t)-246:    return syscall_strcat(memory, stack);
    case (uint32_t)-247:    return syscall_strchr(memory, stack);
    case (uint32_t)-248:    return syscall_strcmp(memory, stack);
    case (uint32_t)-249:    return syscall_strcoll(memory, stack);
    case (uint32_t)-240:    return syscall_strcpy(memory, stack);
    case (uint32_t)-251:    return syscall_strcspn(memory, stack);
    case (uint32_t)-252:    return syscall_strerror(memory, stack);
    case (uint32_t)-253:    return syscall_strlen(memory, stack);
    case (uint32_t)-254:    return syscall_strncat(memory, stack);
    case (uint32_t)-255:    return syscall_strncmp(memory, stack);
    case (uint32_t)-256:    return syscall_strncpy(memory, stack);
    case (uint32_t)-257:    return syscall_strpbrk(memory, stack);
    case (uint32_t)-258:    return syscall_strrchr(memory, stack);
    case (uint32_t)-259:    return syscall_strspn(memory, stack);
    case (uint32_t)-260:    return syscall_strstr(memory, stack);
    case (uint32_t)-261:    return syscall_strtok(memory, stack);
    case (uint32_t)-262:    return syscall_strxfrm(memory, stack);

    // time
    case (uint32_t)-271:    return syscall_asctime(memory, stack);
    case (uint32_t)-272:    return syscall_clock();
    case (uint32_t)-273:    return syscall_ctime(memory, stack);
    case (uint32_t)-274:    return FLD(syscall_difftime(stack));
    case (uint32_t)-275:    return syscall_gmtime(memory, stack);
    case (uint32_t)-276:    return syscall_localtime(memory, stack);
    case (uint32_t)-277:    return syscall_mktime(memory, stack);
    case (uint32_t)-278:    return syscall_strftime(memory, stack);
    case (uint32_t)-279:    return syscall_time(memory, stack);
    }
    return 0;
}
//------------------------------------------------------------------------------
static size_t Symbol(const char* file, const char* name, size_t address, void* sym_data)
{
    if (file) {
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
        Logger("Symbol : %s.%s is not found", file, name);
    }
    else {
        auto& symbols = *(std::vector<std::pair<std::string, size_t>>*)sym_data;
        symbols.emplace_back(name, address);
    }
    return address;
}
//------------------------------------------------------------------------------
void Debugger::Initialize()
{
}
//------------------------------------------------------------------------------
void Debugger::Shutdown()
{
    delete cpu;
    disasm = std::string();
    status = std::string();
    symbols = std::vector<std::pair<std::string, size_t>>();
    logs = std::string();
}
//------------------------------------------------------------------------------
bool Debugger::Update(const UpdateData& updateData, bool& show)
{
    if (show == false)
        return false;

    bool updated = false;
    ImGui::SetNextWindowSize(ImVec2(1366.0f, 768.0f), ImGuiCond_Appearing);
    if (ImGui::Begin("Debugger", &show, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        std::string file;

        if (ImGui::BeginTable("Debugger", 3)) {
            float left = windowSize.x / 2.0f;
            float right = windowSize.x / 4.0f;
            float height = windowSize.y - 256.0f;
            float halfHeight = height / 2.0f;

            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, left);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, right);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, right);

            // 1st Column
            ImGui::TableNextColumn();
            ImGui::InputTextMultiline("##10", disasm, ImVec2(left, height), ImGuiInputTextFlags_ReadOnly);

            // 2nd Column
            ImGui::TableNextColumn();
            ImGui::InputTextMultiline("##20", status, ImVec2(right, halfHeight), ImGuiInputTextFlags_ReadOnly);

            int stackCount = halfHeight / ImGui::GetTextLineHeightWithSpacing();
            ImGui::SetNextWindowSize(ImVec2(right, halfHeight));
            ImGui::ListBox("##21", &stackIndex, [](void* user_data, int index) -> const char* {
                if (cpu == nullptr)
                    return "";

                auto stackCount = *(int*)user_data / 2;
                size_t stack = cpu->Stack() + (index - stackCount) * sizeof(uint32_t);
                auto* value = (uint32_t*)cpu->Memory(stack);
                if (value == nullptr)
                    return "";

                static char temp[32];
                snprintf(temp, 32, "%s%08zX %08X", stackCount == index ? ">" : " ", stack, *value);
                return temp;
            }, &stackCount, stackCount);

            // 3rd Column
            ImGui::TableNextColumn();
            ImGui::SetNextWindowSize(ImVec2(right, halfHeight));
            if (ImGui::ListBox("##30", &symbolIndex, [](void* user_data, int index) {
                auto& symbols = *(std::vector<std::pair<std::string, size_t>>*)user_data;
                return symbols[index].first.c_str();
            }, &symbols, (int)symbols.size())) {
                cpu->Jump(symbols[symbolIndex].second);
                refresh = true;
            }

            if (ImGui::Button(ICON_FA_PLAY))        { refresh = true; if (cpu) cpu->Run();    } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_RIGHT)) { refresh = true; if (cpu) cpu->Step(0);  } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_DOWN))  { refresh = true; if (cpu) cpu->Step(1);  } ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ARROW_UP))    { refresh = true; if (cpu) cpu->Step(-1); }
            ImGui::Separator();
            static const char* const samples[] = {
                "malloc.exe",
                "printf.exe",
                "x86.dll",
                "x86_arithmetic.exe",
            };
            for (auto sample : samples) {
                float pos = ImGui::CalcTextSize(sample).x + ImGui::GetStyle().FramePadding.x * 2.0f;
                float avail = ImGui::GetContentRegionAvail().x;
                if (pos > avail)
                    ImGui::NewLine();
                if (ImGui::Button(sample)) {
                    file = xxGetExecutablePath();
                    file += "/../../../../../SDK/miCPU/format/sample/pe.x86/";
                    file += sample;
                }
                ImGui::SameLine();
            }

            ImGui::EndTable();
        }
        ImGui::InputTextMultiline("##100", logs, ImVec2(windowSize.x, 256.0f - 16.0f), ImGuiInputTextFlags_ReadOnly);

        if (file.empty() == false) {
            delete cpu;
            status.clear();
            disasm.clear();
            logs.clear();
            stackIndex = 0;
            symbolIndex = 0;
            symbols.clear();
            refresh = true;

            cpu = new x86_i386;
            cpu->Initialize(simple_allocator<16>::construct(16777216));
            cpu->Exception(Exception);

            PE pe;
            pe.Load(file.c_str(), [](size_t base, size_t size, void* userdata) {
                miCPU* cpu = (miCPU*)userdata;
                return cpu->Memory(base, size);
            }, cpu, Symbol, &symbols, Logger);

            if (symbols.empty() == false) {
                cpu->Jump(symbols.front().second);
            }
        }

        if (refresh) {
            refresh = false;
            ImGui::BeginChild("##100");
            if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
            if (cpu) {
                status = cpu->Status();
                disasm = cpu->Disassemble(64);
            }
        }
    }
    ImGui::End();

    return updated;
}
//------------------------------------------------------------------------------
