/*
 * thread.cpp
 *
 *  Created on: May 28, 2019
 *      Author: OS1
 */

#include "thread.h"
#include "SCHEDULE.H"
#include "PCB.h"
#include "utils.h"
#include "list.h"

Thread::Thread(StackSize stackSize, Time timeSlice) : myPCB(0) {
	lock;
	myPCB = PCB::create_pcb(PCB::wrapper, timeSlice, stackSize);
	unlock;
	myPCB->owner = this;
	threads.add(this);
}

void Thread::start() {
	Scheduler::put(myPCB);
}

void Thread::waitToComplete() {
	while(!myPCB->finished);
}

Thread::~Thread() {
	waitToComplete();
	threads.remove(this);
	delete myPCB;
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread *getThreadById(ID id) {
	// TODO: implement this
	return 0;
}
