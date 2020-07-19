
/**
 * @file main.c
 * @author Pierre Fromager (info@pier-infor.fr)
 * @brief thread advanced control
 * @version 0.1
 * @date 2020-07-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#define LOGGER_LOG_LEVEL LOG_INFO

#include "logger.h"
#include "tac.h"
#include "dstack.h"

#include "locale.h"
#include "assert.h"
#include "signal.h"

#define LOCALE_FR_UTF8 "fr_FR.utf8"
#define TZ_PARIS "Europe/Paris"
#define COMMON_FMT "%05d,%s,%05d,%16lu,%d,%s,%d\n"
#define HEADER_FMT "Start %s core %d freq %05d ms ssize %d prio %d\n"
#define HEADER_DSTACK_FMT "%s dstack size %s pop : %d\n"

#define T1_SSIZE 0x4096
#define T1_NAME "producer"
#define T1_PR 10
#define T1_FQ 100

#define T2_SSIZE 0x4096
#define T2_NAME "consumer"
#define T2_PR 40
#define T2_FQ (T1_FQ * 10)

pthread_attr_t th1_attr, th2_attr;
pthread_t th1, th2, thhb;

pthread_mutex_t tmx1, tmx2, dsatck_mutex, counterMutex, exitMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tic1, tic2, exit_cond = PTHREAD_COND_INITIALIZER;

volatile unsigned int line_counter = 0;
volatile unsigned int data_counter = 0;

DStack s;

void init();
void create_threads();
void sigint_handler();
void *t1(void *arg);
void *t2(void *arg);
void *hb();
void tcommon(struct targs *ta);
void tdebug(const char *taskname, unsigned int run);
void theader(char *name, int coren, int freq, int ssize, unsigned int prio);
void cycle();

int main(void)
{
  assert(sysconf(_SC_CLK_TCK) == 100);
  init();
  dstk_init(&s);
  signal(SIGINT, sigint_handler);
  create_threads();
  log_debug("Exit main\n");
  return 0;
}

void init()
{
  setlocale(LC_ALL, LOCALE_FR_UTF8);
  setlocale(LC_NUMERIC, LOCALE_FR_UTF8);
  setlocale(LC_TIME, LOCALE_FR_UTF8);
  setenv(T_TZ, TZ_PARIS, 1);
  tzset();
  srand(time(NULL));
}

void sigint_handler()
{
  pthread_cond_signal(&exit_cond);
}

void create_threads()
{
  int err;
  void *stack1, *stack2;

  struct targs *t1_args = (struct targs *)malloc(sizeof(struct targs));

  t1_args->name = T1_NAME;
  t1_args->ssize = T1_SSIZE;
  t1_args->prio = T1_PR;
  t1_args->coren = CPU_CORE_0;
  t1_args->freq = T1_FQ;
  set_th_attr(
      &th1_attr,
      &stack1,
      t1_args->ssize,
      t1_args->prio,
      PTHREAD_CREATE_DETACHED,
      PTHREAD_EXPLICIT_SCHED,
      SCHED_RR);
  show_th_attr(&th1_attr);

  struct targs *t2_args = (struct targs *)malloc(sizeof(struct targs));

  t2_args->name = T2_NAME;
  t2_args->ssize = T2_SSIZE;
  t2_args->prio = T2_PR;
  t2_args->coren = CPU_CORE_1;
  t2_args->freq = T2_FQ;
  set_th_attr(
      &th2_attr,
      &stack2,
      t2_args->ssize,
      t2_args->prio,
      PTHREAD_CREATE_DETACHED,
      PTHREAD_EXPLICIT_SCHED,
      SCHED_RR);
  show_th_attr(&th2_attr);

  err = pthread_create(&th1, &th1_attr, t1, (void *)t1_args);
  log_e(err, "thread th1 not created");
  err = pthread_create(&th2, &th2_attr, t2, (void *)t2_args);
  log_e(err, "thread th2 not created");

  err = pthread_create(&thhb, NULL, hb, NULL);
  log_e(err, "thread heartbeat not created");
  pthread_join(thhb, NULL);

  log_debug("Freeing args mallocs\n");
  free(t1_args);
  free(t2_args);
}

void *hb()
{
  pthread_cond_wait(&exit_cond, &exitMutex);
  return NULL;
}

void *t1(void *ta)
{
  static unsigned int run = 0;
  struct targs *parms;
  parms = (struct targs *)ta;
  tcommon(ta);
  for (;;)
  {
    tdebug(parms->name, ++run);
    pthread_mutex_lock(&dsatck_mutex);
    data_counter++;
    dstk_push(&s, data_counter);
    log_debug("%s dstack size : %d\n", parms->name, s.size);
    pthread_mutex_unlock(&dsatck_mutex);
    task_wait(parms->freq, tic1, tmx1);
  }
  return NULL;
}

void *t2(void *ta)
{
  static unsigned int run = 0;
  struct targs *parms;
  parms = (struct targs *)ta;
  Item itm;
  tcommon(ta);
  for (;;)
  {
    tdebug(parms->name, ++run);
    pthread_mutex_lock(&dsatck_mutex);
    if (!dstk_isempty(&s))
    {
      log_debug(HEADER_DSTACK_FMT, parms->name, "pre", s.size);
      while (!dstk_isempty(&s))
      {
        itm = dstk_pop(&s);
        log_debug("dstack pop : %d\n", itm);
      }
      log_debug(HEADER_DSTACK_FMT, parms->name, "post", s.size);
    }
    pthread_mutex_unlock(&dsatck_mutex);
    task_wait(parms->freq, tic2, tmx2);
  }
  return NULL;
}

void tcommon(struct targs *ta)
{
  struct targs *parms;
  parms = (struct targs *)ta;
  attach_core(parms->coren);
  theader(parms->name, parms->coren, parms->freq, parms->ssize, parms->prio);
}

void tdebug(const char *taskname, unsigned int run)
{
  char dt[TIMESTAMP_SIZE];
  pthread_t thid = pthread_self();
  pid_t tid = syscall(SYS_gettid);
  set_datetimems(dt);
  cycle();
  log_info(COMMON_FMT, line_counter, dt, run, thid, tid, taskname, data_counter);
}

void theader(char *name, int coren, int freq, int ssize, unsigned int prio)
{
  log_info(HEADER_FMT, name, coren, freq, ssize, prio);
}

void cycle()
{
  line_counter++;
}