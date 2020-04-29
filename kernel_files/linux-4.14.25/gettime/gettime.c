#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#include "gettime.h"

asmlinkage long sys_gettime(struct timespec *ts) // 385/333
{
  getnstimeofday(ts);
  return 0;
}
