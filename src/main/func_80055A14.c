/* if active, shuts down via func_80055C80 and clears the two state words */
extern s32 D_80077BC0;
extern s32 D_80077BC4;
void func_80055C80(void);
void func_80055A14(void) {
    if (D_80077BC0 != 0) {
        func_80055C80();
        D_80077BC0 = 0;
        D_80077BC4 = 0;
    }
}
