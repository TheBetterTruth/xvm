#include "opcodes.h"

#include <stdexcept>
#include "vm_thread.h"

Int const I_0			=		0;
Int const I_1			=		1;
Int const I_2			=		2;
Int const I_3			=		3;

template <typename T>
inline T read(uint8_t const*& _Bytes) {
	T value = *reinterpret_cast<T const*>(_Bytes);
	_Bytes += sizeof(T);
	return value;
}

void op_i0(VMThread* _Thread, uint8_t const*& _Inst) {
	_Thread->store<Int>(read<int16_t>(_Inst), I_0);
}

void op_i1(VMThread* _Thread, uint8_t const*& _Inst) {
	_Thread->store<Int>(read<int16_t>(_Inst), I_1);
}

void op_i2(VMThread* _Thread, uint8_t const*& _Inst) {
	_Thread->store<Int>(read<int16_t>(_Inst), I_2);
}

void op_i3(VMThread* _Thread, uint8_t const*& _Inst) {
	_Thread->store<Int>(read<int16_t>(_Inst), I_3);
}

void op_ic(VMThread* _Thread, uint8_t const*& _Inst) {
	auto l_index = read<int16_t>(_Inst);
	auto value = read<Int>(_Inst);

	_Thread->store<Int>(l_index, value);
}

void op_iadd(VMThread* _Thread, uint8_t const*& _Inst) {
	Int left = _Thread->load<Int>(read<int16_t>(_Inst));
	Int right = _Thread->load<Int>(read<int16_t>(_Inst));

	Int result = left + right;
	_Thread->store<Int>(read<int16_t>(_Inst), result);
}

void op_isub(VMThread* _Thread, uint8_t const*& _Inst) {
	Int left = _Thread->load<Int>(read<int16_t>(_Inst));
	Int right = _Thread->load<Int>(read<int16_t>(_Inst));

	Int result = left - right;
	_Thread->store<Int>(read<int16_t>(_Inst), result);
}

void op_imul(VMThread* _Thread, uint8_t const*& _Inst) {
	Int left = _Thread->load<Int>(read<int16_t>(_Inst));
	Int right = _Thread->load<Int>(read<int16_t>(_Inst));

	Int result = left * right;
	_Thread->store<Int>(read<int16_t>(_Inst), result);
}

void op_idiv(VMThread* _Thread, uint8_t const*& _Inst) {
	Int left = _Thread->load<Int>(read<int16_t>(_Inst));
	Int right = _Thread->load<Int>(read<int16_t>(_Inst));

	Int result = left - right;
	_Thread->store<Int>(read<int16_t>(_Inst), result);
}

void op_imod(VMThread* _Thread, uint8_t const*& _Inst) {
	Int left = _Thread->load<Int>(read<int16_t>(_Inst));
	Int right = _Thread->load<Int>(read<int16_t>(_Inst));

	Int result = left % right;
	_Thread->store<Int>(read<int16_t>(_Inst), result);
}


void op_cmpi(VMThread* _Thread, uint8_t const*& _Inst) {
	Int left = _Thread->load<Int>(read<int16_t>(_Inst));
	Int right = _Thread->load<Int>(read<int16_t>(_Inst));

	Byte result = 0;
	if (left == right)
		result = 0;
	else if (left > right) {
		result = 1;
	}
	else if (left < right) {
		result = 2;
	}

	_Thread->store(read<int16_t>(_Inst), result);
}

void op_ifeq(VMThread* _Thread, uint8_t const*& _Inst) {
	Byte value = _Thread->load<Byte>(read<int16_t>(_Inst));
	Byte result = 0;

	if (value != 0) {
		result = 1;
	}
	
	_Thread->store<Byte>(read<int16_t>(_Inst), result);
}

void op_jif(VMThread* _Thread, uint8_t const*& _Inst) {
	Byte value = _Thread->load<Byte>(read<int16_t>(_Inst));

	int32_t jmp = read<int32_t>(_Inst);

	if (value == 0) {
		_Inst += jmp;
	}
}

void op_jmp(VMThread* _Thread, uint8_t const*& _Inst) {
	int32_t jmp = read<int32_t>(_Inst);
	_Inst += jmp;
}

void op_ret(VMThread* _Thread, uint8_t const*& _Inst) {
	if (_Thread->current_callframe() > 0) {
		int params_size = _Thread->call_frame().m_info->m_params_size;
		int return_size = _Thread->call_frame().m_info->m_return_size;
		void* return_value = _Thread->call_frame().m_local_frame + _Thread->call_frame().m_info->m_stack_size - return_size;

		_Thread->pop_callframe();

		_Thread->call_frame().m_stack_top -= params_size;

		if (return_size > 0) {
			_Thread->store(-1, return_size, return_value);	
		}
	}
	else {
		_Thread->pop_callframe();
	}
}

void op_ilstore(VMThread* _Thread, uint8_t const*& _Inst) {
	_Thread->store<Int>(read<int16_t>(_Inst), _Thread->load<Int>(-1));
}

void op_ilload(VMThread* _Thread, uint8_t const*& _Inst) {
	_Thread->store<Int>(-1, _Thread->load<Int>(read<int16_t>(_Inst)));
}

void op_call(VMThread* _Thread, uint8_t const*& _Inst) {
	uint16_t index = read<uint16_t>(_Inst);
	_Thread->call_function(index);
}

void op_nop(VMThread* _Thread, uint8_t const*& _Inst) {
	throw std::runtime_error("NOP");
}


void op_not(VMThread* _Thread, uint8_t const*& _Inst) {
	Byte value = _Thread->load<Byte>(read<int16_t>(_Inst));

	if (value == 0)
		value = 1;
	else
		value = 0;

	_Thread->store<Byte>(read<int16_t>(_Inst), value);
}