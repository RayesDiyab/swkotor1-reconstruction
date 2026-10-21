#pragma once
#include "d24452e6d2-hash_text.h"
#include "cebae40286-string_priority.h"

// ECX receiver, ignored EDX, no stack words, fullEAX receiver, RET0.
// A live aligned writable prefix through+33 is required; no full class claim.
extern "C" void* __fastcall key_table_construct_0040d030(void*,void*) noexcept(false);
