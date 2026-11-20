/* alInit: set the audio globals once and create the synthesizer */
typedef struct { s32 unk_0; } ALGlobals;
extern ALGlobals *D_80077BC0;  /* alGlobals */
extern void *D_80077BC4;       /* alSynth */
void func_80056010(void *c);   /* alSynNew */
void func_800559D0(ALGlobals *g, void *c)
{
    if (D_80077BC0 == NULL) {
        D_80077BC0 = g;
        if (D_80077BC4 == NULL) {
            D_80077BC4 = g;
            func_80056010(c);
        }
    }
}
