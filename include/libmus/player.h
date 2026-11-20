#ifndef LIBMUS_PLAYER_H
#define LIBMUS_PLAYER_H
/* libmus (Software Creations music player) as linked in this game: the structures the player code reads and
 * writes, named from what the code does with each field (evidence: work/sm, the song/effect/pointer-bank formats in
 * the asset notes). unk_XX: offset known, purpose not proven. Shared by every libmus unit (agent sm). */

typedef s32 musHandle;                  /* song/effect handle: a channel's handle (+0x44); 0 = none */

/* MusInitialize's configuration block (0x44 bytes; the order and meaning of every word is what
 * func_8004C6FC = MusInitialize reads) */
typedef struct musConfig_s {
    u32 control_flag;                   /* 0x00 */
    s32 channels;                       /* 0x04 song channels; max_channels = channels + 4 effect voices */
    void *sched;                        /* 0x08 scheduler (func_80050AF0) */
    s32 thread_priority;                /* 0x0C given to every synth voice */
    u8 *heap;                           /* 0x10 */
    s32 heap_length;                    /* 0x14 */
    u8 *ptr;                            /* 0x18 pointer bank (MusPtrBankInitialize) */
    u8 *wbk;                            /* 0x1C wave bank ROM address */
    void *default_fxbank;               /* 0x20 (MusFxBankInitialize) */
    s32 fifo_length;                    /* 0x24 command FIFO entries (clamped 64..1024) */
    s32 syn_updates;                    /* 0x28 */
    s32 syn_output_rate;                /* 0x2C */
    s32 syn_rsp_cmds;                   /* 0x30 */
    s32 syn_retraceCount;               /* 0x34 */
    s32 syn_num_dma_bufs;               /* 0x38 */
    s32 syn_dma_buf_size;               /* 0x3C */
    void *diskrom_handle;               /* 0x40 */
} musConfig;

/* envelope: 7 bytes in the song's envelope table (command 0x90) or inline after command 0x84 */
typedef struct env_s {
    u8 speed;                           /* 0 = 1; channel env_speed_calc = 0x400 / speed */
    u8 init_vol;
    u8 attack_time;
    u8 peak_vol;
    u8 decay_time;
    u8 sustain_vol;
    u8 release_time;
} env_t;

/* drum table entry (6 bytes; command 0x9E points the channel at one) */
typedef struct drum_s {
    u16 wave;                           /* wave-map index */
    u16 env;                            /* envelope index */
    u8 pan;                             /* stored >> 1 */
    u8 note;
} drum_t;

/* song header (0x38 bytes); the offsets are turned into pointers once by func_8004F1A0 (flags = 1) */
typedef struct song_s {
    u32 version;                        /* 0x00 0x00000215 */
    s32 num_channels;                   /* 0x04 entries of each per-track table */
    s32 num_waves;                      /* 0x08 entries of the wave map */
    u8 **ChannelData;                   /* 0x0C per track: command stream (NULL = unused track) */
    u8 **VolumeData;                    /* 0x10 per track: volume stream */
    u8 **PitchBendData;                 /* 0x14 per track: pitch-bend stream */
    env_t *EnvelopeData;                /* 0x18 */
    drum_t *DrumData;                   /* 0x1C */
    u16 *WaveTable;                     /* 0x20 wave map: song wave -> bank entry (0xFFFF = rest) */
    u8 *MasterTrack;                    /* 0x24 */
    s32 flags;                          /* 0x28 1 = relocated */
    u32 unk_2C[3];                      /* 0x2C never read */
} song_t;

/* one effect of an effect bank */
typedef struct fxentry_s {
    u8 *fxdata;                         /* command stream */
    s32 priority;                       /* default priority */
} fxentry_t;

/* effect bank header (MusFxBankInitialize relocates it once) */
typedef struct fx_s {
    s32 number_of_components;           /* 0x00 entries of effects[] */
    s32 number_of_effects;              /* 0x04 returned by MusFxBankNumberOfEffects */
    s32 num_waves;                      /* 0x08 entries of the wave map */
    s32 flags;                          /* 0x0C bit 0 = relocated */
    struct ptr_bank_s *ptr_addr;        /* 0x10 pointer bank (NULL = current; MusFxBankSet/GetPtrBank) */
    u16 *wave_table;                    /* 0x14 wave map: effect wave -> bank entry */
    fxentry_t effects[1];               /* 0x18 */
} fx_t;

