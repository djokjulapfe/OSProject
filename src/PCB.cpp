/*
 * PCB.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include <dos.h>
#include "PCB.h"
#include "utils.h"
#include <iostream.h>
#include <stdio.h>

//void PCB::create_process(PCB *newPCB, void (*body)()) {
//	unsigned *st = new unsigned[PCB::default_stack_size];
//	st[1023] = 0x200; // Starting PSW
//
//	st[1022] = FP_SEG(body);
//	st[1021] = FP_OFF(body);
//
//	newPCB->ss = FP_SEG(st + 1012);
//	newPCB->sp = FP_OFF(st + 1012);
//
//	newPCB->finished = 0;
//	newPCB->stack = st;
//}

PCB* PCB::mainPCB = 0;
PCB* PCB::running = 0;
unsigned PCB::maxId = 0;

PCB::~PCB() {
	delete[] stack;
}

PCB* PCB::create_pcb(void (*body)(), unsigned int time_slice = 2, unsigned long stack_size = 4096) {
	if (stack_size > (64l << 10)) {
		lock;
		cout << stack_size << " is too large for a stack, reverting to 64K\n";
		unlock;
		stack_size = 64l << 10;
	}

	PCB* newPCB = new PCB();

//	cout << "Stack Size: " << stack_size << endl;

	unsigned *st = new unsigned[stack_size];
	st[stack_size - 1] = 0x200; // Starting PSW

	st[stack_size - 2] = FP_SEG(body);
	st[stack_size - 3] = FP_OFF(body);

//	unsigned *stack_pointer = st + stack_size - 12;

	newPCB->ss = FP_SEG(st + stack_size - 12);
	newPCB->sp = FP_OFF(st + stack_size - 12);

//	cout << "New stack\n";
//	cout << "   " << hex << newPCB->ss << newPCB->sp << endl;;
//	cout << st << endl;
//	printf("New stack:\n");
//	printf("%04x%04x\n", newPCB->ss, newPCB->sp);
//	printf("%08lx\n", st);

	newPCB->finished = 0;
	newPCB->paused = 0;
	newPCB->stack = st;
	newPCB->timeQuantum = time_slice;
	newPCB->id = PCB::maxId++;

	return newPCB;
}

void PCB::wrapper() {
	PCB::running->owner->run();
	exit_thread();
}
