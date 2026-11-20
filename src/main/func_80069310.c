/* ldiv */
typedef struct { long quot; long rem; } ldiv_t;
ldiv_t func_80069310(long num, long denom) {
    ldiv_t ret;
    ret.quot = num / denom;
    ret.rem = num - denom * ret.quot;
    if (ret.quot < 0 && ret.rem > 0) {
        ret.quot += 1;
        ret.rem -= denom;
    }
    return ret;
}
