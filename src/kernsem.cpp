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
//	cout << "Wait called in thread " << PCB::running->id << ", state of semaphore: " << state  << ", maxTimeToWait: " << maxTimeToWait<< endl;
//	unlock;
	if (state > 0) {
		state--;
		return 1;
	} else {
		state--;
		PCB::running->paused = 1;
		waiting.add(PCB::running);
		if (maxTimeToWait != 0) {
			sleeping.add_sleeper(PCB::running, maxTimeToWait);
//			lock;
//			cout << "Added to sleeping\n";
//			cout << "Current sleeping status: " << sleeping.has_head() << endl;
//			unlock;
		}
		dispatch();
//		cout << "Returning from sleep, status is " << PCB::running->paused << endl;
		if (PCB::running->paused == 2) return 0;
		else return 1;
	}
}

int KernelSem::signal(int n) {
	// TODO: test this
	int ret = 0;
	if (n > 0) {
		if (state < 0) {
			if (state + n > 0) ret = -state;
			else ret = n;
		}
	} else if (n < 0) ret = n;
	if (n == 0) {
		n = 1;
	}
//	lock;
//	cout << "Signal called in thread " << PCB::running->id << ", state of semaphore: " << state << endl;
//	unlock;
	while (state < 0 && n > 0) {
		PCB* tmp = waiting.pop();
		if (tmp->paused == 1) {
			sleeping.remove(tmp);
		}
		tmp->paused = 0;
		Scheduler::put(tmp);
		state++;
		n--;
	}
	if (n > 0) state += n;
//	lock;
//	cout << "New state of semaphore: " << state << endl;
//	unlock;
	return ret;
}

int KernelSem::val() const {
	return state;
}
