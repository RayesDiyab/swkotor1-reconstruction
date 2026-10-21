// lane-m46-startup: material_owner::Material members declared in texture-epoch/include/material.h that the
// M45 non-3D consumers reference:
//   0047B110 Material::Init (texture-epoch-cleanup-initialize)       - donor task-17 material-init-r1 init.cpp
//   0047ABD0 Material::ParseField (Init's only parser; new name)       - donor task-17 material-init-r1 parse.cpp
//   0047B980 Material::~Material (gui-render-aur-gui-image-object)    - donor task-17 material-ownership-r1 material.cpp
//   00830298 Material registry list (cold zero) + 0073BC90 dispose     - donor task-17 material-ownership-r1 registry.cpp
// All four re-verified against swkotor.exe (lane-m46-startup/dis/0047b110.asm, 0047abd0.asm, 0047b980.asm).
// Adaptations: the M45 stage names of the resource reader (aurres-stream: aurres_get_0044c740,
// aurres_data_bytes_0044bd10, resource_next_0044bfa0, aurres_release_0044c8f0) and the font-txi readers;
// ParseField is a free D1 fastcall entry (material.h has no parse_field member and is a pinned stage file).
// Material::Material 0047B290 is NOT provided here (3D consumers only; lane-3d-model join).
// The static initializer 0073B0D0 (atexit registration of 0073BC90) is NOT reproduced: it is an original
// __xc initializer that needs an R7 allowlist entry (D60/D61 path); recorded deviation, shutdown-only effect.
#include "856ceecd30-material.h"
#include "d08b97bcca-allocator_live.h"
#include <cstdint>

extern "C" {
// aurres-stream (aurres.hpp:44-52, identical declarations).
void* __cdecl aurres_data_bytes_0044bd10(std::uint32_t count, void* descriptor);
char* __cdecl resource_next_0044bfa0();
void* __cdecl aurres_get_0044c740(const char* name, const char* ext, std::int32_t volatile* size, std::uint32_t memory_only);
std::uint32_t __cdecl aurres_release_0044c8f0(void* descriptor, std::int32_t size);
// font-txi-readers (font_txi.hpp:64-68) and string-compare-existing (0070ACAF _stricmp).
char* __cdecl firstword_00463530(char*);
void __cdecl parse_cstr_00463590(char*, char*);
void __cdecl parse_float_004635c0(char*, volatile float*);
void __cdecl parse_int_004635e0(char*, volatile std::int32_t*);
void __cdecl parse_bool_00463680(char*, volatile bool*);
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// 0047ABD0 Material::ParseField (thiscall RET4) in D1 form.
void __fastcall material_parse_field_0047abd0(material_owner::Material*, void*, char*);
}

namespace material_owner {
// 00830298: the Material registry (CExoArrayList<Material*>), .bss zero. Only writers: Material ctor 0047B290
// (append) and dtor 0047B980 (RemoveReverse32), dispose 0073BC90. Exactly one definition (this unit).
extern List historical_storage_00830298;   // [M46 compose] single storage: lane-m46-3dmodel m46_material_donor.cpp (task-17 registry slice; D71 Material family owner)

// 0073BC90: free(registry.data) if set; data/size/capacity are not cleared.
void __cdecl dispose_0073bc90() {
    const Word data = historical_storage_00830298.data;
    if (data) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));
}
}

namespace material_slots {
using Ref = font::Texture;
// Virtual slots of the texture object in Material::textures[0] (CAurTexture vtable, slot offsets observed at
// 0047B134 (+34, AL predicate), 0047B143 (+94 data), 0047B14F (+90 size), 0047B1F3/0047B278 (+A4 SetTxiLoaded)
// and 0047B22E (+A0 SetTxiData, table captured before 0044BD10)). D21 naked veneers: reload the live table and
// tail-jump the slot with ECX = receiver, preserving the original thiscall stack contract; no C++ casts.
__declspec(naked) unsigned char __fastcall slot_loaded_34(Ref*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 34h] } }
__declspec(naked) const char* __fastcall slot_data_94(Ref*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 94h] } }
__declspec(naked) std::int32_t __fastcall slot_size_90(Ref*, void*) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 90h] } }
__declspec(naked) void __fastcall slot_set_loaded_a4(Ref*, void*, std::uint32_t) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax + 0A4h] } }
// EDX carries the table captured at 0047B21D before the 0044BD10 call (original `call [ebp+A0h]`).
__declspec(naked) void __fastcall slot_set_data_captured_a0(Ref*, void**, const void*, std::int32_t) { __asm { jmp dword ptr [edx + 0A0h] } }
inline char* trim(char* cursor) { while (*cursor == ' ' || *cursor == '\t') ++cursor; return cursor; }   // 0047B195..0047B1A0 / 0047B240..0047B24D
}

