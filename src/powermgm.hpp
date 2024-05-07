#include "ble/callback.h"

#define POWERMGM_LOOP _BV(0)
#define POWERMGM_INIT _BV(1)
#define POWERMGM_SLEEP_START _BV(2)
#define POWERMGM_SLEEP_END _BV(3)
#define POWERMGM_PLUGGED_IN _BV(4)
#define POWERMGM_UNPLUGGED _BV(5)

void powermgmInit();
void powermgmLoop();
bool powermgmRegisterCB(CALLBACK_FUNC, EventBits_t, const char *);
bool powermgmRegisterCBPrio(CALLBACK_FUNC, EventBits_t, const char *, callback_prio_t);