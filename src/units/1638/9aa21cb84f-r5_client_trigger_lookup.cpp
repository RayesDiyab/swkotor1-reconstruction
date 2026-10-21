// 005F1220 CClientExoAppInternal::GetTriggerByGameObjectID
// 005ED5D0 CClientExoApp::GetTriggerByGameObjectID
//
// Original: GOG KOTOR 1 Windows x86 swkotor.exe (image base 0x00400000),
// SHA-256 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435.
//
// 005F1220 semantic contract (005F1220..005F1265):
//   The original zeroes a local object-pointer out slot, reads the id stack
//   argument, loads the CGameObjectArray* from internal+0x14, and calls the
//   selected lookup 004D8230 with (array, id, &out). It compares the returned
//   found/not-found byte with the selected 0074666C value 0 (found), then
//   returns null on not-found, on a null object, or when the object type byte
//   +8 differs from the selected 00746636 value 7. On a client trigger it loads
//   the object vtable from [object] and calls vtable+0x3C with ECX=object,
//   returning the slot result with RET4.
// 005ED5D0 semantic contract (005ED5D0..005ED5D7):
//   Loads the CClientExoAppInternal* from CClientExoApp+4 and tail-forwards the
//   same id to 005F1220.
//
// The full original instruction transcription is intentionally omitted from
// this checkpoint copy. The immutable private review/original evidence remains
// in the source-owner workspace.
//
// The slot is called with ECX = object and EDX = the object's vtable pointer,
// matching the original register state at the indirect call. The selected real
// table 007F9200 has the self-return 00539110 at +3C; it ignores EDX, but the
// register value is preserved for any faithful slot. The lookup's EDX was the
// by-product id in the original; its selected declaration marks EDX unused, so
// this body passes the conventional null unused home (the selected body ignores
// EDX).
//
// No trap, placeholder, numeric-address alias or fabricated success is used.
// A null receiver, a null object table, a null vtable or a null slot faults in
// the original and faults here; no guard is added.
#include "6726d44060-r5_client_trigger_lookup.h"

namespace {

using r5_client_trigger::at;
using r5_client_trigger::load_ptr;
using r5_client_trigger::load_u8;

// Original bytes at 0074666C (found/not-found) and 00746636 (type 7). The
// rebuilt image does not preserve those cells at the same VAs, so the proven
// byte values are used directly.
constexpr unsigned char kObjectFound0074666c = 0;
constexpr unsigned char kClientTriggerType00746636 = 7;

// The selected +0x3C slot is a real self-return, but the callable contract is
// a potentially throwing virtual method: no stack arguments, RET0, ECX = object,
// EDX = the unused home (the object's vtable pointer in the original sequence).
using TriggerSlot = void* (__fastcall*)(void*, void*) noexcept(false);

static_assert(sizeof(TriggerSlot) == 4, "x86 trigger slot pointer must be 4 bytes");

} // namespace

extern "C" __declspec(noinline) void* __fastcall kotor_fastcall_005f1220(void* internal,
                                                                         void* /*unused_edx*/,
                                                                         unsigned int id) noexcept(false) {
    // 005F1221..005F1236: the original zeroes its local out slot before the
    // lookup and passes the table, id and &out in that order.
    void* object = nullptr;
    const unsigned char status =
        kotor_fastcall_004d8230(load_ptr(internal, 0x14), nullptr, id, &object);

    // 005F123B..005F1241: `cmp al,[0074666C]; jne return_zero`. The selected
    // 004D8230 returns 0 when found and 1 when absent; polarity is preserved.
    if (status != kObjectFound0074666c) return nullptr;

    // 005F1243..005F1249: found but a null object still returns zero.
    if (object == nullptr) return nullptr;

    // 005F124B..005F1254: type byte +8 must be 7 (client trigger).
    if (load_u8(object, 8) != kClientTriggerType00746636) return nullptr;

    // 005F1256..005F1258: EDX = [ECX] (vtable), call [EDX+0x3C] with ECX = object.
    void* const vtable = load_ptr(object, 0);
    TriggerSlot slot;
    std::memcpy(&slot, at(vtable, 0x3C), sizeof slot);
    return slot(object, vtable);
}

extern "C" void* __fastcall kotor_fastcall_005ed5d0(void* client, void* unused_edx,
                                                    unsigned int id) noexcept(false) {
    // 005ED5D0..005ED5D3: mov ecx,[ecx+4]; jmp 005F1220. The tail target keeps
    // the same id; the outer does not consume or transform it.
    return kotor_fastcall_005f1220(load_ptr(client, 4), unused_edx, id);
}