namespace material_owner {
// 0047B110..0047B288 Material::Init(const char* name), thiscall RET4, 0x2004-byte frame (__chkstk 006FC090).
void __thiscall Material::init_0047b110(const char* name) {
    using namespace material_slots;
    char scratch[8192];                                                          // [esp+14h]
    std::int32_t volatile token = 0;                                             // [esp+10h] = 0 at 0047B124
    font::Texture* const initial = textures[0].target;                           // 0047B11E
    if (initial && slot_loaded_34(initial, nullptr) != 0) {                      // 0047B120..0047B139 (AL only)
        const char* cursor = slot_data_94(textures[0].target, nullptr);           // 0047B13F..0047B143 (reloaded)
        std::int32_t remaining = slot_size_90(textures[0].target, nullptr);       // 0047B149..0047B14F (reloaded)
        if (!cursor || remaining <= 0) return;                                    // 0047B155..0047B161
        do {
            std::int32_t copied = 0;                                              // 0047B167
            do {
                scratch[copied] = *cursor++;                                      // 0047B170..0047B178
                ++copied;
            } while (copied < remaining && scratch[copied - 1] != '\n');          // 0047B179..0047B182 (LF inclusive)
            remaining -= copied;                                                  // 0047B184
            char* line = nullptr;                                                 // 0047B188 (esi = 0 when negative)
            if (remaining >= 0) {                                                 // 0047B186 jns
                scratch[copied] = 0;                                              // 0047B18C
                line = trim(scratch);                                             // 0047B191..0047B1A0
                material_parse_field_0047abd0(this, nullptr, line);               // 0047B1A2..0047B1A5
            }
            if (remaining == 0 || !line) return;                                  // 0047B1AA..0047B1B4
        } while (remaining > 0);                                                  // 0047B1BA..0047B1BC
        return;
    }
    void* const resource = aurres_get_0044c740(name, ".txi", &token, 1);          // 0047B1CB..0047B1DF ([0073F09C] ".txi")
    if (!resource) {                                                              // 0047B1E9..0047B1EB
        slot_set_loaded_a4(textures[0].target, nullptr, 1);                       // 0047B1ED..0047B1F3 (no null test)
        return;
    }
    std::int32_t t = token;                                                       // 0047B206
    const std::int32_t length = t < 0 ? static_cast<std::int32_t>(~static_cast<std::uint32_t>(t)) : t;   // 0047B20A..0047B215 (or -1; sub)
    if (length > 0) {                                                             // 0047B217..0047B219
        font::Texture* const captured = textures[0].target;                       // 0047B21B
        void** const table = captured->vtable;                                    // 0047B21D
        const void* const data = aurres_data_bytes_0044bd10(0, resource);         // 0047B21F..0047B223
        slot_set_data_captured_a0(captured, table, data, length);                 // 0047B22B..0047B22E
        for (char* line = resource_next_0044bfa0(); line; line = resource_next_0044bfa0())   // 0047B234..0047B25E
            material_parse_field_0047abd0(this, nullptr, trim(line));            // 0047B240..0047B252
        t = token;                                                                // 0047B260 (current token)
    }
    (void)aurres_release_0044c8f0(resource, t);                                   // 0047B264..0047B266
    font::Texture* const final_target = textures[0].target;                       // 0047B26B
    if (final_target) slot_set_loaded_a4(final_target, nullptr, 1);               // 0047B270..0047B278
}

// [M46 compose] 0047B980 Material::~Material: single owner lane-m46-3dmodel (task-17 donor m46_material_donor.cpp, same symbol ??1Material@material_owner@@QAE@XZ; D71) - definition removed here.
}  // namespace material_owner

