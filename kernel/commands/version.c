#include "../info.h"
#include "../vga.h"

void cmd_version() {
    term_printf("OopsOS - Version: %s (x86_64)\n", kernel_version);
}