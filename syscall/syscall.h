#pragma once

#ifdef __cplusplus
extern "C" {
#endif

size_t syscall_i386_execute(void* data, size_t index, int(*syslog)(const char*, va_list), int(*log)(const char*, va_list));
size_t syscall_i386_symbol(const char* file, const char* name);
const char* syscall_i386_name(size_t index);

// assert
int syscall_assert(const void* stack);

// ctype
int syscall_isalnum(const void* stack);
int syscall_isalpha(const void* stack);
int syscall_iscntrl(const void* stack);
int syscall_isdigit(const void* stack);
int syscall_isgraph(const void* stack);
int syscall_islower(const void* stack);
int syscall_isprint(const void* stack);
int syscall_ispunct(const void* stack);
int syscall_isspace(const void* stack);
int syscall_isupper(const void* stack);
int syscall_isxdigit(const void* stack);
int syscall_tolower(const void* stack);
int syscall_toupper(const void* stack);

// locale
int syscall_localeconv(const void* memory);
int syscall_setlocale(const void* memory, const void* stack, struct allocator_t* allocator);

// math
double syscall_acos(const void* stack);
double syscall_acosh(const void* stack);
double syscall_asin(const void* stack);
double syscall_asinh(const void* stack);
double syscall_atan(const void* stack);
double syscall_atan2(const void* stack);
double syscall_atanh(const void* stack);
double syscall_cbrt(const void* stack);
double syscall_ceil(const void* stack);
double syscall_copysign(const void* stack);
double syscall_cos(const void* stack);
double syscall_cosh(const void* stack);
double syscall_erf(const void* stack);
double syscall_erfc(const void* stack);
double syscall_exp(const void* stack);
double syscall_exp2(const void* stack);
double syscall_expm1(const void* stack);
double syscall_fabs(const void* stack);
double syscall_fdim(const void* stack);
double syscall_floor(const void* stack);
double syscall_fma(const void* stack);
double syscall_fmax(const void* stack);
double syscall_fmin(const void* stack);
double syscall_fmod(const void* stack);
int syscall_fpclassify(const void* stack);
double syscall_frexp(const void* memory, const void* stack);
double syscall_hypot(const void* stack);
int syscall_ilogb(const void* stack);
int syscall_isfinite(const void* stack);
int syscall_isgreater(const void* stack);
int syscall_isgreaterequal(const void* stack);
int syscall_isinf(const void* stack);
int syscall_isless(const void* stack);
int syscall_islessequal(const void* stack);
int syscall_islessgreater(const void* stack);
int syscall_isnan(const void* stack);
int syscall_isnormal(const void* stack);
int syscall_isunordered(const void* stack);
double syscall_ldexp(const void* stack);
double syscall_lgamma(const void* stack);
long long syscall_llrint(const void* stack);
long long syscall_llround(const void* stack);
double syscall_log(const void* stack);
double syscall_log10(const void* stack);
double syscall_log1p(const void* stack);
double syscall_log2(const void* stack);
double syscall_logb(const void* stack);
long syscall_lrint(const void* stack);
long syscall_lround(const void* stack);
double syscall_modf(const void* memory, const void* stack);
float syscall_nan(const void* memory, const void* stack);
double syscall_nanf(const void* memory, const void* stack);
long double syscall_nanl(const void* memory, const void* stack);
double syscall_nearbyint(const void* stack);
double syscall_nextafter(const void* stack);
double syscall_nexttoward(const void* stack);
double syscall_pow(const void* stack);
double syscall_remainder(const void* stack);
double syscall_remquo(const void* memory, const void* stack);
double syscall_rint(const void* stack);
double syscall_round(const void* stack);
double syscall_scalbln(const void* stack);
double syscall_scalbn(const void* stack);
int syscall_signbit(const void* stack);
double syscall_sin(const void* stack);
double syscall_sinh(const void* stack);
double syscall_sqrt(const void* stack);
double syscall_tan(const void* stack);
double syscall_tanh(const void* stack);
double syscall_tgamma(const void* stack);
double syscall_trunc(const void* stack);

// setjmp
int syscall_longjmp(const void* memory, const void* stack);
int syscall_setjmp(const void* memory, const void* stack);

// signal
int syscall_raise(const void* stack);

// stdio
int syscall_clearerr(const void* memory, const void* stack);
int syscall_fclose(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_feof(const void* memory, const void* stack);
int syscall_ferror(const void* memory, const void* stack);
int syscall_fflush(const void* memory, const void* stack);
int syscall_fgetc(const void* memory, const void* stack);
int syscall_fgetpos(const void* memory, const void* stack);
size_t syscall_fgets(const void* memory, const void* stack);
size_t syscall_fopen(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_fprintf(const void* memory, const void* stack, int(*function)(const char*, va_list));
int syscall_fputc(const void* memory, const void* stack);
int syscall_fputs(const void* memory, const void* stack);
size_t syscall_fread(const void* memory, const void* stack);
size_t syscall_freopen(const void* memory, const void* stack);
int syscall_fscanf(const void* memory, const void* stack);
int syscall_fseek(const void* memory, const void* stack);
int syscall_fsetpos(const void* memory, const void* stack);
long syscall_ftell(const void* memory, const void* stack);
size_t syscall_fwrite(const void* memory, const void* stack);
int syscall_getc(const void* memory, const void* stack);
int syscall_getchar();
size_t syscall_gets(const void* memory, const void* stack);
int syscall_perror(const void* memory, const void* stack);
int syscall_printf(const void* memory, const void* stack, int(*function)(const char*, va_list));
int syscall_putc(const void* memory, const void* stack);
int syscall_putchar(const void* stack);
int syscall_puts(const void* memory, const void* stack);
int syscall_remove(const void* memory, const void* stack);
int syscall_rename(const void* memory, const void* stack);
int syscall_rewind(const void* memory, const void* stack);
int syscall_scanf(const void* memory, const void* stack);
int syscall_setbuf(const void* memory, const void* stack);
int syscall_setvbuf(const void* memory, const void* stack);
int syscall_snprintf(const void* memory, const void* stack);
int syscall_sprintf(const void* memory, const void* stack);
int syscall_sscanf(const void* memory, const void* stack);
size_t syscall_tmpfile(const void* memory, struct allocator_t* allocator);
size_t syscall_tmpnam(const void* memory, const void* stack);
int syscall_ungetc(const void* memory, const void* stack);
int syscall_vfprintf(const void* memory, const void* stack);
int syscall_vfscanf(const void* memory, const void* stack);
int syscall_vprintf(const void* memory, const void* stack, int(*function)(const char*, va_list));
int syscall_vscanf(const void* memory, const void* stack);
int syscall_vsnprintf(const void* memory, const void* stack);
int syscall_vsprintf(const void* memory, const void* stack);

// stdlib
int syscall__Exit(const void* stack);
int syscall_abort();
int syscall_abs(const void* stack);
int syscall_at_quick_exit(const void* memory, const void* stack);
int syscall_atexit(const void* memory, const void* stack);
float syscall_atof(const void* memory, const void* stack);
int syscall_atoi(const void* memory, const void* stack);
long syscall_atol(const void* memory, const void* stack);
long long syscall_atoll(const void* memory, const void* stack);
int syscall_bsearch(const void* memory, const void* stack);
size_t syscall_calloc(const void* stack, struct allocator_t* allocator);
div_t syscall_div(const void* stack);
int syscall_exit(const void* stack);
int syscall_free(const void* stack, struct allocator_t* allocator);
size_t syscall_getenv(const void* memory, const void* stack);
long syscall_labs(const void* stack);
ldiv_t syscall_ldiv(const void* stack);
long long syscall_llabs(const void* stack);
lldiv_t syscall_lldiv(const void* stack);
int syscall_malloc(const void* stack, struct allocator_t* allocator);
int syscall_mblen(const void* memory, const void* stack);
size_t syscall_mbstowcs(const void* memory, const void* stack);
int syscall_mbtowc(const void* memory, const void* stack);
int syscall_qsort(const void* memory, const void* stack);
int syscall_quick_exit(const void* stack);
int syscall_rand();
int syscall_realloc(const void* stack, struct allocator_t* allocator);
int syscall_srand(const void* stack);
double syscall_strtod(const void* memory, const void* stack);
float syscall_strtof(const void* memory, const void* stack);
long syscall_strtol(const void* memory, const void* stack);
long double syscall_strtold(const void* memory, const void* stack);
long long syscall_strtoll(const void* memory, const void* stack);
long syscall_strtoul(const void* memory, const void* stack);
long long syscall_strtoull(const void* memory, const void* stack);
int syscall_system(const void* memory, const void* stack);
size_t syscall_wcstombs(const void* memory, const void* stack);
int syscall_wctomb(const void* memory, const void* stack);

// string
size_t syscall_memchr(const void* memory, const void* stack);
int syscall_memcmp(const void* memory, const void* stack);
size_t syscall_memcpy(const void* memory, const void* stack);
size_t syscall_memmove(const void* memory, const void* stack);
size_t syscall_memset(const void* memory, const void* stack);
size_t syscall_strcat(const void* memory, const void* stack);
size_t syscall_strchr(const void* memory, const void* stack);
int syscall_strcmp(const void* memory, const void* stack);
int syscall_strcoll(const void* memory, const void* stack);
size_t syscall_strcpy(const void* memory, const void* stack);
size_t syscall_strcspn(const void* memory, const void* stack);
size_t syscall_strerror(const void* memory, const void* stack, int(*syslog)(const char*, va_list));
size_t syscall_strlen(const void* memory, const void* stack);
size_t syscall_strncat(const void* memory, const void* stack);
int syscall_strncmp(const void* memory, const void* stack);
size_t syscall_strncpy(const void* memory, const void* stack);
size_t syscall_strpbrk(const void* memory, const void* stack);
size_t syscall_strrchr(const void* memory, const void* stack);
size_t syscall_strspn(const void* memory, const void* stack);
size_t syscall_strstr(const void* memory, const void* stack);
size_t syscall_strtok(const void* memory, const void* stack);
size_t syscall_strxfrm(const void* memory, const void* stack);

// time
int syscall_asctime(const void* memory, const void* stack);
int syscall_clock();
int syscall_ctime(const void* memory, const void* stack);
double syscall_difftime(const void* stack);
int syscall_gmtime(const void* memory, const void* stack);
int syscall_localtime(const void* memory, const void* stack);
int syscall_mktime(const void* memory, const void* stack);
int syscall_strftime(const void* memory, const void* stack);
int syscall_time(const void* memory, const void* stack);

// wchar
int syscall_btowc(const void* memory, const void* stack);
int syscall_fgetwc(const void* memory, const void* stack);
int syscall_fgetws(const void* memory, const void* stack);
int syscall_fputwc(const void* memory, const void* stack);
int syscall_fputws(const void* memory, const void* stack);
int syscall_fwide(const void* memory, const void* stack);
int syscall_fwprintf(const void* memory, const void* stack);
int syscall_fwscanf(const void* memory, const void* stack);
int syscall_getwc(const void* memory, const void* stack);
int syscall_getwchar(const void* memory, const void* stack);
size_t syscall_mbrlen(const void* memory, const void* stack);
size_t syscall_mbrtowc(const void* memory, const void* stack);
int syscall_mbsinit(const void* memory, const void* stack);
size_t syscall_mbsrtowcs(const void* memory, const void* stack);
int syscall_putwc(const void* memory, const void* stack);
int syscall_putwchar(const void* memory, const void* stack);
int syscall_swprintf(const void* memory, const void* stack);
int syscall_swscanf(const void* memory, const void* stack);
int syscall_ungetwc(const void* memory, const void* stack);
int syscall_vfwprintf(const void* memory, const void* stack);
int syscall_vfwscanf(const void* memory, const void* stack);
int syscall_vswprintf(const void* memory, const void* stack);
int syscall_vswscanf(const void* memory, const void* stack);
int syscall_vwprintf(const void* memory, const void* stack);
int syscall_vwscanf(const void* memory, const void* stack);
size_t syscall_wcrtomb(const void* memory, const void* stack);
int syscall_wcscat(const void* memory, const void* stack);
int syscall_wcschr(const void* memory, const void* stack);
int syscall_wcscmp(const void* memory, const void* stack);
int syscall_wcscoll(const void* memory, const void* stack);
int syscall_wcscpy(const void* memory, const void* stack);
size_t syscall_wcscspn(const void* memory, const void* stack);
size_t syscall_wcsftime(const void* memory, const void* stack);
size_t syscall_wcslen(const void* memory, const void* stack);
int syscall_wcsncat(const void* memory, const void* stack);
int syscall_wcsncmp(const void* memory, const void* stack);
int syscall_wcsncpy(const void* memory, const void* stack);
int syscall_wcspbrk(const void* memory, const void* stack);
int syscall_wcsrchr(const void* memory, const void* stack);
size_t syscall_wcsrtombs(const void* memory, const void* stack);
size_t syscall_wcsspn(const void* memory, const void* stack);
int syscall_wcsstr(const void* memory, const void* stack);
double syscall_wcstod(const void* memory, const void* stack);
float syscall_wcstof(const void* memory, const void* stack);
int syscall_wcstok(const void* memory, const void* stack);
long syscall_wcstol(const void* memory, const void* stack);
long double syscall_wcstold(const void* memory, const void* stack);
long long syscall_wcstoll(const void* memory, const void* stack);
long syscall_wcstoul(const void* memory, const void* stack);
long long syscall_wcstoull(const void* memory, const void* stack);
int syscall_wcsxfrm(const void* memory, const void* stack);
int syscall_wctob(const void* memory, const void* stack);
int syscall_wmemchr(const void* memory, const void* stack);
int syscall_wmemcmp(const void* memory, const void* stack);
int syscall_wmemcpy(const void* memory, const void* stack);
int syscall_wmemmove(const void* memory, const void* stack);
int syscall_wmemset(const void* memory, const void* stack);
int syscall_wprintf(const void* memory, const void* stack);
int syscall_wscanf(const void* memory, const void* stack);

// wctype
int syscall_iswalnum(const void* stack);
int syscall_iswalpha(const void* stack);
int syscall_iswblank(const void* stack);
int syscall_iswcntrl(const void* stack);
int syscall_iswctype(const void* stack);
int syscall_iswdigit(const void* stack);
int syscall_iswgraph(const void* stack);
int syscall_iswlower(const void* stack);
int syscall_iswprint(const void* stack);
int syscall_iswpunct(const void* stack);
int syscall_iswspace(const void* stack);
int syscall_iswupper(const void* stack);
int syscall_iswxdigit(const void* stack);
int syscall_towctrans(const void* stack);
int syscall_towlower(const void* stack);
int syscall_towupper(const void* stack);
int syscall_wctrans(const void* memory, const void* stack);
int syscall_wctype(const void* memory, const void* stack);

#ifdef __cplusplus
}
#endif
