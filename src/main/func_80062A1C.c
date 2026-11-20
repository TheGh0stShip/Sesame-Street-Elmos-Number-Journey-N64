/* hand-written assembly: *list = (*list)->next */
__asm__(
".set noreorder\n"
".globl func_80062A1C\n"
"func_80062A1C:\n"
"lw $2, 0($4)\n"
"lw $25, 0($2)\n"
"jr $31\n"
"sw $25, 0($4)\n"
".set reorder\n"
);
