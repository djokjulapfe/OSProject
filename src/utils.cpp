/*
 * utils.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <stdio.h>
#include "utils.h"
#include "PCB.h"
#include "SCHEDULE.H"

unsigned tss;
unsigned tsp;

void interrupt timer() {
//	lock;
//	cout << running_countdown << endl;
//	unlock;
	if (!cswitch_demanded && running_countdown >= 0) running_countdown--;
	if (running_countdown == 0 || cswitch_demanded) {
//		lock;
//		cout << "Context switch\n";
//		unlock;

		asm {
			mov tss, ss
			mov tsp, sp
		}

		PCB::running->ss = tss;
		PCB::running->sp = tsp;

//		lock;
//		cout << "From " << PCB::running->id << "\n";
//		unlock;

		if (!PCB::running->finished && !PCB::running->paused /* && PCB::running->id != 0*/) Scheduler::put(PCB::running);
		PCB::running = Scheduler::get();
		if (PCB::running == 0) PCB::running = PCB::mainPCB;

//		lock;
//		cout << "To   " << PCB::running->id << "\n";
//		unlock;

		tss = PCB::running->ss;
		tsp = PCB::running->sp;

		running_countdown = PCB::running->timeQuantum;

		asm {
			mov ss, tss
			mov sp, tsp
		}
	}
	if (!cswitch_demanded) {
		tick();
		asm int 60h;
	}
	cswitch_demanded = 0;
	cswitch_schedule = 1;
}

void dispatch() {
	lock;
	cswitch_demanded = 1;
	timer();
	unlock;
}

void exit_thread() {
	PCB::running->finished = 1;
	dispatch();
}

void init_timer() {
	asm {
		cli
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr es:0022h
		mov word ptr es:0182h, ax
		mov ax, word ptr es:0020h
		mov word ptr es:0180h, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		pop ax
		pop es
		sti
	}
}

void restore_timer() {
	asm {
		cli
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr es:0182h
		mov word ptr es:0022h, ax
		mov ax, word ptr es:0180h
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

