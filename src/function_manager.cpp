#include "function_manager.h"

#include <string.h>
#include <stdexcept>

FunctionManager::FunctionManager(uint64_t _FunctionCount) 
	: m_function_table(static_cast<FunctionEntry*>(malloc(_FunctionCount))), m_loaded_functions(reinterpret_cast<FunctionInfo*>(malloc(_FunctionCount * sizeof(FunctionInfo)))) {}

FunctionInfo const* FunctionManager::register_function(char const* _Signature, uint64_t _Id, uint32_t _ParamsSize, uint8_t _Flags, uint32_t _StackSize, uint32_t _ReturnSize, uint8_t const*_Instructions) {
	m_loaded_functions[_Id].m_signature = _Signature;
	m_loaded_functions[_Id].m_params_size = _ParamsSize;
	m_loaded_functions[_Id].m_stack_size = _StackSize;
	m_loaded_functions[_Id].m_return_size = _ReturnSize;
	m_loaded_functions[_Id].m_flags = _Flags;
	m_loaded_functions[_Id].m_instructions = _Instructions;
	m_loaded_functions[_Id].loaded = true;

	return m_loaded_functions + (_Id * sizeof(FunctionInfo));
}

FunctionInfo const* FunctionManager::get_function(uint64_t _Id) {
	FunctionInfo const* result = get_loaded_function(_Id);

	if (result == nullptr) {
		throw new std::runtime_error("Function not found");
	}

	return result;
}

FunctionInfo const* FunctionManager::get_loaded_function(uint64_t _Id) {
	if (m_loaded_functions[_Id].loaded) {
		return &m_loaded_functions[_Id];
	}
	
	return load_function(_Id);
}

FunctionInfo const* FunctionManager::load_function(uint64_t _Id) {
	uint8_t const* bytecode = m_function_table[_Id].m_info;
	char const* signature = reinterpret_cast<char const*>(bytecode);
	bytecode += strlen(signature)+1;
	uint8_t flags = *(bytecode++);
	uint32_t params_size = *reinterpret_cast<uint32_t const*>(bytecode);
	bytecode += sizeof(int32_t);
	uint32_t stack_size = *reinterpret_cast<uint32_t const*>(bytecode);
	bytecode += sizeof(int32_t);
	uint32_t return_size = *reinterpret_cast<uint32_t const*>(bytecode);
	bytecode += sizeof(int32_t);
	uint8_t const* instructions = bytecode;

	return register_function(signature, _Id, params_size, flags, stack_size, return_size, instructions);
}

FunctionManager::~FunctionManager() {
	free(m_function_table);
	free(m_loaded_functions);
}