// 0047ABD0..0047ADE6 Material::ParseField(char* line), thiscall RET4. Seven firstword/_stricmp tests in order
// (literals 0074309C bumpmaptexture/+0E, 00743088 bumpyshinytexture/+11, 00743078 envmaptexture/+0D,
// 007410FC blending/+08, 00743070 decal/+05 -> +40 bool, 00743060 renderbmlmtype/+0E -> +44 int,
// 00743054 wateralpha/+0A -> +48 float); only byte 0 of the four 32-byte name locals is cleared.
// Non-empty names: new(20h) (006FA7E6, unchecked) replaces +1C/+20/+24 (old pointer not released), NUL-inclusive
// copy. Blend: 9-byte "additive" -> [states.data]=0,[modes.data]=2; 13-byte "punchthrough" -> 2,3.
extern "C" void __fastcall material_parse_field_0047abd0(material_owner::Material* self, void*, char* input) {
    char bump[32], shiny[32], environment[32], blend[32];                        // [esp+2Ch],[4Ch],[6Ch],[0Ch]
    bump[0] = 0; shiny[0] = 0; environment[0] = 0; blend[0] = 0;                  // 0047ABE5..0047ABF1
    if (kotor_stricmp_0070acaf("bumpmaptexture", firstword_00463530(input)) == 0) parse_cstr_00463590(input + 0x0e, bump);          // 0047ABF5..0047AC15
    if (kotor_stricmp_0070acaf("bumpyshinytexture", firstword_00463530(input)) == 0) parse_cstr_00463590(input + 0x11, shiny);      // 0047AC1D..0047AC3E
    if (kotor_stricmp_0070acaf("envmaptexture", firstword_00463530(input)) == 0) parse_cstr_00463590(input + 0x0d, environment);    // 0047AC46..0047AC67
    if (kotor_stricmp_0070acaf("blending", firstword_00463530(input)) == 0) parse_cstr_00463590(input + 0x08, blend);               // 0047AC6F..0047AC90
    if (kotor_stricmp_0070acaf("decal", firstword_00463530(input)) == 0) parse_bool_00463680(input + 0x05, &self->decal);           // 0047AC98..0047ACB8
    if (kotor_stricmp_0070acaf("renderbmlmtype", firstword_00463530(input)) == 0) parse_int_004635e0(input + 0x0e, &self->render_type); // 0047ACC0..0047ACE0
    if (kotor_stricmp_0070acaf("wateralpha", firstword_00463530(input)) == 0) parse_float_004635c0(input + 0x0a, &self->water_alpha);  // 0047ACE8..0047AD08
    const char* names_in[3] = {bump, shiny, environment};
    for (unsigned i = 0; i < 3; ++i) {                                            // 0047AD10..0047AD7E
        const char* from = names_in[i];
        if (from[0] == 0) continue;
        char* const to = static_cast<char*>(live_operator_new(0x20));             // 006FA7E6 (no null test)
        self->names[i] = to;                                                      // +1C / +20 / +24
        char* d = to;
        char c;
        do { c = *from++; *d++ = c; } while (c != 0);
    }
    if (blend[0] == 0) return;                                                    // 0047AD80..0047AD84
    static const char additive_007410f0[9] = "additive";
    static const char punchthrough_007410e0[13] = "punchthrough";
    auto equal = [](const char* a, const char* b, unsigned n) { for (unsigned i = 0; i < n; ++i) if (a[i] != b[i]) return false; return true; };
    if (equal(blend, additive_007410f0, 9)) {                                     // 0047AD86..0047AD99 repe cmpsb 9
        *reinterpret_cast<volatile std::uint32_t*>(static_cast<std::uintptr_t>(self->states.data)) = 0;   // 0047AD9B..0047AD9F
        *reinterpret_cast<volatile std::uint32_t*>(static_cast<std::uintptr_t>(self->modes.data)) = 2;    // 0047ADA1..0047ADA6
    } else if (equal(blend, punchthrough_007410e0, 13)) {                                  // 0047ADB6..0047ADC8 repe cmpsb 13
        *reinterpret_cast<volatile std::uint32_t*>(static_cast<std::uintptr_t>(self->states.data)) = 2;   // 0047ADCA..0047ADCD
        *reinterpret_cast<volatile std::uint32_t*>(static_cast<std::uintptr_t>(self->modes.data)) = 3;    // 0047ADD3..0047ADD6
    }
}
