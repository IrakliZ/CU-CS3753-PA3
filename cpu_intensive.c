#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_FORKS 5
#define DEFAULT_ITERATIONS 1000000

int main(int argc, char* argv[]) {
	int policy;
	int forks;
	int iterations;
	struct sched_param param;
	int i;
	int pid;

	if (argc < 2) {
		iterations = DEFAULT_ITERATIONS;
	}

	if (argc < 3) {
		policy = SCHED_OTHER;
	}

	if (argc < 4) {
		forks = NUM_FORKS;
	}

	if(argc > 1){
		iterations = atol(argv[1]);
		if(iterations < 1){
			fprintf(stderr, "Bad iterations value\n");
			exit(EXIT_FAILURE);
		}
	}

	if(argc > 2){
		if(!strcmp(argv[2], "SCHED_OTHER")){
			policy = SCHED_OTHER;
		}
		else if(!strcmp(argv[2], "SCHED_FIFO")){
			policy = SCHED_FIFO;
		}
		else if(!strcmp(argv[2], "SCHED_RR")){
			policy = SCHED_RR;
		}
		else{
			fprintf(stderr, "Unhandeled scheduling policy\n");
			exit(EXIT_FAILURE);
		}
	}

	if(argc > 3){
		forks = atol(argv[3]);
		if(forks < 1){
			fprintf(stderr, "Bad forks value\n");
			exit(EXIT_FAILURE);
		}
	}

	param.sched_priority = sched_get_priority_max(policy);

	/* Set new scheduler policy */
	fprintf(stdout, "Current Scheduling Policy: %d\n", sched_getscheduler(0));
	fprintf(stdout, "Setting Scheduling Policy to: %d\n", policy);
	if(sched_setscheduler(0, policy, &param)){
		perror("Error setting scheduler policy");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "New Scheduling Policy: %d\n", sched_getscheduler(0));

	for (i = 0; i < forks; i++) {
		if ((pid = fork()) == -1) {
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			execl("pi", "pi", argv[1], NULL);
			exit(EXIT_SUCCESS);
		} else {
			printf("%d: pid: %d\n", i, pid);
		}
	}

	for (i = 0; i < forks; ++i) {
		pid = wait(NULL);
		printf("%d: waiting pid: %d\n", i+1, pid);
	}

	return 0;

}
