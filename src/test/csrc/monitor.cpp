#include "macro.h"
#include <inttypes.h>

static int g_trapCode = STATE_RUNNING;
static uint64_t g_trapPC = 0;



bool is_finish() {
    return g_trapCode != STATE_RUNNING;
}

