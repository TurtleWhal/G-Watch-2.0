#include "lvgl.h"

lv_obj_t *screenCreate(uint8_t, uint8_t);

// class WatchScreen
// {
// public:
//     WatchScreen(void *crt, void *prd, lv_obj_t *scn)
//     {
//         c = crt;
//         p = prd;
//         scr = scn;
//     };
//     WatchScreen(){scr = lv_obj_create(nullptr);};
//     void create() { c; };
//     void periodic() { p; };
//     lv_obj_t *scr;

// private:
//     void *c;
//     void *p;
// };

// class WatchScreens
// {
// public:
//     WatchScreen clock;
// };
// WatchScreens screens;