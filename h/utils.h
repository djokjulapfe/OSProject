/*
 * utils.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef UTILS_H_
#define UTILS_H_

// TODO: use stackable locks
#define lock asm {\
	pushf;\
	cli;\
}
#define unlock asm popf

struct PCB;
class ThreadList;

extern volatile unsigned running_countdown, cswitch_demanded, cswitch_schedule;
extern void tick();
extern volatile unsigned long total_time;

void interrupt timer();
void dispatch();
void exit_thread();

void init_timer();
void restore_timer();

#endif /* UTILS_H_ */
