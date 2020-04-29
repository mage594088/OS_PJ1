#ifndef _RUN_H_
#define _RUN_H_

#define __NR_gettime 333
#define __NR_printtime 334

#define TIME_QUANTUM 500

#ifdef DEBUG
#define MSG(string, args...) fprintf(stderr, string, ## args)
#else
#define MSG(string, args...)
#endif

#define RUN(t)										\
	do {											\
		for (int _i = 0; _i < (t); _i++) {			\
			volatile unsigned long _j;				\
			for (_j = 0; _j < 1000000UL; _j++) {}	\
		} 											\
	} while (0)

typedef struct process {
	char name[32];
	int ready_time;
	int execution_time;
	pid_t pid;
} Process;

typedef struct {
	struct timespec start;
	struct timespec end;
} ProcessTime;

typedef enum {
    FIFO,
    PSJF,
    RR,
    SJF
} SCHED_POLICY;

void gettime(struct timespec *ts);
void printtime(char *str, int pid, struct timespec *start, struct timespec *end);
struct timespec timediff(struct timespec start, struct timespec end);
void time_msg(int pid, struct timespec start, struct timespec end);

void RunFIFO(Process *p, ProcessTime *ts);
void RunPSJF(Process *p, ProcessTime *ts);
void RunRR(Process *p, ProcessTime *ts);
void RunSJF(Process *p, ProcessTime *ts);

#endif