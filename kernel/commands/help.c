#include "../libc.h"
#include "commands.h"
#include "../vga.h"

void cmd_help() {
    term_printf("Build-in commands:\n");
    term_printf("------------------\n        ");

    for (int i = 0; commands[i].name != NULL; i++) {
        if (commands[i].description) {
            int name_len = strlen(commands[i].name) + 2;

            // check if word exceeds VGA_WIDTH
            if (terminal_column + name_len >= VGA_WIDTH) {
                term_printf("\n");
            }

            term_printf("%s ", commands[i].name);
        }
    }
    term_printf("\n");
}