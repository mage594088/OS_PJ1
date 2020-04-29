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

int main (int argc, char *argv[]) {
	// Read test data by file or typing on keyboard
	FILE *fp;
	if (argc != 1){		
		fp=fopen(argv[1], "r");
	}
	else{
		fp=stdin;
	}
		
	// Main process runs on the second core
	struct sched_param param;
	param.sched_priority=sched_get_priority_max(SCHED_FIFO);
	if (sched_setscheduler(0, SCHED_FIFO, &param)==-1) {
		fprintf(stderr, "Scheduling policy error: %s.\n", strerror(errno));
		exit(1);
	}

	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);
	sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set);
	if (CPU_COUNT(&cpu_set)<2) {
		fprintf(stderr, "Can't run on single core.\n");
		exit(1);
	}
	CPU_ZERO(&cpu_set);
	CPU_SET(1, &cpu_set);
	if (sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set) != 0) {
		fprintf(stderr, "Sched_setaffinity error: %s.\n", strerror(errno));
		exit(1);
	}

	// Read scheduling policy
	char policy_str[8];  
	if (fscanf(fp, "%s", policy_str)==EOF) {
		fprintf(stderr, "Scanf error.\n");
		exit(1);
	}
	SCHED_POLICY sched_policy;
	if (strcmp(policy_str, "FIFO")==0) {
		sched_policy=FIFO;
	} else if (strcmp(policy_str, "PSJF")==0) {
		sched_policy=PSJF;
	} else if (strcmp(policy_str, "RR")==0) {
		sched_policy=RR;
	} else if (strcmp(policy_str, "SJF")==0) {
		sched_policy=SJF;
	} else {
		fprintf(stderr, "Wrong scheduling policy.\n");
		exit(0);
	}

	// Number of processes
	int n, i, j, k;; 
	if (fscanf(fp, "%d", &n)==EOF) {
		fprintf(stderr, "Scanf error.\n");
		exit(1);
	}

	// Child processes
	Process* process=malloc(n * sizeof(Process)); 
	ProcessTime* process_time_list=mmap (NULL, n * sizeof(ProcessTime), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (process==NULL) {
		fprintf(stderr, "Malloc error.\n");
		exit(1);
	}
	if (process_time_list==MAP_FAILED) {
		fprintf(stderr, "Mmap error: %s.\n", strerror(errno));
		exit(1);
	}
	for (i=0; i<n; i++) {
		if (fscanf(fp, "%s%d%d", process[i].name, &(process[i].ready_time), &(process[i].execution_time))==EOF) {
			fprintf(stderr, "Scanf error.\n");
			exit(1);
		}
		MSG("%s\t%d\t%d.\n", process[i].name, process[i].ready_time, process[i].execution_time);
	}

	// Sorting according to ready time by selection sort
	for (i=0; i<n; i++){
		for(j=i+1, k=i; j<n; j++)
			if(process[j].ready_time<process[k].ready_time)
				k=j;
		typeof(process[k]) tmp=process[k];
		process[k]=process[i];
		process[i]=tmp;
	}

	// Check sorting
	for (i=0; i<n; i++)
		MSG("%s\t%d\t%d.\n", process[i].name, process[i].ready_time, process[i].execution_time);	

	// Fork
	for (i=0; i<n; i++) {
		process[i].pid=fork();
		// Fork Error
		if (process[i].pid<0) {
			fprintf(stderr, "Fork error: %s.\n", strerror(errno));
			exit(1);
		}
		// Child process
		else if (process[i].pid==0) {
			switch (sched_policy) {
			case FIFO:
				RunFIFO(&(process[i]), &(process_time_list[i]));
				break;
			case PSJF:
				RunPSJF(&(process[i]), &(process_time_list[i]));
				break;
			case RR:
				RunRR(&(process[i]), &(process_time_list[i]));
				break;
			case SJF:
				RunSJF(&(process[i]), &(process_time_list[i]));
				break;			
			default:
				break;
			}
			_exit(0);
		}
		// Parent process
		else {
			// Set the child processes to min priority
			printf("%s %d\n", process[i].name, process[i].pid);
			param.sched_priority=sched_get_priority_min(SCHED_FIFO);
			if (sched_setparam(process[i].pid, &param)==-1) {
				fprintf(stderr, "Min priority error: %s.\n", strerror(errno));
				exit(1);
			}
		}
	}

	// Child process priorities by scheduling policy
	int priority=sched_get_priority_min(SCHED_FIFO);
	// Without priority
	if (sched_policy==FIFO || sched_policy==RR) { 
		for (i=n-1; i>=0; i--) {
			param.sched_priority=priority;
			if (sched_setparam(process[i].pid, &param)==-1) {
				fprintf(stderr, "Scheduling priority error: %s.\n", strerror(errno));
				exit(1);
			}
		}
	}
	// With priority
	else if (sched_policy==PSJF || sched_policy==SJF) { 
		Process **process_ptr=malloc(n * sizeof(Process*));
		for (i=0; i<n; i++)
			process_ptr[i]=&(process[i]);
		
		// Sorting according to execution time by selection sort
		for (i=0; i<n; i++){
			for(j=i+1, k=i; j<n; j++)
				if(process_ptr[j]->execution_time<process_ptr[k]->execution_time)
					k=j;
			typeof(process_ptr[k]) tmp=process_ptr[k];
			process_ptr[k]=process_ptr[i];
			process_ptr[i]=tmp;
		}
		
		for (i=n-1; i>=0; i--) {
			// Same execution time has same priority
			if (i<n-1 && process_ptr[i]->execution_time<process_ptr[i+1]->execution_time)
				priority++;
			param.sched_priority=priority;
			MSG("Process: %s, execution_time: %d, priority: %d.\n",
			    process_ptr[i]->name, process_ptr[i]->execution_time, param.sched_priority);
			if (sched_setparam(process_ptr[i]->pid, &param)==-1) {
				fprintf(stderr, "Set scheduling priority error: %s.\n", strerror(errno));
				exit(1);
			}
		}
		free(process_ptr);
	}

	int time=0, max_ready_time=0;
	// Get max ready time so we know when to stop
	for (i=0; i<n; i++) 
		if (process[i].ready_time > max_ready_time)
			max_ready_time=process[i].ready_time;

	// Loop as time increases
	while (time<=max_ready_time) {
		// Check if there are processes ready
		for (i=0; i<n; i++) {
			if (process[i].ready_time==time) { // READY!
				// Move the child process to run on the first core
				CPU_ZERO(&cpu_set);
				CPU_SET(0, &cpu_set);
				if (sched_setaffinity(process[i].pid, sizeof(cpu_set_t), &cpu_set) != 0) {
					fprintf(stderr, "Sched_setaffinity error: %s\n", strerror(errno));
					exit(1);
				}
				// Get the starting time of child process
				gettime(&(process_time_list[i].start));
				MSG("%s %d STARTS.\n", process[i].name, process[i].pid);
			}
		}
		RUN(1);
		time++;
	}

	for (i=0; i<n; i++) {
		waitpid(process[i].pid, NULL, 0);
	}
#ifdef PRINT_TIME
	for (i=0; i<n; i++) {
		time_msg(process[i].pid, process_time_list[i].start, process_time_list[i].end);
	}
#endif
	free(process);
	fclose(fp);

	return 0;
}