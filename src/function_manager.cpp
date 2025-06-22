#include "function_manager.h"

#include <string.h>
#include <stdexcept>

FunctionManager::FunctionManager(uint32_t _FunctionCount) 
	: m_function_table(reinterpret_cast<FunctionInfo*>(malloc(_FunctionCount * sizeof(FunctionInfo)))) {}

FunctionInfo const* FunctionManager::register_function(char const* _Signature, uint32_t _Id, uint16_t _ParamsSize, uint8_t _Flags, uint16_t _StackSize, uint16_t _ReturnSize, uint8_t const*_Instructions) {
	m_function_table[_Id].m_signature = _Signature;
	m_function_table[_Id].m_params_size = _ParamsSize;
	m_function_table[_Id].m_stack_size = _StackSize;
	m_function_table[_Id].m_return_size = _ReturnSize;
	m_function_table[_Id].m_flags = _Flags;
	m_function_table[_Id].m_instructions = _Instructions;
	m_function_table[_Id].loaded = true;

	return m_function_table + (_Id * sizeof(FunctionInfo));
}

FunctionInfo const* FunctionManager::get_function(uint32_t _Id) {
	if (m_function_table[_Id].loaded) {
		return &m_function_table[_Id];
	}
	else {
		throw std::runtime_error("No function with id");
	}
}

FunctionManager::~FunctionManager() {
	free(m_function_table);
}
