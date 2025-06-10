#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>

struct FunctionEntry {
	char const* m_signature;
	uint8_t const* m_info;
};

struct FunctionInfo {
	char const* m_signature;
	uint32_t m_params_size;
	uint32_t m_locals_count;
	uint64_t const* m_locals_sizes;
	uint32_t m_stack_size;
	uint8_t m_flags;
	uint8_t const* m_instructions;

	FunctionInfo(char const* _Signature, uint32_t _ParamsSize, uint32_t _LocalsCount, uint64_t const* _LocalsSizes, uint32_t _StackSize, uint8_t _Flags, uint8_t const* _Instructions)
		: m_signature(_Signature),  m_params_size(_ParamsSize), m_locals_count(_LocalsCount), m_locals_sizes(_LocalsSizes), m_stack_size(_StackSize), m_flags(_Flags), m_instructions(_Instructions) {}
};

#endif