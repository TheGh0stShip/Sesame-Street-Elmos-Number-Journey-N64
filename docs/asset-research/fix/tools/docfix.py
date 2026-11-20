import sys, os
B = '${N64DK_LAB}/sesassets/'
def sub(path, old, new):
    p = B + path; t = open(p).read()
    assert t.count(old) == 1, (path, old[:60], t.count(old))
    open(p, 'w').write(t.replace(old, new))
def app(path, txt):
    p = B + path; t = open(p).read()
    if 'Corrections from r3' not in t: open(p, 'w').write(t.rstrip('\n') + '\n\n' + txt.strip('\n') + '\n')
# lvlD
sub('lvlD/FORMAT.md', '| 0x02 | u16 | for animated meshes = unique vertex count + 1. Not read by func_80018530, func_8000092C or func_80000450 |',
    '| 0x02 | u16 | no reader anywhere (copied only as part of word 0). unique+1 in animated meshes (54/54); arbitrary in static meshes: tool output |')
sub('lvlD/FORMAT.md', '| 0x18 | ptr | 0 in ROM; runtime |', '| 0x18 | ptr | 0 in ROM; runtime: set to B + nverts*16 (scratch after buffer B), no known reader |')
sub('lvlD/FORMAT.md', '| 0x20 | u32 | work size: bytes of runtime vertex buffers (0 if not animated) |',
    '| 0x20 | u32 | work size: bytes of runtime vertex buffers (0 if not animated). Also gates func_80018530: it returns 0 when +0x20 == 0. The type-7 in-place path (func_80023118) memsets +0x20 bytes from +0x18, i.e. nverts*16 bytes past its block |')
sub('lvlD/FORMAT.md', 'func_8000092C uses the box centre ((min+max)/2) to offset the actor position before drawing.',
    'func_8000092C computes the box centre ((min+max)/2) into its by-value position arguments and never reads them again: the box has no runtime effect (no culling, no centring). It equals the exact min/max of the ROM Vtx (108/108).')
sub('lvlD/FORMAT.md', '+0x00..+0x1F: 8 floats the loader never reads.',
    '+0x00..+0x1F: the pre-baked runtime item header, never read (the loader recomputes it): u32 type 3, u32 owner 0, u32 mask 0, u32 0, f32 plane nx, ny, nz, d of the quad\'s own corners c0,c1,c2 (39/39).')
# lvlC
sub('lvlC/FORMAT.md', '| 0x04 | u32 | id (u16 kind code such as 0x2005; for type 7/12 it selects the model, for 11 the data) |',
    '| 0x04 | s32 | id. Most handlers use the low u16 (+6 -> actor+0xA). Read as s32 by type 8 (pickup slot 1..30, else 1), type 9 (float arg of func_80049ABC), type 11 (switch); type 18 writes 0x6000 here |')
sub('lvlC/FORMAT.md', '| 0x08 | s32 | param (type 9: digits split as /100000000 and %10000) |',
    '| 0x08 | s32 | initial actor flags (copied to actor+0xC): 0x80 = hold the last anim tick instead of wrapping (func_800272C8); bit 1 remove; bit 8 anim finished. Type 9 instead splits it (/100000000, %10000) |')
sub('lvlC/FORMAT.md', '| 10 | func_80034CD0 | func_80036A90 | |', '| 10 | func_80034CD0 | func_80036A90 | interaction point (switch on id; id 2: player near + button -> mode 22, player state 35) |')
sub('lvlC/FORMAT.md', '| 13/17/16 | func_80026ABC/AF8/B34 | 13/14/15: func_80027F2C/F68/FA4 | speech trigger, banks 0/1/2 (func_80026B70) |',
    '| 13/17/16 | func_80026ABC/AF8/B34 | 13/14/15: func_80027F2C/F68/FA4 | speaker, func_80026B70 [func_80027FE0]: Letter 13->bank 0, 17->1, 16->2; Number 13->0, 14->1, 15->2 |')
sub('lvlC/FORMAT.md', '| 18 | - | inline in func_8001FBE0 | Number: id rewritten to 0x6000 |', '| 18 | - | inline in func_8001FBE0 | Number: writes s32 0x6000 into rec+4, then calls the type-11 handler func_800408D0 |')
sub('lvlC/FORMAT.md', '| 19 | func_80030D8C | func_80033368 | |', '| 19 | func_80030D8C | func_80033368 | proximity trigger: player within dx²+dz² < 300000 -> player state 0x22, removes itself |')
sub('lvlC/FORMAT.md', '(func_80044B90 -> func_80044E3C). Stack depth limit 128.',
    '(func_80044B90 -> func_80044E3C). DrawWorld ignores the plane and culls with the box only (no culling in levels 11/19); it\n  prints "bsp stack blown" at depth 128. Collision ignores the box; it spins forever at depth 129. A leaf\'s word0 becomes\n  the hit-surface id D_80115378.')
sub('lvlC/FORMAT.md', '+0x20 front child, +0x24 back child.', '+0x20 front child (n.p - d >= 0, p = mover (x, z, -y)), +0x24 back child.')
sub('lvlC/FORMAT.md', '(pad is 0 everywhere)', '(pad is 0 everywhere and never read)')
sub('lvlC/FORMAT.md', '(0x28 bytes: +4 u16 vertex count,', '(0x28 bytes: +0 u16 flags, +2 u16 unread, +4 u16 vertex count,')
# packC
sub('packC/FORMAT.md', '| 0x00 | u32 | flags / kind (e.g. 0x2000xxxx for skinned or morphing meshes, a small count otherwise) |',
    '| 0x00 | u16 | flags: 0x2000 = morph-animated, 0x8000 = runtime buffer toggle |\n| 0x02 | u16 | never read (unique+1 for animated meshes, arbitrary otherwise) |')
sub('packC/FORMAT.md', '| 0x24 | ptr | 0 or bounding box: f32[6] min xyz, max xyz (class `bbox`, usually stored right before the header) |',
    '| 0x24 | ptr | 0 or bounding box: f32[6] min xyz, max xyz = exact vertex extents (class `bbox`, usually stored right before the header); read only by dead code in func_8000092C, no runtime effect |')
sub('packC/FORMAT.md', 'hold 8 f32 that `func_80023118` does not read (probably a plane or bounds).',
    'are the pre-baked runtime item header, never read: u32 type 3, u32 0 (owner), u32 0 (mask), u32 0, f32 plane nx, ny, nz, d of the quad\'s own corners (39/39).')
# sprA
app('sprA/FORMAT.md', '''## Corrections from r3 (r3/FORMATS_PROVEN.md §1)
- The streaming thread in Number is func_80027D90 (size D_800A7A50, ROM address D_80106C74, buffer D_80118244).
- When keys[-1] == ticks (139/141 anims) the last key row is never read; in loop anims the last row is dead data (it is not key 0).
- tick >= ticks leaves the mesh unrebuilt. Interpolation truncates the difference and the product to s16, divides toward zero.
- Number 0x6482F0 and 0x758920 have keys[-1] < ticks and take the "Frame passed last key!" path.
- Number 0x79CD90-0x7AB810 (2 anims, 214 vertices) is unreferenced: D_0079CD90 is only the end of the 0x78E0B0 clip.''')
# lvlH
sub('lvlH/FORMAT.md', '(observed ordering: values 0..2 <= values 3..5).', '(exact vertex extents, proven; read only by dead code in func_8000092C, no runtime effect).')
sub('lvlH/REPORT.md', 'Caveats: bounds meaning inferred from values;', 'Caveats: bounds proven (r3) = exact vertex extents, read only by dead code;')
print('ok')
