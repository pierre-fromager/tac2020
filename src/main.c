
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

#include "main_common.h"

volatile unsigned int line_counter = 0;
volatile unsigned int data_counter = 0;

void create_threads();
void sigint_handler();
void *t1(void *arg);
void *t2(void *arg);
void *hb();
void cycle();

pthread_attr_t th1_attr, th2_attr;
pthread_t th1, th2, thhb;

pthread_mutex_t tmx1, tmx2, dsatck_mutex, exit_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tic1, tic2, exit_cond = PTHREAD_COND_INITIALIZER;

DStack s;

int main(void)
{
  assert(sysconf(_SC_CLK_TCK) == 100);
  init();
  dstk_init(&s);
  signal(SIGINT, sigint_handler);
  create_threads();
  dstk_destroy(&s);
  log_debug(MSG_EXIT_MAIN);
  return 0;
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
  log_e(err, EMSG_TH1_FAIL);
  err = pthread_create(&th2, &th2_attr, t2, (void *)t2_args);
  log_e(err, EMSG_TH2_FAIL);
  err = pthread_create(&thhb, NULL, hb, NULL);
  log_e(err, EMSG_THHB_FAIL);
  err = pthread_join(thhb, NULL);
  log_e(err, EMSG_JOIN_THHB_FAIL);
  log_debug(MSG_FREE_ARGS);
  free(t1_args);
  free(t2_args);
}

void *hb()
{
  pthread_cond_wait(&exit_cond, &exit_mutex);
  return NULL;
}

void *t1(void *ta)
{
  static unsigned int run = 0;
  struct targs *parms;
  parms = (struct targs *)ta;
  Item item;
  tcommon(ta);
  for (;;)
  {
    cycle();
    tdebug(parms->name, ++run, line_counter, data_counter);
    pthread_mutex_lock(&dsatck_mutex);
    data_counter++;
    item.intval = data_counter;
    strcpy(item.strval, (data_counter % 2 == 0) ? "even" : "odd");
    dstk_push(&s, item);
    log_debug(MSG_DSTACK_PUSH, parms->name, s.size, item.intval, item.strval);
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
  Item item;
  tcommon(ta);
  for (;;)
  {
    cycle();
    tdebug(parms->name, ++run, line_counter, data_counter);
    pthread_mutex_lock(&dsatck_mutex);
    if (!dstk_isempty(&s))
    {
      log_debug(HEADER_DSTACK_FMT, parms->name, MSG_PRE, s.size);
      while (!dstk_isempty(&s))
      {
        item = dstk_pop(&s);
        log_debug(MSG_DSTACK_POP, item.intval, item.strval);
      }
      log_debug(HEADER_DSTACK_FMT, parms->name, MSG_POST, s.size);
    }
    pthread_mutex_unlock(&dsatck_mutex);
    task_wait(parms->freq, tic2, tmx2);
  }
  return NULL;
}

void cycle()
{
  line_counter++;
}