/* pointer bank header ("N64 PtrTablesV2"; relocated once by func_8004E9E0, flags bit 31) */
typedef struct ptr_bank_s {
    char header_name[16];               /* 0x00 */
    s32 flags;                          /* 0x10 < 0: relocated */
    u8 unk_14[12];                      /* 0x14 */
    s32 count;                          /* 0x20 wave-table entries */
    u8 *basenote;                       /* 0x24 base note per entry */
    f32 *detune;                        /* 0x28 tune per entry (detune/100 + basenote - 0x30 after relocation) */
    void **wave_list;                   /* 0x2C ALWaveTable * per entry */
} ptr_bank_t;

/* a player channel (0x13C bytes): max_channels of them at mus_channels; channel i >= 4 drives synth voice i - 4,
 * the first 4 have no voice (song master tracks); effects take channels from index 4 (D_800A00E0) */
typedef struct channel_s {
    u32 flags;                          /* 0x00 bit 0 paused, bit 1 master track */
    u8 *pdata;                          /* 0x04 command stream; NULL = channel free */
    void *sample;                       /* 0x08 wave to start on the voice (ALWaveTable *) */
    u32 channel_frame;                  /* 0x0C channel clock, 1/256 tick */
    s32 stopping;                       /* 0x10 fade-out frames left; -1 = not stopping */
    u32 volume_frame;                   /* 0x14 next volume-stream event */
    u32 pitchbend_frame;                /* 0x18 next pitch-bend-stream event */
    s32 stopping_speed;                 /* 0x1C fade-out length */
    f32 vib_amount;                     /* 0x20 vibrato depth (semitones) */
    f32 pitchbend_precalc;              /* 0x24 (bend - 64) * bendrange */
    f32 old_frequency;                  /* 0x28 last pitch sent to the voice */
    f32 base_note;                      /* 0x2C note + tune + transpose */
    f32 freqoffset;                     /* 0x30 distort + MusHandleSetFreqOffset */
    u8 *ppitchbend;                     /* 0x34 pitch-bend stream */
    u8 *pvolume;                        /* 0x38 volume stream */
    u32 note_end_frame;                 /* 0x3C clock of the next event */
    u32 note_start_frame;               /* 0x40 clock of the current note's start */
    musHandle handle;                   /* 0x44 */
    s32 priority;                       /* 0x48 */
    f32 port_base;                      /* 0x4C portamento start pitch */
    f32 port_current;                   /* 0x50 portamento pitch now */
    u32 release_frame;                  /* 0x54 clock at which the release starts */
    f32 env_attack_calc;                /* 0x58 (peak - init) / attack_time */
    f32 env_decay_calc;                 /* 0x5C (sustain - peak) / decay_time */
    f32 env_release_calc;               /* 0x60 1 / release_time */
    s32 env_speed_calc;                 /* 0x64 0x400 / speed */
    f32 vibrato;                        /* 0x68 current vibrato offset */
    f32 bendrange;                      /* 0x6C semitones per bend unit (default 1/32) */
    f32 pitchbend;                      /* 0x70 */
    song_t *song_addr;                  /* 0x74 song channel: its song */
    fx_t *fx_addr;                      /* 0x78 effect voice: its effect bank */
    ptr_bank_t *sample_bank;            /* 0x7C */
    u8 *pbase;                          /* 0x80 command stream start (goto base) */
    drum_t *pdrums;                     /* 0x84 drum mode: current drum entry */
    u8 *ppitchbendbase;                 /* 0x88 */
    u8 *pvolumebase;                    /* 0x8C */
    f32 distort;                        /* 0x90 fine detune (command 0x8F) */
    u32 sweep_frame;                    /* 0x94 next pan-sweep step */
    s16 temscale;                       /* 0x98 tempo scale (MusHandleSetTempo, 0x80 = unity) */
    u16 length;                         /* 0x9A current note length (ticks; 0x7FFF = held) */
    s16 channel_tempo;                  /* 0x9C clock step per frame = tempo * temscale >> 7 */
    s16 volscale;                       /* 0x9E MusHandleSetVolume (0x80 = unity) */
    u16 old_volume;                     /* 0xA0 last volume sent to the voice */
    u16 volume_count;                   /* 0xA2 volume-stream hold counter */
    u16 pitchbend_count;                /* 0xA4 pitch-bend-stream hold counter */
    s16 fx_number;                      /* 0xA6 effect number of an effect voice */
    s16 tempo;                          /* 0xA8 tempo word (command 0x85) */
    u16 ticks;                          /* 0xAA ticks since the note started */
    u16 fixed_length;                   /* 0xAC command 0x8B; 0 = notes carry a length */
    u16 wave;                           /* 0xAE wave-map index */
    s16 panscale;                       /* 0xB0 MusHandleSetPan (0x80 = unity) */
    u16 cutoff;                         /* 0xB2 */
    u16 endit;                          /* 0xB4 */
    u8 vib_delay;                       /* 0xB6 */
    u8 ignore;                          /* 0xB7 next note reads a length anyway */
    u8 port;                            /* 0xB8 portamento time */
    u8 transpose;                       /* 0xB9 */
    u8 ignore_transpose;                /* 0xBA */
    u8 velocity;                        /* 0xBB */
    u8 volume;                          /* 0xBC */
    u8 pan;                             /* 0xBD 0..63 */
    u8 old_pan;                         /* 0xBE last pan sent */
    u8 env_speed;                       /* 0xBF */
    u8 env_init_vol;                    /* 0xC0 */
    u8 env_max_vol;                     /* 0xC1 */
    u8 env_sustain_vol;                 /* 0xC2 */
    u8 env_phase;                       /* 0xC3 1 attack, 2 decay, 3 sustain, 4 release, 5 done */
    u8 env_current;                     /* 0xC4 */
    u8 env_count;                       /* 0xC5 frames to the next envelope step */
    u8 env_attack_time;                 /* 0xC6 */
    u8 env_decay_time;                  /* 0xC7 */
    u8 env_release_time;                /* 0xC8 */
    u8 playing;                         /* 0xC9 the synth voice is running */
    u8 reverb;                          /* 0xCA command 0xA2 */
    u8 reverb_base;                     /* 0xCB MusHandleSetReverb */
    u8 old_reverb;                      /* 0xCC last reverb sent */
    u8 release_start_vol;               /* 0xCD envelope level when the release began */
    u8 wobble_on_speed;                 /* 0xCE */
    u8 wobble_off_speed;                /* 0xCF */
    u8 wobble_count;                    /* 0xD0 */
    s8 wobble_current;                  /* 0xD1 */
    u8 velocity_on;                     /* 0xD2 notes carry a velocity byte */
    u8 default_velocity;                /* 0xD3 */
    u8 sweep_speed;                     /* 0xD4 */
    u8 vib_speed;                       /* 0xD5 vibrato period (frames) */
    u8 env_trigger_off;                 /* 0xD6 envelope does not restart on new notes */
    u8 trigger_off;                     /* 0xD7 legato: no new sample start */
    u8 wobble_amount;                   /* 0xD8 */
    u8 sweep_timer;                     /* 0xD9 */
    u8 sweep_dir;                       /* 0xDA */
    u8 for_stack_count;                 /* 0xDB loop depth */
    f32 vib_precalc;                    /* 0xDC 2 pi / vib_speed */
    u8 *for_stack[4];                   /* 0xE0 loop start */
    u8 *for_stackvol[4];                /* 0xF0 */
    u8 *for_stackpb[4];                 /* 0x100 */
    u16 for_vol_count[4];               /* 0x110 */
    u16 for_pb_count[4];                /* 0x118 */
    u8 for_count[4];                    /* 0x120 0xFF = forever */
    u8 for_volume[4];                   /* 0x124 */
    f32 for_pitchbend[4];               /* 0x128 */
    u8 unk_138[4];                      /* 0x138 */
} channel_t;

