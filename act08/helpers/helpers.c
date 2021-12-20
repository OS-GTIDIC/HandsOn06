#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "helpers.h"

int getRandom (int val)
{
  return (rand () % val);
}

void logger(const char* tag, const char* message) {
   time_t now;
   time(&now);
   FILE* f = fopen(LOG_FILE,"a");
   fprintf(f,"%s [%s]: %s", ctime(&now), tag, message);
   fflush(stdout);
   fclose(f);
}