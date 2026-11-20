/* guPerspective: perspective projection matrix (guPerspectiveF inlined) */
float func_80057E70(float); /* sinf */
float func_800566A0(float); /* cosf */
void func_80057040(float mf[4][4]); /* guMtxIdentF */
typedef struct { long m[4][4]; } Mtx;
void func_80056F00(float mf[4][4], Mtx *m); /* guMtxF2L */
#define sinf func_80057E70
#define cosf func_800566A0
static void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	float	cot;
	int	i, j;

	func_80057040(mf);

	fovy *= 3.1415926 / 180.0;
	cot = cosf (fovy/2) / sinf (fovy/2);

	mf[0][0] = cot / aspect;
	mf[1][1] = cot;
	mf[2][2] = (near + far) / (near - far);
	mf[2][3] = -1;
	mf[3][2] = (2 * near * far) / (near - far);
	mf[3][3] = 0;

	for (i=0; i<4; i++)
	    for (j=0; j<4; j++)
		mf[i][j] *= scale;

	if (perspNorm != (u16 *) NULL) {
	    if (near+far<=2.0) {
		*perspNorm = (u16) 0xFFFF;
	    } else  {
		*perspNorm = (u16) ((2.0*65536.0)/(near+far));
		if (*perspNorm<=0) 
		    *perspNorm = (u16) 0x0001;
	    }
	}
}

void func_800575C0(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	float	mf[4][4];

	guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);

	func_80056F00(mf, m);
}
