#include "../vga.h"
#include "../info.h"

void cmd_info() {
    term_prinf_color_all(VGA_COLOR_WHITE, VGA_COLOR_BLUE, "OopsOS");
    term_printf(" - Version %s\n", kernel_version);
    // TODO: add other info (cpu, mem, etc...)
}