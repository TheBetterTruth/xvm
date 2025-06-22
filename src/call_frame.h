#ifndef CALL_FRAME_H
#define CALL_FRAME_H

#include "function.h"

struct CallFrame {
	FunctionInfo const* m_info;
	uint8_t* m_local_frame;
	uint8_t* m_stack_top;
	uint8_t const* m_instruction_pointer;
	uint8_t* m_return_address;

	CallFrame() = default;
};

#endif