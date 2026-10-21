// Lane-sound faithful bodies. Addresses cite swkotor.exe (GOG K1) instructions.
#include "1e73fd022d-sound_path.h"
#include "39c670fc66-allocator_live.h"
#include <cstring>

namespace {
using kotor_sound_path::U;
void* pointer(const void* object) { void* value; std::memcpy(&value, object, 4); return value; }
void publish(void* object, void* value) { std::memcpy(object, &value, 4); }
U word(const void* object, unsigned offset) {
    U value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, 4); return value;
}
U address(const void* p) { U value; std::memcpy(&value, &p, 4); return value; }
// Unwind state that frees a raw game allocation with 006FA390 (_free):
// 00723FF0 (CExoSound ctor, FuncInfo 00774568 state 0) and 00726138
// (StartMenuMusic, FuncInfo 00776CF0 state 1).
struct AllocationGuard {
    void* value;
    ~AllocationGuard() { if (value) live_free(value); }
};
}

namespace kotor_sound_path {

// 005D5BC0 CExoSound::CExoSound(n2D, n3D, eax, hardware), RET 10h, EAX=this.
void* __fastcall SoundConstruct005d5bc0(void* self, void*, U voices2d, U voices3d, U eax_mode, U hardware) {
    publish(self, nullptr);                                         // 005D5BD9
    if (*sound_lifecycle29_required::word_00832894) return self;    // 005D5BDF-005D5BE6 (Disable Sound)
    void* raw = live_operator_new(0x14c);                          // 005D5BE8-005D5BED
    AllocationGuard guard{raw};                                     // 005D5BFB state 0
    void* internal = raw ? kotor_sound_internal::Construct005d86e0(raw, nullptr) // 005D5C07
                         : nullptr;                                 // 005D5C0E
    guard.value = nullptr;                                          // 005D5C12 state -1
    publish(self, internal);                                        // 005D5C1A
    if (internal)                                                   // 005D5C1C
        (void)kotor_sound_internal::Initialize005da8a0(
            internal, nullptr, voices2d, voices3d, eax_mode, hardware); // 005D5C1E-005D5C34, EAX ignored
    return self;                                                    // 005D5C3D
}

// 005D6320 CExoSound::Set2D3DBias(float). Clamp to [0.1 (0073E798), 1.9 (0074C60C)].
// Original EAX is caller EAX with AX=FPU status word; no caller consumes it
// (005F2570). It is not reproduced: this body returns 0.
U __fastcall SoundSetBias005d6320(void* self, void*, float bias) {
    void* internal = pointer(self);                                 // 005D6320
    if (!internal) return 0;                                        // 005D6324
    const float high = 1.89999997615814208984375f;                  // bits 3FF33333 @0074C60C
    const float low = 0.100000001490116119384765625f;               // bits 3DCCCCCD @0073E798
    float stored;
    if (!(bias < high)) stored = high;                              // 005D6326-005D6335 JP (also NaN) -> 005D636F
    else if (!(low <= bias)) stored = low;                          // 005D6337-005D6351 (C0|C3 clear)
    else if (!(bias < high)) stored = high;                         // 005D6354-005D6363 (re-test, as observed)
    else stored = bias;                                             // 005D6365-005D6369
    std::memcpy(static_cast<unsigned char*>(internal) + 0x60, &stored, 4);
    return 0;
}

// 005D6380 CExoSound::Get2D3DBias: ST0 = internal+60h, or 0.5 (0073E9AC).
float __fastcall SoundGetBias005d6380(void* self, void*) {
    void* internal = pointer(self);
    if (!internal) return 0.5f;                                     // 005D638A
    float value; std::memcpy(&value, static_cast<unsigned char*>(internal) + 0x60, 4); // 005D6386
    return value;
}

// 005D63A0 / 005D63B0: MOV EAX,[ECX]; MOV AL,[EAX+28h|29h]; null -> EAX=0x18/0x10.
U __fastcall SoundVoices2d005d63a0(void* self, void*) {
    void* internal = pointer(self);
    if (!internal) return 0x18u;                                    // 005D63AA (EAX was zero)
    return (address(internal) & 0xffffff00u) | static_cast<const unsigned char*>(internal)[0x28];
}
U __fastcall SoundVoices3d005d63b0(void* self, void*) {
    void* internal = pointer(self);
    if (!internal) return 0x10u;                                    // 005D63BA
    return (address(internal) & 0xffffff00u) | static_cast<const unsigned char*>(internal)[0x29];
}

// 005D6430 / 005D6440: store DWORD at internal+0 / internal+4; EAX = internal (0 if null).
U __fastcall SoundSetStreamingEffects005d6430(void* self, void*, U value) {
    void* internal = pointer(self);
    if (internal) std::memcpy(internal, &value, 4);                 // 005D643A
    return address(internal);
}
U __fastcall SoundSetNonStreamingEffects005d6440(void* self, void*, U value) {
    void* internal = pointer(self);
    if (internal) std::memcpy(static_cast<unsigned char*>(internal) + 4, &value, 4); // 005D644A
    return address(internal);
}

// 005D6450 CExoSound::GetEAX: internal+100h or 0.
U __fastcall SoundGetEax005d6450(void* self, void*) {
    void* internal = pointer(self);
    return internal ? word(internal, 0x100) : 0u;                   // 005D6456 / 005D645D
}

// 005E58A0 CExoString::IsEmpty: data==0 or *data==0 -> 1, else 0.
// [M47 compose] 005E58A0 CExoString::IsEmpty: single body = earliest-staged owner @frame_LogIsEmpty_005e58a0@8 (lane-r4-lowers, M44; D68) (definition removed; declaration kept)
U __fastcall StringIsEmpty005e58a0(String* self, void*);

// 00406DE0 CResRef::operator!=(CExoString const&), RET 4. IsEmpty result is
// discarded (00406DEB); compares min(len+1,16) bytes, string vs ResRef, REPE CMPSB.
U __fastcall RefDiffersString00406de0(const Ref* self, void*, String* text) {
    (void)StringIsEmpty005e58a0(text, nullptr);                     // 00406DEB
    const char* chars = async_string_cstr_005e5670(text, nullptr);  // 00406DF2
    U count = async_string_length_005e5790(text, nullptr) + 1u;     // 00406DFB-00406E00
    if (count > 16u) count = 16u;                                   // 00406E01-00406E06 (unsigned JBE)
    const unsigned char* left = reinterpret_cast<const unsigned char*>(chars);
    for (U i = 0; i < count; ++i)                                   // 00406E0F REPE CMPSB
        if (left[i] != self->bytes[i]) return 1;                    // 00406E13 SETNE
    return 0;
}

// 005F6440 CClientExoAppInternal::StopMenuMusic, RET 0.
void __fastcall StopMenuMusic005f6440(Client* client, void*) {
    void* source = client->ptr(0x238);                              // 005F6443
    if (!source) return;                                            // 005F644B
    table29_required::gui_fade_005f1b20(client, nullptr, source, 0x1f4); // 005F644E-005F6454
    void* current = client->ptr(0x238);                             // 005F6459 reload
    if (current) {                                                  // 005F645F
        table29_required::gui_sound_destroy_005d6200(current, nullptr); // 005F6465
        live_free(current);                                         // 005F646B
    }
    client->put(0x238, 0);                                          // 005F6473
}

// 005F9AF0 CClientExoAppInternal::StartMenuMusic(theme, volume, priority), RET 0Ch.
void __fastcall StartMenuMusic005f9af0(Client* client, void*, U theme, U volume, U priority) {
    String name(theme ? "mus_theme_cult" : "mus_theme_rep");        // 005F9B0D-005F9B22 (0074EBFC / 0074EBEC); EH state 0 -> 005E5C20
    Ref scratch;                                                     // [esp+0Ch], 16 bytes, no lifetime
    U differs = 0;
    if (void* source = client->ptr(0x238)) {                        // 005F9B27-005F9B37
        Ref* playing = table29_required::gui_file_playing_005d6230(source, nullptr, &scratch); // 005F9B43
        differs = RefDiffersString00406de0(playing, nullptr, &name) ? 1u : 0u; // 005F9B4A-005F9B5A
    }
    if (client->get(0x1ec) == 0 && !differs) return;                // 005F9B5C-005F9B68 (String dtor at 005F9C3B)
    client->put(0x1ec, 0);                                          // 005F9B70
    if (differs) StopMenuMusic005f6440(client, nullptr);            // 005F9B7A-005F9B7E
    if (void* source = client->ptr(0x238)) {                        // 005F9B83-005F9B8B
        if (table29_required::gui_playing_005d5af0(source, nullptr)) return; // 005F9C00-005F9C07
        async_ref_construct_string_00406d60(&scratch, nullptr, &name);      // 005F9C09-005F9C12
        (void)table29_required::gui_play_005d5b10(client->ptr(0x238), nullptr, &scratch, 8, 0, 1, 1); // 005F9C17-005F9C2A
        return;
    }
    void* raw = live_operator_new(4);                               // 005F9B8D-005F9B8F
    void* created;
    {
        AllocationGuard guard{raw};                                 // 005F9B9D state 1 -> 00726138
        created = raw ? table29_required::gui_sound_construct_005d5a50(raw, nullptr) : nullptr; // 005F9BA6 / 005F9BAD
        guard.value = nullptr;                                      // 005F9BB3 state 0
    }
    client->put(0x238, address(created));                           // 005F9BB8
    table29_required::gui_loop_005d5b40(created, nullptr, 1);        // 005F9BAF-005F9BBE (ECX=EAX, not reloaded)
    table29_required::gui_volume_005d5b50(client->ptr(0x238), nullptr, volume);       // 005F9BC3-005F9BCE
    table29_required::gui_priority_005d5ba0(client->ptr(0x238), nullptr, priority);   // 005F9BD3-005F9BDE
    async_ref_construct_string_00406d60(&scratch, nullptr, &name);  // 005F9BE3-005F9BEC
    (void)table29_required::gui_play_005d5b10(client->ptr(0x238), nullptr, &scratch, 8, 0, 1, 0); // 005F9BF1-005F9C2A
}
}

