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
#include "kernsem.h"

int userMain(int argc, const char* argv[]) {
	test_all();
	return 0;
}

void tick() {
//	lock;
//	cout << "t\n";
//	unlock;
}

void test_all() {

	test_PCB();
	test_thread();
	test_kernelsem();
	test_semaphore();
	test_events();
	test_signals();
	test_deadlock();
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
//		delay(Db);k4
		dispatch();
	}
	exit_thread();
}

PCB* test_p[2];

void test_PCB() {
	lock;
	cout << "Testing PCB and dispatch system.\n";

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

KernelSem test_ksem_producer(0), test_ksem_consumer(2), test_ksem_main(0);

int test_buffer[2];
int test_buffer_index;

void test_producer() {
	for (int i = 0; i < 9; i++) {
//		lock;
//		cout << "Producer " << PCB::running->id << " waiting\n";
//		unlock;
		test_ksem_consumer.wait();
		lock;
		test_buffer[test_buffer_index] = i;
		test_buffer_index = (test_buffer_index + 1) % 2;
		cout << "Produced " << i << endl;
		test_ksem_producer.signal();
		unlock;
	}
	lock;
	cout << "A producer finished\n";
	unlock;
	exit_thread();
}

void test_consumer() {
	int local_sum;
	for (int i = 0; i < 10; i++) {
		lock;
		cout << "Consumer " << PCB::running->id << " waiting\n";
		unlock;
		if (test_ksem_producer.wait(2) == 0) cout << "Waited too long!\n";
		if (test_ksem_producer.wait(2) == 0) cout << "Waited too long!\n";
		lock;
		local_sum += test_buffer[0] + test_buffer[1];
		cout << "Consumed\n";
		unlock;
		test_ksem_consumer.signal(2);
	}
	lock;
	cout << "The sum is " << local_sum << endl;
	unlock;
	test_ksem_main.signal();
	exit_thread();
}

void test_kernelsem() {
	lock;
	cout << "Testing kernel level semaphores.\n";

	running_countdown = 1;

	PCB *p1, *p2, *c;
	p1 = PCB::create_pcb(test_producer, 2, 4096);
	p2 = PCB::create_pcb(test_producer, 2, 4096);
	c = PCB::create_pcb(test_consumer, 2, 4096);

	cout << "p1, p2, c = " << p1->id << ", " << p2->id << ", " << c->id << endl;

	Scheduler::put(p1);
	Scheduler::put(p2);
	Scheduler::put(c);

	unlock;

	lock;
	cout << "Main waiting on main semaphore\n";
	unlock;
	test_ksem_main.wait();
	lock;
	cout << "Someone called test_ksem_main.signal()\n";
	cout << "Totoal time " << total_time << endl;
	unlock;

	delete p1;
	delete p2;
	delete c;
}

void test_semaphore() {}

void test_events() {}

void test_signals() {}

void test_deadlock() {}

