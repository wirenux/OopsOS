#include "../info.h"
#include "../vga.h"

void cmd_version(int argc, char **argv) {
    (void) argc;
    (void) argv;
    term_printf("OopsOS - Version: %s (x86_64)\n", kernel_version);
}