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

volatile unsigned cswitch_demanded;
volatile unsigned cswitch_schedule;
volatile unsigned running_countdown;
ThreadList threads;
volatile unsigned long total_time; // x 55ms

extern int userMain(int argc, const char* argv[]);

int main(int argc, const char* argv[]) {

	PCB::mainPCB = new PCB;
	PCB::mainPCB->stack = 0;
	PCB::mainPCB->owner = 0;
	PCB::mainPCB->finished = 0;
	PCB::mainPCB->paused = 0;
	PCB::mainPCB->timeQuantum = 1;
	PCB::mainPCB->id = 0;
	PCB::maxId++;
	PCB::running = PCB::mainPCB;

	init_timer();

	int ret = userMain(argc, argv);

	restore_timer();

	cout << "Done, press return to continue...";

	getchar();

	// TODO: remove this
	cout << "Successfully finished!" << endl;
	return ret;
}
