#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H
#define RAND_MAX 0x7FFF
s32 rand(void);
void srand(u32 seed);
#endif
