
#include "tac.h"
#include "logger.h"

void set_th_attr(
    pthread_attr_t *attr,
    void *stack,
    int ssize,
    int prio,
    int detach,
    int inhsched,
    int schedpol)
{
  int err;
  struct sched_param sparam;
  err = pthread_attr_init(attr);
  log_e(err, "pthread_attr_init");
  err = pthread_attr_setdetachstate(attr, detach);
  log_e(err, "spthread_attr_setdetachstate");
  err = pthread_attr_setinheritsched(attr, inhsched);
  log_e(err, "pthread_attr_setinheritsched");
  err = pthread_attr_setschedpolicy(attr, schedpol);
  log_e(err, "pthread_attr_setschedpolicy");
  err = pthread_attr_getschedparam(attr, &sparam);
  log_e(err, "pthread_attr_getschedparam");
  sparam.sched_priority = prio;
  err = pthread_attr_setschedparam(attr, &sparam);
  log_e(err, "pthread_attr_setschedparam");
  err = posix_memalign(&stack, sysconf(_SC_PAGESIZE), ssize);
  log_e(err, "posix_memalign");
  err = pthread_attr_setstack(attr, stack, ssize);
  log_e(err, "pthread_attr_setstack");
}

int task_wait(int tms, pthread_cond_t cond, pthread_mutex_t mutex)
{
  int err;
  struct timeval tv;
  struct timespec ts;
  gettimeofday(&tv, NULL);
  ts.tv_sec = tv.tv_sec;
  ts.tv_nsec = tv.tv_usec * 1000;
  ts.tv_nsec += tms * 1000000;
  ts.tv_sec += ts.tv_nsec / 1000000000L;
  ts.tv_nsec = ts.tv_nsec % 1000000000L;
  //pthread_mutex_lock(&mutex);
  err = pthread_cond_timedwait(&cond, &mutex, &ts);
  //pthread_mutex_unlock(&mutex);
  return err;
}

void set_datetimems(char *dt)
{
  long ms;
  time_t sec;
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  sec = spec.tv_sec;
  ms = round(spec.tv_nsec / 1.0e6);
  if (ms > 999)
  {
    sec++;
    ms = 0;
  }
  struct tm *pTime = localtime(&sec);
  char buf[TIMESTAMP_SIZE];
  char usec_buf[15];
  strftime(buf, TIMESTAMP_SIZE, TIMESTAMP_FMT, pTime);
  sprintf(usec_buf, "%s.%03ld", buf, ms);
  strcpy(dt, usec_buf);
}

void attach_core(int cnum)
{
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cnum, &cpuset);
  sched_setaffinity(0, sizeof(cpuset), &cpuset);
}

void show_th_attr(pthread_attr_t *attr)
{
  int err, i, sched_pol;
  size_t v;
  void *stkaddr;
  struct sched_param sp;
  err = pthread_attr_getdetachstate(attr, &i);
  log_e(err, "pthread_attr_getdetachstate");
  log_color(MAG, "Detach state\t\t %s\n", _attr_detachstate(i));
  err = pthread_attr_getscope(attr, &i);
  log_e(err, "pthread_attr_getscope");
  log_color(MAG, "Scope\t\t\t %s\n", _attr_scope(i));
  err = pthread_attr_getinheritsched(attr, &i);
  log_e(err, "pthread_attr_getinheritsched");
  log_color(MAG, "Inherit scheduler\t %s\n", _attr_inheritsched(i));
  err = pthread_attr_getschedpolicy(attr, &sched_pol);
  log_e(err, "pthread_attr_getschedpolicy");
  log_color(MAG, "Scheduling policy\t %s\n", _attr_schedpolicy(sched_pol));
  log_color(
      MAG,
      "Schedule prio min-max\t %d-%d\n",
      sched_get_priority_min(sched_pol),
      sched_get_priority_max(sched_pol));
  err = pthread_attr_getschedparam(attr, &sp);
  log_e(err, "pthread_attr_getschedparam");
  log_color(MAG, "Scheduling priority\t %d\n", sp.sched_priority);
  err = pthread_attr_getguardsize(attr, &v);
  log_e(err, "pthread_attr_getguardsize");
  log_color(MAG, "Guard size\t\t %zu\n", v);
  err = pthread_attr_getstack(attr, &stkaddr, &v);
  log_e(err, "pthread_attr_getstack");
  log_color(MAG, "Stack address\t\t %p\n", stkaddr);
  log_color(MAG, "Stack size\t\t 0x%zx bytes\n", v);
}

char const *_attr_detachstate(int code)
{
  return (code == PTHREAD_CREATE_DETACHED)
             ? "PTHREAD_CREATE_DETACHED"
             : (code == PTHREAD_CREATE_JOINABLE)
                   ? "PTHREAD_CREATE_JOINABLE"
                   : PTHREAD_STATE_NC;
}

char const *_attr_scope(int code)
{
  return (code == PTHREAD_SCOPE_SYSTEM)
             ? "PTHREAD_SCOPE_SYSTEM"
             : (code == PTHREAD_SCOPE_PROCESS)
                   ? "PTHREAD_SCOPE_PROCESS"
                   : PTHREAD_STATE_NC;
}

char const *_attr_inheritsched(int code)
{
  return (code == PTHREAD_INHERIT_SCHED)
             ? "PTHREAD_INHERIT_SCHED"
             : (code == PTHREAD_EXPLICIT_SCHED)
                   ? "PTHREAD_EXPLICIT_SCHED"
                   : PTHREAD_STATE_NC;
}

char const *_attr_schedpolicy(int code)
{
  return (code == SCHED_OTHER)
             ? "SCHED_OTHER"
             : (code == SCHED_FIFO)
                   ? "SCHED_FIFO"
                   : (code == SCHED_RR)
                         ? "SCHED_RR"
                         : PTHREAD_STATE_NC;
}