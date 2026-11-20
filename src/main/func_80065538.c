/* __rmonGetExceptionStatus: fill a thread-status reply for a break exception */
typedef int TVid;
typedef struct {
    int length;
    char code;
    char type;
    short error;
    char rev;
    char method;
    short notused2;
} KKHeader;
typedef struct {
    unsigned int addr;
    short major;
    short minor;
} KKFaultInfo;
typedef struct {
    int flags;
    short why;
    short what;
    TVid tid;
    TVid pid;
    unsigned int instr;
    KKFaultInfo info;
    int priority;
    int start;
    int rv;
    unsigned int args[6];
} KKThreadStatus;
typedef struct {
    KKHeader header;
    KKThreadStatus status;
} KKStatusEvent;

void func_80065538(KKStatusEvent *reply) {
    reply->status.flags = 1; /* OS_STATE_STOPPED */
    reply->status.why = 2;
    reply->status.what = 0;
    reply->status.rv = 0;
    reply->status.info.major = 2;
    reply->status.info.minor = 4;
    reply->header.code = 4; /* KK_CODE_THREAD_STATUS */
    reply->header.error = 0;
    reply->header.length = sizeof(*reply);
}
