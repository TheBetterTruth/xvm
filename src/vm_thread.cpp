#include "vm_thread.h"

#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "function_manager.h"
#include "opcodes.h"

typedef void(*OpFunc)(VMThread*, uint8_t const*&);

int exec = 0;

OpFunc op_lookup[] = {
	op_nop, op_i0, op_i1, op_i2, op_i3, op_ic,
	op_iadd, op_ilstore, op_ilload, op_ret, op_cmpi, op_ifeq, op_jif,
	op_not, op_jmp, op_imul, op_idiv, op_imod, op_isub, op_call
};

const int op_size = sizeof(op_lookup) / sizeof(OpFunc);

VMThread::VMThread(uint64_t _FunctionsCount) : m_function_manager(_FunctionsCount) {
}

void VMThread::execute(uint64_t _EntryFuncId) {
	call_function(_EntryFuncId);
	run();
}

void VMThread::run() {
	while (m_current_call_frame >= 0) {
		uint8_t op = *m_call_frames[m_current_call_frame].m_instruction_pointer;
		m_call_frames[m_current_call_frame].m_instruction_pointer += 1;

		if (op > op_size) {
			printf("Unexpected op: %d\n", op);
			throw std::runtime_error("");
		}
		OpFunc func = op_lookup[op];
		func(this, m_call_frames[m_current_call_frame].m_instruction_pointer);
	}
}

uint8_t* const VMThread::stack_top() {
	if (m_current_call_frame < 0) {
		return m_stack;
	}
	else {
		return call_frame().m_stack_top;
	}
}

int VMThread::current_callframe() {
	return m_current_call_frame;
}

void VMThread::stack_push(void const* _Value, size_t _Size) {
	memcpy(stack_top(), _Value, _Size);
	call_frame().m_stack_top += _Size;
}

void VMThread::stack_pop(size_t _Size) {
	call_frame().m_stack_top -= _Size;
}

void const* VMThread::stack_peek(size_t _Size) {
	return stack_top() - _Size;
}


void VMThread::call_function(uint64_t _Id) {
		push_callframe(m_function_manager.get_function(_Id));
}

void VMThread::store(int16_t _Offset, size_t _Size, void const* _Value) {
	if (_Offset < 0) {
		stack_push(_Value, _Size);
	}
	else {
		store_local(_Offset, _Value, _Size);
	}
}

void const* VMThread::load(int16_t _Offset, size_t _Size) {
	if (_Offset < 0) {
		stack_pop(_Size);
		return stack_top();
	}
	else {
		return load_local(_Offset);
	}
}

void const* VMThread::load_local(int16_t _Offset) {
	return call_frame().m_local_frame + _Offset;
}

void VMThread::store_local(int16_t _Offset, void const* _Value, size_t _Size) {
	memcpy(call_frame().m_local_frame + _Offset, _Value, _Size);
}


CallFrame& VMThread::call_frame() {
	return m_call_frames[m_current_call_frame];
}

void VMThread::push_callframe(FunctionInfo const* _Info) {
	uint8_t* top = stack_top() - _Info->m_params_size;	

	++m_current_call_frame;
	
	call_frame().m_info = _Info;
	call_frame().m_local_frame = top;
	call_frame().m_stack_top = top + _Info->m_stack_size;
	call_frame().m_instruction_pointer = _Info->m_instructions;
}

FunctionManager& VMThread::function_manager() {
	return m_function_manager;
}

void VMThread::pop_callframe() {
	m_current_call_frame -= 1;
}

uint8_t const*& VMThread::inst() {
	return call_frame().m_instruction_pointer;
}

VMThread::~VMThread() {
}