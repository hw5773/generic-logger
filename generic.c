#include "generic.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

logger_t *
init_logger(const char *log_prefix, int flags)
{
  fstart("log_prefix: %s", log_prefix);
  logger_t *ret;
  ret = (logger_t *)malloc(sizeof(logger_t));
  memset(ret, 0x0, sizeof(logger_t));

  ret->log_prefix = log_prefix;
  ret->ops = &gops;
  ret->flags = flags;
  init_names(ret);

  ffinish("ret: %p", ret);
  return ret;
}

void init_names(logger_t *logger)
{
  fstart("logger: %p", logger);
  assert(logger != NULL);

  FILE *fp;
  unsigned char buf[BUF_SIZE] = {0};
  char *ptr, *tmp, *name;
  int i, val, len;

  fp = fopen(NAME_FILE, "r");
  
  while (feof(fp) == 0)
  {
    memset(buf, 0x0, BUF_SIZE);
    fgets(buf, BUF_SIZE, fp);
    ptr = strtok(buf, " ");

    if (!ptr)
      continue;

    if (ptr[0] != '#')
      continue;

    // name
    name = NULL;
    ptr = strtok(NULL, " ");

    if (!ptr)
      continue;

    tmp = strstr(ptr, "HPC");
    if (!tmp)
      continue;

    len = strlen(ptr);
    name = (char *)malloc(len+1);
    memset(name, 0x0, len+1);
    memcpy(name, ptr, len);

    // value
    ptr = strtok(NULL, " ");
    len = strlen(ptr);
    if (ptr[len-1] == '\n')
      ptr[len-1] = 0;
    val = atoi(ptr);

    logger->name[val] = name;
    dmsg("logger->name[%d] = %s", val, name);
  }

  fclose(fp);
  
  ffinish();
}

void 
fin_logger(logger_t *logger)
{
  fstart("logger: %p", logger);
  assert(logger != NULL);

  unsigned char log_cpu_file_name[MAX_FILE_NAME_LEN] = {0, };
  unsigned char log_time_file_name[MAX_FILE_NAME_LEN] = {0, };
  int et;
  FILE *cfp, *tfp;
  int i, flags;

  flags = logger->flags;
  et = (int) (get_current_nanoseconds() / 1000000000);

  if (flags & HPC_LF_CPU)
  {
    snprintf((char *) log_cpu_file_name, MAX_FILE_NAME_LEN, "%s_cpu_%d.csv",
        logger->log_prefix, et);
    cfp = fopen(log_cpu_file_name, "w");
    for (i=0; i<NUM_OF_LOGS; i++)
    {
      if (logger->log[i].cpu > 0)
      {
        fprintf(cfp, "%d, %s, %lu\n", i, logger->name[i], logger->log[i].cpu);
      }
    }

    fclose(cfp);
  }

  if (flags & HPC_LF_TIME)
  {
    snprintf((char *) log_time_file_name, MAX_FILE_NAME_LEN, "%s_time_%d.csv",
        logger->log_prefix, et);
    tfp = fopen(log_time_file_name, "w");
    for (i=0; i<NUM_OF_LOGS; i++)
    {
      if (logger->log[i].time > 0)
      {
        fprintf(tfp, "%d, %s, %lu\n", i, logger->name[i], logger->log[i].time);
      }
    }
    fclose(tfp);
  }

  free(logger);
  ffinish();
}

int 
generic_add(logger_t *logger, int name)
{
  fstart("logger: %p, name: %d", logger, name);
  assert(logger != NULL);
  assert(name >= 0);
  
  int flags;
  flags = logger->flags;

  if (flags & HPC_LF_CPU)
  {
    logger->log[name].cpu = get_processor_nanoseconds();
  }

  if (flags & HPC_LF_TIME)
  {
    logger->log[name].time = get_current_nanoseconds();
  }

  ffinish();
  return SUCCESS;
}

int 
generic_interval(logger_t *logger, int start, int end)
{
  fstart("logger: %p, start: %d, end: %d", logger, start, end);
  assert(logger != NULL);
  assert(start >= 0);
  assert(end > start);

  int flags;

  const char *nstart;
  const char *nend;

  unsigned long cstart;
  unsigned long cend;
  unsigned long tstart;
  unsigned long tend;

  flags = logger->flags;

  nstart = logger->name[start];
  nend = logger->name[end];

  cstart = logger->log[start].cpu;
  cend = logger->log[end].cpu;
  tstart = logger->log[start].time;
  tend = logger->log[end].time;

  if (flags & HPC_LF_CPU)
  {
    imsg("CPU Time from %s to %s: %lu ns", nstart, nend, cend - cstart); 
  }

  if (flags & HPC_LF_TIME)
  {
    imsg("Elapsed Time from %s to %s: %lu ns", nstart, nend, tend - tstart);
  }

  ffinish();
  return SUCCESS;
}

int
generic_print(logger_t *logger, int name, int flags)
{
  fstart("logger: %p, name: %d", logger, name);
  assert(logger != NULL);
  assert(name >= 0);
  assert(flags >= 0);
  
  if (flags & HPC_LF_CPU)
  {
    imsg("CPU Time at %s: %lu ns", logger->name[name], logger->log[name].cpu);
  }

  if (flags & HPC_LF_TIME)
  {
    imsg("Timestamp at %s: %lu ns", logger->name[name], logger->log[name].time);
  }

  ffinish();
  return SUCCESS;
}

int 
generic_print_all(logger_t *logger)
{
  fstart("logger: %p", logger);
  assert(logger != NULL);

  int i, flags;
  flags = logger->flags;

  if (flags & HPC_LF_CPU)
  {
    for (i=0; i<NUM_OF_LOGS; i++)
    {
      if (logger->log[i].cpu > 0)
      {
        generic_print(logger, i, HPC_LF_CPU);
      }
    }
  }

  if (flags & HPC_LF_TIME)
  {
    for (i=0; i<NUM_OF_LOGS; i++)
    {
      if (logger->log[i].time > 0)
      {
        generic_print(logger, i, HPC_LF_TIME);
      }
    }
  }

  ffinish();
  return SUCCESS;
}
