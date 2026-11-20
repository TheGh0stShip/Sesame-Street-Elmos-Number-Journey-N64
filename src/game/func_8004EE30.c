#include "common.h"

s32 MusFxBankNumberOfEffects(void *arg0) {
    return (*(s32 *)((s8 *)(arg0) + (4)));
}
