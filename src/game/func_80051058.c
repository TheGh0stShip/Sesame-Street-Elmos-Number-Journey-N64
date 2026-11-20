#include "common.h"

void *func_80051058(void *arg0, u8 *arg1)
{
  (*(s16 *)((s8 *)(arg0) + (0xB4))) = (s16) (*arg1);
  (*(s16 *)((s8 *)(arg0) + (0xB2))) = 0;
  return arg1 + 1;
}