/* command FIFO entry (8 bytes; MusHandlePause/UnPause, MusSetFxType post them, the frame handler func_8004D654
 * runs them: type 0 pause, 1 unpause, 2 effect type) */
typedef struct mus_msg_s {
    u8 type;
    s32 data;                           /* handle or effect type */
} mus_msg_t;

/* command handler: returns the new stream pointer (NULL ends the channel) */
typedef u8 *(*command_func_t)(channel_t *, u8 *);

/* command bytes (the handler table D_80076E10 has 45 entries, byte - 0x80) */
enum {
    MUS_STOP = 0x80, MUS_WAVE, MUS_PORT, MUS_PORTOFF, MUS_DEFA, MUS_TEMPO, MUS_CUTOFF, MUS_ENDIT, MUS_VIBUP,
    MUS_VIBDOWN, MUS_VIBOFF, MUS_LENGTH, MUS_IGNORE, MUS_TRANS, MUS_IGNORE_TRANS, MUS_DISTORT, MUS_ENVELOPE,
    MUS_ENVOFF, MUS_ENVON, MUS_TROFF, MUS_TRON, MUS_FOR, MUS_NEXT, MUS_WOBBLE, MUS_WOBBLEOFF, MUS_VELON,
    MUS_VELOFF, MUS_VELOCITY, MUS_PAN, MUS_STEREO, MUS_DRUMS, MUS_DRUMSOFF, MUS_PRINT, MUS_GOTO, MUS_REVERB,
    MUS_RANDNOTE, MUS_RANDVOLUME, MUS_RANDPAN, MUS_VOLUME, MUS_STARTFX, MUS_BENDRANGE, MUS_SWEEP, MUS_CHANGEFX,
    MUS_MARKER, MUS_LENGTH0, MUS_NUM_COMMANDS = MUS_LENGTH0 - MUS_STOP + 1
};
extern command_func_t D_80076E10[];     /* jump table, MUS_NUM_COMMANDS entries */
extern s32 max_channels;                /* D_800A00D4: musConfig.channels + 4 */
extern channel_t *mus_channels;         /* D_800A00DC: max_channels channels */

