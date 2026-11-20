/* memcpy */
void *func_80060E7C(void *s1, const void *s2, u32 n) {
    char *su1 = (char *)s1;
    const char *su2 = (const char *)s2;
    while (n > 0) { *su1 = *su2; su1++; su2++; n--; }
    return s1;
}
