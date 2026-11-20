struct gd_1_u8 { u8 pad[0x1]; u8 v; };
struct gd_2_s16 { u8 pad[0x2]; s16 v; };
struct gd_2_u8 { u8 pad[0x2]; u8 v; };
struct gd_4_f32 { u8 pad[0x4]; f32 v; };
struct gd_4_s16 { u8 pad[0x4]; s16 v; };
struct gd_8_f32 { u8 pad[0x8]; f32 v; };
struct gd_C_f32 { u8 pad[0xC]; f32 v; };
struct gd_10_f32 { u8 pad[0x10]; f32 v; };
struct gd_14_f32 { u8 pad[0x14]; f32 v; };
struct gd_14_s32 { u8 pad[0x14]; s32 v; };
struct gd_18_f32 { u8 pad[0x18]; f32 v; };
struct gd_18_s32 { u8 pad[0x18]; s32 v; };
struct gd_1C_f32 { u8 pad[0x1C]; f32 v; };
struct gd_1C_s32 { u8 pad[0x1C]; s32 v; };
struct gd_20_f32 { u8 pad[0x20]; f32 v; };
struct gd_24_f32 { u8 pad[0x24]; f32 v; };
struct gd_28_f32 { u8 pad[0x28]; f32 v; };
struct gd_2C_s32 { u8 pad[0x2C]; s32 v; };
struct gd_34_s32 { u8 pad[0x34]; s32 v; };
struct gd_38_f32 { u8 pad[0x38]; f32 v; };
struct gd_38_s32 { u8 pad[0x38]; s32 v; };
struct gd_3C_s32 { u8 pad[0x3C]; s32 v; };
struct gd_40_f32 { u8 pad[0x40]; f32 v; };
struct gd_44_f32 { u8 pad[0x44]; f32 v; };
struct gd_48_f32 { u8 pad[0x48]; f32 v; };
struct gd_4C_f32 { u8 pad[0x4C]; f32 v; };
struct gd_50_f32 { u8 pad[0x50]; f32 v; };
struct gd_54_f32 { u8 pad[0x54]; f32 v; };
f32 func_80056690(f32);
f64 func_8006A1F0();

