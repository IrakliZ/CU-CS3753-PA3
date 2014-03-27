#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

#define MAXFILENAMELENGTH 80
#define DEFAULT_OUTPUTFILENAMEBASE "output/mixedrwoutput"
#define PI_ITERATIONS 10000
#define RADIUS (RAND_MAX / 2)
#define PROC_ITERATIONS 100

inline double dist(double x0, double y0, double x1, double y1){
	return sqrt(pow((x1-x0),2) + pow((y1-y0),2));
}

inline double zeroDist(double x, double y){
	return dist(0, 0, x, y);
}

int main(int argc, char* argv[]) {
	int process_iter;
	int iterations;

	long i;
	long j;
	double x, y;
	double inCircle = 0.0;
	double inSquare = 0.0;
	double pCircle = 0.0;
	double piCalc = 0.0;

	int rv;
	int outputFD;
	char buffer[40];
	char outputFilename[MAXFILENAMELENGTH];
	char outputFilenameBase[MAXFILENAMELENGTH];

	if(argc < 2) {
		process_iter = PROC_ITERATIONS;
	} else {
		process_iter = atol(argv[1]);
		printf("%d\n", process_iter);
		if(process_iter < 1){
			fprintf(stderr, "Bad overall iterations value\n");
			exit(EXIT_FAILURE);
		}
	}

	if(argc < 3) {
		iterations = PI_ITERATIONS;
	} else {
		iterations = atol(argv[2]);
		if(iterations < 1){
			fprintf(stderr, "Bad pi iterations value\n");
			exit(EXIT_FAILURE);
		}
	}

  /* Set up output files to default */
	strncpy(outputFilenameBase, DEFAULT_OUTPUTFILENAMEBASE, MAXFILENAMELENGTH);

  /* Open Output File Descriptor in Write Only mode with standard permissions*/
	rv = snprintf(outputFilename, MAXFILENAMELENGTH, "%s-%d",
		outputFilenameBase, getpid());
	if(rv > MAXFILENAMELENGTH){
		fprintf(stderr, "Output filenmae length exceeds limit of %d characters.\n",
			MAXFILENAMELENGTH);
		exit(EXIT_FAILURE);
	}
	else if(rv < 0){
		perror("Failed to generate output filename");
		exit(EXIT_FAILURE);
	}
	if((outputFD = open(outputFilename, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) < 0){
		perror("Failed to open output file");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < process_iter; i++) {
		for(j=0; j<iterations; j++){
			x = (random() % (RADIUS * 2)) - RADIUS;
			y = (random() % (RADIUS * 2)) - RADIUS;
			if(zeroDist(x,y) < RADIUS){
				inCircle++;
			}
			inSquare++;
		}

    /* Finish calculation */
		pCircle = inCircle/inSquare;
		piCalc = pCircle * 4.0;
		snprintf(buffer, 40, "The pi is calculated to be: %-10f\n", piCalc);
		write(outputFD, buffer, 39);
	}
    /* Close Output File Descriptor */
	if(close(outputFD)){
		perror("Failed to close output file");
		exit(EXIT_FAILURE);
	}

	return 0;
}
