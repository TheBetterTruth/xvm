#include <stdexcept>
#include <iostream>
#include <stdlib.h>

#include "function_manager.h"
#include "vm_thread.h"
#include "opcodes.h"

uint8_t looptest[] = {
	OP_I_0,		1, 0,
	OP_I_C,		2, 0,			64, 66, 15, 0,
	OP_CMPI,	1, 0,			2, 0,				0, 0,
	OP_IFEQ,	0, 0,			0, 0,
	OP_JIF,		0, 0,			15, 0, 0, 0,
	OP_I_1,		0, 0,
	OP_IADD,	1, 0,			0, 0,			1, 0,
	OP_JMP,		222, 255, 255, 255,
	OP_RET,		
};

uint64_t looptestsz[] = {sizeof(Int), sizeof(Int), sizeof(Int)};

uint8_t arithtest[] = {
	OP_I_0,		1, 0,
	OP_I_0,		2, 0,
	OP_I_1,		3, 0,
	OP_I_C,		4, 0,			64, 66, 15, 0,
	OP_I_2,		5, 0,
	OP_I_3,		6, 0,
	OP_I_C,		7, 0,			5, 0, 0, 0,
	OP_CMPI,	1, 0,			4, 0,				0, 0,
	OP_IFEQ,	0, 0,			0, 0,
	OP_JIF,		0, 0,			54, 0, 0, 0,

	OP_MULI,	1, 0,			5, 0,				0, 0,
	OP_DIVI,	1, 0,			6, 0,				0, 0,
	OP_SUBI,	0, 0,			0, 0,				0, 0,

	OP_MODI,	1, 0,			7, 0,				0, 0,
	OP_IADD,	0, 0,			0, 0,				0, 0,

	OP_IADD,	2, 0,			0, 0,				2, 0,
	OP_IADD,	1, 0,			3, 0,				1, 0,

	OP_JMP,		183, 255, 255, 255,
	OP_RET,
};

uint64_t arithtestsz[] = {sizeof(Int), sizeof(Int), sizeof(Int), sizeof(Int), sizeof(Int), sizeof(Int), sizeof(Int)};

uint8_t add[] = {
	OP_IADD,	1, 0,		2, 0,		3, 0,
	OP_RET,
};

uint64_t addsz[] = {sizeof(Int), sizeof(Int), sizeof(Int)};

uint8_t calltest[] = {
	OP_I_0,		1, 0,
	OP_I_0,		2, 0,
	OP_I_C,		3, 0,		64, 66, 15, 0,
	OP_I_1,		4, 0,
	OP_CMPI,	2, 0,		3, 0,				0, 0,
	OP_IFEQ,	0, 0,		0, 0,


	OP_JIF,		0, 0,		31, 0, 0, 0,
	OP_ILOAD,	1, 0,
	OP_ILOAD,	2, 0,
	OP_CALL,	'a', 'd', 'd', '(', 'I', 'I', ')', 'I', '\0',
	OP_ILSTORE, 1, 0,
	OP_IADD,	2, 0,		4, 0,				2, 0,
	OP_JMP,		206, 255, 255, 255,
	OP_RET,
};

uint64_t calltestsz[] = {sizeof(Int), sizeof(Int), sizeof(Int), sizeof(Int), sizeof(Int)};

void run_test(VMThread* _Thread, char const* _Function) {
	auto start = std::chrono::high_resolution_clock::now();
	_Thread->call_function(_Function);
	_Thread->run();

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1000000;
	printf("took %.4fms\n", elapsed);
}

int main() {
	VMThread* thread = new VMThread("");

	FunctionManager::register_function("add(II)I", 8, 0, 3, addsz, sizeof(Int) * 3, add);

	FunctionManager::register_function("looptest(V)V", 0, 0, 3, looptestsz, sizeof(Int) * 3, looptest);
	FunctionManager::register_function("arithtest(V)V", 0, 0, 7, arithtestsz, sizeof(Int) * 7, arithtest);
	FunctionManager::register_function("calltest(V)V", 0, 0, 4,  calltestsz, sizeof(Int) * 4, calltest);

	printf("XVM execution test (single-thread; single-file, no pkg)\n");
	
	printf("Test #1: let i = 0; while i < 1_000_000 { i = i + 1; }\n");
	run_test(thread, "looptest(V)V");

	delete thread;
	thread = new VMThread("");

	printf("Test #2: let x,i = 0; while i < 1_000_000 { x = x + i * 2 - i / 3 + i % 5; i = i + 1; }\n");
	run_test(thread, "arithtest(V)V");
	
	delete thread;
	thread = new VMThread("");
	
	printf("Test #3: let x,i = 0; while i < 1_000_000 { x = add(x, i); i = i + 1; } where add(a,b) => a + b;\n");;
	run_test(thread, "calltest(V)V");

	delete thread;

	FunctionManager::cleanup();

	return 0;
}