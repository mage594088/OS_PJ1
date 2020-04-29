#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sched.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/syscall.h>

#include "run.h"

void gettime(struct timespec *ts) {
	syscall(__NR_gettime, ts);
	return;
}
void printtime(char *str, int pid, struct timespec *start, struct timespec *end) {
	syscall(__NR_printtime, str, pid, start, end);
	return;
}

struct timespec timediff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if (end.tv_nsec < start.tv_nsec) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}

void time_msg(int pid, struct timespec start, struct timespec end) { // Show time info of a process
	struct timespec diff = timediff(start, end);
	fprintf(stderr, "%d %09lu.%09lu %09lu.%09lu %02lu.%09lu\n", pid,
	        (start).tv_sec, (start).tv_nsec, (end).tv_sec, (end).tv_nsec, (diff).tv_sec, (diff).tv_nsec);
}

// Different Scheduling
void RunFIFO(Process *p, ProcessTime *ts) {
#ifdef DEBUG
	struct sched_param _tmp;
	sched_getparam(0, &_tmp);
	printf"%s Priority: %d\n", p->name, _tmp.sched_priority);
#endif
	RUN(p->execution_time);
	gettime(&(ts->end));
	printtime("Project1", getpid(), &(ts->start), &(ts->end));
	printf("%s %d ENDS\n", p->name, getpid());
}

void RunPSJF(Process *p, ProcessTime *ts) {
	RunFIFO(p, ts);
}

void RunRR(Process *p, ProcessTime *ts) {
#ifdef DEBUG
	struct sched_param _tmp;
	sched_getparam(0, &_tmp);
	printf("%s Priority: %d\n", p->name, _tmp.sched_priority);
#endif
	while (p->execution_time > TIME_QUANTUM) {
		RUN(TIME_QUANTUM);
		p->execution_time -= TIME_QUANTUM;
		printf("%s %d yields\n", p->name, getpid());
		if (sched_yield() == -1) {
			fprintf(stderr, "sched_yield error: %s\n", strerror(errno));
			_exit(1);
		}
	}
	RUN(p->execution_time);
	gettime(&(ts->end));
	printtime("Project1", getpid(), &(ts->start), &(ts->end));
	printf("%s %d ENDS\n", p->name, getpid());
}

void RunSJF(Process *p, ProcessTime *ts) {
#ifdef DEBUG
	struct sched_param _tmp;
	sched_getparam(0, &_tmp);
	printf("%s Priority: %d\n", p->name, _tmp.sched_priority);
#endif
	struct sched_param _param;
	_param.sched_priority = sched_get_priority_max(SCHED_FIFO);
	sched_setparam(0, &_param);  // Set to max priority to prevent preemption
	RUN(p->execution_time);
	gettime(&(ts->end));
	printtime("Project1", getpid(), &(ts->start), &(ts->end));
	printf("%s %d ENDS\n", p->name, getpid());
}