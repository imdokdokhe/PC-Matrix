/*
	Assignment 3
	12/2/2016
	Tony Lee
	
 *  prodcons module
 *  Producer Consumer module
 * 
 *  Implements routines for the producer consumer module based on 
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"

// Producer consumer data structures
int ** bigmatrix[MAX];
int ready = 0;
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

// Producer consumer locks and conditions
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

// PRODUCER-CONSUMER put() get()

//puts matricies into the buffer and adds to count.
void put(int ** value)
{
	bigmatrix[fill_ptr] = value;
	fill_ptr = (fill_ptr + 1) % MAX;
	count++;
}

//gets matricies from the buffer and subtracts from count.
int ** get() 
{
	int ** tmp = bigmatrix[use_ptr];
	use_ptr = (use_ptr + 1) % MAX;
	count--;
    return tmp;
}

// PRODUCER - return total of all arrays produced
void *prod_worker(void *arg)
{
	int *sum = malloc(sizeof(int));
	counter_t *c = (counter_t *) arg;
    while (getCount(c) < LOOPS)
    {
        pthread_mutex_lock(&mutex);
        while (count == MAX) {
            pthread_cond_wait(&empty, &mutex);
        } 
        //create big matrix
        int gridCount = getCount(c) % MAX;
        bigmatrix[gridCount] = AllocMatrix(ROW, COL);
        GenMatrix(bigmatrix[gridCount], ROW, COL);
        put(bigmatrix[gridCount]);
        
		if(getCount(c) < LOOPS) {
			*sum += SumMatrix(bigmatrix[gridCount], ROW, COL);
		} else {
			SumMatrix(bigmatrix[gridCount], ROW, COL);
		}
        increment(c);
        // signal parent
           
        
        pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex); 
    }
    
    return sum;
}

// CONSUMER - return total of all arrays consumed
void *cons_worker(void *arg)
{
	int *sum = malloc(sizeof(int));
    counter_t *c = (counter_t *) arg;
    while (getCount(c) < LOOPS)
    {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            pthread_cond_wait(&fill, &mutex);
        }
        
        int ** tmp;
	    tmp = get();
	    
		if(getCount(c) < LOOPS) {
        	*sum += SumMatrix(tmp, ROW, COL);
		} else {
			SumMatrix(tmp, ROW, COL);
		}
        FreeMatrix(tmp, ROW, COL); 
    	increment(c);
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
    return sum;
}

