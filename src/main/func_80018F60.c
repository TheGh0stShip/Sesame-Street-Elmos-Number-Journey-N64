typedef struct Entry {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
} Entry;

extern s32 D_80075090;
extern u8 D_800A7F70;
extern u8 D_8006FE5C;
extern u8 D_8006FE5D;
extern u8 D_8006FE5E;
extern u8 D_8006FE5F;
extern u8 D_80118240;
extern Entry D_800750A0[];
extern Entry D_800750C8[];
extern s32 func_800217C4(s32);

/* Picks a new random choice (different from the last one) for the current mode and loads its parameters. */
void func_80018F60(void) {
    s32 tries;

    switch (D_80075090) {
    case 0:
        tries = 100;
        do {
            D_8006FE5C = D_800A7F70 = func_800217C4(9) + 1;
        } while (D_8006FE5C == D_80118240 && --tries != 0);
        D_80118240 = D_8006FE5C;
        D_8006FE5E = 0;
        break;
    case 1:
        tries = 100;
        do {
            D_800A7F70 = func_800217C4(10);
        } while (D_800A7F70 == D_80118240 && --tries != 0);
        D_80118240 = D_800A7F70;
        D_8006FE5C = D_800750A0[D_800A7F70].unk_0 - 1;
        D_8006FE5D = D_800750A0[D_800A7F70].unk_2 - 1;
        D_8006FE5F = 0;
        D_800A7F70 = D_800750A0[D_800A7F70].unk_3;
        D_8006FE5E = 0;
        break;
    case 2:
        tries = 100;
        do {
            D_800A7F70 = func_800217C4(0x48);
        } while (D_800A7F70 == D_80118240 && --tries != 0);
        D_80118240 = D_800A7F70;
        D_8006FE5C = D_800750C8[D_800A7F70].unk_0 - 1;
        D_8006FE5D = D_800750C8[D_800A7F70].unk_2 - 1;
        D_8006FE5F = D_800750C8[D_800A7F70].unk_1;
        D_800A7F70 = D_800750C8[D_800A7F70].unk_3;
        D_8006FE5E = 0;
        break;
    }
}
