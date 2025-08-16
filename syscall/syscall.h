#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
int syscall_setlocale(const void* memory, const void* stack);

// math
double syscall_abs(const void* stack);
double syscall_acos(const void* stack);
double syscall_asin(const void* stack);
double syscall_atan(const void* stack);
double syscall_atan2(const void* stack);
double syscall_ceil(const void* stack);
double syscall_cos(const void* stack);
double syscall_cosh(const void* stack);
double syscall_exp(const void* stack);
double syscall_fabs(const void* stack);
double syscall_floor(const void* stack);
double syscall_fmod(const void* stack);
double syscall_frexp(const void* memory, const void* stack);
double syscall_ldexp(const void* stack);
double syscall_log(const void* stack);
double syscall_log10(const void* stack);
double syscall_modf(const void* stack);
double syscall_pow(const void* stack);
double syscall_sin(const void* stack);
double syscall_sinh(const void* stack);
double syscall_sqrt(const void* stack);
double syscall_tan(const void* stack);
double syscall_tanh(const void* stack);

// setjmp
int syscall_longjmp(const void* stack);
int syscall_setjmp(const void* stack);

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
int syscall_fgets(const void* memory, const void* stack);
int syscall_fopen(const void* memory, const void* stack, struct allocator_t* allocator);
int syscall_fprintf(const void* memory, const void* stack);
int syscall_fputc(const void* memory, const void* stack);
int syscall_fputs(const void* memory, const void* stack);
int syscall_fread(const void* memory, const void* stack);
int syscall_freopen(const void* memory, const void* stack);
int syscall_fscanf(const void* memory, const void* stack);
int syscall_fseek(const void* memory, const void* stack);
int syscall_fsetpos(const void* memory, const void* stack);
int syscall_ftell(const void* memory, const void* stack);
int syscall_fwrite(const void* memory, const void* stack);
int syscall_getc(const void* memory, const void* stack);
int syscall_getchar();
int syscall_gets(const void* memory, const void* stack);
int syscall_perror(const void* memory, const void* stack);
int syscall_printf(const void* memory, const void* stack, int(*function)(const char*, va_list) = vprintf);
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
int syscall_tmpfile(const void* memory, struct allocator_t* allocator);
int syscall_tmpnam(const void* memory, const void* stack);
int syscall_ungetc(const void* memory, const void* stack);
int syscall_vfprintf(const void* memory, const void* stack);
int syscall_vfscanf(const void* memory, const void* stack);
int syscall_vprintf(const void* memory, const void* stack, int(*function)(const char*, va_list) = vprintf);
int syscall_vscanf(const void* memory, const void* stack);
int syscall_vsnprintf(const void* memory, const void* stack);
int syscall_vsprintf(const void* memory, const void* stack);

// calloc / malloc / realloc / free
uint32_t syscall_calloc(const void* stack, struct allocator_t* allocator);
uint32_t syscall_malloc(const void* stack, struct allocator_t* allocator);
uint32_t syscall_realloc(const void* stack, struct allocator_t* allocator);
uint32_t syscall_free(const void* stack, struct allocator_t* allocator);
uint32_t syscall_expand(const void* stack, struct allocator_t* allocator);
uint32_t syscall_msize(const void* stack, struct allocator_t* allocator);
uint32_t syscall_recalloc(const void* stack, struct allocator_t* allocator);

// string
int syscall_memchr(const void* memory, const void* stack);
int syscall_memcmp(const void* memory, const void* stack);
int syscall_memcpy(const void* memory, const void* stack);
int syscall_memmove(const void* memory, const void* stack);
int syscall_memset(const void* memory, const void* stack);
int syscall_strcat(const void* memory, const void* stack);
int syscall_strchr(const void* memory, const void* stack);
int syscall_strcmp(const void* memory, const void* stack);
int syscall_strcoll(const void* memory, const void* stack);
int syscall_strcpy(const void* memory, const void* stack);
int syscall_strcspn(const void* memory, const void* stack);
int syscall_strerror(const void* memory, const void* stack);
int syscall_strlen(const void* memory, const void* stack);
int syscall_strncat(const void* memory, const void* stack);
int syscall_strncmp(const void* memory, const void* stack);
int syscall_strncpy(const void* memory, const void* stack);
int syscall_strpbrk(const void* memory, const void* stack);
int syscall_strrchr(const void* memory, const void* stack);
int syscall_strspn(const void* memory, const void* stack);
int syscall_strstr(const void* memory, const void* stack);
int syscall_strtok(const void* memory, const void* stack);
int syscall_strxfrm(const void* memory, const void* stack);

#ifdef __cplusplus
}
#endif
