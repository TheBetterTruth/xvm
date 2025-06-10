#include "vm_thread.h"

#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "function_manager.h"
#include "opcodes.h"

Int const I_0			=		0;
Int const I_1			=		1;
Int const I_2			=		2;
Int const I_3			=		3;

VMThread::VMThread(char const* _EntryFunction) {
}

void VMThread::execute(char const* _EntryFunction) {
	call_function(_EntryFunction);
	run();
}

void VMThread::run() {
	while (!m_call_frames.empty()) {
		switch(*(inst()++)) {
			case OP_I_0:
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int), &I_0);
				inst() += sizeof(int16_t);
				break;
			case OP_I_1:
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int), &I_1);
				inst() += sizeof(int16_t);
				break;
			case OP_I_2:
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int), &I_2);
				inst() += sizeof(int16_t);
				break;
			case OP_I_3:
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int), &I_3);
				inst() += sizeof(int16_t);
				break;
			case OP_I_C: {
				uint16_t index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);

				store(index, sizeof(Int), inst());
				inst() += sizeof(Int);
				break;
			}
			case OP_IADD: {
				uint16_t l_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);
				uint16_t r_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);

				Int left = *static_cast<Int const*>(load(l_index, sizeof(Int)));
				Int right = *static_cast<Int const*>(load(r_index, sizeof(Int)));

				Int result = left + right;
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(int), &result);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_MULI: {
				uint16_t l_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);
				uint16_t r_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);

				Int left = *static_cast<Int const*>(load(l_index, sizeof(Int)));
				Int right = *static_cast<Int const*>(load(r_index, sizeof(Int)));

				Int result = left * right;
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(int), &result);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_DIVI: {
				uint16_t l_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);
				uint16_t r_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);

				Int left = *static_cast<Int const*>(load(l_index, sizeof(Int)));
				Int right = *static_cast<Int const*>(load(r_index, sizeof(Int)));

				Int result = left / right;
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(int), &result);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_SUBI: {
				uint16_t l_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);
				uint16_t r_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);

				Int left = *static_cast<Int const*>(load(l_index, sizeof(Int)));
				Int right = *static_cast<Int const*>(load(r_index, sizeof(Int)));

				Int result = left - right;
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(int), &result);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_MODI: {
				uint16_t l_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);
				uint16_t r_index = *reinterpret_cast<uint16_t const*>(inst());
				inst() += sizeof(int16_t);

				Int left = *static_cast<Int const*>(load(l_index, sizeof(Int)));
				Int right = *static_cast<Int const*>(load(r_index, sizeof(Int)));

				Int result = left % right;
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(int), &result);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_CMPI: {
				Int const left = *static_cast<Int const*>(load(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int)));
				inst() += sizeof(int16_t);

				Int const right = *static_cast<Int const*>(load(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int)));
				inst() += sizeof(int16_t);

				Byte value = 0;

				if (left == right) {
					value = 1;
				}
				else if (left > right) {
					value = 1;
				}
				else if (left < right) {
					value = 2;
				}

				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Byte), &value);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_IFEQ: {
				Byte value = *reinterpret_cast<Byte const*>(load(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Byte)));
				inst() += sizeof(int16_t);

				Byte result = 0;

				if (value == 1) {
					result = 1;
				}

				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Byte), &result);
				inst() += sizeof(int16_t);
				break;
			}
			case OP_JIF: {
				Byte value = *reinterpret_cast<Byte const*>(load(*reinterpret_cast<Byte const*>(inst()), sizeof(Byte)));
				inst() += sizeof(int16_t);

				int32_t jmp = *reinterpret_cast<int32_t const*>(inst());
				inst() += sizeof(int32_t);

				if (value == 1) {
					inst() += jmp;
				}
				else {
				}

				break;
			}
			case OP_JMP: {
				int32_t jmp = *reinterpret_cast<int32_t const*>(inst());
				inst() += sizeof(int32_t);

				inst() += jmp;
				break;
			}
			case OP_RET: {
				int params_size = call_frame()->m_info->m_params_size;
				int return_size = call_frame()->m_info->m_locals_sizes[call_frame()->m_info->m_locals_count-1];

				void* return_value = reinterpret_cast<void*>(call_frame()->m_local_frame[call_frame()->m_info->m_locals_count-1]);

				pop_callframe();

				if (!m_call_frames.empty()) {
					call_frame()->m_stack_top -= params_size;
					store(0, return_size, return_value);
				}

				break;
			}
			case OP_ILSTORE:
				store(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int), load(0, sizeof(Int)));
				inst() += sizeof(int16_t);
				break;
			case OP_ILOAD:
				store(0, sizeof(Int), load(*reinterpret_cast<uint16_t const*>(inst()), sizeof(Int))),
				inst() += sizeof(int16_t);
				break;
			case OP_CALL: {
				char const* signature = reinterpret_cast<char const*>(inst());
				inst() += strlen(signature)+1;

				int* top = reinterpret_cast<int*>(stack_top() - 8);
				call_function(signature);

				break;
			}
			default:
				std::cout << "Unknown opcode" << *(inst()-1) << "\n";
				break;
		}
	}
}

