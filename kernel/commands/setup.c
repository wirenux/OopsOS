#include "../info.h"
#include "../vga.h"
#include "../libc.h"

void cmd_setup() {
    char buf[64];

    term_printf("Enter username: ");
    term_readline(buf, sizeof(buf));
    if (buf[0] != '\0') {   // empty (user just press enter)
        strcpy(username, buf);
    }

    term_printf("Enter hostname: ");
    term_readline(buf, sizeof(buf));
    if (buf[0] != '\0') {   // same
        strcpy(hostname, buf);
    }

    term_printf("Done\n");
}