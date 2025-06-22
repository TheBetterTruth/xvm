#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>

struct FunctionInfo {
	bool loaded = false;
	uint32_t m_id;
	char const* m_signature;
	uint16_t m_params_size;
	uint16_t m_stack_size;
	uint16_t m_return_size;
	uint8_t m_flags;
	uint8_t const* m_instructions;
};

#endif