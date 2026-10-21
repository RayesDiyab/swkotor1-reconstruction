// lane-m47-core: CSWMessage / CSWCMessage lifetime (genuine bodies for the StartServices service 0064B510, which the
// M46 service-tail-8 unit currently reaches only through an unbound `bindings.dispatch(0x004D60F0)` route).
//   004D60F0 CSWMessage::CSWMessage   (table 007465A4 = {004D6660}; buffer operator new(80h) at 004D60FE)
//   004D6130 CSWMessage::~CSWMessage  (frees +4 and +10h)        004D6660 scalar deleting destructor (slot 0)
//   0064B510 CSWCMessage::CSWCMessage (base ctor, then table 00751C6C = {00654CA0})
//   0064B530 CSWCMessage::~CSWCMessage (table store; JMP 004D6130)  00654CA0 scalar deleting destructor (slot 0)
// First frame: StartServices calls 0064B510 (service adapter kotor_menu_service_0064B510). Evidence: lane
// dis/004d60f0.asm, dis/004d6130_004d6175.asm, dis/004d6660.asm, dis/0064b510.asm, dis/0064b530_0064b550.asm,
// dis/00654ca0.asm; tables read with tools/rd.py (both one-slot: the next .rdata words are string bytes).
// Allocation family: 006FA7E6 live_operator_new / 006FA390 live_free.
#include "39c670fc66-allocator_live.h"
#include <cstdint>
#include <cstring>

namespace {
using U32 = std::uint32_t;
void sw(void* p, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
void* pw(const void* p, U32 off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
}

extern "C" {
struct SwMessageMethods { void* (__fastcall* deleting_destroy)(void*, void*, U32); };
void* __fastcall kotor_fastcall_004d6660(void* self, void*, U32 flags) noexcept;
void* __fastcall kotor_fastcall_00654ca0(void* self, void*, U32 flags) noexcept;
extern const SwMessageMethods sw_message_methods_007465a4;
extern const SwMessageMethods swc_message_methods_00751c6c;
const SwMessageMethods sw_message_methods_007465a4{&kotor_fastcall_004d6660};
const SwMessageMethods swc_message_methods_00751c6c{&kotor_fastcall_00654ca0};

// 004D60F0 CSWMessage::CSWMessage.
__declspec(noinline) void* __fastcall kotor_fastcall_004d60f0(void* self, void*) {
    sw(self, 0, static_cast<U32>(reinterpret_cast<std::uintptr_t>(&sw_message_methods_007465a4))); // 004D60F8
    void* const buffer = live_operator_new(0x80);                                // 004D60F3..FE
    std::memcpy(static_cast<unsigned char*>(self) + 4, &buffer, 4);               // 004D6103
    sw(self, 0x0c, 0); sw(self, 0x24, 0); sw(self, 0x28, 0);                      // 004D6106..0E
    sw(self, 0x2c, 0); sw(self, 0x30, 0); sw(self, 0x10, 0);                      // 004D6111..17
    sw(self, 0x08, 0x80);                                                         // 004D611D
    return self;
}
// 004D6130 CSWMessage::~CSWMessage.
__declspec(noinline) void __fastcall kotor_fastcall_004d6130(void* self, void*) noexcept {
    void* const buffer = pw(self, 4);                                             // 004D6133
    sw(self, 0, static_cast<U32>(reinterpret_cast<std::uintptr_t>(&sw_message_methods_007465a4))); // 004D6138
    if (buffer) { live_free(buffer); sw(self, 4, 0); }                            // 004D613E..49
    if (void* const second = pw(self, 0x10)) { live_free(second); sw(self, 0x10, 0); } // 004D6150..60
}
// 004D6660 CSWMessage scalar deleting destructor (same body as 004D6130 inlined, then optional free).
__declspec(noinline) void* __fastcall kotor_fastcall_004d6660(void* self, void*, U32 flags) noexcept {
    kotor_fastcall_004d6130(self, nullptr);                                       // 004D6663..90
    if ((flags & 1u) != 0u) live_free(self);                                      // 004D6697..A4
    return self;
}
// 0064B510 CSWCMessage::CSWCMessage.
// [M45 compose] 0064B510 CSWCMessage ctor body removed: existing M40 body services_tail5::Message::Construct (service-tail-8; also behind the router entry _kotor_menu_service_0064B510) (definition removed; declaration kept)
__declspec(noinline) void* __fastcall kotor_fastcall_0064b510(void* self, void*);
// 0064B530 CSWCMessage::~CSWCMessage: table 00751C6C, JMP 004D6130.
__declspec(noinline) void __fastcall kotor_fastcall_0064b530(void* self, void*) noexcept {
    sw(self, 0, static_cast<U32>(reinterpret_cast<std::uintptr_t>(&swc_message_methods_00751c6c)));
    kotor_fastcall_004d6130(self, nullptr);
}
// 00654CA0 CSWCMessage scalar deleting destructor.
__declspec(noinline) void* __fastcall kotor_fastcall_00654ca0(void* self, void*, U32 flags) noexcept {
    kotor_fastcall_0064b530(self, nullptr);                                       // 00654CA3
    if ((flags & 1u) != 0u) live_free(self);                                      // 00654CA8..B5
    return self;
}
}