uint8_t* const VMThread::stack_top() {
	if (m_call_frames.empty()) {
		return m_stack;
	}
	else {
		return call_frame()->m_stack_top;
	}
}

void VMThread::stack_push(void const* _Value, size_t _Size) {
	if (call_frame()->m_stack_top + _Size > m_stack + XM_THREAD_STACK_SIZE) {
		throw std::runtime_error("Stack overflow");
	}

	memcpy(stack_top(), _Value, _Size);
	call_frame()->m_stack_top += _Size;
}

void VMThread::stack_pop(size_t _Size) {
	call_frame()->m_stack_top -= _Size;
}

void const* VMThread::stack_peek(size_t _Size) {
	return stack_top() - _Size;
}


void VMThread::call_function(char const* _Signature) {
	push_callframe(FunctionManager::get_function(_Signature));
}


uint64_t VMThread::get_local_address(uint32_t _LocalIndex) {
	return call_frame()->m_local_frame[_LocalIndex];
}

void* VMThread::get_local_pointer(uint32_t _LocalIndex) {
	return reinterpret_cast<void*>(static_cast<size_t>(get_local_address(_LocalIndex)));
}

void VMThread::set_local_address(uint32_t _LocalIndex, uint64_t _Address) {
	call_frame()->m_local_frame[_LocalIndex] = _Address;
}

void VMThread::store(uint32_t _Index, size_t _Size, void const* _Value) {
	if (_Index == 0) {
		stack_push(_Value, _Size);
	}
	else {
		store_local(_Index-1, _Value, _Size);
	}
}

void const* VMThread::load(uint32_t _Index, size_t _Size) {
	if (_Index == 0) {
		void const* ret = stack_peek(_Size);
		stack_pop(_Size);
		return ret;
	}
	else {
		return load_local(_Index-1);
	}
}

void const* VMThread::load_local(uint32_t _LocalIndex) {
	return reinterpret_cast<void const*>(call_frame()->m_local_frame[_LocalIndex]);
}

void VMThread::store_local(uint32_t _LocalIndex, void const* _Value, size_t _Size) {
	memcpy(get_local_pointer(_LocalIndex), _Value, _Size);
}


CallFrame* VMThread::call_frame() {
	return m_call_frames.top();
}

void VMThread::push_callframe(FunctionInfo const* _Info) {
	uint8_t* top = stack_top() - _Info->m_params_size;	


	uint32_t local_bytes = _Info->m_stack_size;
	uint32_t pointer_table_bytes = sizeof(int64_t)* _Info->m_locals_count;
	uint32_t frame_size = local_bytes + pointer_table_bytes;

	if (top + frame_size> stack_top() + XM_THREAD_STACK_SIZE) {
		throw std::runtime_error("Stacko overflow");
	}


	CallFrame* cf = new CallFrame(_Info, top, top);


	uint32_t offset = 0;
	for (int i = 0; i < _Info->m_locals_count; i++) {
		cf->m_local_frame[i] = static_cast<uint64_t>(reinterpret_cast<size_t>(cf->m_stack_frame + offset));

		if (offset < _Info->m_params_size) {
			memcpy(reinterpret_cast<void*>(cf->m_local_frame[i]), top + offset, _Info->m_locals_sizes[i]);
		}

		offset += _Info->m_locals_sizes[i];
	}

	m_call_frames.push(cf);
}

void VMThread::pop_callframe() {
	delete m_call_frames.top();
	m_call_frames.pop();
}

uint8_t const*& VMThread::inst() {
	return call_frame()->m_instruction_pointer;
}

VMThread::~VMThread() {
	while (!m_call_frames.empty()) {
		delete m_call_frames.top();
		m_call_frames.pop();
	}
}