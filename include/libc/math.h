#ifndef LIBC_MATH_H
#define LIBC_MATH_H
/* the libm linked in this game (CORDIC-based, SVID error protocol: matherr / errno). agent sm */
/* struct exception {type, name, arg1, arg2, retval} is declared in structs.h (shared with the game) */
extern s32 errno;                       /* 33 EDOM, 34 ERANGE */
/* sin (func_80068630) and cos (func_80068710) keep their address names: the old PROVIDE(sin = 0x80068710) /
 * PROVIDE(cos = 0x80068630) that the game unit func_8002E140.c links against have the two swapped */
f64 tan(f64 x);
f64 asin(f64 x);
f64 acos(f64 x);
f64 atan(f64 x);
f64 atan2(f64 y, f64 x);
f64 sqrt(f64 x);
f64 floor(f64 x);
f64 ceil(f64 x);
f64 trunc(f64 x);
f64 fmod(f64 x, f64 y);
s32 matherr(struct exception *e);
#endif
