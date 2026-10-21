// Original swkotor.exe 0069EDC0 (1042 normal bytes, [0069EDC0,0069F1D2)), Ghidra label
// CSWCAnimBase::GetBoundingRect (Gob*, rect*) - the 0069F630 GetBoundingRect method forwards self+B8.
// Plain stdcall, two stack words, RET 8. ECX is not an input (overwritten at
// 0069EDC5 before any use). EAX result is a full DWORD: 1 (0069F1B1) or 0
// (0069F1C6 XOR EAX,EAX).
#pragma once
#include <cstdint>
namespace aurora_model { struct Gob; }
extern "C" std::uint32_t __stdcall anim_base_get_bounding_rect_0069edc0(aurora_model::Gob* gob, void* rect);
