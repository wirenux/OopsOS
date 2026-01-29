#ifndef COMMANDS_H
#define COMMANDS_H

#include "../shell.h"

void cmd_ascii();
void cmd_clear();
void cmd_date(int argc, char **argv);
void cmd_echo(int argc, char* argv[]);
void cmd_help();
void cmd_reboot();
void cmd_setup();
void cmd_shutdown();
void cmd_version();
void cmd_whoami();
void cmd_xmas_tree();
void cmd_info();

extern Command commands[];

#endif
