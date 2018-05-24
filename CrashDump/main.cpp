
#include "CrashLog.h"

int main(void)
{
  CRASH_DUMP

  int a = 0;
  int i = 1 / a;

  return 0;
}
