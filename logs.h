#ifndef __LOGS_H__
#define __LOGS_H__

#include <time.h>
#include <sys/time.h>

#define MAX_NAME_LEN 64

typedef struct log_st
{
  unsigned char name[MAX_NAME_LEN];
  unsigned long time;
  clock_t cpu;
} log_t;

#endif /* __LOGS_H__ */
