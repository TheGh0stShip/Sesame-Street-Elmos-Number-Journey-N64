#include "common.h"


void func_800443A0(s32 arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11, f32 arg12, f32 arg13) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    s32 pad0;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s32 pad1;
    f32 sp30;
    f32 sp34;
    f32 sp38;
    s32 pad2;
    f32 sp40;
    f32 sp44;
    f32 sp48;
    s32 pad3;
    f32 sp50;
    f32 sp54;
    f32 sp58;
    s32 pad4;
    f32 sp60;
    f32 sp64;
    f32 sp68;
    s32 pad5;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    f32 sp88;
    f32 sp8C;
    f32 sp90;
    f32 sp94;
    f32 sp98;
    f32 sp9C;
    f32 spA0;
    s32 pad6;
    f32 spA8;
    f32 spAC;
    f32 spB0;
    s32 pad7;
    f32 spB8;
    f32 spBC;
    f32 spC0;
    s32 pad8;
    f32 spC8;
    f32 spCC;
    f32 spD0;
    s32 pad9;
    f32 spD8;
    f32 spDC;
    f32 spE0;
    s32 pad10;
    f32 spE8;
    f32 spEC;
    f32 spF0;
    s32 pad11;
    f32 spF8;
    f32 spFC;
    f32 sp100;
    s32 pad12;
    f32 sp108;
    f32 sp10C;
    f32 sp110;

    sp10 = arg8 - arg5;
    sp14 = arg9 - arg6;
    sp18 = arg10 - arg7;
    sp40 = (sp18 * ((struct gd_4_f32 *)arg2)->v) - (sp14 * ((struct gd_8_f32 *)arg2)->v);
    sp44 = (sp10 * ((struct gd_8_f32 *)arg2)->v) - (sp18 * (*(f32 *)arg2));
    sp48 = (sp14 * (*(f32 *)arg2)) - (sp10 * ((struct gd_4_f32 *)arg2)->v);
    func_80044348(((s32)&sp40));
    sp70 = ((sp40 * arg5) + (sp44 * arg6)) + (sp48 * arg7);
    sp7C = (((sp40 * ((struct gd_10_f32 *)arg0)->v) + (sp44 * ((struct gd_14_f32 *)arg0)->v)) + (sp48 * ((struct gd_18_f32 *)arg0)->v)) - sp70;
    if (((*(f32 *)arg0) - sp7C) < 0.001f) {
        return;
    }
    sp20 = arg11 - arg8;
    sp24 = arg12 - arg9;
    sp28 = arg13 - arg10;
    sp50 = (sp28 * ((struct gd_4_f32 *)arg2)->v) - (sp24 * ((struct gd_8_f32 *)arg2)->v);
    sp54 = (sp20 * ((struct gd_8_f32 *)arg2)->v) - (sp28 * (*(f32 *)arg2));
    sp58 = (sp24 * (*(f32 *)arg2)) - (sp20 * ((struct gd_4_f32 *)arg2)->v);
    func_80044348(((s32)&sp50));
    sp74 = ((sp50 * arg8) + (sp54 * arg9)) + (sp58 * arg10);
    sp80 = (((sp50 * ((struct gd_10_f32 *)arg0)->v) + (sp54 * ((struct gd_14_f32 *)arg0)->v)) + (sp58 * ((struct gd_18_f32 *)arg0)->v)) - sp74;
    if (((*(f32 *)arg0) - sp80) < 0.001f) {
        return;
    }
    sp30 = arg5 - arg11;
    sp34 = arg6 - arg12;
    sp38 = arg7 - arg13;
    sp60 = (sp38 * ((struct gd_4_f32 *)arg2)->v) - (sp34 * ((struct gd_8_f32 *)arg2)->v);
    sp64 = (sp30 * ((struct gd_8_f32 *)arg2)->v) - (sp38 * (*(f32 *)arg2));
    sp68 = (sp34 * (*(f32 *)arg2)) - (sp30 * ((struct gd_4_f32 *)arg2)->v);
    func_80044348(((s32)&sp60));
    sp78 = ((sp60 * arg11) + (sp64 * arg12)) + (sp68 * arg13);
    sp84 = (((sp60 * ((struct gd_10_f32 *)arg0)->v) + (sp64 * ((struct gd_14_f32 *)arg0)->v)) + (sp68 * ((struct gd_18_f32 *)arg0)->v)) - sp78;
    if (((*(f32 *)arg0) - sp84) < 0.001f) {
        return;
    }
    spD8 = 0;
    spDC = 0;
    spE0 = 0;
    if (sp7C > 0.0f) {
        spD8 = sqrtf((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
        spE8 = sp10 / spD8;
        spEC = sp14 / spD8;
        spF0 = sp18 / spD8;
        spC8 = ((struct gd_10_f32 *)arg0)->v - arg5;
        spCC = ((struct gd_14_f32 *)arg0)->v - arg6;
        spD0 = ((struct gd_18_f32 *)arg0)->v - arg7;
        sp90 = ((spE8 * spC8) + (spEC * spCC)) + (spF0 * spD0);
        if (sp80 > 0.0f) {
            if (sp90 <= spD8) {
                spA8 = arg5 + (spE8 * sp90);
                spAC = arg6 + (spEC * sp90);
                spB0 = arg7 + (spF0 * sp90);
            } else {
                spDC = sqrtf((((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28)));
                spF8 = sp20 / spDC;
                spFC = sp24 / spDC;
                sp100 = sp28 / spDC;
                spC8 = ((struct gd_10_f32 *)arg0)->v - arg8;
                spCC = ((struct gd_14_f32 *)arg0)->v - arg9;
                spD0 = ((struct gd_18_f32 *)arg0)->v - arg10;
                sp90 = ((spF8 * spC8) + (spFC * spCC)) + (sp100 * spD0);
                if (sp90 <= 0.0f) {
                    spA8 = arg8;
                    spAC = arg9;
                    spB0 = arg10;
                } else {
                    spA8 = arg8 + (spF8 * sp90);
                    spAC = arg9 + (spFC * sp90);
                    spB0 = arg10 + (sp100 * sp90);
                }
            }
        } else {
            if (sp84 > 0.0f) {
                if (sp90 >= 0.0f) {
                    spA8 = arg5 + (spE8 * sp90);
                    spAC = arg6 + (spEC * sp90);
                    spB0 = arg7 + (spF0 * sp90);
                } else {
                    spE0 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
                    sp108 = sp30 / spE0;
                    sp10C = sp34 / spE0;
                    sp110 = sp38 / spE0;
                    spC8 = ((struct gd_10_f32 *)arg0)->v - arg11;
                    spCC = ((struct gd_14_f32 *)arg0)->v - arg12;
                    spD0 = ((struct gd_18_f32 *)arg0)->v - arg13;
                    sp90 = ((sp108 * spC8) + (sp10C * spCC)) + (sp110 * spD0);
                    if (sp90 >= spE0) {
                        spA8 = arg5;
                        spAC = arg6;
                        spB0 = arg7;
                    } else {
                        spA8 = arg11 + (sp108 * sp90);
                        spAC = arg12 + (sp10C * sp90);
                        spB0 = arg13 + (sp110 * sp90);
                    }
                }
            } else {
                if (sp90 <= 0.0f) {
                    spA8 = arg5;
                    spAC = arg6;
                    spB0 = arg7;
                } else {
                    if (sp90 >= spD8) {
                        spA8 = arg8;
                        spAC = arg9;
                        spB0 = arg10;
                    } else {
                        spA8 = arg5 + (spE8 * sp90);
                        spAC = arg6 + (spEC * sp90);
                        spB0 = arg7 + (spF0 * sp90);
                    }
                }
            }
        }
    } else {
        if (sp80 > 0.0f) {
            spDC = sqrtf((((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28)));
            spF8 = sp20 / spDC;
            spFC = sp24 / spDC;
            sp100 = sp28 / spDC;
            spC8 = ((struct gd_10_f32 *)arg0)->v - arg8;
            spCC = ((struct gd_14_f32 *)arg0)->v - arg9;
            spD0 = ((struct gd_18_f32 *)arg0)->v - arg10;
            sp90 = ((spF8 * spC8) + (spFC * spCC)) + (sp100 * spD0);
            if (sp84 > 0.0f) {
                if (sp90 <= spDC) {
                    spA8 = arg8 + (spF8 * sp90);
                    spAC = arg9 + (spFC * sp90);
                    spB0 = arg10 + (sp100 * sp90);
                } else {
                    spE0 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
                    sp108 = sp30 / spE0;
                    sp10C = sp34 / spE0;
                    sp110 = sp38 / spE0;
                    spC8 = ((struct gd_10_f32 *)arg0)->v - arg11;
                    spCC = ((struct gd_14_f32 *)arg0)->v - arg12;
                    spD0 = ((struct gd_18_f32 *)arg0)->v - arg13;
                    sp90 = ((sp108 * spC8) + (sp10C * spCC)) + (sp110 * spD0);
                    if (sp90 <= 0.0f) {
                        spA8 = arg11;
                        spAC = arg12;
                        spB0 = arg13;
                    } else {
                        spA8 = arg11 + (sp108 * sp90);
                        spAC = arg12 + (sp10C * sp90);
                        spB0 = arg13 + (sp110 * sp90);
                    }
                }
            } else {
                if (sp90 <= 0.0f) {
                    spA8 = arg8;
                    spAC = arg9;
                    spB0 = arg10;
                } else {
                    if (sp90 >= spDC) {
                        spA8 = arg11;
                        spAC = arg12;
                        spB0 = arg13;
                    } else {
                        spA8 = arg8 + (spF8 * sp90);
                        spAC = arg9 + (spFC * sp90);
                        spB0 = arg10 + (sp100 * sp90);
                    }
                }
            }
        } else {
            if (sp84 > 0.0f) {
                spE0 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
                sp108 = sp30 / spE0;
                sp10C = sp34 / spE0;
                sp110 = sp38 / spE0;
                spC8 = ((struct gd_10_f32 *)arg0)->v - arg11;
                spCC = ((struct gd_14_f32 *)arg0)->v - arg12;
                spD0 = ((struct gd_18_f32 *)arg0)->v - arg13;
                sp90 = ((sp108 * spC8) + (sp10C * spCC)) + (sp110 * spD0);
                if (sp90 <= 0.0f) {
                    spA8 = arg11;
                    spAC = arg12;
                    spB0 = arg13;
                } else {
                    if (sp90 >= spE0) {
                        spA8 = arg5;
                        spAC = arg6;
                        spB0 = arg7;
                    } else {
                        spA8 = arg11 + (sp108 * sp90);
                        spAC = arg12 + (sp10C * sp90);
                        spB0 = arg13 + (sp110 * sp90);
                    }
                }
            } else {
                spA8 = ((struct gd_10_f32 *)arg0)->v - (arg3 * (*(f32 *)arg2));
                spAC = ((struct gd_14_f32 *)arg0)->v - (arg3 * ((struct gd_4_f32 *)arg2)->v);
                spB0 = ((struct gd_18_f32 *)arg0)->v - (arg3 * ((struct gd_8_f32 *)arg2)->v);
            }
        }
    }
    spC8 = spA8 - ((struct gd_10_f32 *)arg0)->v;
    spCC = spAC - ((struct gd_14_f32 *)arg0)->v;
    spD0 = spB0 - ((struct gd_18_f32 *)arg0)->v;
    sp88 = sqrtf((((spC8 * spC8) + (spCC * spCC)) + (spD0 * spD0)));
    if (((*(f32 *)arg0) - sp88) < 0.001f) {
        return;
    }
    if ((((struct gd_2C_s32 *)arg0)->v & 1) != 0) {
        ((struct gd_34_s32 *)arg0)->v = *(s32 *)arg1;
        ((struct gd_38_s32 *)arg0)->v = 0;
        ((struct gd_3C_s32 *)arg0)->v = arg1;
        return;
    }
    sp7C = (((sp40 * ((struct gd_4_f32 *)arg0)->v) + (sp44 * ((struct gd_8_f32 *)arg0)->v)) + (sp48 * ((struct gd_C_f32 *)arg0)->v)) - sp70;
    sp80 = (((sp50 * ((struct gd_4_f32 *)arg0)->v) + (sp54 * ((struct gd_8_f32 *)arg0)->v)) + (sp58 * ((struct gd_C_f32 *)arg0)->v)) - sp74;
    sp84 = (((sp60 * ((struct gd_4_f32 *)arg0)->v) + (sp64 * ((struct gd_8_f32 *)arg0)->v)) + (sp68 * ((struct gd_C_f32 *)arg0)->v)) - sp78;
    if (sp7C > 0.0f) {
        if (spD8 == 0.0f) {
            spD8 = sqrtf((((sp10 * sp10) + (sp14 * sp14)) + (sp18 * sp18)));
            spE8 = sp10 / spD8;
            spEC = sp14 / spD8;
            spF0 = sp18 / spD8;
        }
        spC8 = ((struct gd_4_f32 *)arg0)->v - arg5;
        spCC = ((struct gd_8_f32 *)arg0)->v - arg6;
        spD0 = ((struct gd_C_f32 *)arg0)->v - arg7;
        sp90 = ((spE8 * spC8) + (spEC * spCC)) + (spF0 * spD0);
        if (sp80 > 0.0f) {
            if (sp90 <= spD8) {
                spB8 = arg5 + (spE8 * sp90);
                spBC = arg6 + (spEC * sp90);
                spC0 = arg7 + (spF0 * sp90);
            } else {
                if (spDC == 0.0f) {
                    spDC = sqrtf((((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28)));
                    spF8 = sp20 / spDC;
                    spFC = sp24 / spDC;
                    sp100 = sp28 / spDC;
                }
                spC8 = ((struct gd_4_f32 *)arg0)->v - arg8;
                spCC = ((struct gd_8_f32 *)arg0)->v - arg9;
                spD0 = ((struct gd_C_f32 *)arg0)->v - arg10;
                sp90 = ((spF8 * spC8) + (spFC * spCC)) + (sp100 * spD0);
                if (sp90 <= 0.0f) {
                    spB8 = arg8;
                    spBC = arg9;
                    spC0 = arg10;
                } else {
                    spB8 = arg8 + (spF8 * sp90);
                    spBC = arg9 + (spFC * sp90);
                    spC0 = arg10 + (sp100 * sp90);
                }
            }
        } else {
            if (sp84 > 0.0f) {
                if (sp90 >= 0.0f) {
                    spB8 = arg5 + (spE8 * sp90);
                    spBC = arg6 + (spEC * sp90);
                    spC0 = arg7 + (spF0 * sp90);
                } else {
                    if (spE0 == 0.0f) {
                        spE0 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
                        sp108 = sp30 / spE0;
                        sp10C = sp34 / spE0;
                        sp110 = sp38 / spE0;
                    }
                    spC8 = ((struct gd_4_f32 *)arg0)->v - arg11;
                    spCC = ((struct gd_8_f32 *)arg0)->v - arg12;
                    spD0 = ((struct gd_C_f32 *)arg0)->v - arg13;
                    sp90 = ((sp108 * spC8) + (sp10C * spCC)) + (sp110 * spD0);
                    if (sp90 >= spE0) {
                        spB8 = arg5;
                        spBC = arg6;
                        spC0 = arg7;
                    } else {
                        spB8 = arg11 + (sp108 * sp90);
                        spBC = arg12 + (sp10C * sp90);
                        spC0 = arg13 + (sp110 * sp90);
                    }
                }
            } else {
                if (sp90 <= 0.0f) {
                    spB8 = arg5;
                    spBC = arg6;
                    spC0 = arg7;
                } else {
                    if (sp90 >= spD8) {
                        spB8 = arg8;
                        spBC = arg9;
                        spC0 = arg10;
                    } else {
                        spB8 = arg5 + (spE8 * sp90);
                        spBC = arg6 + (spEC * sp90);
                        spC0 = arg7 + (spF0 * sp90);
                    }
                }
            }
        }
    } else {
        if (sp80 > 0.0f) {
            if (spDC == 0.0f) {
                spDC = sqrtf((((sp20 * sp20) + (sp24 * sp24)) + (sp28 * sp28)));
                spF8 = sp20 / spDC;
                spFC = sp24 / spDC;
                sp100 = sp28 / spDC;
            }
            spC8 = ((struct gd_4_f32 *)arg0)->v - arg8;
            spCC = ((struct gd_8_f32 *)arg0)->v - arg9;
            spD0 = ((struct gd_C_f32 *)arg0)->v - arg10;
            sp90 = ((spF8 * spC8) + (spFC * spCC)) + (sp100 * spD0);
            if (sp84 > 0.0f) {
                if (sp90 <= spDC) {
                    spB8 = arg8 + (spF8 * sp90);
                    spBC = arg9 + (spFC * sp90);
                    spC0 = arg10 + (sp100 * sp90);
                } else {
                    if (spE0 == 0.0f) {
                        spE0 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
                        sp108 = sp30 / spE0;
                        sp10C = sp34 / spE0;
                        sp110 = sp38 / spE0;
                    }
                    spC8 = ((struct gd_4_f32 *)arg0)->v - arg11;
                    spCC = ((struct gd_8_f32 *)arg0)->v - arg12;
                    spD0 = ((struct gd_C_f32 *)arg0)->v - arg13;
                    sp90 = ((sp108 * spC8) + (sp10C * spCC)) + (sp110 * spD0);
                    if (sp90 <= 0.0f) {
                        spB8 = arg11;
                        spBC = arg12;
                        spC0 = arg13;
                    } else {
                        spB8 = arg11 + (sp108 * sp90);
                        spBC = arg12 + (sp10C * sp90);
                        spC0 = arg13 + (sp110 * sp90);
                    }
                }
            } else {
                if (sp90 <= 0.0f) {
                    spB8 = arg8;
                    spBC = arg9;
                    spC0 = arg10;
                } else {
                    if (sp90 >= spDC) {
                        spB8 = arg11;
                        spBC = arg12;
                        spC0 = arg13;
                    } else {
                        spB8 = arg8 + (spF8 * sp90);
                        spBC = arg9 + (spFC * sp90);
                        spC0 = arg10 + (sp100 * sp90);
                    }
                }
            }
        } else {
            if (sp84 > 0.0f) {
                if (spE0 == 0.0f) {
                    spE0 = sqrtf((((sp30 * sp30) + (sp34 * sp34)) + (sp38 * sp38)));
                    sp108 = sp30 / spE0;
                    sp10C = sp34 / spE0;
                    sp110 = sp38 / spE0;
                }
                spC8 = ((struct gd_4_f32 *)arg0)->v - arg11;
                spCC = ((struct gd_8_f32 *)arg0)->v - arg12;
                spD0 = ((struct gd_C_f32 *)arg0)->v - arg13;
                sp90 = ((sp108 * spC8) + (sp10C * spCC)) + (sp110 * spD0);
                if (sp90 <= 0.0f) {
                    spB8 = arg11;
                    spBC = arg12;
                    spC0 = arg13;
                } else {
                    if (sp90 >= spE0) {
                        spB8 = arg5;
                        spBC = arg6;
                        spC0 = arg7;
                    } else {
                        spB8 = arg11 + (sp108 * sp90);
                        spBC = arg12 + (sp10C * sp90);
                        spC0 = arg13 + (sp110 * sp90);
                    }
                }
            } else {
                spB8 = ((struct gd_4_f32 *)arg0)->v - (arg4 * (*(f32 *)arg2));
                spBC = ((struct gd_8_f32 *)arg0)->v - (arg4 * ((struct gd_4_f32 *)arg2)->v);
                spC0 = ((struct gd_C_f32 *)arg0)->v - (arg4 * ((struct gd_8_f32 *)arg2)->v);
            }
        }
    }
    spC8 = spB8 - ((struct gd_4_f32 *)arg0)->v;
    spCC = spBC - ((struct gd_8_f32 *)arg0)->v;
    spD0 = spC0 - ((struct gd_C_f32 *)arg0)->v;
    sp8C = sqrtf((((spC8 * spC8) + (spCC * spCC)) + (spD0 * spD0)));
    sp94 = sp8C - (*(f32 *)arg0);
    sp98 = (*(f32 *)arg0) - sp88;
    sp9C = sp94 + sp98;
    if (sp9C == 0.0f) {
        return;
    }
    spA0 = sp94 / sp9C;
    spA0 = spA0 * ((struct gd_28_f32 *)arg0)->v;
    if (spA0 < 0.0f) {
        spA0 = 0;
    }
    if (spA0 < ((struct gd_28_f32 *)arg0)->v) {
        if (spA0 < ((struct gd_38_f32 *)arg0)->v) {
            ((struct gd_34_s32 *)arg0)->v = *(s32 *)arg1;
            ((struct gd_38_f32 *)arg0)->v = spA0;
            ((struct gd_3C_s32 *)arg0)->v = arg1;
            ((struct gd_40_f32 *)arg0)->v = spA8;
            ((struct gd_44_f32 *)arg0)->v = spAC;
            ((struct gd_48_f32 *)arg0)->v = spB0;
            ((struct gd_4C_f32 *)arg0)->v = *(f32 *)arg2;
            ((struct gd_50_f32 *)arg0)->v = ((struct gd_4_f32 *)arg2)->v;
            ((struct gd_54_f32 *)arg0)->v = ((struct gd_8_f32 *)arg2)->v;
        }
    }
}
