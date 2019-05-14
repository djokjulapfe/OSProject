#include <stdio.h>
#include <dos.h>
#include <stdarg.h>
#include "SCHEDULE.H"

#define SEG_MUL (1 << 16)

const unsigned STACK_SIZE = 512;

unsigned next_thread;

class PCB {
public:
	unsigned id;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned ax;
	unsigned bx;
	unsigned cx;
	unsigned dx;
	unsigned *stack;
};

PCB* p[3];
PCB* running;

#define dispatch(X) { \
	asm mov tmp_rega, ax; \
	asm mov tmp_regb, bx; \
	asm mov tmp_regc, cx; \
	asm mov tmp_regd, dx; \
	Scheduler::put(p[X]); \
	_dispatch(); \
	asm mov ax, tmp_rega; \
	asm mov bx, tmp_regb; \
	asm mov cx, tmp_regc; \
	asm mov dx, tmp_regd; \
}

unsigned tmp_sp, tmp_ss, tmp_bp, tmp_rega, tmp_regb, tmp_regc, tmp_regd;

void _dispatch() {
//	printf("Dispatching to thread %d\n", next_thread);
	// Save current context

//	asm mov tmp_reg, ax;
	running->ax = tmp_rega;
	running->bx = tmp_regb;
	running->cx = tmp_regc;
	running->dx = tmp_regd;

	asm {
		mov tmp_ss, ss
		mov tmp_sp, sp
		mov tmp_bp, bp
	}

	running->ss = tmp_ss;
	running->sp = tmp_sp;
	running->bp = tmp_bp;

//	printf("from ss:sp, bp: %04X:%04X, %04X\n", running->ss, running->sp, running->bp);
//	printf("to   ss:sp, bp: %04X:%04X, %04X\n", p[next_thread]->ss, p[next_thread]->sp, p[next_thread]->bp);
//	running = p[next_thread];

	printf("From %d\n", running->id);
//	Scheduler::put(running);
	running = Scheduler::get();
	printf("To   %d\n", running->id);

	// Restore next context
	tmp_ss = running->ss;
	tmp_sp = running->sp;
	tmp_bp = running->bp;

	asm {
		mov ss, tmp_ss
		mov sp, tmp_sp
		mov bp, tmp_bp
	}

	tmp_rega = running->ax;
	tmp_regb = running->bx;
	tmp_regc = running->cx;
	tmp_regd = running->dx;
}

void createThread(PCB * const newPCB, void (*body)()) {
	unsigned *stack = newPCB->stack = new unsigned[STACK_SIZE];
	unsigned ss, sp, bp;

	stack[STACK_SIZE - 1] = FP_SEG(body);
	stack[STACK_SIZE - 2] = FP_OFF(body);
	bp = FP_OFF(stack + STACK_SIZE - 3);
	stack[STACK_SIZE - 3] = bp;
	stack = stack + STACK_SIZE - 3 - 1;

	ss = FP_SEG(stack);
	sp = FP_OFF(stack);

//	printf("stack: %04X|%04X|%04X\n", newPCB->stack[STACK_SIZE - 1], newPCB->stack[STACK_SIZE - 2], newPCB->stack[STACK_SIZE - 3]);
//	printf("  %04X|%04X|%04X|%04X\n", newPCB->stack[STACK_SIZE - 4], newPCB->stack[STACK_SIZE - 5], newPCB->stack[STACK_SIZE - 6], newPCB->stack[STACK_SIZE - 6]);
//	printf("Created thread with ss:sp, bp = %04X:%04X, %04X\n", ss, sp, bp);
//	printf("stack = %08lX\n", stack);

	newPCB->ss = ss;
	newPCB->sp = sp;
	newPCB->bp = bp;
}

