#include <stdio.h>
#include <dos.h>

#define SEG_MUL (1 << 16)

const unsigned STACK_SIZE = 512;

unsigned cur_thread;

unsigned sp0seg, sp0off, bp0;
unsigned sp1seg, sp1off, bp1;

int nextThread() {
	return (cur_thread + 1) % 2;
}

void a();

void dispatch() {
	printf("Dispatching to thread %d\n", nextThread());
	if (cur_thread == 0) {
		asm {
			mov sp0seg, ss
			mov sp0off, sp
			mov bp0, bp
		}

		printf("Previous sp, bp: %04X:%04X, %04X\n", sp0seg, sp0off, bp0);

		cur_thread = nextThread();

		printf("Next sp, bp:     %04X:%04X, %04X\n", sp1seg, sp1off, bp1);

		asm {
			mov ss, sp1seg
			mov sp, sp1off
			mov bp, bp1
		}
	} else {
		asm {
			mov sp1seg, ss
			mov sp1off, sp
			mov bp1, bp
		}

		printf("Previous sp, bp: %04X:%04X, %04X\n", sp1seg, sp1off, bp1);

		cur_thread = nextThread();

		printf("Next     sp, bp: %04X:%04X, %04X\n", sp0seg, sp0off, bp0);

		asm {
			mov ss, sp0seg
			mov sp, sp0off
			mov bp, bp0
		}
	}
	printf("Exiting dispatch.\n");
	if (cur_thread == 1) {
//		printf("DISPATCHING TWICE\n");
//		dispatch();
	}
}

// funkcija koju izvrsava nit 1
void a() {
	printf("Entering a\n");
	dispatch();
}

int main() {
//	printf("sizeof(unsigned), sizeof(long), sizeof(int*): \n");
//	printf("%d, %d, %d\n", sizeof(unsigned), sizeof(long), sizeof(int*));

	unsigned tpc_seg;
	unsigned tpc_off;

	asm {
		mov sp0seg, ss
		mov sp0off, sp
		mov bp0, bp
	}

//	printf("Current pc: %04X:%04X\n", tpc_seg, tpc_off);

	unsigned * sp1 = new unsigned[STACK_SIZE];
	sp1[STACK_SIZE - 1] = FP_SEG(a);
	sp1[STACK_SIZE - 2] = FP_OFF(a);
	bp1 = FP_OFF(sp1 + STACK_SIZE - 3);
	sp1[STACK_SIZE - 3] = bp1;
	sp1 = sp1 + STACK_SIZE - 4;
//	sp1[1] = FP_OFF(sp1);
	sp1seg = FP_SEG(sp1);
	sp1off = FP_OFF(sp1);

	printf("main: %04X:%04X\n", FP_SEG(main), FP_OFF(main));
	printf("a: %04X:%04X\n", FP_SEG(a), FP_OFF(a));
	printf("sp0, bp0: %04X:%04X, %04X\n", sp0seg, sp0off, bp0);
	printf("sp1, bp1: %04X:%04X, %04X\n", sp1seg, sp1off, bp1);

//	printf("stack1: %04X|%04X|%04X|%04X|%04X|%04X\n", sp1[5], sp1[4], sp1[3], sp1[2], sp1[1], sp1[0]);
//	printf("stack1: %04X|%04X|%04X\n", sp1[2], sp1[1], sp1[0]);

	cur_thread = 0;

	dispatch();

	printf("End.\n");
	getchar();
	return 0;
}
