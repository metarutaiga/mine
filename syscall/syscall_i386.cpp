#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#include "x86/x86_i386.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYMBOL_INDEX 1

#define CALLBACK_ARGUMENT \
    x86_instruction& x86,   \
    x87_instruction& x87,   \
    void* memory,           \
    void* stack,            \
    allocator_t* allocator, \
    int(*log)(const char*, va_list)

#define EAX(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
    }
#define EDXEAX(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x86.regs[0].d = uint32_t(temp); \
        x86.regs[2].d = uint32_t(temp >> 32); \
    }
#define FLD(value) \
    [](CALLBACK_ARGUMENT) { \
        auto temp = value; \
        x87.sts[x87.status._TOP -= 1].d = temp; \
    }

static const struct {
    const char* name;
    void (*syscall)(CALLBACK_ARGUMENT);
} syscall_tables[] = {

    // assert
    { "assert",         EAX(syscall_assert(stack))                      },

    // ctype
    { "isalnum",        EAX(syscall_isalnum(stack))                     },
    { "isalpha",        EAX(syscall_isalpha(stack))                     },
    { "iscntrl",        EAX(syscall_iscntrl(stack))                     },
    { "isdigit",        EAX(syscall_isdigit(stack))                     },
    { "isgraph",        EAX(syscall_isgraph(stack))                     },
    { "islower",        EAX(syscall_islower(stack))                     },
    { "isprint",        EAX(syscall_isprint(stack))                     },
    { "ispunct",        EAX(syscall_ispunct(stack))                     },
    { "isspace",        EAX(syscall_isspace(stack))                     },
    { "isupper",        EAX(syscall_isupper(stack))                     },
    { "isxdigit",       EAX(syscall_isxdigit(stack))                    },
    { "tolower",        EAX(syscall_tolower(stack))                     },
    { "toupper",        EAX(syscall_toupper(stack))                     },

    // locale
    { "localeconv",     EAX(syscall_localeconv(memory))                 },
    { "setlocale",      EAX(syscall_setlocale(memory, stack))           },

    // math
    { "acos",           FLD(syscall_acos(stack))                        },
    { "acosh",          FLD(syscall_acosh(stack))                       },
    { "asin",           FLD(syscall_asin(stack))                        },
    { "asinh",          FLD(syscall_asinh(stack))                       },
    { "atan",           FLD(syscall_atan(stack))                        },
    { "atan2",          FLD(syscall_atan2(stack))                       },
    { "atanh",          FLD(syscall_atanh(stack))                       },
    { "cbrt",           FLD(syscall_cbrt(stack))                        },
    { "ceil",           FLD(syscall_ceil(stack))                        },
    { "copysign",       FLD(syscall_copysign(stack))                    },
    { "cos",            FLD(syscall_cos(stack))                         },
    { "cosh",           FLD(syscall_cosh(stack))                        },
    { "erf",            FLD(syscall_erf(stack))                         },
    { "erfc",           FLD(syscall_erfc(stack))                        },
    { "exp",            FLD(syscall_exp(stack))                         },
    { "exp2",           FLD(syscall_exp2(stack))                        },
    { "expm1",          FLD(syscall_expm1(stack))                       },
    { "fabs",           FLD(syscall_fabs(stack))                        },
    { "fdim",           FLD(syscall_fdim(stack))                        },
    { "floor",          FLD(syscall_floor(stack))                       },
    { "fma",            FLD(syscall_fma(stack))                         },
    { "fmax",           FLD(syscall_fmax(stack))                        },
    { "fmin",           FLD(syscall_fmin(stack))                        },
    { "fmod",           FLD(syscall_fmod(stack))                        },
    { "fpclassify",     EAX(syscall_fpclassify(stack))                  },
    { "frexp",          FLD(syscall_frexp(memory, stack))               },
    { "hypot",          FLD(syscall_hypot(stack))                       },
    { "ilogb",          EAX(syscall_ilogb(stack))                       },
    { "isfinite",       EAX(syscall_isfinite(stack))                    },
    { "isgreater",      EAX(syscall_isgreater(stack))                   },
    { "isgreaterequal", EAX(syscall_isgreaterequal(stack))              },
    { "isinf",          EAX(syscall_isinf(stack))                       },
    { "isless",         EAX(syscall_isless(stack))                      },
    { "islessequal",    EAX(syscall_islessequal(stack))                 },
    { "islessgreater",  EAX(syscall_islessgreater(stack))               },
    { "isnan",          EAX(syscall_isnan(stack))                       },
    { "isnormal",       EAX(syscall_isnormal(stack))                    },
    { "isunordered",    EAX(syscall_isunordered(stack))                 },
    { "ldexp",          FLD(syscall_ldexp(stack))                       },
    { "lgamma",         FLD(syscall_lgamma(stack))                      },
    { "llrint",         EDXEAX(syscall_llrint(stack))                   },
    { "llround",        EDXEAX(syscall_llround(stack))                  },
    { "log",            FLD(syscall_log(stack))                         },
    { "log10",          FLD(syscall_log10(stack))                       },
    { "log1p",          FLD(syscall_log1p(stack))                       },
    { "log2",           FLD(syscall_log2(stack))                        },
    { "logb",           FLD(syscall_logb(stack))                        },
    { "lrint",          EAX(syscall_lrint(stack))                       },
    { "lround",         EAX(syscall_lround(stack))                      },
    { "modf",           FLD(syscall_modf(memory, stack))                },
    { "nan",            FLD(syscall_nan(memory, stack))                 },
    { "nanf",           FLD(syscall_nanf(memory, stack))                },
    { "nanl",           FLD(syscall_nanl(memory, stack))                },
    { "nearbyint",      FLD(syscall_nearbyint(stack))                   },
    { "nextafter",      FLD(syscall_nextafter(stack))                   },
    { "nexttoward",     FLD(syscall_nexttoward(stack))                  },
    { "pow",            FLD(syscall_pow(stack))                         },
    { "remainder",      FLD(syscall_remainder(stack))                   },
    { "remquo",         FLD(syscall_remquo(memory, stack))              },
    { "rint",           FLD(syscall_rint(stack))                        },
    { "round",          FLD(syscall_round(stack))                       },
    { "scalbln",        FLD(syscall_scalbln(stack))                     },
    { "scalbn",         FLD(syscall_scalbn(stack))                      },
    { "signbit",        EAX(syscall_signbit(stack))                     },
    { "sin",            FLD(syscall_sin(stack))                         },
    { "sinh",           FLD(syscall_sinh(stack))                        },
    { "sqrt",           FLD(syscall_sqrt(stack))                        },
    { "tan",            FLD(syscall_tan(stack))                         },
    { "tanh",           FLD(syscall_tanh(stack))                        },
    { "tgamma",         FLD(syscall_tgamma(stack))                      },
    { "trunc",          FLD(syscall_trunc(stack))                       },

    // setjmp
    { "longjmp",        EAX(syscall_longjmp(memory, stack))             },
    { "setjmp",         EAX(syscall_setjmp(memory, stack))              },

    // signal
    { "raise",          EAX(syscall_raise(stack))                       },

    // stdio
    { "clearerr",       EAX(syscall_clearerr(memory, stack))            },
    { "fclose",         EAX(syscall_fclose(memory, stack, allocator))   },
    { "feof",           EAX(syscall_feof(memory, stack))                },
    { "ferror",         EAX(syscall_ferror(memory, stack))              },
    { "fflush",         EAX(syscall_fflush(memory, stack))              },
    { "fgetc",          EAX(syscall_fgetc(memory, stack))               },
    { "fgetpos",        EAX(syscall_fgetpos(memory, stack))             },
    { "fgets",          EAX(syscall_fgets(memory, stack))               },
    { "fopen",          EAX(syscall_fopen(memory, stack, allocator))    },
    { "fprintf",        EAX(syscall_fprintf(memory, stack))             },
    { "fputc",          EAX(syscall_fputc(memory, stack))               },
    { "fputs",          EAX(syscall_fputs(memory, stack))               },
    { "fread",          EAX(syscall_fread(memory, stack))               },
    { "freopen",        EAX(syscall_freopen(memory, stack))             },
    { "fscanf",         EAX(syscall_fscanf(memory, stack))              },
    { "fseek",          EAX(syscall_fseek(memory, stack))               },
    { "fsetpos",        EAX(syscall_fsetpos(memory, stack))             },
    { "ftell",          EAX(syscall_ftell(memory, stack))               },
    { "fwrite",         EAX(syscall_fwrite(memory, stack))              },
    { "getc",           EAX(syscall_getc(memory, stack))                },
    { "getchar",        EAX(syscall_getchar())                          },
    { "gets",           EAX(syscall_gets(memory, stack))                },
    { "perror",         EAX(syscall_perror(memory, stack))              },
    { "printf",         EAX(syscall_printf(memory, stack, log))         },
    { "putc",           EAX(syscall_putc(memory, stack))                },
    { "putchar",        EAX(syscall_putchar(stack))                     },
    { "puts",           EAX(syscall_puts(memory, stack))                },
    { "remove",         EAX(syscall_remove(memory, stack))              },
    { "rename",         EAX(syscall_rename(memory, stack))              },
    { "rewind",         EAX(syscall_rewind(memory, stack))              },
    { "scanf",          EAX(syscall_scanf(memory, stack))               },
    { "setbuf",         EAX(syscall_setbuf(memory, stack))              },
    { "setvbuf",        EAX(syscall_setvbuf(memory, stack))             },
    { "snprintf",       EAX(syscall_snprintf(memory, stack))            },
    { "sprintf",        EAX(syscall_sprintf(memory, stack))             },
    { "sscanf",         EAX(syscall_sscanf(memory, stack))              },
    { "tmpfile",        EAX(syscall_tmpfile(memory, allocator))         },
    { "tmpnam",         EAX(syscall_tmpnam(memory, stack))              },
    { "ungetc",         EAX(syscall_ungetc(memory, stack))              },
    { "vfprintf",       EAX(syscall_vfprintf(memory, stack))            },
    { "vfscanf",        EAX(syscall_vfscanf(memory, stack))             },
    { "vprintf",        EAX(syscall_vprintf(memory, stack, log))        },
    { "vscanf",         EAX(syscall_vscanf(memory, stack))              },
    { "vsnprintf",      EAX(syscall_vsnprintf(memory, stack))           },
    { "vsprintf",       EAX(syscall_vsprintf(memory, stack))            },

    // stdlib
    { "_Exit",          EAX(syscall__Exit(stack))                       },
    { "abort",          EAX(syscall_abort())                            },
    { "abs",            EAX(syscall_abs(stack))                         },
    { "at_quick_exit",  EAX(syscall_at_quick_exit(memory, stack))       },
    { "atexit",         EAX(syscall_atexit(memory, stack))              },
    { "atof",           FLD(syscall_atof(memory, stack))                },
    { "atoi",           EAX(syscall_atoi(memory, stack))                },
    { "atol",           EAX(syscall_atol(memory, stack))                },
    { "atoll",          EDXEAX(syscall_atoll(memory, stack))            },
    { "bsearch",        EAX(syscall_bsearch(memory, stack))             },
    { "calloc",         EAX(syscall_calloc(stack, allocator))           },
    { "div",            EAX(syscall_div(stack).quot)                    },
    { "exit",           EAX(syscall_exit(stack))                        },
    { "free",           EAX(syscall_free(stack, allocator))             },
    { "getenv",         EAX(syscall_getenv(memory, stack))              },
    { "labs",           EAX(syscall_labs(stack))                        },
    { "ldiv",           EAX(syscall_ldiv(stack).quot)                   },
    { "llabs",          EDXEAX(syscall_llabs(stack))                    },
    { "lldiv",          EDXEAX(syscall_lldiv(stack).quot)               },
    { "malloc",         EAX(syscall_malloc(stack, allocator))           },
    { "mblen",          EAX(syscall_mblen(memory, stack))               },
    { "mbstowcs",       EAX(syscall_mbstowcs(memory, stack))            },
    { "mbtowc",         EAX(syscall_mbtowc(memory, stack))              },
    { "qsort",          EAX(syscall_qsort(memory, stack))               },
    { "quick_exit",     EAX(syscall_quick_exit(stack))                  },
    { "rand",           EAX(syscall_rand())                             },
    { "realloc",        EAX(syscall_realloc(stack, allocator))          },
    { "srand",          EAX(syscall_srand(stack))                       },
    { "strtod",         FLD(syscall_strtod(memory, stack))              },
    { "strtof",         FLD(syscall_strtof(memory, stack))              },
    { "strtol",         EAX(syscall_strtol(memory, stack))              },
    { "strtold",        FLD(syscall_strtold(memory, stack))             },
    { "strtoll",        EDXEAX(syscall_strtoll(memory, stack))          },
    { "strtoul",        EAX(syscall_strtoul(memory, stack))             },
    { "strtoull",       EDXEAX(syscall_strtoull(memory, stack))         },
    { "system",         EAX(syscall_system(memory, stack))              },
    { "wcstombs",       EAX(syscall_wcstombs(memory, stack))            },
    { "wctomb",         EAX(syscall_wctomb(memory, stack))              },

    // string
    { "memchr",         EAX(syscall_memchr(memory, stack))              },
    { "memcmp",         EAX(syscall_memcmp(memory, stack))              },
    { "memcpy",         EAX(syscall_memcpy(memory, stack))              },
    { "memmove",        EAX(syscall_memmove(memory, stack))             },
    { "memset",         EAX(syscall_memset(memory, stack))              },
    { "strcat",         EAX(syscall_strcat(memory, stack))              },
    { "strchr",         EAX(syscall_strchr(memory, stack))              },
    { "strcmp",         EAX(syscall_strcmp(memory, stack))              },
    { "strcoll",        EAX(syscall_strcoll(memory, stack))             },
    { "strcpy",         EAX(syscall_strcpy(memory, stack))              },
    { "strcspn",        EAX(syscall_strcspn(memory, stack))             },
    { "strerror",       EAX(syscall_strerror(memory, stack))            },
    { "strlen",         EAX(syscall_strlen(memory, stack))              },
    { "strncat",        EAX(syscall_strncat(memory, stack))             },
    { "strncmp",        EAX(syscall_strncmp(memory, stack))             },
    { "strncpy",        EAX(syscall_strncpy(memory, stack))             },
    { "strpbrk",        EAX(syscall_strpbrk(memory, stack))             },
    { "strrchr",        EAX(syscall_strrchr(memory, stack))             },
    { "strspn",         EAX(syscall_strspn(memory, stack))              },
    { "strstr",         EAX(syscall_strstr(memory, stack))              },
    { "strtok",         EAX(syscall_strtok(memory, stack))              },
    { "strxfrm",        EAX(syscall_strxfrm(memory, stack))             },

    // time
    { "asctime",        EAX(syscall_asctime(memory, stack))             },
    { "clock",          EAX(syscall_clock())                            },
    { "ctime",          EAX(syscall_ctime(memory, stack))               },
    { "difftime",       FLD(syscall_difftime(stack))                    },
    { "gmtime",         EAX(syscall_gmtime(memory, stack))              },
    { "localtime",      EAX(syscall_localtime(memory, stack))           },
    { "mktime",         EAX(syscall_mktime(memory, stack))              },
    { "strftime",       EAX(syscall_strftime(memory, stack))            },
    { "time",           EAX(syscall_time(memory, stack))                },
};

size_t syscall_i386_execute(void* data, size_t index, int(*syslog)(const char*, va_list), int(*log)(const char*, va_list))
{
    index = uint32_t(-index - SYMBOL_INDEX);

    size_t count = sizeof(syscall_tables) / sizeof(syscall_tables[0]);
    if (index < count) {
        syslog("%s\n", (va_list)&syscall_tables[index].name);

        auto* cpu = (x86_i386*)data;
        auto& x86 = cpu->x86;
        auto& x87 = cpu->x87;
        auto* memory = cpu->Memory();
        auto* stack = memory + cpu->Stack();
        auto* allocator = cpu->Allocator();
        auto syscall = syscall_tables[index].syscall;
        syscall(x86, x87, memory, stack, allocator, log);
    }

    return 0;
}

size_t syscall_i386_symbol(const char* file, const char* name)
{
    if (file == nullptr)
        return 0;

    size_t count = sizeof(syscall_tables) / sizeof(syscall_tables[0]);
    for (size_t index = 0; index < count; ++index) {
        if (strcmp(syscall_tables[index].name, name) == 0)
            return (uint32_t)(-index - SYMBOL_INDEX);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
