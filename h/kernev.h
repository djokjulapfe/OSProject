/*
 * KernelEv.h
 *
 *  Created on: Jun 1, 2019
 *      Author: OS1
 */

#ifndef H_KERNEV_H_
#define H_KERNEV_H_

#include "PCB.h"
#include "kernsem.h"

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class IVTEntry;
extern IVTEntry* entryList;
extern volatile int waitingSwitch;

class KernelEv {
public:
	KernelEv(IVTNo number);
	~KernelEv();

	void signal();
	void wait();

private:
	KernelSem* mySemaphore;
	PCB* myPCB;
	IVTNo ivtNumber;
};

class IVTEntry {
public:
	IVTEntry(IVTNo number, pInterrupt newRoutine);

	IVTNo ivtNumber;
	pInterrupt oldInterrupt;
	pInterrupt newInterrupt;
	KernelEv* myEvent;
	IVTEntry* nextNode;
};

#define PREPAREENTRY(number, flag)\
void interrupt interrupt##number (...);\
IVTEntry* ivtEntry##number = new IVTEntry(number, interrupt##number);\
void interrupt interrupt##number (...) {\
	if (flag) {\
		ivtEntry##number->oldInterrupt();\
	}\
	ivtEntry##number->myEvent->signal();\
}

#endif /* H_KERNEV_H_ */