void f(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp10;
    s32 sp14;
    s32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    f32 sp2C;
    f32 sp30;
    f32 sp34;
    f32 sp38;
    f32 sp3C;
    f32 sp40;
    f32 sp44;
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    f32 sp54;
    f32 sp58;
    f32 sp5C;
    f32 sp60;
    f32 sp64;
    f32 sp68;
    f32 sp6C;
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    f32 sp80;
    f32 sp84;
    f32 sp88;
    f32 sp8C;
    f32 sp90;
    s32 pad0;
    f32 sp98;
    f32 sp9C;
    f32 spA0;
    s32 pad1;
    f32 spA8;
    f32 spAC;
    f32 spB0;
    s32 pad2;
    f32 spB8;
    f32 spBC;
    f32 spC0;
    s32 pad3;
    f32 spC8;
    f32 spCC;
    f32 spD0;
    s32 pad4;
    f32 spD8;
    f32 spDC;
    f32 spE0;
    s32 pad5;
    f32 spE8;
    f32 spEC;
    f32 spF0;
    s32 pad6;
    f32 spF8;
    f32 spFC;
    f32 sp100;
    s32 pad7;
    f32 sp108;
    f32 sp10C;
    f32 sp110;
    s32 pad8;
    f32 sp118;
    f32 sp11C;
    f32 sp120;
    s32 pad9;
    f32 sp128;
    f32 sp12C;
    f32 sp130;
    s32 pad10;
    f32 sp138;
    f32 sp13C;
    f32 sp140;
    s32 pad11;
    f32 sp148;
    f32 sp14C;
    f32 sp150;
    s32 pad12;
    f32 sp158;
    f32 sp15C;
    f32 sp160;
    s32 pad13;
    f32 sp168;
    f32 sp16C;
    f32 sp170;
    s32 pad14;
    f32 sp178;
    f32 sp17C;
    f32 sp180;
    s32 pad15;
    f32 sp188;
    f32 sp18C;
    f32 sp190;

    sp14 = ((struct gd_14_s32 *)arg2)->v;
    sp148 = ((struct gd_1C_f32 *)arg0)->v;
    sp14C = ((struct gd_24_f32 *)arg0)->v;
    sp150 = -((struct gd_20_f32 *)arg0)->v;
    sp158 = ((struct gd_10_f32 *)arg0)->v;
    sp15C = ((struct gd_18_f32 *)arg0)->v;
    sp160 = -((struct gd_14_f32 *)arg0)->v;
    sp168 = ((struct gd_4_f32 *)arg0)->v;
    sp16C = ((struct gd_C_f32 *)arg0)->v;
    sp170 = -((struct gd_8_f32 *)arg0)->v;
    sp18 = 0;
    while (!(sp18 >= ((struct gd_18_s32 *)arg2)->v)) {
        sp128 = 0;
        sp12C = 0;
        sp130 = 0;
        sp10 = ((struct gd_1C_s32 *)arg2)->v + (sp18 * 4);
        sp1C = (f32)(*(s16 *)(((*(u8 *)sp10) * 16) + sp14));
        sp20 = (f32)((struct gd_2_s16 *)(((*(u8 *)sp10) * 16) + sp14))->v;
        sp24 = (f32)((struct gd_4_s16 *)(((*(u8 *)sp10) * 16) + sp14))->v;
        sp28 = (f32)(*(s16 *)((((struct gd_1_u8 *)sp10)->v * 16) + sp14));
        sp2C = (f32)((struct gd_2_s16 *)((((struct gd_1_u8 *)sp10)->v * 16) + sp14))->v;
        sp30 = (f32)((struct gd_4_s16 *)((((struct gd_1_u8 *)sp10)->v * 16) + sp14))->v;
        sp34 = (f32)(*(s16 *)((((struct gd_2_u8 *)sp10)->v * 16) + sp14));
        sp38 = (f32)((struct gd_2_s16 *)((((struct gd_2_u8 *)sp10)->v * 16) + sp14))->v;
        sp3C = (f32)((struct gd_4_s16 *)((((struct gd_2_u8 *)sp10)->v * 16) + sp14))->v;
        sp98 = sp28 - sp1C;
        sp9C = sp2C - sp20;
        spA0 = sp30 - sp24;
        spB8 = sp1C - sp34;
        spBC = sp20 - sp38;
        spC0 = sp24 - sp3C;
        spD8 = (spA0 * spBC) - (sp9C * spC0);
        spDC = (sp98 * spC0) - (spA0 * spB8);
        spE0 = (sp9C * spB8) - (sp98 * spBC);
        sp40 = ((spD8 * sp148) + (spDC * sp14C)) + (spE0 * sp150);
        if (sp40 >= 0.0f) {
            goto L2355;
        }
        sp44 = ((spD8 * spD8) + (spDC * spDC)) + (spE0 * spE0);
        if (sp44 > 1e-05f) {
            sp44 = func_80056690(sp44);
            spD8 = spD8 / sp44;
            spDC = spDC / sp44;
            spE0 = spE0 / sp44;
        } else {
            goto L2355;
        }
        sp68 = ((spD8 * sp1C) + (spDC * sp20)) + (spE0 * sp24);
        sp48 = (((spD8 * sp158) + (spDC * sp15C)) + (spE0 * sp160)) - sp68;
        if (((*(f32 *)arg0) - sp48) < 0.001f) {
            goto L2355;
        }
        sp4C = (((spD8 * sp168) + (spDC * sp16C)) + (spE0 * sp170)) - sp68;
        if (sp4C < 1e-05f) {
            goto L2355;
        }
        spE8 = (sp9C * spE0) - (spA0 * spDC);
        spEC = (spA0 * spD8) - (sp98 * spE0);
        spF0 = (sp98 * spDC) - (sp9C * spD8);
        sp44 = ((spE8 * spE8) + (spEC * spEC)) + (spF0 * spF0);
        if (sp44 > 1e-05f) {
            sp44 = func_80056690(sp44);
            spE8 = spE8 / sp44;
            spEC = spEC / sp44;
            spF0 = spF0 / sp44;
        } else {
            goto L2355;
        }
        sp6C = ((spE8 * sp1C) + (spEC * sp20)) + (spF0 * sp24);
        sp78 = (((spE8 * sp158) + (spEC * sp15C)) + (spF0 * sp160)) - sp6C;
        if (((*(f32 *)arg0) - sp78) < 0.001f) {
            goto L2355;
        }
        spA8 = sp34 - sp28;
        spAC = sp38 - sp2C;
        spB0 = sp3C - sp30;
        spF8 = (spAC * spE0) - (spB0 * spDC);
        spFC = (spB0 * spD8) - (spA8 * spE0);
        sp100 = (spA8 * spDC) - (spAC * spD8);
        sp44 = ((spF8 * spF8) + (spFC * spFC)) + (sp100 * sp100);
        if (sp44 > 1e-05f) {
            sp44 = func_80056690(sp44);
            spF8 = spF8 / sp44;
            spFC = spFC / sp44;
            sp100 = sp100 / sp44;
        } else {
            goto L2355;
        }
        sp70 = ((spF8 * sp28) + (spFC * sp2C)) + (sp100 * sp30);
        sp7C = (((spF8 * sp158) + (spFC * sp15C)) + (sp100 * sp160)) - sp70;
        if (((*(f32 *)arg0) - sp7C) < 0.001f) {
            goto L2355;
        }
        sp108 = (spBC * spE0) - (spC0 * spDC);
        sp10C = (spC0 * spD8) - (spB8 * spE0);
        sp110 = (spB8 * spDC) - (spBC * spD8);
        sp44 = ((sp108 * sp108) + (sp10C * sp10C)) + (sp110 * sp110);
        if (sp44 > 1e-05f) {
            sp44 = func_80056690(sp44);
            sp108 = sp108 / sp44;
            sp10C = sp10C / sp44;
            sp110 = sp110 / sp44;
        } else {
            goto L2355;
        }
        sp74 = ((sp108 * sp34) + (sp10C * sp38)) + (sp110 * sp3C);
        sp80 = (((sp108 * sp158) + (sp10C * sp15C)) + (sp110 * sp160)) - sp74;
        if (((*(f32 *)arg0) - sp80) < 0.001f) {
            goto L2355;
        }
        sp84 = 0;
        sp88 = 0;
        sp8C = 0;
        if (sp78 > 1e-05f) {
            sp84 = ((sp98 * sp98) + (sp9C * sp9C)) + (spA0 * spA0);
            if (sp84 > 1e-05f) {
                sp84 = func_80056690(sp84);
                sp118 = spE8 / sp84;
                sp11C = spEC / sp84;
                sp120 = spF0 / sp84;
            } else {
                goto L2355;
            }
            spC8 = sp158 - sp1C;
            spCC = sp15C - sp20;
            spD0 = sp160 - sp24;
            sp90 = ((spE8 * spC8) + (spEC * spCC)) + (spF0 * spD0);
            if (sp7C > 1e-05f) {
                if (sp90 <= sp84) {
                    sp178 = sp1C + sp90;
                    sp17C = sp20 + sp90;
                    sp180 = sp24 + sp90;
                } else {
                    sp88 = ((spA8 * spA8) + (spAC * spAC)) + (spB0 * spB0);
                    if (sp88 > 1e-05f) {
                        sp88 = (f32)func_8006A1F0(((f64)sp88));
                        sp128 = spA8 / sp88;
                        sp12C = spAC / sp88;
                        sp130 = spB0 / sp88;
                    } else {
                        goto L2355;
                    }
                    spC8 = sp158 - sp28;
                    spCC = sp15C - sp2C;
                    spD0 = sp160 - sp30;
                    sp90 = ((sp128 * spC8) + (sp130 * spD0)) + (sp130 * spD0);
                    if (sp90 < 1e-05f) {
                        sp178 = sp28;
                        sp17C = sp2C;
                        sp180 = sp30;
                    } else {
                        sp178 = sp28 + (sp128 * sp90);
                        sp17C = sp2C + (sp12C * sp90);
                        sp180 = sp30 + (sp130 * sp90);
                    }
                }
            } else {
                if (sp80 > 1e-05f) {
                    if (sp90 > 1e-05f) {
                        sp178 = sp1C + (sp118 * sp90);
                        sp17C = sp20 + (sp11C * sp90);
                        sp180 = sp24 + (sp120 * sp90);
                    } else {
                        sp8C = ((spB8 * spB8) + (spBC * spBC)) + (spC0 * spC0);
                        if (sp8C > 1e-05f) {
                            sp8C = (f32)func_8006A1F0(((f64)sp8C));
                            sp138 = spB8 / sp8C;
                            sp13C = spBC / sp8C;
                            sp140 = spC0 / sp8C;
                        } else {
                            goto L2355;
                        }
                        spC8 = sp158 - sp34;
                        spCC = sp15C - sp38;
                        spD0 = sp160 - sp3C;
                        sp90 = ((sp138 * spC8) + (sp13C * spCC)) + (sp140 * spD0);
                        if (sp90 >= sp8C) {
                            sp178 = sp1C;
                            sp17C = sp20;
                            sp180 = sp24;
                        } else {
                            sp178 = sp34 + (sp138 * sp90);
                            sp17C = sp38 + (sp13C * sp90);
                            sp180 = sp3C + (sp140 * sp90);
                        }
                    }
                } else {
                    if (sp90 <= 0.0f) {
                        sp178 = sp1C;
                        sp17C = sp20;
                        sp180 = sp24;
                    } else {
                        if (sp90 >= sp84) {
                            sp178 = sp28;
                            sp17C = sp2C;
                            sp180 = sp30;
                        } else {
                            sp178 = sp1C + (sp118 * sp90);
                            sp17C = sp20 + (sp11C * sp90);
                            sp180 = sp24 + (sp120 * sp90);
                        }
                    }
                }
            }
        } else {
            if (sp7C > 1e-05f) {
                sp88 = ((spA8 * spA8) + (spAC * spAC)) + (spB0 * spB0);
                if (sp88 > 1e-05f) {
                    sp88 = (f32)func_8006A1F0(((f64)sp88));
                    sp128 = spA8 / sp88;
                    sp12C = spAC / sp88;
                    sp130 = spB0 / sp88;
                } else {
                    goto L2355;
                }
                spC8 = sp158 - sp28;
                spCC = sp15C - sp2C;
                spD0 = sp160 - sp30;
                sp90 = ((sp128 * spC8) + (sp12C * spCC)) + (sp130 * spD0);
                if (sp80 > 1e-05f) {
                    if (sp90 <= sp88) {
                        sp178 = sp28 + (sp128 * sp90);
                        sp17C = sp2C + (sp12C * sp90);
                        sp180 = sp30 + (sp130 * sp90);
                    } else {
                        sp8C = ((spB8 * spB8) + (spBC * spBC)) + (spC0 * spC0);
                        if (sp8C > 1e-05f) {
                            sp8C = (f32)func_8006A1F0(((f64)sp8C));
                            sp138 = spB8 / sp8C;
                            sp13C = spBC / sp8C;
                            sp140 = spC0 / sp8C;
                        }
                        spC8 = sp158 - sp34;
                        spCC = sp15C - sp38;
                        spD0 = sp160 - sp3C;
                        sp90 = ((sp138 * spC8) + (sp13C * spCC)) + (sp140 * spD0);
                        if (sp90 < 1e-05f) {
                            sp178 = sp34;
                            sp17C = sp38;
                            sp180 = sp3C;
                        } else {
                            sp178 = sp34 + (sp138 * sp90);
                            sp17C = sp38 + (sp13C * sp90);
                            sp180 = sp3C + (sp140 * sp90);
                        }
                    }
                } else {
                    if (sp90 <= 0.0f) {
                        sp178 = sp28;
                        sp17C = sp2C;
                        sp180 = sp30;
                    } else {
                        if (sp90 >= sp88) {
                            sp178 = sp34;
                            sp17C = sp38;
                            sp180 = sp3C;
                        } else {
                            sp178 = sp28 + (sp128 * sp90);
                            sp17C = sp2C + (sp12C * sp90);
                            sp180 = sp30 + (sp130 * sp90);
                        }
                    }
                }
            } else {
                if (sp80 > 1e-05f) {
                    sp8C = ((spB8 * spB8) + (spBC * spBC)) + (spC0 * spC0);
                    if (sp8C > 1e-05f) {
                        sp8C = (f32)func_8006A1F0(((f64)sp8C));
                        sp138 = spB8 / sp8C;
                        sp13C = spBC / sp8C;
                        sp140 = spC0 / sp8C;
                    } else {
                        goto L2355;
                    }
                    spC8 = sp158 - sp34;
                    spCC = sp15C - sp38;
                    spD0 = sp160 - sp3C;
                    sp90 = ((sp138 * spC8) + (sp13C * spCC)) + (sp140 * spD0);
                    if (sp90 <= 0.0f) {
                        sp178 = sp34;
                        sp17C = sp38;
                        sp180 = sp3C;
                    } else {
                        if (sp90 >= sp8C) {
                            sp178 = sp1C;
                            sp17C = sp20;
                            sp180 = sp24;
                        } else {
                            sp178 = sp34 + sp90;
                            sp17C = sp38 + sp90;
                            sp180 = sp3C + sp90;
                        }
                    }
                } else {
                    sp178 = sp158 - (sp48 * spD8);
                    sp17C = sp15C - (sp48 * spDC);
                    sp180 = sp160 - (sp48 * spE0);
                }
            }
        }
        spC8 = sp178 - sp158;
        spCC = sp17C - sp15C;
        spD0 = sp180 - sp160;
        sp50 = (f32)func_8006A1F0(((f64)(((spC8 * spC8) + (spCC * spCC)) + (spD0 * spD0))));
        if (((*(f32 *)arg0) - sp50) < 0.001f) {
            goto L2355;
        }
        if ((((struct gd_2C_s32 *)arg0)->v & 1) != 0) {
            ((struct gd_34_s32 *)arg0)->v = *(s32 *)arg1;
            ((struct gd_38_s32 *)arg0)->v = 0;
            ((struct gd_3C_s32 *)arg0)->v = arg1;
            return;
        }
        sp78 = (((spE8 * sp168) + (spEC * sp16C)) + (spF0 * sp170)) - sp6C;
        sp7C = (((spF8 * sp168) + (spFC * sp16C)) + (sp100 * sp170)) - sp70;
        sp80 = (((sp108 * sp168) + (sp10C * sp16C)) + (sp110 * sp170)) - sp74;
        if (sp78 > 1e-05f) {
            if (sp84 == 0.0f) {
                sp84 = ((sp98 * sp98) + (sp9C * sp9C)) + (spA0 * spA0);
                if (sp84 > 1e-05f) {
                    sp84 = (f32)func_8006A1F0(((f64)sp84));
                    sp118 = sp98 / sp84;
                    sp11C = sp9C / sp84;
                    sp120 = spA0 / sp84;
                } else {
                    goto L2355;
                }
            }
            spC8 = sp168 - sp1C;
            spCC = sp16C - sp20;
            spD0 = sp170 - sp24;
            sp90 = ((sp118 * spC8) + (sp11C * spCC)) + (sp120 * spD0);
            if (sp7C > 1e-05f) {
                if (sp90 <= sp84) {
                    sp188 = sp1C + (sp118 * sp90);
                    sp18C = sp20 + (sp11C * sp90);
                    sp190 = sp24 + (sp120 * sp90);
                } else {
                    if (sp88 == 0.0f) {
                        sp88 = ((spA8 * spA8) + (spAC * spAC)) + (spB0 * spB0);
                        if (sp88 > 1e-05f) {
                            sp88 = (f32)func_8006A1F0(((f64)sp88));
                            sp128 = spA8 / sp88;
                            sp12C = spAC / sp88;
                            sp130 = spB0 / sp88;
                        } else {
                            goto L2355;
                        }
                    }
                    spC8 = sp168 - sp28;
                    spCC = sp16C - sp2C;
                    spD0 = sp170 - sp30;
                    sp90 = ((sp128 * spC8) + (sp12C * spCC)) + (sp130 * spD0);
                    if (sp90 <= 0.0f) {
                        sp188 = sp28;
                        sp18C = sp2C;
                        sp190 = sp30;
                    } else {
                        sp188 = sp28 + (sp128 * sp90);
                        sp18C = sp2C + (sp12C * sp90);
                        sp190 = sp30 + (sp130 * sp90);
                    }
                }
            } else {
                if (sp80 > 1e-05f) {
                    if (sp90 >= 1e-05f) {
                        sp188 = sp1C + (sp118 * sp90);
                        sp18C = sp20 + (sp11C * sp90);
                        sp190 = sp24 + (sp120 * sp90);
                    } else {
                        if (sp8C == 0.0f) {
                            sp8C = ((spB8 * spB8) + (spBC * spBC)) + (spC0 * spC0);
                            if (sp8C > 1e-05f) {
                                sp8C = (f32)func_8006A1F0(((f64)sp8C));
                                sp138 = spB8 / sp8C;
                                sp13C = spBC / sp8C;
                                sp140 = spC0 / sp8C;
                            } else {
                                goto L2355;
                            }
                        }
                        spC8 = sp168 - sp34;
                        spCC = sp16C - sp38;
                        spD0 = sp170 - sp3C;
                        sp90 = ((sp138 * spC8) + (sp13C * spCC)) + (sp140 * spD0);
                        if (sp90 >= sp8C) {
                            sp188 = sp1C;
                            sp18C = sp20;
                            sp190 = sp24;
                        } else {
                            sp188 = sp34 + (sp138 * sp90);
                            sp18C = sp38 + (sp13C * sp90);
                            sp190 = sp3C + (sp140 * sp90);
                        }
                    }
                } else {
                    if (sp90 <= 0.0f) {
                        sp188 = sp1C;
                        sp18C = sp20;
                        sp190 = sp24;
                    } else {
                        if (sp90 >= sp84) {
                            sp188 = sp28;
                            sp18C = sp2C;
                            sp190 = sp30;
                        } else {
                            sp188 = sp1C + (sp118 * sp90);
                            sp18C = sp20 + (sp11C * sp90);
                            sp190 = sp24 + (sp120 * sp90);
                        }
                    }
                }
            }
        } else {
            if (sp7C > 1e-05f) {
                if (sp88 == 0.0f) {
                    sp88 = ((spA8 * spA8) + (spAC * spAC)) + (spB0 * spB0);
                    if (sp88 < 1e-05f) {
                        sp88 = (f32)func_8006A1F0(((f64)sp88));
                        sp128 = spA8 / sp88;
                        sp12C = spAC / sp88;
                        sp130 = spB0 / sp88;
                    }
                }
                spC8 = sp168 - sp28;
                spCC = sp16C - sp2C;
                spD0 = sp170 - sp30;
                sp90 = ((sp128 * spC8) + (sp12C * spCC)) + (sp130 * spD0);
                if (sp80 > 1e-05f) {
                    if (sp90 <= sp88) {
                        sp188 = sp28 + (sp128 * sp90);
                        sp18C = sp2C + (sp12C * sp90);
                        sp190 = sp30 + (sp130 * sp90);
                    } else {
                        if (sp8C == 0.0f) {
                            sp8C = ((spB8 * spB8) + (spBC * spBC)) + (spC0 * spC0);
                            if (sp8C > 1e-05f) {
                                sp8C = (f32)func_8006A1F0(((f64)sp8C));
                                sp138 = spB8 / sp8C;
                                sp13C = spBC / sp8C;
                                sp140 = spC0 / sp8C;
                            } else {
                                goto L2355;
                            }
                        }
                        spC8 = sp168 - sp34;
                        spCC = sp16C - sp38;
                        spD0 = sp170 - sp3C;
                        sp90 = ((sp138 * spC8) + (sp13C * spCC)) + (sp140 * spD0);
                        if (sp90 <= 0.0f) {
                            sp188 = sp34;
                            sp18C = sp38;
                            sp190 = sp3C;
                        } else {
                            sp188 = sp34 + (sp138 * sp90);
                            sp18C = sp38 + (sp13C * sp90);
                            sp190 = sp3C + (sp140 * sp90);
                        }
                    }
                } else {
                    if (sp90 <= 0.0f) {
                        sp188 = sp28;
                        sp18C = sp2C;
                        sp190 = sp30;
                    } else {
                        if (sp90 >= sp88) {
                            sp188 = sp34;
                            sp18C = sp38;
                            sp190 = sp3C;
                        } else {
                            sp188 = sp28 + (sp128 * sp90);
                            sp18C = sp2C + (sp12C * sp90);
                            sp190 = sp30 + (sp130 * sp90);
                        }
                    }
                }
            } else {
                if (sp80 > 1e-05f) {
                    if (sp8C == 0.0f) {
                        sp8C = ((spB8 * spB8) + (spB8 * spB8)) + (spB8 * spB8);
                        if (sp8C > 1e-05f) {
                            sp8C = (f32)func_8006A1F0(((f64)sp8C));
                            sp138 = spB8 / sp8C;
                            sp13C = spBC / sp8C;
                            sp140 = spC0 / sp8C;
                        } else {
                            goto L2355;
                        }
                    }
                    spC8 = sp168 - sp34;
                    spCC = sp16C - sp38;
                    spD0 = sp170 - sp3C;
                    sp90 = ((sp138 * spC8) + (sp13C * spCC)) + (sp140 * spD0);
                    if (sp90 <= 0.0f) {
                        sp188 = sp34;
                        sp18C = sp38;
                        sp190 = sp3C;
                    } else {
                        if (sp90 >= sp8C) {
                            sp188 = sp1C;
                            sp18C = sp20;
                            sp190 = sp24;
                        } else {
                            sp188 = sp34 + (sp138 * sp90);
                            sp18C = sp38 + (sp13C * sp90);
                            sp190 = sp3C + (sp140 * sp90);
                        }
                    }
                } else {
                    sp188 = sp168 - (sp4C * spD8);
                    sp18C = sp16C - (sp4C * spDC);
                    sp190 = sp170 - (sp4C * spE0);
                }
            }
        }
        spC8 = sp188 - sp168;
        spCC = sp18C - sp16C;
        spD0 = sp190 - sp170;
        sp54 = (f32)func_8006A1F0(((f64)(((spC8 * spC8) + (spCC * spCC)) + (spD0 * spD0))));
        sp58 = sp54 - (*(f32 *)arg0);
        sp5C = (*(f32 *)arg0) - sp50;
        sp60 = sp58 + sp5C;
        if (sp60 == 0.0f) {
            goto L2355;
        }
        sp64 = sp58 / sp60;
        sp64 = sp64 * ((struct gd_28_f32 *)arg0)->v;
        if (sp64 < 0.0f) {
            sp64 = 0;
        }
        if (sp64 < ((struct gd_28_f32 *)arg0)->v) {
            if (sp64 < ((struct gd_38_f32 *)arg0)->v) {
                ((struct gd_34_s32 *)arg0)->v = *(s32 *)arg1;
                ((struct gd_38_f32 *)arg0)->v = sp64;
                ((struct gd_3C_s32 *)arg0)->v = arg1;
                ((struct gd_40_f32 *)arg0)->v = sp178;
                ((struct gd_44_f32 *)arg0)->v = -sp180;
                ((struct gd_48_f32 *)arg0)->v = sp17C;
                ((struct gd_4C_f32 *)arg0)->v = spD8;
                ((struct gd_50_f32 *)arg0)->v = -spE0;
                ((struct gd_54_f32 *)arg0)->v = spDC;
            }
        }
L2355: ;
        sp18++;
    }
}
