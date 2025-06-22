#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>

struct FunctionEntry {
	uint64_t m_id;
	uint8_t* m_info;
};

struct FunctionInfo {
	bool loaded = false;
	uint64_t m_id;
	char const* m_signature;
	uint32_t m_params_size;
	uint32_t m_stack_size;
	uint32_t m_return_size;
	uint8_t m_flags;
	uint8_t const* m_instructions;
};

#endif