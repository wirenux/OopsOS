#include "../libc.h"
#include "commands.h"
#include "../vga.h"

// TODO: fix text

void cmd_help(int argc, char **argv) {
    (void)argc;
    (void)argv;
    term_printf("Available commands:\n");
    term_printf("   ");
    for (int i = 0; commands[i].name != NULL; i++) {
        if (commands[i].description)  // only print if description exists
            term_printf("%s, ", commands[i].name);
    }
    term_printf("\n");
}