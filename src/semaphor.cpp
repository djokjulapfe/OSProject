
/*
 * semaphor.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernsem.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->val();
}


