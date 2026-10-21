#pragma once
#include "00a0946121-resources.h"
#include <cstdint>
namespace aurora_model { struct Part; }
extern "C" {
void __fastcall kotor_fastcall_0060bb20(void*, void*);
std::uint32_t __fastcall recovered_spell_load_conjure_00695f30(void*, void*,
    menu_resources::Ref head, menu_resources::Ref hand, menu_resources::Ref ground,
    menu_resources::Ref sound_first, menu_resources::Ref sound_second, void* creature);
void __cdecl recovered_spell_conjure_done_00695ef0(void* model, void* event,
    aurora_model::Part* data);
}
