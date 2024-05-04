// #include "screens/screens.hpp"
#include "screens/numbers.hpp"
#include "screens/clock.hpp"
// #include "lvgl.h"
// #include "system.hpp"

ClockScreen clockscreen;
NumbersScreen numbersscreen;

lv_obj_t *scr;

void screenInit() {

    scr = lv_obj_create(nullptr);

    numbersscreen.create(scr, 0, 0);
    clockscreen.create(scr, 1, 0);
    lv_screen_load(scr);
}

void screenPeriodic() {
    numbersscreen.periodic();
    clockscreen.periodic();
}