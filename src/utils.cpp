/*
 * utils.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include "utils.h"
#include "PCB.h"
#include "SCHEDULE.H"

unsigned tss;
unsigned tsp;

void interrupt timer() {
	if (!cswitch_demanded) cntr--;
	if (cntr == 0 || cswitch_demanded) {
		asm {
			mov tss, ss
			mov tsp, sp
		}

		running->ss = tss;
		running->sp = tsp;

		if (!running->finished) Scheduler::put(running);
		running = Scheduler::get();
		if (running == 0) running = mainPCB;
//		running = getNextPCBToExecute();

		tss = running->ss;
		tsp = running->sp;

		cntr = running->timeQuantum;

		asm {
			mov ss, tss
			mov sp, tsp
		}
	}
	if (!cswitch_demanded) asm int 60h;
	cswitch_demanded = 0;
}

void dispatch() {
	lock;
	cout << "Dispatching...\n";
	cswitch_demanded = 1;
	timer();
	unlock;
}

void exit_thread() {
	running->finished = 1;
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
		mov word ptr oldTimerSeg, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOff, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSeg
		mov word ptr es:0182h, ax
		mov ax, oldTimerOff
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void restore() {
	asm {
		cli
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr oldTimerSeg
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOff
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

