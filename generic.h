#ifndef __GENERIC_H__
#define __GENERIC_H__

#include "logger.h"
#include "generic_names.h"
#include "generic_flags.h"

#define NAME_FILE "generic_names.h"

int generic_add(logger_t *logger, int name);
int generic_interval(logger_t *logger, int start, int end);
int generic_print(logger_t *logger, int name, int flags);
int generic_print_all(logger_t *logger);

static logger_ops gops = 
{
  .add = generic_add,
  .interval = generic_interval,
  .print = generic_print,
  .print_all = generic_print_all,
};

#endif /* __GENERIC_H__ */