//void _dispatch() {
//	printf("Dispatching to thread %d\n", next_thread);
//	if (cur_thread == 0) {
//		asm {
//			mov sp0seg, ss
//			mov sp0off, sp
//			mov bp0, bp
//		}
//
////		printf("Previous sp, bp: %04X:%04X, %04X\n", sp0seg, sp0off, bp0);
//
//		cur_thread = next_thread;
//
////		printf("Next sp, bp:     %04X:%04X, %04X\n", sp1seg, sp1off, bp1);
//
//		asm {
//			mov ss, sp1seg
//			mov sp, sp1off
//			mov bp, bp1
//		}
//	} else {
//		asm {
//			mov sp1seg, ss
//			mov sp1off, sp
//			mov bp1, bp
//		}
//
////		printf("Previous sp, bp: %04X:%04X, %04X\n", sp1seg, sp1off, bp1);
//
//		cur_thread = next_thread;
//
////		printf("Next     sp, bp: %04X:%04X, %04X\n", sp0seg, sp0off, bp0);
//
//		asm {
//			mov ss, sp0seg
//			mov sp, sp0off
//			mov bp, bp0
//		}
//	}
//}

// funkcija koju izvrsava nit 1
void a() {
	printf("Entering a\n");
	for (int i = 0; i < 3; ++i) printf("In a() %d\n", i);
	asm mov ax, 10
//	asm mov tmp_reg, ax;
//	printf("DEBUG %d\n", tmp_reg);
//	asm mov ax, tmp_reg;
	dispatch(2);
	asm mov i, ax
	printf("In a() ax = %d\n",i);
	for (i = 0; i < 3; ++i) printf("In a() %d\n", i);
	printf("Exiting a\n");
	dispatch(2);
}

// funkcija koju izvrsava nit 2
void b() {
	printf("Entering b\n");
	for (int i = 0; i < 3; ++i) printf("In b() %d\n", i);
	asm mov ax, 2
	dispatch(1);
	for (i = 0; i < 3; ++i) printf("In b() %d\n", i);
	printf("Exiting b\n");
	dispatch(0);
}

void delete_all() {
	delete p[0]->stack;
	delete p[1]->stack;
	delete p[2]->stack;
	delete p[0];
	delete p[1];
	delete p[2];
}

int main() {
//	printf("sizeof(unsigned), sizeof(long), sizeof(int*): \n");
//	printf("%d, %d, %d\n", sizeof(unsigned), sizeof(long), sizeof(int*));

//	printf("Current pc: %04X:%04X\n", tpc_seg, tpc_off);

//	printf("main: %04X:%04X\n", FP_SEG(main), FP_OFF(main));
//	printf("a: %04X:%04X\n", FP_SEG(a), FP_OFF(a));
//	printf("b: %04X:%04X\n", FP_SEG(b), FP_OFF(b));
//	printf("sp0, bp0: %04X:%04X, %04X\n", sp0seg, sp0off, bp0);
//	printf("sp1, bp1: %04X:%04X, %04X\n", sp1seg, sp1off, bp1);

//	printf("stack1: %04X|%04X|%04X\n", sp1[2], sp1[1], sp1[0]);

	printf("Creating Threads...\n");

	p[1] = new PCB();
	p[2] = new PCB();
	createThread(p[1], a);
	createThread(p[2], b);
	running = p[0] = new PCB();

	p[0]->id = 0;
	p[1]->id = 1;
	p[2]->id = 2;

//	printf("stack1: %04X|%04X|%04X\n", p[1]->stack[STACK_SIZE - 1], p[1]->stack[STACK_SIZE - 2], p[1]->stack[STACK_SIZE - 3]);
//	printf("   %04X|%04X|%04X|%04X\n", p[1]->stack[STACK_SIZE - 4], p[1]->stack[STACK_SIZE - 5], p[1]->stack[STACK_SIZE - 6], p[1]->stack[STACK_SIZE - 6]);
//	printf("stack2: %04X|%04X|%04X\n", p[2]->stack[STACK_SIZE - 1], p[2]->stack[STACK_SIZE - 2], p[2]->stack[STACK_SIZE - 3]);
//	printf("   %04X|%04X|%04X|%04X\n", p[2]->stack[STACK_SIZE - 4], p[2]->stack[STACK_SIZE - 5], p[2]->stack[STACK_SIZE - 6], p[2]->stack[STACK_SIZE - 7]);

	printf("Threads created!\n");

	dispatch(1);

	delete_all();

	printf("End.\n");
	getchar();
	return 0;
}
