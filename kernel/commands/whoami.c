#include "../info.h"
#include "../vga.h"

void cmd_whoami() {
    term_printf("%s\n", username);
}