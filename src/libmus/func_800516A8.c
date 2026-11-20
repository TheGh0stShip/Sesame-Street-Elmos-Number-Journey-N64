#include "common.h"
#include "libmus/player.h"


void *Fbendrange(struct channel_s *arg0, u8 *arg1)
{
  f32 temp_f0_14;
  temp_f0_14 = (f32) (((f64) ((f32) (*arg1))) * D_8007B5C0);
  arg0->bendrange = temp_f0_14;
  arg0->pitchbend_precalc = (f32) (arg0->pitchbend * temp_f0_14);
  return arg1 + 1;
}
