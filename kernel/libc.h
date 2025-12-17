#ifndef LIBC_H
#define LIBC_H

#include <stdint.h>
#include <stddef.h>

int strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

#endif