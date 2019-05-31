/*
 * kernsem.h
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_

#include "thread.h"
#include "list.h"

typedef unsigned Time;

class KernelSem {
public:
	KernelSem(int init = 1);
//	virtual ~KernelSem();

	virtual int wait(Time maxTimeToWait = 0);
	virtual int signal(int n = 0);

	int val() const;

private:
	PCBList waiting;
	volatile int state;
};



#endif /* H_KERNSEM_H_ */