namespace sound_fade29_required {
// 005D5DD0 CExoSound::Render: tail JMP 005D9590 with ECX=internal; null -> RET.
void __fastcall update_005d5dd0(void* self, void*) {
    if (void* internal = pointer(self))
        kotor_sound_internal::Render005d9590(internal, nullptr);
}
}

namespace table29_required {
// 005D5B10 CExoStreamingSoundSource::Play: tail JMP 005DCA30; null -> EAX=0, RET 14h.
std::uint32_t __fastcall gui_play_005d5b10(void* outer, void*, menu_resources::Ref* ref,
    std::uint32_t a2, std::uint32_t a3, std::uint32_t a4, std::uint32_t a5) {
    void* internal = pointer(outer);
    if (!internal) return 0;                                        // 005D5B1B
    return sound_stream_play29::Play005dca30(internal, nullptr, ref, a2, a3, a4, a5);
}

// 005D5B50 CExoStreamingSoundSource::SetVolume: low byte clamped to 7Fh in the
// stack DWORD (upper 24 bits kept), then 005DC930(internal, dword, 0, 3F800000h).
void __fastcall gui_volume_005d5b50(void* outer, void*, std::uint32_t volume) {
    if ((volume & 0xffu) > 0x7fu) volume = (volume & 0xffffff00u) | 0x7fu; // 005D5B50-005D5B5A
    void* internal = pointer(outer);                                // 005D5B5E
    if (internal)
        sound_lower29_required::volume_005dc930(internal, nullptr, volume, 0, 0x3f800000u); // 005D5B64-005D5B70
}

// 005D6230 CExoStreamingSoundSource::GetFilePlaying(out): copy 16 bytes from
// internal+9 DWORD by DWORD; null -> CResRef(out, "" @0073D71C). EAX=out, RET 4.
menu_resources::Ref* __fastcall gui_file_playing_005d6230(void* outer, void*, menu_resources::Ref* out) {
    void* internal = pointer(outer);
    if (internal) {
        const unsigned char* from = static_cast<const unsigned char*>(internal) + 9; // 005D6237
        for (unsigned offset = 0; offset < 16; offset += 4) {       // 005D623C-005D6255
            std::uint32_t value; std::memcpy(&value, from + offset, 4);
            std::memcpy(out->bytes + offset, &value, 4);
        }
        return out;
    }
    out->construct_00406d80(base_ini_reconstruction::warning_empty_0073d71c); // 005D6260-005D6267
    return out;                                                     // 005D626C
}
}
