#include "common.h"

/* math error handler: call matherr, set errno (ERANGE for OVERFLOW/UNDERFLOW, else EDOM) */
f64 func_8006A4E0(s32 type, char *name, f64 arg1, f64 arg2, f64 retval) {
    struct exception e;
    e.type = type;
    e.name = name;
    e.arg1 = arg1;
    e.arg2 = arg2;
    e.retval = retval;
    if (!matherr(&e)) {
        errno = (type == 3 || type == 4) ? 34 : 33;
    }
    return e.retval;
}
