#ifndef FUNCTION_MANAGER_H
#define FUNCTION_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include "function.h"

class FunctionManager {
public:
	static FunctionInfo const* register_function(char const* _Signature, uint32_t _ParamsSize, uint8_t _Flags, uint32_t _LocalsCount, uint64_t const* _LocalsSizes, uint32_t _StackSize, uint8_t const* _Instructions);

	static FunctionInfo const* get_function(char const* _Signature);
	static void cleanup();
private:
	// <signature, function_info>
	static std::map<char const*, FunctionInfo* const> m_loaded_functions;
	static std::map<char const*, FunctionEntry const> m_function_table;

	static FunctionInfo const* get_loaded_function(char const* _Signature);
	static FunctionInfo const* load_function(char const* _Signature);
};


#endif