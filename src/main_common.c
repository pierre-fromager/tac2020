
#include "main_common.h"

void init()
{
    setlocale(LC_ALL, LOCALE_FR_UTF8);
    setlocale(LC_NUMERIC, LOCALE_FR_UTF8);
    setlocale(LC_TIME, LOCALE_FR_UTF8);
    setenv(T_TZ, TZ_PARIS, 1);
    tzset();
    srand(time(NULL));
}

void tcommon(struct targs *ta)
{
    struct targs *parms;
    parms = (struct targs *)ta;
    attach_core(parms->coren);
    theader(parms->name, parms->coren, parms->freq, parms->ssize, parms->prio);
}

void tdebug(
    const char *taskname,
    unsigned int run,
    unsigned int line_counter,
    unsigned int data_counter)
{
    char dt[TIMESTAMP_SIZE];
    pthread_t thid = pthread_self();
    pid_t tid = syscall(SYS_gettid);
    set_datetimems(dt);

    log_info(COMMON_FMT, line_counter, dt, run, thid, tid, taskname, data_counter);
}

void theader(char *name, int coren, int freq, int ssize, unsigned int prio)
{
    log_info(HEADER_FMT, name, coren, freq, ssize, prio);
}
