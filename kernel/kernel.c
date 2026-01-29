#include <stdbool.h>
#include "vga.h"
#include "keyboard.h"
#include "shell.h"
#include "libc.h"
#include "info.h"
#include "commands/commands.h"
#include "libc.h"

void kernel_main(void) {
    struct rtc_time t;
    rtc_read_time(&t);
    // Init
    term_init();

    // Splash screen
    cmd_ascii(0, NULL);
    term_printf("   OopsOS v%s (x86_64) (c) %d WireNux\n", kernel_version, t.year);
    term_printf("   Press ENTER to start...\n");

    // Wait for ENTER
    while (1) {
        if (keyboard_data_available()) {
            if (inb(0x60) == 0x1C) {
                break;
            }
        }
    }

    // Clear and start shell
    cmd_clear(0, NULL);

    term_shell();
}
