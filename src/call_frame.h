#ifndef CALL_FRAME_H
#define CALL_FRAME_H

#include "function.h"

struct CallFrame {
	FunctionInfo const* m_info;
	uint8_t* const m_stack_frame;
	uint64_t* const m_local_frame;
	uint8_t* m_stack_top;
	uint8_t const* m_instruction_pointer;
	uint8_t* m_return_address;

	CallFrame(FunctionInfo const* _Info, uint8_t* _StackFrame, uint8_t* _ReturnAddress)
		: m_info(_Info), m_stack_frame(_StackFrame), m_local_frame(reinterpret_cast<uint64_t*>(m_stack_frame + _Info->m_stack_size)),
			m_stack_top(reinterpret_cast<uint8_t*>(m_local_frame + sizeof(uint64_t) * _Info->m_locals_count)), m_instruction_pointer(_Info->m_instructions), m_return_address(_ReturnAddress) {}
};

#endif