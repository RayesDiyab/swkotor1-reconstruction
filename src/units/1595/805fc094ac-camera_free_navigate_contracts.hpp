#pragma once
// Task B source-only contract header for 006413C0 / 00641610.
// The two bodies return the original EAX (0/1). The frozen caller declares void and
// discards the result; the frozen declaration/header/object stay byte-for-byte unchanged.
// The two body .cpp files emit /alternatename joins from the frozen mangled imports to
// these typed bodies; the result stays discarded and no cast is used.
#include <cstdint>

extern "C" {
// 006413C0 CSWCModule::SetFreeLookCamera
// ECX=module, EDX=unused, [esp+4]=U32 id; RET 4; EAX=0 early / 1 normal.
std::uint32_t __fastcall kotor_fastcall_006413c0(void* module, void*, std::uint32_t id);

// 00641610 CSWCModule::SetNavigateCamera
// ECX=module, EDX=unused, no stack arg; RET 0; EAX=0 behavior-null / 1 normal.
std::uint32_t __fastcall kotor_fastcall_00641610(void* module, void*);

// Missing constructor providers: exact original contracts, no body in this delivery.
// 00638670 CSWCameraNavigate::CSWCameraNavigate
// ECX=self, EDX=unused, [esp+4]=void* owner; RET 4; EAX=self.
void* __fastcall kotor_missing_00638670_navigate_ctor(void* self, void*, void* owner);

// 0063A5D0 CSWCameraFreeLook::CSWCameraFreeLook
// ECX=self, EDX=unused, [esp+4]=void* owner, [esp+8]=void* part,
// [esp+0xC]=std::uint32_t effect; RET 0xC; EAX=self.
void* __fastcall kotor_missing_0063a5d0_freelook_ctor(
    void* self, void*, void* owner, void* part, std::uint32_t effect);

// Existing selected provider (already compiled; not implemented in this delivery):
// 00610490 CSWCCreature::GetFreeLookVideoEffect
// ECX=creature, EDX=unused; RET 0; EAX=full DWORD effect value.
// Null [creature+0x21C] or C2da status 0 returns 0xFFFFFFFF; nonzero status returns the
// full output DWORD (which may itself be -1). The B body forwards EAX unchanged.
std::uint32_t __fastcall kotor_fastcall_00610490(void* creature, void*);
}

namespace menu_tables_m41 {
// 0061B6F0 CClientOptions::SetCameraMode (existing 0366 provider, reused)
void __fastcall SetCameraMode_0061b6f0(void* options, void*, unsigned mode);
}
