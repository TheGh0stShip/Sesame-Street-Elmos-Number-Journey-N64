#include "common.h"
#include "libmus/player.h"


void func_80050D7C(s32 arg0, s32 arg1, s32 arg2)
{
    s32 pad[2]; /* n64dk: frame padding, reconstruction scaffolding */
  s32 var_a3_11;
  struct channel_s *var_v1_9;
  var_v1_9 = mus_channels;
  var_a3_11 = 0;
  if (max_channels > 0)
  {
    do
    {
      var_a3_11 += 1;
      if (var_v1_9->handle == arg0)
      {
        var_v1_9->flags = (s32) ((var_v1_9->flags & arg1) | arg2);
      }
      var_v1_9 = (struct channel_s *) (((s8 *) var_v1_9) + 0x13C);
    }
    while (var_a3_11 < max_channels);
  }
}
