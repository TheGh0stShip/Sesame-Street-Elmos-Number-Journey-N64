/* returns D_80077F48 when D_80077F40 is set, else 0 */
extern s32 D_80077F40;
extern s32 D_80077F48;
s32 func_80064620(void)
{
    if (D_80077F40 == 0)
        return 0;
    return D_80077F48;
}
