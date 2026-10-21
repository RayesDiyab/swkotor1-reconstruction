#pragma once
#include "1b3cb951f1-hash_text.h"
#include "2012d71fa5-string_priority.h"

// ECX receiver, ignored EDX, no stack words, fullEAX receiver, RET0.
// A live aligned writable prefix through+33 is required; no full class claim.
extern "C" void* __fastcall key_table_construct_0040d030(void*,void*) noexcept(false);
