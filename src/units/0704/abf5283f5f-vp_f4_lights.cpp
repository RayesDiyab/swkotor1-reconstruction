// lane-3d-vp fork F4: vertex-program light text and light shader constants (swkotor.exe GOG K1, base 00400000).
//   004A30E0 LightManager::SetupVertexProgramLights           cdecl(text, allowed_lights, unused), EAX = char*
//   004A50D0 LightManager::SetUpVertexProgramLightShaderConstants  thiscall(LightManager*), 2 stack words, RET 8
// Evidence: lane-3d-vp/d/004A30E0.s, d/004A50D0.s (dumpbin /disasm of the pinned exe). Every statement cites the
// original instruction(s) it reproduces.
// D30-FINAL: products/quotients the original keeps on the x87 stack are double; float only at fstp dword stores.
// Unwind: 004A50D0 FuncInfo 007180B8 has one state (the local CExoArrayList, freed at 004A565F..004A5660); no callee
// on the reachable path throws (texture_list_assign_0046bc80 grows through the M41 allocator, which returns null), so
// the cleanup is emitted only on the normal path (D30-FINAL unwind rule).
#include <cstdio>
#include <cstring>
#include "4bf744a1f4-vp_nv_ati.h"

// ---------------------------------------------------------------- existing owners (exact copies)
namespace kotor_renderer_gl_cells { extern std::uint16_t format_0078e388; }   // lane-renderer-bodies renderer_gl_cells.h
extern "C" {
// lane-startup-misc texture_memory.cpp (declaration as lane-3d-construct light_nodes.cpp): CExoArrayList<T*>::operator=
// 0046BC80, thiscall RET 4, EAX = this.
kotor_reconstruction::ArrayMutations* __fastcall texture_list_assign_0046bc80(kotor_reconstruction::ArrayMutations* self, void*,
                                                                              const kotor_reconstruction::ArrayMutations* source);
// ---------------------------------------------------------------- LOWERS (NEW; owner sublane "core")
// Matrix::Matrix 00437AF0: thiscall, RET 0, writes the 3x3 identity into [ecx+0..+20h], EAX = this (00437AF3/00437B94).
float* __fastcall matrix_identity_00437af0(float* matrix9, void* unused_edx);
}

// ---------------------------------------------------------------- cells owned here
// Referenced only by 004A30E0 (full-image xref over exe-text.asm): 00830608 (004A3413/004A3478),
// 00830A08 (004A33D4/004A33FF/004A344B), 00830E08 (004A322C..004A341D). BSS. Sizes inferred from the gaps to the
// next referenced addresses (00830A08, 00830E08, 00831E08).
namespace kotor_vp_cells {
char light_text_00830608[0x400];
char light_text_00830a08[0x400];
char light_text_00830e08[0x1000];
// .data, cold {0.8f,0.8f,0.8f} / {0.2f,0.2f,0.2f} (0x3F4CCCCD / 0x3E4CCCCD); no writer instruction in the image.
// Readers: 004A5231..004A525D (here) and LightManager::EnableHeadLight 004A2FF7/004A3026 (lights sublane: borrow).
kotor_scene3d::Vector headlight_a_007a194c = {0.8f, 0.8f, 0.8f};
kotor_scene3d::Vector headlight_b_007a1958 = {0.2f, 0.2f, 0.2f};
}

namespace {
using namespace kotor_vp;
using kotor_scene3d::cells;
using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;

constexpr U kEnvTarget = 0x8620;               // push 8620h (GL_VERTEX_PROGRAM_ARB / _NV)
constexpr U kOneBits = 0x3F800000;             // push 3F800000h
constexpr U kNine_00742f00 = 0x41100000;       // .rdata 00742F00 = 9.0f (004A54F9 fdivr)
const char kEmpty_0073d71c[1] = {0};           // .rdata 0073D71C "" (004A30ED mov eax,73D71Ch)

// mov eax,[0078E5EC]; test; push ...; je NV; call [007BB850] / call [007BB7AC] (target 8620h, stdcall).
void env4(U index, float x, float y, float z, float w) {
    if (programs_arb())
        kotor_renderer_gl_procs::glProgramEnvParameter4fARB_007bb850(kEnvTarget, index, x, y, z, w);
    else
        kotor_renderer_gl_procs::glProgramParameter4fNV_007bb7ac(kEnvTarget, index, x, y, z, w);
}
// Inlined strlen/rep movs append (e.g. 004A323E..004A3268): copies strlen(source)+1 bytes to the end of text.
void append(char* text, const char* source) {
    char* end = text;
    while (*end != 0) ++end;
    std::memcpy(end, source, std::strlen(source) + 1);
}
U word_at(U address, U index) {                                      // [data+index*4]
    U value;
    std::memcpy(&value, as_object(address + index * 4), 4);
    return value;
}
}

