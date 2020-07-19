/**
 * @brief main common
 * 
 */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MAIN_COMMON_H
#define MAIN_COMMON_H

#define LOCALE_FR_UTF8 "fr_FR.utf8"
#define TZ_PARIS "Europe/Paris"
#define COMMON_FMT "%05u,%s,%05u,%16lu,%u,%s,%u\n"
#define HEADER_FMT "Start %s core %d freq %05d ms ssize %d prio %u\n"
#define HEADER_DSTACK_FMT "%s dstack size %s pop : %d\n"

#define T1_SSIZE 0x4096
#define T1_NAME "producer"
#define T1_PR 10
#define T1_FQ 100

#define T2_SSIZE 0x4096
#define T2_NAME "consumer"
#define T2_PR 40
#define T2_FQ (T1_FQ * 10)

#define LOGGER_LOG_LEVEL LOG_DEBUG

#include "logger.h"
#include "tac.h"
#include "dstack.h"
#include "logger.h"
#include "locale.h"
#include "assert.h"
#include "signal.h"

    void init();
    void tcommon(struct targs *ta);
    void tdebug(
        const char *taskname,
        unsigned int run,
        unsigned int line_counter,
        unsigned int data_counter);
    void theader(char *name, int coren, int freq, int ssize, unsigned int prio);

#endif // ifndef MAIN_COMMON_H

#ifdef __cplusplus
}
#endif