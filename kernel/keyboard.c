#include "keyboard.h"
#include "libc.h"

bool shift_pressed = false;

char scancode_to_ascii[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',0,
    '\\','z','x','c','v','b','n','m',',','.','/',0, '*',0, ' '
};

char shift_map(char c) {        // handle all shift special key
    switch(c) {
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        case '`': return '~';
        default: return c;
    }
}

bool keyboard_data_available(void) {
    return inb(0x64) & 1;
}

// keyboard.c
char handle_scancode(uint8_t scancode) {
    if (scancode == 0x2A || scancode == 0x36) {      // Shift press
        shift_pressed = true;
        return 0;
    }
    if (scancode == 0xAA || scancode == 0xB6) {      // Shift release
        shift_pressed = false;
        return 0;
    }

    // ignore key releases
    if (scancode & 0x80) return 0;

    char c = scancode_to_ascii[scancode];
    if (!c) return 0;

    if (shift_pressed) {
        // convert lower case to UPPER CASE
        if (c >= 'a' && c <= 'z') {
            c -= 32;
        } else {
            // handle special chars
            c = shift_map(c);
        }
    }

    return c;
}
