/* __assert: prints an assertion fault message */
extern char D_8007B890[]; /* "\nASSERTION FAULT: %s, %d: \"%s\"\n" */
void func_80061178(const char *fmt, ...); /* osSyncPrintf */
void func_80059540(const char *exp, const char *filename, int line) {
    func_80061178(D_8007B890, filename, line, exp);
}
