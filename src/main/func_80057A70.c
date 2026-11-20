/* guRotateRPYF: roll/pitch/yaw rotation matrix (float) */
float func_80057E70(float); /* sinf */
float func_800566A0(float); /* cosf */
void func_80057040(float mf[4][4]); /* guMtxIdentF */
typedef struct { long m[4][4]; } Mtx;
void func_80056F00(float mf[4][4], Mtx *m); /* guMtxF2L */
#define sinf func_80057E70
#define cosf func_800566A0
void func_80057A70(float mf[4][4], float r, float p, float h)
{
	static float	dtor = 3.1415926 / 180.0;
	float	sinr, sinp, sinh;
	float	cosr, cosp, cosh;

	r *= dtor;
	p *= dtor;
	h *= dtor;
	sinr = sinf(r);
	cosr = cosf(r);
	sinp = sinf(p);
	cosp = cosf(p);
	sinh = sinf(h);
	cosh = cosf(h);

	func_80057040(mf);

	mf[0][0] = cosp*cosh;
	mf[0][1] = cosp*sinh;
	mf[0][2] = -sinp;

	mf[1][0] = sinr*sinp*cosh - cosr*sinh;
	mf[1][1] = sinr*sinp*sinh + cosr*cosh;
	mf[1][2] = sinr*cosp;

	mf[2][0] = cosr*sinp*cosh + sinr*sinh;
	mf[2][1] = cosr*sinp*sinh - sinr*cosh;
	mf[2][2] = cosr*cosp;
}
