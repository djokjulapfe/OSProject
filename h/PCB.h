/*
 * PCB.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"

class PCB {
public:
	unsigned *stack;
	Thread* owner;
	unsigned ss;
	unsigned sp;
	unsigned finished;
	int timeQuantum;
	int id;

	~PCB();

	static PCB* create_pcb(void (*body)(), unsigned int time_slice, unsigned long stack_size);
	static void wrapper();
	static PCB* running;
	static PCB* mainPCB;
	static unsigned maxId;
};

#endif /* PCB_H_ */
