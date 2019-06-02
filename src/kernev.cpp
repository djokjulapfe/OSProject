/*
 * kernev.cpp
 *
 *  Created on: Jun 1, 2019
 *      Author: OS1
 */

#include <dos.h>
#include "kernev.h"

KernelEv::KernelEv(IVTNo number) : ivtNumber(number) {
	mySemaphore = new KernelSem(0);
	myPCB = PCB::running;
	IVTEntry* head = entryList;
	for (;head != 0; head = head->nextNode) {
		if (head->ivtNumber == ivtNumber) {
			lock;
			head->myEvent = this;
			head->oldInterrupt = getvect(ivtNumber);
			setvect(ivtNumber, head->newInterrupt);
			unlock;
			break;
		}
	}
}

KernelEv::~KernelEv() {
	IVTEntry* head = entryList;
	for (;head != NULL; head = head->nextNode) {
		if (head->ivtNumber == ivtNumber) {
			lock;
			setvect(ivtNumber, head->oldInterrupt);
			unlock;
			break;
		}
	}
}

void KernelEv::signal() {
	lock;
	if (mySemaphore->val() < 1) {
		mySemaphore->signal();
	}
	dispatch();
	unlock;
}

void KernelEv::wait() {
	if (myPCB == PCB::running) {
		mySemaphore->wait();
	}
}

IVTEntry::IVTEntry(IVTNo number, pInterrupt newRoutine) : ivtNumber(number), newInterrupt(newRoutine), myEvent(0), nextNode(0), oldInterrupt(0) {
	if (entryList == 0) {
		entryList = this;
	}
	else {
		IVTEntry* head = entryList;
		for(;head->nextNode != 0; head = head->nextNode);
		head->nextNode = this;
	}
}

