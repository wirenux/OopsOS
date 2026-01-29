#include "commands.h"
#include "../vga.h"
#include "../libc.h"

void cmd_reboot() {
    term_printf("Rebooting...\n");
    while (inb(0x64) & 0x02);
    outb(0x64, 0xFE);
}

void cmd_shutdown() {
    term_printf("Shutting down...\n");
    cmd_clear();
    cmd_ascii();
    term_printf("\nYOU CAN NOW PRESS THE POWER BUTTON\n");

    asm volatile("cli");
    while(1) {
        asm volatile("hlt");
    }
}