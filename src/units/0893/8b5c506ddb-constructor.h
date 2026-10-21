#pragma once
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This packet implements the observed MSVC Windows x86 transport.
#endif
// Existing current-header ports: ECX receiver, unused EDX, two stack DWORDs,
// RET8/full EAX receiver. This packet creates no CGameObject class or table.
namespace table29_required {
void* __fastcall gui_game_object_004d7d60(void*,void*,std::uint32_t,std::uint32_t);
}
namespace kotor_binding_required {
void* __fastcall game_object_004D7D60(void*,void*,std::uint32_t,std::uint32_t);
}
