#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#include "printtime.h"

asmlinkage long sys_printtime(char *str, int pid, struct timespec *start, struct timespec *end)  // 386/334
{
  printk("[%s] %d %09lu.%09lu %09lu.%09lu\n", str, pid, start->tv_sec, start->tv_nsec, end->tv_sec, end->tv_nsec);
  return 0;
}
