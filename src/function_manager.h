#ifndef FUNCTION_MANAGER_H
#define FUNCTION_MANAGER_H

#include <string>
#include <map>
#include <vector>
#include "function.h"

class FunctionManager {
public:
	FunctionManager(uint32_t _FunctionCount);
	~FunctionManager();
	FunctionInfo const* register_function(char const* _Signature, uint32_t _Id, uint16_t _ParamsSize, uint8_t _Flags, uint16_t _StackSize, uint16_t _ReturnSize, uint8_t const* _Instructions);
	FunctionInfo const* get_function(uint32_t _Id);
private:
	FunctionInfo* m_function_table;
};


#endif