#ifndef VM_THREAD_H
#define VM_THREAD_H

#define XM_THREAD_STACK_SIZE  (1024 * 64) // 64 KB per thread stack
#define XM_TOTAL_STACK_LIMIT (1024 * 1024 * 16) // 16 MB total stack for all threads

#include <stdint.h>
#include <thread>
#include <stack>

#include "xvm_native.h"
#include "call_frame.h"
#include "function.h"

class VMThread {
public:
	VMThread(char const* _EntryFunction);

	inline uint8_t* const stack_top();
	void stack_push(void const* _Value, size_t _Size);
	inline void stack_pop(size_t _Size);
	void const* stack_peek(size_t _Size);

	void call_function(char const* _Signature);

	void run();

	inline uint64_t get_local_address(uint32_t _LocalIndex);
	inline void* get_local_pointer(uint32_t _LocalIndex);
	inline void set_local_address(uint32_t _LocalIndex, uint64_t _Address);

	inline void store(uint32_t _Index, size_t _Size, void const* _Value);
	inline void const* load(uint32_t _Index, size_t _Size);

	~VMThread();
private:
	uint8_t m_stack[XM_THREAD_STACK_SIZE];
	std::stack<CallFrame*> m_call_frames;

	void execute(char const* _Signature);

	inline void store_local(uint32_t _LocalIndex, void const* _Value, size_t _Size);
	inline void const* load_local(uint32_t _LocalIndex);

	inline CallFrame* call_frame();
	inline void push_callframe(FunctionInfo const* _Info);
	inline void pop_callframe();


	inline uint8_t const*& inst();
};

#endif