#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TAC_H
#define TAC_H

#define _GNU_SOURCE
//#define _POSIX_C_SOURCE 200112L
#define _POSIX_C_SOURCE 200809L

#include "logger.h"
#include <errno.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define T_TZ "TZ"
#define TIMESTAMP_SIZE (12 + 1)
#define TIMESTAMP_FMT "%H:%M:%S"
#define CPU_CORE_0 0
#define CPU_CORE_1 1
#define CPU_CORE_2 2
#define CPU_CORE_3 3
#define CPU_CORE_5 4
#define CPU_CORE_6 5
#define CPU_CORE_7 6
#define CPU_CORE_8 7
#define PTHREAD_STATE_NC "N/C"

    struct targs
    {
        char *name;
        unsigned int ssize;
        unsigned int prio;
        unsigned int coren;
        unsigned int freq;
    };

    void set_th_attr(
        pthread_attr_t *attr,
        void *stack,
        int ssize,
        int prio,
        int detach,
        int inhsched,
        int schedpol);

    void show_th_attr(pthread_attr_t *attr);

    int task_wait(
        int tms,
        pthread_cond_t cond,
        pthread_mutex_t mutex);

    void set_datetimems(char *dt);

    void attach_core(int cnum);

    char const *_attr_detachstate(int code);
    char const *_attr_scope(int code);
    char const *_attr_inheritsched(int code);
    char const *_attr_schedpolicy(int code);

#endif // ifndef TAC_H

#ifdef __cplusplus
}
#endif