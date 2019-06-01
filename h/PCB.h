/*
 * PCB.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "list.h"

class KernelSem;

class PCB {
public:
	unsigned *stack;
	Thread *owner;
	KernelSem *waiter;
	unsigned ss;
	unsigned sp;
	unsigned finished;
	unsigned paused; // 0 - awake, 1 - sleeping, 2 - woke up
	int timeQuantum;
	int id;

	~PCB();

	static PCB* create_pcb(void (*body)(), unsigned int time_slice, unsigned long stack_size);
	static void wrapper();
	static PCB* running;
	static PCB* mainPCB;
	static PCB* waitPCB;
	static void wait_for_sleepers();
	static unsigned maxId;
};

extern PCBList sleeping;

#endif /* PCB_H_ */
