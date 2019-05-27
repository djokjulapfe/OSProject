/*
 * tests.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include <iostream.h>
#include "tests.h"
#include "utils.h"

void test_all() {
	test_interrupts();
	test_dispatch_system();
	test_thread();
	test_semaphore();
	test_events();
	test_signals();
}

// Test interrupts

void test_interrupts() {
	cout << "Testing interupts...\n";
}

void test_dispatch_system() {}

void test_thread() {}

void test_semaphore() {}

void test_events() {}

void test_signals() {}

