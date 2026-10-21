// lane-m46-startup: CClientExoAppInternal pause leaves consumed by aurora-window server_pause.cpp:25-26
// (005F7FA0 SetPauseState, 005F2AC0 ServerToClientObjectId). Evidence: dis/005f7fa0.asm, range 005F2AC0..005F2AD8.
#include "096ca033a8-ini_cleanup.h"      // base_ini_reconstruction::destroy_list_005e9ab0 (ini-list-cleanup owner)
#include "d08b97bcca-allocator_live.h"
#include <cstdint>
#include <cstring>

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

extern "C" {
// Consumers' declarations (server_pause.cpp:25-26).
void __fastcall kotor_fastcall_005f7fa0(void* internal, void*, std::uint32_t type, std::uint32_t state, void* list);
std::uint32_t __fastcall kotor_fastcall_005f2ac0(void* internal, void*, std::uint32_t id);
// CWorldTimer owners (server_pause.cpp:38-41, r4_lowers frame_R4_004adbd0) - identical declarations.
void __fastcall frame_R4_004adbd0(void* timer, void*);
void __fastcall kotor_fastcall_004adff0(void* timer, void*);
void __fastcall kotor_fastcall_004adef0(void* timer, void*, const void* source);
std::uint64_t __fastcall kotor_fastcall_004adc70(const void* timer, void*);
void __fastcall kotor_fastcall_004ae030(void* timer, void*);
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                      // CClientExoApp::GetModule (r4-lowers)
// CExoLinkedList accessors (resource-list-accessor-exports; key_entry_from_table.h:5-6).
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
// New names required (not in M45): ClearPauseExclusionList 005F7E20 (RET 0), AddToExclusionList 005F7F10 (RET 4).
void __fastcall client_internal_clear_pause_exclusion_list_005f7e20(void* internal, void*);
void __fastcall client_internal_add_to_exclusion_list_005f7f10(void* internal, void*, std::uint32_t object_id);
}

namespace {
using Word = std::uint32_t;
Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
void* pointer_at(const void* o, Word off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(o, off))); }
// Area +184h object virtual slots +60/+64 (no published typed table in M45): D21 naked tail veneers.
__declspec(naked) void __fastcall vcall_60(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 60h] } }
__declspec(naked) void __fastcall vcall_64(void*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 64h] } }
// t = 004ADC70(timer); [timer+14h] = low; [timer+18h] = high.
void snapshot(void* timer) {
    const std::uint64_t v = kotor_fastcall_004adc70(timer, nullptr);
    put_word(timer, 0x14, static_cast<Word>(v));
    put_word(timer, 0x18, static_cast<Word>(v >> 32));
}
void area_object_slot60(void* module) {   // if module && [module+48] && [[..]+184] -> slot +60
    if (!module) return;
    void* const area = pointer_at(module, 0x48);
    if (!area) return;
    if (void* const o = pointer_at(area, 0x184)) vcall_60(o, nullptr);
}
}

// 005F2AC0..005F2AD8 ServerToClientObjectId(id): id == 7F000000h ? 7F000000h : id | 80000000h. RET 4 (ECX unused).
extern "C" std::uint32_t __fastcall kotor_fastcall_005f2ac0(void*, void*, std::uint32_t id) {
    return id == 0x7f000000u ? 0x7f000000u : (id | 0x80000000u);
}

