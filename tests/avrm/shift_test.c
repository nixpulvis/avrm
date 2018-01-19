#include <avrm/shift.h>

int main(void) {
    ShiftConfig shift_config = { 7, 8 };
    ShiftLatchConfig shift_latch_config = { 9, 10, 11 };

    shift_init(shift_config);
    shift_latch_init(shift_latch_config);

    shift_byte(shift_config, 42);

    byte buff[10];

    shift(shift_config, buff, 10);
    shift_latch(shift_latch_config, buff, 10);

    return 0;
}
