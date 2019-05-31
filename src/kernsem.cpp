/*
 * kernsem.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#include "kernsem.h"
#include "PCB.h"
#include "utils.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int init) : state(init) {}

int KernelSem::wait(Time maxTimeToWait) {
//	lock;
//	cout << "Wait called in thread " << PCB::running->id << ", state of semaphore: " << state << endl;
//	unlock;
	if (state > 0) {
		state--;
		return 0;
	} else {
		state--;
		PCB::running->paused = 1;
		waiting.add(PCB::running);
		dispatch();
	}
	return 0;
}

int KernelSem::signal(int n) {
	if (n == 0) n = 1;
//	lock;
//	cout << "Signal called in thread " << PCB::running->id << ", state of semaphore: " << state << endl;
//	unlock;
	while (state < 0 && n > 0) {
		PCB* tmp = waiting.pop();
		tmp->paused = 0;
		Scheduler::put(tmp);
		state++;
		n--;
	}
	if (n > 0) state += n;
	return 0;
}

int KernelSem::val() const {
	return state;
}
