/*
 * main.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "PCB.h"
#include "utils.h"

PCB* p[3];
PCB* running;
PCB* mainPCB;

volatile unsigned cswitch_demanded;
volatile unsigned cntr;

unsigned oldTimerSeg, oldTimerOff;

unsigned Na = 10;
unsigned Nb = 10;
unsigned Da = 100;
unsigned Db = 100;

void a() {
	for (int i = 0; i < Na; i++) {
		lock;
		cout << "in a(), i = " << i << endl;
		unlock;
		delay(Da);
	}
	exit_thread();
}

void b() {
	for (int i = 0; i < Nb; i++) {
		lock;
		cout << "in b(), i = " << i << endl;
		unlock;
		delay(Db);
	}
	exit_thread();
}

void doSomething() {
	lock;

	p[1] = PCB::create_pcb(a, 4, 100);
	p[2] = PCB::create_pcb(b, 2, 100);

	p[0] = new PCB();
	running = p[0];

	unlock;

	for (int i = 0; i < 10; i++) {
		lock;
		cout << "in main(), i = " << i << endl;
		unlock;
		delay(300);
	}
}

int main() {
	running = 0;

	inic();

	doSomething();

	restore();

	cout << "Successfully finished!" << endl;
	delete p[1];
	delete p[2];
	cout << "Memory cleared!" << endl;
	return 0;
}
