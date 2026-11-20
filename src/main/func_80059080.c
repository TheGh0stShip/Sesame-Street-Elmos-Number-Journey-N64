/* __osDequeueThread: unlinks a thread from a singly linked queue */
typedef struct OSThread_s { struct OSThread_s *next; } OSThread;
void func_80059080(OSThread **queue, OSThread *t) {
    OSThread *pred = (OSThread *)queue;
    OSThread *succ = pred->next;
    while (succ != NULL) {
        if (succ == t) {
            pred->next = t->next;
            return;
        }
        pred = succ;
        succ = pred->next;
    }
}
