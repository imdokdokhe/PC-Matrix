/*
	Assignment 3
	12/2/2016
	Tony Lee
    
 *  counter header
 *  Function prototypes, data, and constants for synchronized counter module
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Fall 2016
 */

// SYNCHRONIZED COUNTER

// counter struct
typedef struct __counter_t {
	int value;
	pthread_mutex_t lock;
} counter_t;

// counter methods
void init(counter_t *c);
void increment(counter_t *c);
void decrement(counter_t *c);
int getCount(counter_t *c);

