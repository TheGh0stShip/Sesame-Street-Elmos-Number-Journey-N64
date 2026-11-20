/* guNormalize */
float sqrtf(float); /* func_80056690 */
void func_80061D30(float *x, float *y, float *z)
{
    float m;

    m = 1 / sqrtf((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
    *x *= m;
    *y *= m;
    *z *= m;
}
