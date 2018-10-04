#ifndef _FLUX_STRING_H
#define _FLUX_STRING_H

#include <stdint.h>

int strcmp(const char*, const char*);
//void* memchr(const void*, int, size_t);
//int memcmp(const void*, const void*, size_t);
extern void* memcpy(void*, const void*, size_t);
//void* memmove(void *dest, const void *src, size_t n);
extern void* memset(void*, int, size_t);
//char* strcat(char *dest, const char *src);
//char* strncat(char *dest, const char *src, size_t n);
//char* strchr(const char *str, int c);
//int strcmp(const char *str1, const char *str2);
//int strncmp(const char *str1, const char *str2, size_t n);;
//int strcoll(const char *str1, const char *str2);
//char* strcpy(char *dest, const char *src);
//char* strncpy(char *dest, const char *src, size_t n);
//size_t strcspn(const char *str1, const char *str2);
//char* strerror(int errnum);
size_t strlen(const char *str);
//char* strpbrk(const char *str1, const char *str2);
//char* strrchr(const char *str, int c);
//size_t strspn(const char *str1, const char *str2);
//char* strstr(const char *haystack, const char *needle);
//char* strtok(char *str, const char *delim);
//size_t strxfrm(char *dest, const char *src, size_t n);

#endif