/* SM_PROTOTYPES (generated by work/sm_mus.py) */
void func_8004DE70(s32 arg0, s32 arg1, s32 arg2);
void func_8004DEFC(s32 arg0);
u8 *func_8004DF20(channel_t *v, u8 *p);
void MusSetMasterVolume(s32 arg0, s32 arg1);
s32 MusStartSong(void);
s32 MusStartSongFromMarker(s32 id, s32 marker);
s32 MusStartEffect(s32 arg0);
s32 MusStartEffect2(s32 idx, s32 arg1, s32 arg2, s32 search, s32 arg4);
void MusStop(s32 mask, s32 arg);
s32 MusAsk(s32 flags);
s32 MusHandleStop(s32 arg0, s32 arg1);
s32 MusHandleAsk(s32 key);
s32 MusHandleSetVolume(s32 id, s16 val);
s32 MusHandleSetPan(s32 id, s16 val);
s32 MusHandleSetFreqOffset(s32 id, f32 val);
s32 MusHandleSetTempo(s32 id, s32 v);
s32 MusHandleSetReverb(s32 id, s32 val);
void MusPtrBankInitialize(s32 arg0);
void *MusPtrBankSetSingle(void *arg0);
void MusPtrBankSetCurrent(void *arg0);
s32 MusPtrBankGetCurrent(void);
s32 MusHandleGetPtrAddr(s32 id);
void MusHandlePause(s32 a);
void MusHandleUnPause(void *p);
void MusSetFxType(void *p);
void MusSetSongFxChange(s32 arg0);
s32 MusFxBankNumberOfEffects(void *arg0);
void MusFxBankSetCurrent(s32 arg0);
void MusFxBankSetSingle(s32 arg0);
s32 MusFxBankGetCurrent(void);
void MusFxBankSetPtrBank(void *arg0, s32 arg1);
s32 MusFxBankGetPtrBank(void *arg0);
void MusSetScheduler(s32 arg0);
s32 MusHandleWaveCount(s32 id);
s32 MusHandleWaveAddress(s32 id);
void MusSetMarkerCallback(s32 arg0);
void func_8004EF60(s32 n);
void func_8004EFC4(struct mus_msg_s *arg0);
s32 func_8004F044(mus_msg_t *m);
s32 func_8004F0DC(void);
void func_8004F3CC(channel_t *t, s32 ch);
void func_8004F914(channel_t *o, s32 idx);
void func_8004F990(channel_t *c, s32 idx);
void func_8004FAE4(channel_t *v, s32 idx, f32 bend);
void func_8004FBF0(channel_t *o);
void func_8004FC60(channel_t *e);
void func_8004FE54(struct channel_s *arg0);
void func_8004FE70(channel_t *p);
f32 func_8004FF28(channel_t *b);
f32 func_8004FF80(channel_t *o);
void func_8004FFE4(channel_t *s);
void func_800500A8(channel_t *t);
f32 func_800501BC(f32 x);
void func_80050350(s32 base, s32 arg1);
s32 func_800506B8(song_t *owner, s32 idx);
void func_80050880(s32 *p, s32 add, s32 n);
s32 func_80050968(fx_t *b, s32 prog, s32 key, s32 vel, s32 prio);
s32 func_80050B10(song_t *s);
void func_80050D7C(s32 arg0, s32 arg1, s32 arg2);
s32 Fstop(struct channel_s *arg0);
u8 *Fwave(channel_t *c, u8 *p);
u8 *Fport(channel_t *p, u8 *s);
s32 func_80050E40(void *arg0, s32 arg1);
u8 *Fdefa(channel_t *v, u8 *p);
u8 *Ftempo(channel_t *p, u8 *cmd);
void *func_80051058(void *arg0, u8 *arg1);
u8 *Fcutoff(channel_t *p, u8 *s);
u8 *Fvibup(channel_t *c, u8 *p);
u8 *Fvibdown(channel_t *c, u8 *p);
s32 func_80051174(struct channel_s *arg0, s32 arg1);
u8 *Flength(channel_t *p, u8 *ptr);
s32 func_800511B4(void *arg0, s32 arg1);
void *func_800511C4(void *arg0, u8 *arg1);
s32 func_800511D4(void *arg0, s32 arg1);
u8 *Fdistort(channel_t *p, u8 *ptr);
s32 func_8005123C(void *arg0, s32 arg1);
s32 func_8005124C(void *arg0, s32 arg1);
s32 func_80051258(void *arg0, s32 arg1);
s32 func_80051268(void *arg0, s32 arg1);
u8 *Ffor(channel_t *c, u8 *p);
s32 Fnext(channel_t *p, s32 ret);
u8 *Fwobble(channel_t *c, u8 *p);
s32 func_800513A4(void *arg0, s32 arg1);
s32 func_800513B0(void *arg0, s32 arg1);
s32 func_800513C0(void *arg0, s32 arg1);
void *func_800513CC(void *arg0, u8 *arg1);
void *func_800513E0(void *arg0, u8 *arg1);
s32 func_800513F4(s32 arg0, s32 arg1);
u8 *Fdrums(channel_t *p, u8 *ptr);
s32 func_80051444(void *arg0, s32 arg1);
s32 func_80051450(s32 arg0, s32 arg1);
s32 Fgoto(channel_t *p, u8 *ptr);
void *func_800514C8(void *arg0, u8 *arg1);
u8 *FrandNote(channel_t *p, u8 *ptr);
u8 *FrandVolume(channel_t *c, u8 *p);
u8 *FrandPan(channel_t *c, u8 *p);
void *func_800515C8(void *arg0, u8 *arg1);
u8 *Fstartfx(channel_t *obj, u8 *p);
void *Fbendrange(struct channel_s *arg0, u8 *arg1);
void *func_800516E4(void *arg0, u8 *arg1);
u8 *Fchangefx(void *unused, u8 *p);
u8 *Fmarker(channel_t *s, u8 *p);
s32 func_800517A8(void *arg0, s32 arg1);
void func_800517C0(s32 arg0, s32 arg1);
s32 func_80051A98(s16 idx);
void func_80051C30(s32 *param);
void func_80051F10(void *arg0);
s32 func_80051F28(void);
s32 func_80051F6C(void);
s32 ChangeCustomEffect(s32 idx);
void *func_80052010(s32 count, s32 size);
void func_80052184(void);
s32 *func_80052254(void);
void func_80052260(s32 a);
void func_80052460(s32 arg0);
void func_800524D0(void);
void func_80052748(void);
void func_800528F0(u8 *base, s32 len);
s32 func_8005296C(void);
s32 *func_80052984(void);
void func_80052990(s8 *arg0, s8 arg1, s32 arg2);
void func_800529BC(u8 *dst, u8 *src, s32 n);
void func_80053574(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_80053620(s32 a, s32 b, u32 c, s32 pct);
s32 func_800536C4(u8 *p);
s32 n_alLoadParam(void *filter, s32 paramID, void *param);
s32 n_alResampleParam(void);
s32 func_800552F0(void *filter, s32 id, void *param);
void n_alClose(void);
void func_80055C80(void);
void func_8005642C(void);
s32 func_800564C4(s32 arg0);
s32 func_80056514(s32 arg0);
s32 func_800565C0(s16 idx);
#endif
