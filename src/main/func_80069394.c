/* lldiv */
typedef struct { long long quot; long long rem; } lldiv_t;

lldiv_t func_80069394(long long num, long long denom)
{
    lldiv_t ret;

    ret.quot = num / denom;
    ret.rem = num - denom * ret.quot;
    if (ret.quot < 0 && ret.rem > 0) {
        ret.quot += 1;
        ret.rem -= denom;
    }
    return ret;
}
