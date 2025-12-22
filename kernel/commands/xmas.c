#include "../vga.h"
#include "../libc.h"

/*
    *
   /.\
  /o..\
  /..o\
 /.o..o\
 /...o.\
/..o....\
^^^[_]^^^
*/

void cmd_xmas_tree() {
    term_printf_color(VGA_COLOR_LIGHT_BROWN, "    *\n"); // yellow star

    const char* tree_lines[] = {
        "   /.\\",
        "  /o..\\",
        "  /..o\\",
        " /.o..o\\",
        " /...o.\\",
        "/..o....\\"
    };

    uint8_t bauble_colors[] = {
        VGA_COLOR_LIGHT_BLUE,
        VGA_COLOR_LIGHT_RED,
        VGA_COLOR_LIGHT_GREEN,
        VGA_COLOR_CYAN,
        VGA_COLOR_MAGENTA
    };

    for (int i = 0; i < 6; i++) {
        for (int j = 0; tree_lines[i][j] != '\0'; j++) {
            char c = tree_lines[i][j];
            if (c == 'o') {
                // Random color for baubles
                uint8_t color = bauble_colors[rand() % 5];
                term_putchar_color(c, color);
            } else {
                term_putchar_color(c, VGA_COLOR_GREEN);
            }
        }
        term_putchar('\n'); // move to next line
    }
    term_printf_color(VGA_COLOR_BROWN, "^^^[_]^^^\n");
}
