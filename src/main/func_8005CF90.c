/* spColor: set sprite colour */
typedef struct { u8 unk_00[0x18]; u8 red, green, blue, alpha; } Sprite;
void func_8005CF90(Sprite *sp, u8 red, u8 green, u8 blue, u8 alpha) {
    sp->red = red; sp->green = green; sp->blue = blue; sp->alpha = alpha;
}
