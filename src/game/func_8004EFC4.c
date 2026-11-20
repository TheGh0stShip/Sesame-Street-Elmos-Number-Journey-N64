#include "common.h"
#include "libmus/player.h"
/* Provisional declarations recovered from machine code by n64dk. Evidence, not truth. */

                    /* extern */

void func_8004EFC4(struct mus_msg_s *arg0)
{
  s32 temp_v1_7;
  temp_v1_7 = arg0->type;
  switch (temp_v1_7)
  {
    case 0:
      func_80050D7C(arg0->data, -2, 1);
      return;

    case 1:
      func_80050D7C(arg0->data, -2, 0);
      return;

    case 2:
      ChangeCustomEffect(arg0->data);
      return;

  }

}
