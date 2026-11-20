typedef long Mtx_t[4][4];
typedef union { Mtx_t m; long long int force_structure_alignment; } Mtx;
float func_80057E70(float); /* sinf */
float func_800566A0(float); /* cosf */
void func_80056F00(float mf[4][4], Mtx *m); /* guMtxF2L */
/* guPositionF: rotation (degrees), scale and translation into a float matrix */
void func_80058010(float mf[4][4], float r, float p, float h, float s, float x, float y, float z)
{
        static float    dtor = 3.1415926 / 180.0;
        float   sinr, sinp, sinh;
        float   cosr, cosp, cosh;

        r *= dtor;
        p *= dtor;
        h *= dtor;
        sinr = func_80057E70(r);
        cosr = func_800566A0(r);
        sinp = func_80057E70(p);
        cosp = func_800566A0(p);
        sinh = func_80057E70(h);
        cosh = func_800566A0(h);

        mf[0][0] = (cosp*cosh) * s;
        mf[0][1] = (cosp*sinh) * s;
        mf[0][2] = (-sinp) * s;
        mf[0][3] = 0.0;

        mf[1][0] = (sinr*sinp*cosh - cosr*sinh) * s;
        mf[1][1] = (sinr*sinp*sinh + cosr*cosh) * s;
        mf[1][2] = (sinr*cosp) * s;
        mf[1][3] = 0.0;

        mf[2][0] = (cosr*sinp*cosh + sinr*sinh) * s;
        mf[2][1] = (cosr*sinp*sinh - sinr*cosh) * s;
        mf[2][2] = (cosr*cosp) * s;
        mf[2][3] = 0.0;

        mf[3][0] = x;
        mf[3][1] = y;
        mf[3][2] = z;
        mf[3][3] = 1.0;
}