// 005F7FA0..005F8285 CClientExoAppInternal::SetPauseState(type byte, state, CExoLinkedList* list), RET 0Ch.
// Timers: +24 (world), +28, +2C, +30. Flags byte +1CC.
extern "C" void __fastcall kotor_fastcall_005f7fa0(void* self, void*, std::uint32_t type_word, std::uint32_t state, void* list) {
    unsigned char* const s = static_cast<unsigned char*>(self);
    const unsigned char flags = s[0x1cc];                                                       // 005F7FA3
    const unsigned char type = static_cast<unsigned char>(type_word);                           // 005F7FA9
    if (static_cast<Word>(flags & type) == state) return;                                       // 005F7FAE..005F7FBC (skips the list block)
    if (state == 1) {                                                                           // 005F7FC2..005F7FC5
        s[0x1cc] = static_cast<unsigned char>(flags | type);                                    // 005F7FCB..005F7FD0
        kotor_fastcall_004adff0(pointer_at(s, 0x24), nullptr);                                  // 005F7FD6
        frame_R4_004adbd0(pointer_at(s, 0x24), nullptr);                                        // 005F7FDE
        frame_R4_004adbd0(pointer_at(s, 0x24), nullptr);                                        // 005F7FE6
        if (s[0x1cc] & 2) {                                                                     // 005F7FEB..005F7FF2
            kotor_fastcall_004adff0(pointer_at(s, 0x28), nullptr);                              // 005F7FF7
            frame_R4_004adbd0(pointer_at(s, 0x28), nullptr);                                    // 005F7FFF
            frame_R4_004adbd0(pointer_at(s, 0x28), nullptr);                                    // 005F8007
        }
        void* const app = pointer_at(kotor_menu_window_cells::manager_007a39fc, 4);
        if (kotor_fastcall_005ed530(app, nullptr)) {                                            // 005F800C..005F801B
            void* const module = kotor_fastcall_005ed530(pointer_at(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);   // 005F8021..005F802A
            if (void* const area = pointer_at(module, 0x48))                                    // 005F802F..005F8034
                if (void* const o = pointer_at(area, 0x184)) vcall_64(o, nullptr);              // 005F803A..005F804C
        }
    } else {
        s[0x1cc] = static_cast<unsigned char>(static_cast<unsigned char>(~type) & flags);       // 005F8054..005F805D
        if (type == 2) {                                                                        // 005F805A..005F8063
            kotor_fastcall_004adff0(pointer_at(s, 0x2c), nullptr);                              // 005F806C
            kotor_fastcall_004adff0(pointer_at(s, 0x30), nullptr);                              // 005F8074
            kotor_fastcall_004adef0(pointer_at(s, 0x2c), nullptr, pointer_at(s, 0x28));         // 005F8079..005F8080
            kotor_fastcall_004adef0(pointer_at(s, 0x30), nullptr, pointer_at(s, 0x28));         // 005F8085..005F808C
            frame_R4_004adbd0(pointer_at(s, 0x2c), nullptr);                                    // 005F8094
            frame_R4_004adbd0(pointer_at(s, 0x30), nullptr);                                    // 005F809C
            frame_R4_004adbd0(pointer_at(s, 0x28), nullptr);                                    // 005F80A4
            snapshot(pointer_at(s, 0x2c)); snapshot(pointer_at(s, 0x30)); snapshot(pointer_at(s, 0x28));   // 005F80A9..005F80D6
            const unsigned char now = s[0x1cc];                                                 // 005F80D9
            if ((now & 2) || !(now & 1)) {                                                      // 005F80DF..005F80E5
                frame_R4_004adbd0(pointer_at(s, 0x24), nullptr);                                // 005F80EA
                snapshot(pointer_at(s, 0x24));                                                  // 005F80EF..005F80FC
                kotor_fastcall_004ae030(pointer_at(s, 0x24), nullptr);                          // 005F8102
                area_object_slot60(pointer_at(s, 0x18));                                        // 005F8107..005F8123
            }
            kotor_fastcall_004ae030(pointer_at(s, 0x28), nullptr);                              // 005F8129
            kotor_fastcall_004ae030(pointer_at(s, 0x2c), nullptr);                              // 005F8131
            kotor_fastcall_004ae030(pointer_at(s, 0x30), nullptr);                              // 005F8139
        } else if (type == 1) {                                                                 // 005F8143..005F8146
            kotor_fastcall_004adff0(pointer_at(s, 0x28), nullptr);                              // 005F814F
            kotor_fastcall_004adff0(pointer_at(s, 0x2c), nullptr);                              // 005F8157
            kotor_fastcall_004adff0(pointer_at(s, 0x30), nullptr);                              // 005F815F
            kotor_fastcall_004adef0(pointer_at(s, 0x28), nullptr, pointer_at(s, 0x24));         // 005F8164..005F816B
            kotor_fastcall_004adef0(pointer_at(s, 0x2c), nullptr, pointer_at(s, 0x24));         // 005F8170..005F8177
            kotor_fastcall_004adff0(pointer_at(s, 0x30), nullptr);                              // 005F817C..005F817F
            frame_R4_004adbd0(pointer_at(s, 0x24), nullptr);                                    // 005F8187
            frame_R4_004adbd0(pointer_at(s, 0x28), nullptr);                                    // 005F818F
            frame_R4_004adbd0(pointer_at(s, 0x2c), nullptr);                                    // 005F8197
            frame_R4_004adbd0(pointer_at(s, 0x30), nullptr);                                    // 005F819F
            snapshot(pointer_at(s, 0x2c)); snapshot(pointer_at(s, 0x30));                       // 005F81A4..005F81C1
            snapshot(pointer_at(s, 0x28)); snapshot(pointer_at(s, 0x24));                       // 005F81C4..005F81E1
            kotor_fastcall_004ae030(pointer_at(s, 0x24), nullptr);                              // 005F81E7
            kotor_fastcall_004ae030(pointer_at(s, 0x28), nullptr);                              // 005F81EF
            kotor_fastcall_004ae030(pointer_at(s, 0x2c), nullptr);                              // 005F81F7
            kotor_fastcall_004ae030(pointer_at(s, 0x30), nullptr);                              // 005F81FF
            area_object_slot60(pointer_at(s, 0x18));                                            // 005F8204..005F8220
        }
    }
    client_internal_clear_pause_exclusion_list_005f7e20(self, nullptr);                         // 005F8223..005F8225
    if (!list) return;                                                                          // 005F822A..005F8230
    void* position = pointer_at(pointer_at(list, 0), 0);                                        // 005F8232..005F8238 ([[list]] head)
    while (position) {                                                                          // 005F823C / 005F8260
        const Word item = unresolved_list_at_pos_005e9b60(pointer_at(list, 0), nullptr, position);   // 005F8240..005F8243
        client_internal_add_to_exclusion_list_005f7f10(self, nullptr, item);                    // 005F8248..005F824B
        (void)unresolved_list_next_005e9b90(pointer_at(list, 0), nullptr, &position);           // 005F8250..005F8257
    }
    if (void* const internal = pointer_at(list, 0)) {                                           // 005F8264..005F8268
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<base_ini_reconstruction::List*>(internal), nullptr);   // 005F826C
        live_free(internal);                                                                    // 005F8271..005F8272
    }
    live_free(list);                                                                            // 005F827A..005F827B
}
