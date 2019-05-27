/*
 * PCB.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

struct PCB {
	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned finished;
	int timeQuantum;
	unsigned id;

	// TODO: check if stack_size is a thread_create argument
	static unsigned long default_stack_size;

	static PCB* create_pcb(void (*body)(), unsigned int time_slice, unsigned long stack_size);

	~PCB();
};

#endif /* PCB_H_ */
