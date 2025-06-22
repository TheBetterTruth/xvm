#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include "function_manager.h"
#include "vm_thread.h"
#include "opcodes.h"

uint8_t looptest[] = {
	OP_I_0,		0, 0,
	OP_I_C,		4, 0,			64, 66, 15, 0,
	OP_CMPI,	0, 0,			4, 0,				255, 255,
	OP_IFEQ,	255, 255,		255, 255,
	OP_JIF,		255, 255,		15, 0, 0, 0,
	OP_I_1,		255, 255,
	OP_IADD,	0, 0,			255, 255,			0, 0,
	OP_JMP,		222, 255, 255, 255,
	OP_RET,		
};

uint8_t arithtest[] = {
	OP_I_0,		0, 0,
	OP_I_0,		4, 0,
	OP_I_1,		8, 0,
	OP_I_C,		12, 0,			64, 66, 15, 0,
	OP_I_2,		16, 0,
	OP_I_3,		20, 0,
	OP_I_C,		24, 0,			20, 0, 0, 0,
	OP_CMPI,	0, 0,			12, 0,				255, 255,
	OP_IFEQ,	255, 255,		255, 255,
	OP_JIF,		255, 255,		54, 0, 0, 0,

	OP_MULI,	0, 0,			16, 0,				255, 255,
	OP_DIVI,	0, 0,			20, 0,				255, 255,
	OP_SUBI,	255, 255,		255, 255,			255, 255,

	OP_MODI,	0, 0,			24, 0,				255, 255,
	OP_IADD,	255, 255,		255, 255,			255, 255,

	OP_IADD,	4, 0,			255, 255,			4, 0,
	OP_IADD,	0, 0,			8, 0,				0, 0,

	OP_JMP,		183, 255, 255, 255,
	OP_RET,
};

uint8_t add[] = {
	OP_IADD,	0, 0,		4, 0,		8, 0,
	OP_RET,
};

uint8_t calltest[] = {
	OP_I_0,		0, 0,
	OP_I_0,		4, 0,
	OP_I_C,		8, 0,		64, 66, 15, 0,
	OP_I_1,		12, 0,
	OP_CMPI,	4, 0,		8, 0,				255, 255,
	OP_IFEQ,	255, 255,	255, 255,


	OP_JIF,		255, 255,	30, 0, 0, 0,
	OP_ILOAD,	0, 0,
	OP_ILOAD,	4, 0,
	OP_CALL,	0, 0, 0, 0, 0, 0, 0, 0,
	OP_ILSTORE, 0, 0,
	OP_IADD,	4, 0,		12, 0,				4, 0,
	OP_JMP,		207, 255, 255, 255,
	OP_RET,
};

void run_test(VMThread* _Thread, uint64_t _Function) {
	auto start = std::chrono::high_resolution_clock::now();
	_Thread->call_function(_Function);
	_Thread->run();

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1000000;
	printf("took %.4fms\n", elapsed);
}

int main() {
	VMThread* thread = new VMThread(4);
	FunctionManager& mang = thread->function_manager();

	mang.register_function("add(II)I", 0, 8, 0, sizeof(Int) * 3, 4, add);

	mang.register_function("looptest(V)V", 1, 0, 0, sizeof(Int) * 2, 0, looptest);
	mang.register_function("arithtest(V)V", 2, 0, 0, sizeof(Int) * 7, 0, arithtest);
	mang.register_function("calltest(V)V", 3, 0, 0, sizeof(Int) * 4, 0, calltest);

	printf("XVM execution test (single-thread; single-file, no pkg)\n");
	
	printf("Test #1: let i = 0; while i < 1_000_000 { i = i + 1; }\n");
	run_test(thread, 1);

	/*printf("Test #2: let x,i = 0; while i < 1_000_000 { x = x + i * 2 - i / 3 + i % 5; i = i + 1; }\n");
	run_test(thread, 2);*/
	
	printf("Test #3: let x,i = 0; while i < 1_000_000 { x = add(x, i); i = i + 1; } where add(a,b) => a + b;\n");;
	run_test(thread, 3);

	delete thread;

	return 0;
}