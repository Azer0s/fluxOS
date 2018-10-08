#ifndef __FLUX__STDIO_H
#define __FLUX__STDIO_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <core/console.h>

int printf(const char* format, ...);
int sprintf(char* buffer, const char* format, ...);
int snprintf(char* buffer, size_t count, const char* format, ...);
int vsnprintf(char* buffer, size_t count, const char* format, va_list va);
int fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...);

#endif