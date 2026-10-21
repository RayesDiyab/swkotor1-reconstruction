// Original swkotor.exe 0069D490 / 0069D4C0 (34 normal bytes each), Ghidra labels
// CSWCAnimBase::AddAnimationFlagInternal / RemoveAnimationFlagInternal.
// Plain stdcall, two stack words (Gob*, flag), RET 8. ECX is not an input
// (loaded from [esp+4] before use). EAX is a full DWORD: 0 when the Gob is null
// (0069D498/0069D4C8 XOR EAX,EAX), otherwise 1 (0069D4AA/0069D4DA), independent
// of the slot's own EAX.
#pragma once
#include <cstdint>
namespace aurora_model { struct Gob; }
extern "C" std::uint32_t __stdcall anim_base_add_animation_flag_internal_0069d490(aurora_model::Gob* gob, std::uint32_t flag);
extern "C" std::uint32_t __stdcall anim_base_remove_animation_flag_internal_0069d4c0(aurora_model::Gob* gob, std::uint32_t flag);
