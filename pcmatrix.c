/*
	Assignment 3
	12/2/2016
	Tony Lee
	
 *  pcmatrix module
 *  Primary module providing control flow for the pcMatrix program
 * 
 *  Creates producer and consumer threads and launches them to produce matrices
 *  and consume matrices.  Each thread produces a total sum of the value of
 *  randomly generated elements.  Producer sum and consumer sum must match.
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "counter.h"
#include "prodcons.h"
#include "pcmatrix.h"

int main (int argc, char * argv[])
{
	time_t t;
	int numw = NUMWORK;
	int *producerSum[numw];
	int *consumerSum[numw];
	int prodTotal = 0, consTotal = 0;
	int rc, i;
	counter_t theProdCounter;
	counter_t theConsCounter;
	init(&theProdCounter);
	init(&theConsCounter);
	pthread_t prod[numw];
	pthread_t cons[numw];

	// Seed the random number generator with the system time
	srand((unsigned) time(&t));

	printf("Producing %d %dx%d matrices.\n",LOOPS, ROW, COL);
	printf("Using a shared buffer of size=%d\n", MAX);
	printf("With %d producer and consumer threads.\n",numw);

	for(i = 0; i < numw; i++) {
		rc = pthread_create(&prod[i], NULL, prod_worker, (void *)&theProdCounter);
		if (rc != 0) {printf("prod[%d] error\n", i);}
		rc = pthread_create(&cons[i], NULL, cons_worker, (void *)&theConsCounter);
		if (rc != 0) {printf("cons[%d] error\n", i);}
	}
	
	for(i = 0; i < numw; i++) {
		pthread_join(prod[i], (void*)&producerSum[i]);
		pthread_join(cons[i], (void*)&consumerSum[i]);
		prodTotal += *producerSum[i];
		consTotal += *consumerSum[i];
	}
	printf("Produced = Consumed --> %d = %d\n", prodTotal, consTotal);
	return 0;
}
