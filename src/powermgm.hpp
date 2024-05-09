#include "ble/callback.h"

#define POWERMGM_LOOP _BV(0)
#define POWERMGM_INIT _BV(1)
#define POWERMGM_SLEEP_START _BV(2)
#define POWERMGM_SLEEP_END _BV(3)
#define POWERMGM_PLUGGED_IN _BV(4)
#define POWERMGM_UNPLUGGED _BV(5)
#define POWERMGM_MUSIC_UPDATE _BV(6)
#define POWERMGM_BLE_CONNECT _BV(7)
#define POWERMGM_BLE_DISCONNECT _BV(8)

void powermgmInit();
void powermgmLoop();
bool powermgmRegisterCB(CALLBACK_FUNC, EventBits_t, const char *);
bool powermgmRegisterCBPrio(CALLBACK_FUNC, EventBits_t, const char *, callback_prio_t);
bool powermgmSendEvent(EventBits_t);