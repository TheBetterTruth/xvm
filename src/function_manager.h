#ifndef FUNCTION_MANAGER_H
#define FUNCTION_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include "function.h"

class FunctionManager {
public:
	FunctionManager(uint64_t _FunctionCount);
	~FunctionManager();
	FunctionInfo const* register_function(char const* _Signature, uint64_t _Id, uint32_t _ParamsSize, uint8_t _Flags, uint32_t _StackSize, uint32_t _ReturnSize, uint8_t const* _Instructions);

	FunctionInfo const* get_function(uint64_t _Id);
private:
	FunctionEntry* m_function_table;
	FunctionInfo* m_loaded_functions;

	FunctionInfo const* get_loaded_function(uint64_t _Id);
	FunctionInfo const* load_function(uint64_t _Id);
};


#endif