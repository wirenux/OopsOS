#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

extern char scancode_to_ascii[128];

bool keyboard_data_available(void);
char handle_scancode(uint8_t scancode);

#endif