// ---------------------------------------------------------------- 004A30E0
// cdecl: [esp+4] text, [esp+8] allowed light count, [esp+0Ch] pushed by the caller (004A2616) but never read.
// EAX on return: 0073D71C ("") when allowed < 1 (004A30ED), else text (004A3518 / 004A359A).
namespace kotor_vertex_program {
char* __cdecl SetupVertexProgramLights_004a30e0(char* text, std::int32_t allowed_lights, std::int32_t condition) {
    (void)condition;                                                  // [esp+0Ch] has no reader
    if (allowed_lights < 1)                                           // 004A30E4..004A30EB (signed jge)
        return const_cast<char*>(kEmpty_0073d71c);                    // 004A30ED (object pointer to .rdata "")
    float matrix[9];
    matrix_identity_00437af0(matrix, nullptr);                        // 004A30FA..004A30FE (local [esp+28h])
    U px = 0, py = 0, pz = 0;                                         // 004A310A..004A311A ([esp+1Ch..24h])
    void* const camera = *cells.current_camera_007bb4e8;              // 004A3103
    if (camera != nullptr) {                                          // 004A3108..004A3122
        float rotation[9];                                            // [esp+4Ch]
        const float* const r = quaternion_get_matrix_0044f470(&field<Quaternion>(camera, 0x88), nullptr, rotation); // 004A3124..004A312F
        void* const current = *cells.current_camera_007bb4e8;         // 004A3134 (reloaded)
        std::memcpy(matrix, r, sizeof matrix);                        // 004A313F..004A314A rep movs 9 dwords
        px = field<U>(current, 0x7C);                                 // 004A313D
        py = field<U>(current, 0x80);                                 // 004A314C
        pz = field<U>(current, 0x84);                                 // 004A314F
    }
    gl_program_env_parameter_0044f820(0x5C, matrix[0], matrix[3], matrix[6], F(px)); // 004A3160..004A3172
    gl_program_env_parameter_0044f820(0x5B, matrix[1], matrix[4], matrix[7], F(py)); // 004A3177..004A3189
    gl_program_env_parameter_0044f820(0x5A, matrix[2], matrix[5], matrix[8], F(pz)); // 004A318E..004A31A0
    env4(0x59, 0.0f, 0.0f, 0.0f, F(kOneBits));                        // 004A31A5..004A31CB
    env4(0x0F, F(kOneBits), F(kOneBits), F(kOneBits), F(kOneBits));   // 004A31D1..004A31FD
    gl_program_env_parameter_0044f820(0x58, F(px), F(py), F(pz), 0.0f); // 004A3203..004A320A
    // 004A320F..004A3231: sprintf(00830E08, head) - the head text is the format string (it contains no '%').
    std::sprintf(kotor_vp_cells::light_text_00830e08,
                 programs_arb() ? kotor_vp_data::text_arb_head_00743e68 : kotor_vp_data::text_nv_head_00743da0);
    append(text, kotor_vp_cells::light_text_00830e08);                // 004A3236..004A3268
    U index = 0x57;                                                   // 004A3219 ebx
    U a = 0x58, b = 0x5C, c = 0x5B, d = 0x5A;                         // 004A3274..004A328C (ebp, [14h], [18h], [10h])
    if (allowed_lights > 0) {                                         // 004A3266..004A326A
        std::int32_t remaining = allowed_lights;                      // [esp+78h] counter
        do {
            env4(index, 0.0f, 0.0f, 0.0f, 0.0f);                      // 004A3295..004A32B4
            --index; --a; --b; --c; --d;                              // 004A32D0..004A32DA
            env4(index, 0.0f, 0.0f, 0.0f, F(kOneBits));               // 004A32C6..004A32F1
            --index; --a; --b; --c; --d;                              // 004A3305..004A330F
            env4(index, 0.0f, 0.0f, 0.0f, F(kOneBits));               // 004A32FB..004A3326
            --index; --a; --b; --c; --d;                              // 004A333A..004A3344
            env4(index, 0.0f, 0.0f, 0.0f, F(kOneBits));               // 004A3330..004A335B
            --index; --a; --b; --c; --d;                              // 004A336F..004A3382
            env4(index, F(kOneBits), F(kOneBits), F(kOneBits), F(kOneBits)); // 004A3365..004A3399
            --index; --a; --b; --c; --d;                              // 004A33A8..004A33AC
            if (programs_arb()) {                                     // 004A33A3..004A33BD
                std::sprintf(kotor_vp_cells::light_text_00830e08, kotor_vp_data::fmt_arb_point_00743c80,
                             static_cast<int>(b), static_cast<int>(a));              // 004A33AF..004A33C9
                std::sprintf(kotor_vp_cells::light_text_00830a08, kotor_vp_data::fmt_arb_light_x_00743c18,
                             static_cast<int>(c));                                   // 004A33CE..004A33D9
                std::sprintf(kotor_vp_cells::light_text_00830608, kotor_vp_data::fmt_arb_light_y_00743bb0,
                             static_cast<int>(d));                                   // 004A33DE..004A3418
            } else {
                std::sprintf(kotor_vp_cells::light_text_00830e08, kotor_vp_data::fmt_nv_point_00743b00,
                             static_cast<int>(b), static_cast<int>(a));              // 004A33EA..004A33F4
                std::sprintf(kotor_vp_cells::light_text_00830a08, kotor_vp_data::fmt_nv_light_x_00743ad0,
                             static_cast<int>(c));                                   // 004A33F9..004A3404
                std::sprintf(kotor_vp_cells::light_text_00830608, kotor_vp_data::fmt_nv_light_y_00743aa0,
                             static_cast<int>(d));                                   // 004A3409..004A3418
            }
            append(text, kotor_vp_cells::light_text_00830e08);        // 004A341D..004A3450
            append(text, kotor_vp_cells::light_text_00830a08);        // 004A344B..004A347D
            append(text, kotor_vp_cells::light_text_00830608);        // 004A3478..004A34AA
            --remaining;                                              // 004A34A2..004A34AC
        } while (remaining != 0);                                     // 004A34B0 jne
    }
    if (programs_arb()) {                                             // 004A34BA..004A34C4
        append(text, kotor_vp_data::text_arb_add_00743a6c);           // 004A34C6..004A34DA (12 dwords + word = 50 bytes)
        append(text, kotor_vp_data::text_arb_alpha_00743a44);         // 004A34DC..004A34F4 (9 dwords + word = 38)
        append(text, kotor_vp_data::text_arb_tail_00743a20);          // 004A34F6..004A3514 (8 dwords + byte = 33)
    } else {
        append(text, kotor_vp_data::text_nv_add_007439fc);            // 004A3520..004A3534 (8 dwords + word = 34)
        // 004A3536..004A3579: inlined dword/word/byte copy of the 19 bytes at 007439E8..007439FA (incl. NUL).
        char* end = text;
        while (*end != 0) ++end;
        std::memcpy(end, kotor_vp_data::text_nv_alpha_007439e8, sizeof kotor_vp_data::text_nv_alpha_007439e8);
        append(text, kotor_vp_data::text_nv_tail_007439c0);           // 004A357C..004A3596 (9 dwords + word + byte = 39)
    }
    return text;                                                      // 004A3518 / 004A359A mov eax,edx
}
}

