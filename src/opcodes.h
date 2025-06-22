#ifndef OPCODES_H
#define OPCODES_H


#define			OP_NOP			0x00
#define			OP_I_0			0x01
#define			OP_I_1			0x02
#define			OP_I_2			0x03
#define			OP_I_3			0x04
#define			OP_I_C			0x05
#define 		OP_IADD			0x06
#define 		OP_ILSTORE		0x07
#define 		OP_ILOAD		0x08
#define 		OP_RET			0x09
#define 		OP_CMPI			0x0A
#define 		OP_IFEQ			0x0B
#define 		OP_JIF			0x0C
#define			OP_NOT			0x0D
#define			OP_JMP			0x0E
#define			OP_MULI			0x0F
#define			OP_DIVI			0x10
#define			OP_MODI			0x11
#define 		OP_SUBI			0x12
#define 		OP_CALL			0x13

#include "vm_thread.h"

void op_i0(VMThread& _Thread, uint8_t const*& _Inst);
void op_i1(VMThread& _Thread, uint8_t const*& _Inst);
void op_i2(VMThread& _Thread, uint8_t const*& _Inst);
void op_i3(VMThread& _Thread, uint8_t const*& _Inst);
void op_ic(VMThread& _Thread, uint8_t const*& _Inst);
void op_iadd(VMThread& _Thread, uint8_t const*& _Inst);
void op_isub(VMThread& _Thread, uint8_t const*& _Inst);
void op_imul(VMThread& _Thread, uint8_t const*& _Inst);
void op_idiv(VMThread& _Thread, uint8_t const*& _Inst);
void op_imod(VMThread& _Thread, uint8_t const*& _Inst);
void op_cmpi(VMThread& _Thread, uint8_t const*& _Inst);
void op_ifeq(VMThread& _Thread, uint8_t const*& _Inst);
void op_jif(VMThread& _Thread, uint8_t const*& _Inst);
void op_jmp(VMThread& _Thread, uint8_t const*& _Inst);
void op_ret(VMThread& _Thread, uint8_t const*& _Inst);
void op_ilstore(VMThread& _Thread, uint8_t const*& _Inst);
void op_ilload(VMThread& _Thread, uint8_t const*& _Inst);
void op_call(VMThread& _Thread, uint8_t const*& _Inst);
void op_nop(VMThread& _Thread, uint8_t const*& _Inst);
void op_not(VMThread& _Thread, uint8_t const*& _Inst);

#endif