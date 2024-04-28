#include "screens/screens.hpp"
#include "lvgl.h"
// #include "system.hpp"

NumbersScreen screen;

void screenInit() {

    screen.create();
    lv_screen_load(screen.scr);
}

void screenPeriodic() {
    screen.periodic();
}