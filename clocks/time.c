#include <stdio.h>      /* puts, printf */
#include <time.h>       /* time_t, struct tm, time, gmtime */


int main ()
{
  time_t rawtime;
  struct tm * ptm;
  time ( &rawtime );
  ptm = gmtime ( &rawtime );
  printf (" %2d:%02d\n", (ptm->tm_hour+3)%24, ptm->tm_min);
  return 0;
}
