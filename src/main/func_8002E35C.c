typedef struct Obj {
    struct Obj *next;
    u8 pad4[4];
    u16 type;
    u8 padA[0x24 - 0xA];
    f32 x;
    u8 pad28[4];
    f32 z;
    u8 pad30[0x54 - 0x30];
    f32 radius;
} Obj;
typedef struct { f32 x, y, z; } Vec3;
typedef struct { u8 pad[0x60]; f32 unk_60; } S6888;
extern Obj *D_80115408;
extern S6888 *D_80106C84;
f32 func_80056690(f32);

/* pushes the point pos out of the first type 7/8/12 object it overlaps (in x/z); returns that object */
Obj *func_8002E35C(Vec3 *pos) {
    register Obj *obj = D_80115408;
    register f32 px = pos->x;
    register f32 pz = pos->z;
    register f32 ox;
    register f32 oz;
    register f32 dx;
    register f32 dz;
    register f32 extra = D_80106C84->unk_60;
    register f32 r;
    register f32 r2;
    register f32 d2;

    while (obj != NULL) {
        if (obj->type == 7 || obj->type == 8 || obj->type == 12) {
            r = obj->radius;
            if (r > 0.0f) {
                r = r + extra;
                r2 = r * r;
                ox = obj->x;
                oz = obj->z;
                dx = px - ox;
                dz = pz - oz;
                d2 = dx * dx;
                d2 = d2 + dz * dz;
                if (d2 < r2) {
                    r2 = 1.0 / func_80056690(d2);
                    dx = dx * r2;
                    dz = dz * r2;
                    dx = dx * r;
                    dz = dz * r;
                    pos->x = ox + dx;
                    pos->z = oz + dz;
                    return obj;
                }
            }
        }
        obj = obj->next;
    }
    return NULL;
}
