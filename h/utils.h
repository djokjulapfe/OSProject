/*
 * utils.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef UTILS_H_
#define UTILS_H_

// TODO: use stackable locks
#define lock asm cli
#define unlock asm sti

struct PCB;
extern volatile unsigned cntr, cswitch_demanded;
extern PCB* p[3];
extern PCB* running;
extern PCB* mainPCB;

extern unsigned oldTimerOff, oldTimerSeg;

void interrupt timer();
void dispatch();
void exit_thread();

void inic();
void restore();

#endif /* UTILS_H_ */
