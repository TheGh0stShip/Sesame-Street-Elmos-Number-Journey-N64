s32 func_8001EBF4();

s32 f(s32 arg0, s32 arg1) {
    s32 sp10;

    sp10 = func_8001EBF4(arg0, (*(s32 *)arg1), (*(s32 *)(arg1 + 4)), (*(s32 *)(arg1 + 8)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)arg1), (*(s32 *)(arg1 + 4)), (*(s32 *)(arg1 + 20)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)arg1), (*(s32 *)(arg1 + 16)), (*(s32 *)(arg1 + 8)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)arg1), (*(s32 *)(arg1 + 16)), (*(s32 *)(arg1 + 20)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)(arg1 + 12)), (*(s32 *)(arg1 + 4)), (*(s32 *)(arg1 + 8)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)(arg1 + 12)), (*(s32 *)(arg1 + 4)), (*(s32 *)(arg1 + 20)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)(arg1 + 12)), (*(s32 *)(arg1 + 16)), (*(s32 *)(arg1 + 8)));
    if (sp10 == 0) {
        return 1;
    }
    sp10 = sp10 & func_8001EBF4(arg0, (*(s32 *)(arg1 + 12)), (*(s32 *)(arg1 + 16)), (*(s32 *)(arg1 + 20)));
    if (sp10 == 0) {
        return 1;
    }
    return 0;
}
