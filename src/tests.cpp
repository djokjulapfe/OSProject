/*
 * tests.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#include "tests.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "utils.h"
#include "list.h"
#include "thread.h"

int userMain(int argc, const char* argv[]) {
	test_all();
	lock;
	cout << threads.count() << endl;
	unlock;
	getchar();
	return 0;
}

void test_all() {

//	test_dispatch_system();
//	test_dispatch_system();
	test_thread();
	test_semaphore();

	test_events();
	test_signals();
	// TODO: add test_deadlock();
}

unsigned Na = 10;
unsigned Nb = 10;
unsigned Da = 100;
unsigned Db = 100;

void test_a() {
	for (int i = 0; i < Na; i++) {
		lock;
		cout << "in a(), i = " << i << endl;
		unlock;
		delay(Da);
	}
	exit_thread();
}

void test_b() {
	for (int i = 0; i < Nb; i++) {
		lock;
		cout << "in b(), i = " << i << endl;
		unlock;
//		delay(Db);
		dispatch();
	}
	exit_thread();
}

PCB* test_p[2];

void test_dispatch_system() {
	lock;
	cout << "Testing dispatch system.\n";

	running_countdown = 5;

	test_p[0] = PCB::create_pcb(test_a, 4, 1024);

	test_p[1] = PCB::create_pcb(test_b, 2, 1024);

	Scheduler::put(test_p[0]);
	Scheduler::put(test_p[1]);

	unlock;

	for (int i = 0; i < 20; i++) {
		lock;
		cout << "in main(), i = " << i << endl;
		unlock;
		delay(100);
	}

	delete test_p[0];
	delete test_p[1];
}

class TestThreadA : public Thread {
public:
	TestThreadA(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run();
};

TestThreadA::TestThreadA(StackSize stackSize, Time timeSlice) : Thread(stackSize, timeSlice) {
}

void TestThreadA::run() {
	for (int i = 0; i < 10; i++) {
		lock;
		cout << "#" << i << " - Hello from thread " << getId() << "!\n";
		unlock;
		delay(100);
	}
}

class TestThreadCount : public Thread {
public:
	TestThreadCount(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run();
};

TestThreadCount::TestThreadCount(StackSize stackSize, Time timeSlice) : Thread(stackSize, timeSlice) {
}

void TestThreadCount::run() {
	for (int i = 0; i < 100; i++) {
		lock;
		cout << "There are currently " << threads.count() << " active threads!\n";
		threads.print_state();
		cout << "Thread 5 ";
		if (Thread::getThreadById(5) == 0)
			cout << "not found";
		else
			cout << "found";
		cout << endl;
		unlock;
		dispatch();
	}
}

void test_thread() {
	lock;
	cout << "Testing threads.\n";
	running_countdown = 0;
	cout << "countdown: " << running_countdown  << endl;

	running_countdown = 5;

	unlock;

	TestThreadCount *count = new TestThreadCount;
	count->start();

	TestThreadA *a, *b;
	a = new TestThreadA;
	b = new TestThreadA;
	a->start();
	b->start();

	lock;
	cout << "Hello from main! - General test\n";
	unlock;

	delay(4000);

	lock;
	cout << "Goodbye from main!\n";
	unlock;

	delete a;

	threads.print_state();
	cout << endl;

	delete b;

	threads.print_state();
	cout << endl;

	a = new TestThreadA;

	lock;
	cout << "Hello from main! - waitToComplete test\n";
	unlock;

	a->start();

	a->waitToComplete();

	lock;
	cout << "Goodbye from main!\n";
	unlock;

	delete a;

	a = new TestThreadA(2048, 4);
	b = new TestThreadA(2048, 1);
	TestThreadA *c = new TestThreadA(2048, 0);

	lock;
	cout << "Hello from main! - Time sharing test\n";
	unlock;

	a->start();
	b->start();

	a->waitToComplete();

	c->start();

	delete a;
	delete b;
	delete c;
	delete count;

	lock;
	cout << "Goodbye from main!\n";
	unlock;
}

void test_semaphore() {}

void test_events() {}

void test_signals() {}

