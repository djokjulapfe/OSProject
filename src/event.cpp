/*
 * event.cpp
 *
 *  Created on: Jun 1, 2019
 *      Author: OS1
 */

#include "../h/Event.h"
#include "kernev.h"

Event::Event(IVTNo ivtNo) {
	myImpl = new KernelEv(ivtNo);
}

Event::~Event() {
	delete myImpl;
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}

