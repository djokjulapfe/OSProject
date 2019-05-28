/*
 * main.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include "utils.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "tests.h"

PCB* running;
PCB* mainPCB;

volatile unsigned cswitch_demanded;
volatile unsigned cntr = 5;

unsigned oldTimerSeg, oldTimerOff;

void doSomething() {
}

int main() {
	test_all();

	cout << "Successfully finished!" << endl;
	return 0;
}
