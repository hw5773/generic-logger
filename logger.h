#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "defines.h"
#include "logs.h"

typedef struct logger_ops_st logger_ops;

typedef struct logger_st
{
  const char *log_prefix;
  log_t log[NUM_OF_LOGS];
  char *name[NUM_OF_LOGS];
  int flags;
  logger_ops *ops;
} logger_t;

typedef struct logger_ops_st
{
  int (*add)(logger_t *logger, int name);
  int (*interval)(logger_t *logger, int start, int end);
  int (*print)(logger_t *logger, int name, int flags);
  int (*print_all)(logger_t *logger);
} logger_ops;


logger_t *init_logger(const char *log_prefix, int flags);
void init_names(logger_t *logger);
void fin_logger(logger_t *logger);

static inline unsigned long get_current_nanoseconds()
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return tp.tv_sec + 1000000000 * tp.tv_nsec;
}

static inline unsigned long get_processor_nanoseconds()
{
  struct timespec tp;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
  return tp.tv_sec + 1000000000 * tp.tv_nsec;
}

#endif /* __LOGGER_H__ */
