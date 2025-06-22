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
#include "function_manager.h"

class VMThread {
public:
	VMThread(uint64_t _EntryFunction);

	void pop_callframe();
	int current_callframe();

	inline uint8_t* const stack_top();

	inline void stack_push(void const* _Value, size_t _Size);
	template <typename T>
	inline void stack_push(T const _Value) {
		*reinterpret_cast<T*>(stack_top()) = _Value;
		call_frame().m_stack_top += sizeof(T);
	}

	inline void stack_pop(size_t _Size);
	template <typename T>
	inline void stack_pop() {
		call_frame().m_stack_top -= sizeof(T);
	}

	inline void const* stack_peek(size_t _Size);
	template <typename T>
	inline T stack_peek() {
		return *reinterpret_cast<T*>(stack_top() - sizeof(T));
	}

	void call_function(uint64_t _Id);

	void run();

	void store(int16_t _Offset, size_t _Size, void const* _Value);
	template <typename T>
	void store(int16_t _Offset, T const _Value) {
		if (_Offset < 0) {
			stack_push<T>(_Value);
		}
		else {
			store_local<T>(_Offset, _Value);
		}
	}

	void const* load(int16_t _Offset, size_t _Size);
	template <typename T>
	T load(int16_t _Offset) {
		if (_Offset < 0) {
			stack_pop<T>();
			return *reinterpret_cast<T*>(stack_top());
		}
		else {
			return load_local<T>(_Offset);
		}
	}

	CallFrame& call_frame();

	FunctionManager& function_manager();

	~VMThread();
private:
	FunctionManager m_function_manager;
	uint8_t m_stack[XM_THREAD_STACK_SIZE];
	CallFrame m_call_frames[40];
	int m_current_call_frame = -1;

	void execute(uint64_t _EntryFuncId);

	inline void store_local(int16_t _Offset, void const* _Value, size_t _Size);
	template <typename T>
	inline void store_local(int16_t _Offset, T _Value) {
		*reinterpret_cast<T*>(call_frame().m_local_frame + _Offset) = _Value;
	}

	inline void const* load_local(int16_t _Offset);
	template <typename T>
	inline T load_local(int16_t _Offset) {
		return *reinterpret_cast<T*>(call_frame().m_local_frame + _Offset);
	}

	
	inline void push_callframe(FunctionInfo const* _Info);


	inline uint8_t const*& inst();
};

#endif