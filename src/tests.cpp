/*
 * tests.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include "tests.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "utils.h"

void test_all() {
	test_dispatch_system();
	test_thread();
	test_semaphore();
	test_events();
	test_signals();
}

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

PCB* p[3];

void test_dispatch_system() {
	lock;
	cout << "Testing dispatch system.\n";
	unlock;

	inic();
	lock;

	p[1] = PCB::create_pcb(a, 4, 4096);

	p[2] = PCB::create_pcb(b, 2, 4096);

	p[0] = new PCB();

	Scheduler::put(p[1]);
	Scheduler::put(p[2]);

	running = mainPCB = p[0];

	unlock;

	for (int i = 0; i < 10; i++) {
		lock;
		cout << "in main(), i = " << i << endl;
		unlock;
		delay(100);
	}

	restore();

}

void test_thread() {}

void test_semaphore() {}

void test_events() {}

void test_signals() {}

