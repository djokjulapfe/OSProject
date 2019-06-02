/*
 * main.cpp
 *
 *  Created on: May 21, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#include "utils.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "tests.h"
#include "list.h"
#include "kernev.h"

volatile unsigned cswitch_demanded;
volatile unsigned cswitch_schedule;
volatile unsigned running_countdown;
ThreadList threads;
PCBList sleeping;
volatile unsigned long total_time; // x 55ms
IVTEntry* entryList;
volatile int waitingSwitch;

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	PCB::mainPCB = new PCB;
	PCB::mainPCB->stack = 0;
	PCB::mainPCB->owner = 0;
	PCB::mainPCB->finished = 0;
	PCB::mainPCB->paused = 0;
	PCB::mainPCB->timeQuantum = 1;
	PCB::mainPCB->id = 0;
	PCB::maxId++;
	PCB::running = PCB::mainPCB;

	PCB::waitPCB = PCB::create_pcb(PCB::wait_for_sleepers, 1, 4096);
	PCB::waitPCB->paused = 1;

	init_timer();

	int ret = userMain(argc, argv);

	restore_timer();

	delete PCB::waitPCB;

	cout << "Done, press return to continue...";

	getchar();

	// TODO: remove this
	cout << "Successfully finished!" << endl;
	return ret;
}
