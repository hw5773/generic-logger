#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include "generic.h"
#include "debug.h"

#define DEFAULT_LOG_PREFIX "log"

int
usage(const char *pname)
{
  emsg(">> Usage: %s -l <log prefix>", pname);
  emsg(">> Example: %s -l test_log", pname);
  exit(1);
}

int
main(int argc, char *argv[])
{
  imsg("Logger test start");

  int c;
  const char *pname;
  const char *log_prefix;
  logger_t *logger;

  pname = argv[0];
  log_prefix = NULL;

  while (1)
  {
    int opt = 0;
    static struct option long_options[] = {
      {"log", required_argument, 0, 'l'},
      {0, 0, 0, 0}
    };

    c = getopt_long(argc, argv, "l:0", long_options, &opt);

    if (c == -1)
      break;

    switch (c)
    {
      case 'l':
        log_prefix = optarg;
        break;

      default:
        usage(pname);
    }
  }

  if (!log_prefix)
    log_prefix = DEFAULT_LOG_PREFIX;

  logger = init_logger(log_prefix, HPC_LF_ALL);
  sleep(1);
  logger->ops->add(logger, HPC_LT_INIT);
  sleep(3);
  logger->ops->add(logger, HPC_LT_FIN);
  logger->ops->interval(logger, HPC_LT_INIT, HPC_LT_FIN);

  logger->ops->print_all(logger);
  fin_logger(logger);
  imsg("Logger test finished");
  return 0;
}
