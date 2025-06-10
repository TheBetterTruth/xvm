// ===========================================================================
// xvm_native.h - Native Interface for the Extensible Virtual Machine (XVM)
// ===========================================================================
//
//	This header defines the native interface between the XVM runtime and
//  native C++ code. It provides the low-level API for registering and
//  invoking native functions, accessing VM-managed values and interacting
//  with the memory and runtime environment of the VM.
//

#ifndef XVM_NATIVE_H
#define XVM_NATIVE_H

#include <stdint.h>

typedef uint8_t Byte;
typedef int16_t Short;
typedef int32_t Int;
typedef int64_t Long;
typedef float Float;
typedef double Double;

union Value {
	Byte* b;
	Short* s;
	Int* i;
	Long* l;
	Float* f;
	Double* d;
	void* p;
};

typedef void(*XMNativeFunction)(Value* _Args, uint8_t _ArgsCount);

void xm_register_native_function(const char* signature, XMNativeFunction func);

#endif