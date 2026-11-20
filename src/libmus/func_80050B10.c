#include "common.h"
#include "libmus/player.h"

/* song_t start: relocates the song header's offsets once, then allocates a master channel and one channel per track */

void func_8005057C(channel_t *c);

static inline void func_reloc(s32 *p, s32 n, s32 b) {
    s32 i;
    s32 base = b;
    for (i = 0; i < n; i++) {
        if (p[i] != 0) {
            p[i] += base;
        }
    }
}


s32 func_80050B10(song_t *s) {
    s32 count = s->num_channels;
    s32 i;
    s32 id;
    channel_t *c;
    s32 base = (s32)s;

    if (s->flags == 0) {
        s->flags = 1;
        func_reloc((s32 *)(*(s32 * (*)[7])&s->ChannelData), 7, (s32)s);
        func_reloc((*(s32 * (*)[7])&s->ChannelData)[0], count, base);
        func_reloc((*(s32 * (*)[7])&s->ChannelData)[1], count, base);
        func_reloc((*(s32 * (*)[7])&s->ChannelData)[2], count, base);
    }
    id = D_800A0560++;
    c = &mus_channels[func_800506B8(s, -1)];
    func_8005057C(c);
    c->velocity_on = 1;
    c->song_addr = s;
    c->flags |= 3;
    c->pdata = c->pbase = (s32)(*(s32 * (*)[7])&s->ChannelData)[6];
    c->handle = id;
    for (i = 0; i < count; i++) {
        if ((*(s32 * (*)[7])&s->ChannelData)[0][i] != 0) {
            c = &mus_channels[func_800506B8(s, i)];
            func_8005057C(c);
            c->velocity_on = 1;
            c->song_addr = s;
            c->flags |= 1;
            c->pvolume = c->pvolumebase = (*(s32 * (*)[7])&s->ChannelData)[1][i];
            c->ppitchbend = c->ppitchbendbase = (*(s32 * (*)[7])&s->ChannelData)[2][i];
            c->pdata = c->pbase = (*(s32 * (*)[7])&s->ChannelData)[0][i];
            c->handle = id;
        }
    }
    D_800A0568 = 0;
    return id;
}