// ---------------------------------------------------------------- 004A50D0
// thiscall ECX = LightManager; [esp+4] allowed light count (decremented in place at 004A5223/004A5225),
// [esp+8] condition (selects the +74h list and a 1.0 scale). RET 8 (004A567B).
// LightManager view (offsets observed): +44h / +74h CExoArrayList<Light*> (004A52C8 / 004A52C2),
// +5Ch/+60h words/count (004A5329/004A5313), +D0h/+D4h words/count (004A5369/004A5354), +F4h byte (004A5309).
// Light view: +4Ch/+50h/+54h colour floats, +58h radius, +68h flag word, +78h scale, +94h/+98h/+9Ch position.
extern "C" void __fastcall light_manager_set_up_vertex_program_light_shader_constants_004a50d0(void* lights, void*,
    I allowed_lights, I condition) {
    void* const camera = *cells.current_camera_007bb4e8;              // 004A50E8
    if (camera == nullptr) return;                                    // 004A50F1..004A50F9 -> 004A5668
    float m[9];                                                       // [esp+60h]
    quaternion_get_matrix_0044f470(&field<Quaternion>(camera, 0x88), nullptr, m); // 004A5101..004A510C (result unused)
    void* const view = *cells.current_camera_007bb4e8;                // 004A5111 (reloaded)
    const U px = field<U>(view, 0x7C), py = field<U>(view, 0x80), pz = field<U>(view, 0x84); // 004A511E..004A5123
    gl_program_env_parameter_0044f820(0x5C, m[0], m[3], m[6], F(px)); // 004A5117..004A5134
    gl_program_env_parameter_0044f820(0x5B, m[1], m[4], m[7], F(py)); // 004A5139..004A5151
    gl_program_env_parameter_0044f820(0x5A, m[2], m[5], m[8], F(pz)); // 004A5156..004A5171
    env4(0x59, 0.0f, 0.0f, 0.0f, F(kOneBits));                        // 004A5176..004A5199
    env4(0x0F, F(kOneBits), F(kOneBits), F(kOneBits), 0.0f);          // 004A519F..004A51C6 (w = ebx = 0)
    {
        void* const eye = *cells.current_camera_007bb4e8;             // 004A51CC
        gl_program_env_parameter_0044f820(0x58, field<float>(eye, 0x7C), field<float>(eye, 0x80),
                                          field<float>(eye, 0x84), F(kOneBits)); // 004A51D1..004A51EA
    }
    U index = 0x57;                                                   // 004A5200 esi
    if (kotor_renderer_gl_cells::format_0078e388 != 0) {              // 004A51F9..004A5205 (word compare)
        void* const eye = *cells.current_camera_007bb4e8;             // 004A520B
        --allowed_lights;                                             // 004A5223..004A5225 (stored back to [esp+4])
        gl_program_env_parameter_0044f820(0x57, field<float>(eye, 0x7C), field<float>(eye, 0x80),
                                          field<float>(eye, 0x84), 0.0f);            // 004A5210..004A522C
        const kotor_scene3d::Vector& b = kotor_vp_cells::headlight_b_007a1958;
        const kotor_scene3d::Vector& a = kotor_vp_cells::headlight_a_007a194c;
        gl_program_env_parameter_0044f820(0x56, b.x, b.y, b.z, F(kOneBits)); // 004A5231..004A524C
        gl_program_env_parameter_0044f820(0x55, a.x, a.y, a.z, F(kOneBits)); // 004A5251..004A526C
        gl_program_env_parameter_0044f820(0x54, 0.0f, 0.0f, 0.0f, F(kOneBits));           // 004A5271..004A527B
        gl_program_env_parameter_0044f820(0x53, F(kOneBits), F(kOneBits), F(kOneBits), F(kOneBits)); // 004A5283..004A5299
        index = 0x52;                                                 // 004A52A1
    }
    PointerList list{};                                               // 004A52A6..004A52AE ([esp+24h] data/size/capacity = 0)
    texture_list_assign_0046bc80(&list, nullptr, static_cast<const PointerList*>(
        at(lights, condition != 0 ? 0x74u : 0x44u)));                 // 004A52B2..004A52D0
    I emitted = 0;                                                    // 004A52DB [esp+10h]
    for (I i = 0; i < list.size; ++i) {                               // 004A52D5..004A52E3 / 004A5542..004A5554
        const I limit = field<I>(lights, 0xD4) > 0 ? allowed_lights - 1 : allowed_lights; // 004A52F0..004A52FD
        if (emitted >= limit) break;                                  // 004A52FF..004A5303 (signed jge)
        const U light_word = word_at(list.data, static_cast<U>(i));   // 004A5326 / 004A5366 [data+i*4]
        if (field<U8>(lights, 0xF4) != 0) {                           // 004A5309..004A5311
            const I count = field<I>(lights, 0x60);                   // 004A5313
            I matches = 0;                                            // 004A5318
            if (count > 0) {                                          // 004A5316..004A5320
                const U words = field<U>(lights, 0x5C);               // 004A5329
                for (I k = 0; k < count; ++k)                         // 004A533C..004A5340
                    if (word_at(words, static_cast<U>(k)) == light_word) ++matches; // 004A5330..004A5334
                if (matches != 0) continue;                           // 004A5342..004A5348
            }
        }
        {
            const I count = field<I>(lights, 0xD4);                   // 004A5354
            I matches = 0;                                            // 004A535C
            if (count > 0) {                                          // 004A535A..004A5364
                const U words = field<U>(lights, 0xD0);               // 004A5369
                for (I k = 0; k < count; ++k)                         // 004A537C..004A5380
                    if (word_at(words, static_cast<U>(k)) == light_word) ++matches; // 004A5370..004A5374
                if (matches != 0) continue;                           // 004A5382..004A5388
            }
        }
        ++emitted;                                                    // 004A538E..004A539C
        float ax = 0.0f, ay = 0.0f, az = 0.0f;                        // 004A53A0..004A53B0 ([30h..38h])
        float bx = 0.0f, by = 0.0f, bz = 0.0f;                        // 004A53B8..004A53C8 ([48h..50h])
        void* const light = as_object(light_word);
        const double scale = condition != 0 ? W(F(kOneBits))          // 004A53D2 fld [0073D764] (1.0f)
                                            : W(field<float>(light, 0x78)); // 004A53DA..004A53DD
        const float sz = S(scale * W(field<float>(light, 0x54)));     // 004A53E3..004A53E8 fmul; fstp 004A53F3/004A5439
        const float sy = S(scale * W(field<float>(light, 0x50)));     // 004A5401 / 004A5447; fstp 004A540C/004A544C
        const float sx = S(scale * W(field<float>(light, 0x4C)));     // 004A5414 / 004A5454; fstp 004A5423/004A545D
        if (field<U>(light, 0x68) != 0) {                             // 004A53E5..004A53ED
            ax = sx; ay = sy; az = sz;                                // 004A5417..004A5433 ([30h],[34h],[38h])
        } else {
            bx = sx; by = sy; bz = sz;                                // 004A5443..004A5461 (edi, ebp, ebx)
        }
        gl_program_env_parameter_0044f820(index, field<float>(light, 0x94), field<float>(light, 0x98),
                                          field<float>(light, 0x9C), 0.0f);          // 004A5465..004A547D
        gl_program_env_parameter_0044f820(index - 1, ax, ay, az, F(kOneBits));      // 004A5482..004A5498
        gl_program_env_parameter_0044f820(index - 2, bx, by, bz, F(kOneBits));      // 004A549D..004A54A7
        env4(index - 3, 0.0f, 0.0f, 0.0f, F(kOneBits));               // 004A54AC..004A54D2
        const double radius = W(field<float>(as_object(word_at(list.data, static_cast<U>(i))), 0x58)); // 004A54D8..004A54E3
        const float falloff = S(W(F(kNine_00742f00)) / ((radius * radius) + (radius * radius))); // 004A54EB..004A54FF
        env4(index - 4, F(kOneBits), 0.0f, falloff, F(kOneBits));     // 004A54F2..004A5533
        index -= 5;                                                   // 004A5496/004A54A4/004A54BD/004A54ED/004A5541
    }
    if (emitted < allowed_lights) {                                   // 004A555A..004A5560 (signed jge)
        for (I n = allowed_lights - emitted; n != 0; --n) {           // 004A5566 / 004A5643..004A5644
            env4(index, 0.0f, 0.0f, 0.0f, 0.0f);                      // 004A5570..004A558F
            env4(index - 1, 0.0f, 0.0f, 0.0f, F(kOneBits));           // 004A5595..004A55B8
            env4(index - 2, 0.0f, 0.0f, 0.0f, F(kOneBits));           // 004A55BE..004A55E1
            env4(index - 3, 0.0f, 0.0f, 0.0f, F(kOneBits));           // 004A55E7..004A560A
            env4(index - 4, F(kOneBits), F(kOneBits), F(kOneBits), F(kOneBits)); // 004A5610..004A563C
            index -= 5;                                               // 004A55A3/55CC/55F5/5624/5642
        }
    }
    if (list.data != 0) live_free(as_object(list.data));             // 004A564A..004A5660 (local list destructor)
}
