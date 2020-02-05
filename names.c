#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define NAME_FILE "generic_names.h"
#define NUM_OF_LOGS 100

int
main(int argc, char *argv[])
{
  FILE *fp;
  unsigned char buf[BUF_SIZE] = {0};
  unsigned char *names[NUM_OF_LOGS] = {0};
  char *ptr, *tmp;
  char *name;
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
    name = (char *)malloc(len);
    memset(name, 0x0, len);
    memcpy(name, ptr, len);

    // value
    ptr = strtok(NULL, " ");
    len = strlen(ptr);
    if (ptr[len-1] = '\n')
      ptr[len-1] = 0;
    val = atoi(ptr);

    names[val] = name;
    printf("name: %s, val: %d\n", name, val);
  }

  fclose(fp);

  for (i=0; i<NUM_OF_LOGS; i++)
  {
    if (names[i])
    {
      printf("name[%d] = %s\n", i, names[i]);
    }
  }
  return 0;
}
