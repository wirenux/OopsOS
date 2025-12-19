#include "shell.h"
#include "vga.h"
#include "libc.h"
#include "keyboard.h"
#include "info.h"
#define CMD_HISTORY_SIZE 128

char last_command[CMD_HISTORY_SIZE] = {0};

char command_buffer[128];
int buffer_index = 0;   // cursor position
int buffer_length = 0;  // total chars in buffer

void cmd_reboot() {
    term_printf("Rebooting...\n");
    while (inb(0x64) & 0x02);
    outb(0x64, 0xFE);
}

void cmd_shutdown() {
    term_printf("Shutting down...\n");
    term_clear(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_column = 0;
    terminal_row = 0;
    term_printf(
        "   ___                   ___  ____\n"
        "  / _ \\  ___  _ __  ___ / _ \\/ ___|\n"
        " | | | |/ _ \\| '_ \\/ __| | | \\___ \\\n"
        " | |_| | (_) | |_) \\__ \\ |_| |___) |\n"
        "  \\___/ \\___/| .__/|___/\\___/|____/\n"
        "             |_|\n"
    );
    term_printf("\nYOU CAN NOW PRESS THE POWER BUTTON\n");

    asm volatile("cli");
    while(1) {
        asm volatile("hlt");
    }
}

void cmd_echo(int argc, char* argv[]) {
    int rainbowFlag = 0;

    for (int i = 1; argv[i]; i++) {
        if (strcmp(argv[i], "-rainbow") == 0) {
            rainbowFlag = 1;
            break;
        }
    }

    uint8_t rainbowColors[] = {
        0x04, // red
        0x0C, // light red
        0x0E, // yellow
        0x02, // green
        0x03, // cyan
        0x09, // blue
        0x05, // magenta
        0x0D  // light magenta
    };

    int colorCount = sizeof(rainbowColors)/sizeof(rainbowColors[0]);
    int colorIndex = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-rainbow") == 0) continue;

        char* arg = argv[i];
        for (int j = 0; arg[j]; j++) {
            char c = arg[j];
            if (rainbowFlag) {
                term_putchar_color(c, rainbowColors[colorIndex % colorCount] | (VGA_COLOR_BLACK << 4));
                colorIndex++;
            } else {
                term_putchar(c);
            }
        }

        if (i < argc - 1) {
            if (rainbowFlag) {
                term_putchar_color(' ', rainbowColors[colorIndex % colorCount] | (VGA_COLOR_BLACK << 4));
                colorIndex++;
            } else {
                term_putchar(' ');
            }
        }
    }


    term_printf("\n");
}

void cmd_clear() {
    term_clear(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_column = 0;
    terminal_row = 0;
}

void cmd_help() {
    term_printf("Available commands:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        term_printf("   %s - %s\n", commands[i].name, commands[i].description);
    }
}

void cmd_version() {
    term_printf("OopsOS - Version: %s\n", kernel_version);
}

void cmd_setup_user() {
    char buf[64];

    term_printf("Enter username: ");
    term_readline(buf, sizeof(buf));

    username = buf;
    term_printf("Done\n");
}

void cmd_whoami() {
    term_printf("%s\n", username);
}

void cmd_date() {
    struct rtc_time t;
    rtc_read_time(&t);

    term_printf(
        "%d/%d/%d %d:%d:%d\n",
        t.day, t.month, t.year,
        t.hour, t.min, t.sec
    );
}

void cmd_ascii() {
    term_printf(
        "   ___                   ___  ____\n"
        "  / _ \\  ___  _ __  ___ / _ \\/ ___|\n"
        " | | | |/ _ \\| '_ \\/ __| | | \\___ \\\n"
        " | |_| | (_) | |_) \\__ \\ |_| |___) |\n"
        "  \\___/ \\___/| .__/|___/\\___/|____/\n"
        "             |_|\n"
    );
}

// === COMMAND TABLE ===

Command commands[] = {
    {"ascii", "Show OopsOS logo",               cmd_ascii},
    {"clear", "Clear the screen",               cmd_clear},
    {"date", "Show current date/time (UTC)",    cmd_date},
    {"echo", "Print something one the screen",  cmd_echo},
    {"help", "Show this help",                  cmd_help},
    {"reboot",   "Reboot the system",           cmd_reboot},
    {"setup_user", "Setup current user",        cmd_setup_user},
    {"shutdown", "Shutdown the system",         cmd_shutdown},
    {"version", "Show version of the kernel",   cmd_version},
    {"whoami", "Show current user",             cmd_whoami},

    {NULL, NULL, NULL}
};


void execute_command(char* input) {
    char* argv[16];
    int argc = 0;

    // Split input by spaces
    char* p = input;
    while (*p && argc < 15) {
        // Skip spaces
        while (*p == ' ') p++;
        if (!*p) break;

        argv[argc++] = p;

        // Find end of token
        while (*p && *p != ' ') p++;
        if (*p) {
            *p = '\0';
            p++;
        }
    }
    argv[argc] = NULL;

    if (argc == 0) return;

    // Find and execute command
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].handler(argc, argv); // pass full argc/argv
            return;
        }
    }

    term_printf("Unknown command: %s \n", argv[0]);
}

void term_shell(void) {
    char command_buffer[128];
    int buffer_index = 0;

    term_printf("> ");

    while (1) {
        if (keyboard_data_available()) {
            uint8_t scancode = inb(0x60);

            // UP
            if (scancode == 0x48) {
                // clear line
                while (buffer_index > 0) {
                    buffer_index--;
                    terminal_column--;
                    term_putchar_at(' ', terminal_row, terminal_column);
                }

                // copy last command
                int len = strlen(last_command);
                for (int i = 0; i < len; i++) {
                    command_buffer[i] = last_command[i];
                    term_putchar(command_buffer[i]);
                }

                buffer_index = len; // cursor at end
                buffer_length = len; // full buffer
                continue;
            }

            // LEFT
            if (scancode == 0x4B && buffer_index > 0) {
                buffer_index--;
                terminal_column--;
                update_cursor(terminal_row, terminal_column);
                continue;
            }

            // RIGHT
            if (scancode == 0x4D && buffer_index < buffer_length) {
                buffer_index++;
                terminal_column++;
                update_cursor(terminal_row, terminal_column);
                continue;
            }

            char c = handle_scancode(scancode);
            if (!c) continue;

            if (c == '\n') {
                command_buffer[buffer_index] = '\0';
                term_putchar('\n');

                if (buffer_index > 0) {
                    strcpy(last_command, command_buffer); // save last cmd
                    execute_command(command_buffer);
                }

                buffer_index = 0;
                term_printf("> ");
                continue;
            }

            if (c == '\b') {
                if (buffer_index > 0) {
                    buffer_index--;
                    terminal_column--;
                    term_putchar_at(' ', terminal_row, terminal_column);
                }
                continue;
            }

            if (buffer_index < 127) {
                command_buffer[buffer_index] = c;       // put char at cursor
                buffer_index++;                         // move cursor forward
                if (buffer_index > buffer_length)       // update buffer length if we added new char
                    buffer_length = buffer_index;
                term_putchar(c);                        // print char on screen
            }
        }
    }
}