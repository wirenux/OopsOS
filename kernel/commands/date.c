#include "../libc.h"
#include "../vga.h"

void cmd_date(int argc, char **argv) { // date -> dd/mm/yy hh:mm:ss | date -t -> 176635....
    struct rtc_time t;
    rtc_read_time(&t);

    if (argc > 1 && strcmp(argv[1], "-t") == 0) {
        term_printf("%d\n", timestamp());
        return;
    }

    term_printf(
        "%s/%s/%d %s:%s:%s\n",
        t.f_day, t.f_month, t.year,
        t.f_hour, t.f_min, t.f_sec
    );
}