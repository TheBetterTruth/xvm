#include "function_manager.h"

#include <string.h>
#include <stdexcept>

std::map<char const*, FunctionInfo* const> FunctionManager::m_loaded_functions;
std::map<char const*, FunctionEntry const> FunctionManager::m_function_table;

 FunctionInfo const* FunctionManager::register_function(char const* _Signature, uint32_t _ParamsSize, uint8_t _Flags, uint32_t _LocalsCount, uint64_t const* _LocalsSizes, uint32_t _StackSize, uint8_t const*_Instructions) {
	FunctionInfo* info = new FunctionInfo(_Signature, _ParamsSize, _LocalsCount, _LocalsSizes, _StackSize, _Flags, _Instructions);
	m_loaded_functions.emplace(_Signature, info);

	return info;
}

FunctionInfo const* FunctionManager::get_function(char const* _Signature) {
	FunctionInfo const* result = get_loaded_function(_Signature);

	if (result == nullptr) {
		throw new std::runtime_error("Function not found");
	}
	return result;
}

FunctionInfo const* FunctionManager::get_loaded_function(char const* _Signature) {
	for (auto& pair : m_loaded_functions) {
		if (strcmp(pair.first, _Signature) == 0) {
			return pair.second;
		}
	}

	return load_function(_Signature);
}

FunctionInfo const* FunctionManager::load_function(char const* _Signature) {
	for (auto& pair : m_function_table) {
		if (strcmp(pair.first, _Signature) == 0) {
			uint8_t const* bytecode = pair.second.m_info;
			uint8_t flags = *(bytecode++);
			uint32_t params_size = *reinterpret_cast<uint32_t const*>(bytecode);
			bytecode += sizeof(int32_t);
			uint32_t locals_count = *reinterpret_cast<uint32_t const*>(bytecode);
			bytecode += sizeof(int32_t);
			uint64_t const* locals_sizes = reinterpret_cast<uint64_t const*>(bytecode);
			bytecode += locals_count * sizeof(int64_t);
			uint32_t stack_size = *reinterpret_cast<uint32_t const*>(bytecode);
			bytecode += sizeof(int32_t);
			uint8_t const* instructions = bytecode;

			return register_function(_Signature, params_size, flags, locals_count, locals_sizes, stack_size, instructions);
		}
	}

	throw std::runtime_error("No found");
}

void FunctionManager::cleanup() {
	for (auto it = m_loaded_functions.begin(); it != m_loaded_functions.end(); it++) {
		delete it->second;
	}
}

