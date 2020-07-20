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

#define MSG_EXIT_MAIN "Exit main\n"
#define MSG_FREE_ARGS "\nFreeing args mallocs\n"
#define MSG_DSTACK_PUSH "%s dstack size : %d - intval : %u, strval : %s\n"
#define MSG_DSTACK_POP "dstack pop - intval : %u, strval : %s\n"
#define MSG_PRE "pre"
#define MSG_POST "post"

#define EMSG_TH1_FAIL "thread th1 not created"
#define EMSG_TH2_FAIL "thread th2 not created"
#define EMSG_THHB_FAIL "thread heartbeat not created"
#define EMSG_JOIN_THHB_FAIL "fail to join thread heartbeat"

#define LOCALE_FR_UTF8 "fr_FR.utf8"
#define TZ_PARIS "Europe/Paris"
#define COMMON_FMT "%05u,%s,%05u,%16lu,%u,%s,%u\n"
#define HEADER_FMT "Start %s core %d freq %05d ms ssize %d prio %u\n"
#define HEADER_DSTACK_FMT "%s dstack size %s pop : %d\n"

#define NUMBER_OF_ITEMS 6

#define T1_SSIZE 0x4096
#define T1_NAME "producer"
#define T1_PR 1
#define T1_FQ 200

#define T2_SSIZE 0x4096
#define T2_NAME "consumer"
#define T2_PR 40
#define T2_FQ (T1_FQ * NUMBER_OF_ITEMS)

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