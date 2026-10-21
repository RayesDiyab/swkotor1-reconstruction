// model_lifetime.cpp - lane-3d-model (D39 continuation): on-path instance work and temporary-array lifetime:
// Gob::DoEvent 00486560, Gob bounding-box slot 0043EEE0, PartEmitter::Initialize 0049CAA0,
// MdlNodeTriMesh::InternalRemoveTemporaryArrays 0043C090 (+ folded 0043C1A0), the PostProcess temporaries'
// destructors 004453B0 / 0043E0F0. Normal paths; unwind states cited, not emitted (D30).
#include "a7e200e217-model_instance.hpp"
#include "2f42e97d01-storage.h"
#include "b22a9fca42-material.h"
#include <new>

using namespace aurora_model;

extern "C" {
// .data cells read only by these bodies' callers (values from the pinned image; no writer instruction found
// for 00798B14; emitter render-mode words 00798B68..00798B8C are read by the PartEmitter family).
volatile U aurora_event_time_gate_00798b14 = 1;
extern volatile U emitter_render_mode_00798b68;   // [M45 compose] storage: lane-3d-mesh gate_00798b68 (D68 single storage)
volatile U emitter_render_normal_00798b6c = 1;
volatile U emitter_render_billboard_local_z_00798b70 = 2;
volatile U emitter_render_billboard_world_z_00798b74 = 3;
volatile U emitter_render_linked_00798b78 = 4;
volatile U emitter_render_aligned_world_z_00798b7c = 5;
volatile U emitter_render_mode_00798b80 = 6;
volatile U emitter_frame_blend_default_00798b84 = 7;   // read as a word at 0049CB7D
volatile U emitter_render_aligned_particle_dir_00798b88 = 8;
volatile U emitter_render_motion_blur_00798b8c = 9;
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void (__fastcall* destroy)(void*, void*));
}

namespace {
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
const GobVTable* gob_vt(Gob* g) { return get<const GobVTable*>(g, 0); }
const PartVTable* part_vt(Part* p) { return get<const PartVTable*>(p, 0); }
void free_member(void* p, U off) {                           // "if (x) { free(x); x = 0; }"
    if (get<U>(p, off) != 0) { live_free(ptr<void>(get<U>(p, off))); put<U>(p, off, 0); }
}
void free_if(void* p, U off) {                               // member-destructor form: free when non-null, no store
    if (get<U>(p, off) != 0) live_free(ptr<void>(get<U>(p, off)));
}
}

// 0043E0F0 MaxFace_PostProcess::~MaxFace_PostProcess (0x14-byte element): free +0, +4, +10 when non-null;
// then [+8] = [+C] = 0.
extern "C" void __fastcall aurora_maxface_postprocess_destroy_0043e0f0(void* self, void*) {
    free_if(self, 0x0);                                                             // 0043E0F3..0043E0FD
    free_if(self, 0x4);                                                             // 0043E105..0043E10D
    free_if(self, 0x10);                                                            // 0043E115..0043E11D
    put<U>(self, 0x8, 0); put<U>(self, 0xC, 0);                                     // 0043E125..0043E128
}

namespace {
// Inlined CExoArrayList<MaxFace_PostProcess>::~ / clear at +48 (004454856..004454AD and 0044551A..00445540):
// eh vector destructor 006FA852(data, 14h, data[-1], 0043E0F0) then free(data - 4).
void destroy_faces(void* self) {
    const U data = get<U>(self, 0x48);
    c2da_destroy_array_006fa852(ptr<void>(data), 0x14, ptr<I>(data)[-1], &aurora_maxface_postprocess_destroy_0043e0f0);
    live_free(ptr<void>(data - 4));
}
}

// 004453B0 MdlNodeTriMesh_PostProcess::~MdlNodeTriMesh_PostProcess: explicit list clears (free + zero), the
// +60 pointer array (each element freed, count re-read), then the member destructors in reverse order (each
// reads its field again; all were zeroed above, so they free nothing on the normal path).
extern "C" void __fastcall aurora_trimesh_postprocess_destroy_004453b0(void* self, void*) {
    free_member(self, 0x0);  put<U>(self, 0x8, 0);  put<U>(self, 0x4, 0);           // 004453CE..004453EC
    free_member(self, 0x54); put<U>(self, 0x5C, 0); put<U>(self, 0x58, 0);          // 004453EF..00445405
    free_member(self, 0xC);  put<U>(self, 0x14, 0); put<U>(self, 0x10, 0);          // 00445408..0044541E
    free_member(self, 0x18); put<U>(self, 0x20, 0); put<U>(self, 0x1C, 0);          // 00445421..00445437
    free_member(self, 0x24); put<U>(self, 0x2C, 0); put<U>(self, 0x28, 0);          // 0044543A..00445450
    free_member(self, 0x30); put<U>(self, 0x38, 0); put<U>(self, 0x34, 0);          // 00445453..00445469
    free_member(self, 0x3C); put<U>(self, 0x44, 0); put<U>(self, 0x40, 0);          // 0044546C..00445482
    if (get<U>(self, 0x48) != 0) { destroy_faces(self); put<U>(self, 0x48, 0); }    // 00445485..004454AA
    put<U>(self, 0x50, 0); put<U>(self, 0x4C, 0);                                   // 004454AD..004454B0
    for (I i = 0; i < get<I>(self, 0x64); ++i) live_free(ptr<void>(ptr<U>(get<U>(self, 0x60))[i])); // 004454B3..004454D5
    free_member(self, 0x60); put<U>(self, 0x68, 0); put<U>(self, 0x64, 0);          // 004454D7..004454ED
    free_if(self, 0x60);                                                            // 004454F0..004454FD (member +60)
    free_if(self, 0x54);                                                            // 00445505..00445512
    if (get<U>(self, 0x48) != 0) destroy_faces(self);                               // 0044551A..00445540
    free_if(self, 0x3C);                                                            // 00445543..00445551
    free_if(self, 0x30);                                                            // 00445559..00445566
    free_if(self, 0x24);                                                            // 0044556E..0044557B
    free_if(self, 0x18);                                                            // 00445583..00445590
    free_if(self, 0xC);                                                             // 00445598..004455A4
    free_if(self, 0x0);                                                             // 004455AC..004455BB
}

// 0043C090 MdlNodeTriMesh::InternalRemoveTemporaryArrays: node[134] (PostProcess object) present -> free each of
// its +60 entries (count +64 re-read), free +60 through 006FC0CD (zero it), [+68] = [+64] = 0; then the global
// face/vertex scratch list sizes 007A8A8C/007A8A80/007A8A74 = 0; delete node[134] (004453B0 + free); node[134] = 0.
extern "C" void __fastcall aurora_trimesh_remove_temporary_body_0043c090(MdlNode* self, void*) {
    if (get<U>(self, 0x134) != 0) {                                                 // 0043C095..0043C09F
        for (I i = 0; i < get<I>(ptr<void>(get<U>(self, 0x134)), 0x64); ++i)        // 0043C0A1..0043C0D1
            live_free(ptr<void>(ptr<U>(get<U>(ptr<void>(get<U>(self, 0x134)), 0x60))[i]));
        void* const post = ptr<void>(get<U>(self, 0x134));                          // 0043C0D3
        free_member(post, 0x60);                                                    // 0043C0D9..0043C0EC
        put<U>(post, 0x68, 0); put<U>(post, 0x64, 0);                               // 0043C0EE..0043C0F1
    }
    put<U>(&historical_storage_007a8a88, 4, 0);                                     // 0043C0F4 (007A8A8C)
    put<U>(&historical_storage_007a8a7c, 4, 0);                                     // 0043C0FA (007A8A80)
    put<U>(&historical_storage_007a8a70, 4, 0);                                     // 0043C100 (007A8A74)
    void* const post = ptr<void>(get<U>(self, 0x134));                              // 0043C106
    if (post != nullptr) {                                                          // 0043C10C..0043C10E
        aurora_trimesh_postprocess_destroy_004453b0(post, nullptr);                 // 0043C110..0043C112
        live_free(post);                                                            // 0043C117..0043C118
    }
    put<U>(self, 0x134, 0);                                                         // 0043C120
}
// 0043C1A0 (saved name MdlNodeTriMesh::InternalRemoveTemporaryArrays, skin table entry): jmp 0043C090.
extern "C" void __fastcall aurora_skin_remove_temporary_body_0043c1a0(MdlNode* self, void*) {
    aurora_trimesh_remove_temporary_body_0043c090(self, nullptr);
}

// 00486560 Gob::DoEvent(event, arg), RET8. A stack SafePointer to this Gob is registered in the Gob's own
// registration list (+4, growth cap ? 2*cap : 8) so a callback that destroys the Gob ends the scan. Callbacks
// (+190 data, +194 count read once before registration) run last-to-first when the record name (inline at +0)
// is empty, '*' or _stricmp-equal to event; with [00798B14] != 0 a record fires only when record time [+28] >
// Gob time [+2C] (x87 fcomp/test ah,41h), or when record byte [+2C] != 0 and vtbl[180] (IsInViewVolume) AL != 0.
// The callback receives (gob, event, arg ? arg : record[+24]). Return: the original leaves an undefined EAX
// (last value of the inlined RemoveReverse32 / loop registers); the canonical slot type returns U, so 0 is
// returned (no caller on this path consumes it: 0043F239, 00449ED7) - documented deviation.
extern "C" U __fastcall gob_do_event_00486560(Gob* self, void*, const char* event, U arg) {
    I index = get<I>(self, 0x194) - 1;                                              // 0048657B..0048658E
    Gob* volatile guard = self;                                                     // 00486584 ([esp+0Ch])
    List* const registrations = list_at(self, 4);                                   // 00486581
    if (registrations->size == registrations->capacity)                             // 00486588..00486591
        menu_list_growth::allocate_005e03c0(registrations, nullptr, registrations->capacity ? static_cast<U>(registrations->capacity) * 2u : 8u); // 00486593..004865A3
    ptr<U>(registrations->data)[registrations->size] = addr(const_cast<Gob**>(&guard)); // 004865A8..004865B1
    registrations->size = registrations->size + 1;                                  // 004865B4
    if (guard == nullptr) return 0;                                                 // 004865B7..004865C5
    for (;;) {
        if (index < 0) {                                                            // 004865D0..004865D2
            Gob* const live = guard;                                                // 0048663F
            if (live != nullptr) list_at(live, 4)->RemoveReverse32(addr(const_cast<Gob**>(&guard))); // 00486641..0048664B
            return 0;
        }
        unsigned char* const record = ptr<unsigned char>(ptr<U>(get<U>(self, 0x190))[index]); // 004865D4..004865DA
        const char first = static_cast<char>(record[0]);                            // 004865DD
        if (first == 0 || first == '*' ||
            kotor_stricmp_0070acaf(reinterpret_cast<const char*>(record), event) == 0) { // 004865DF..004865F3
            bool fire = true;
            if (aurora_event_time_gate_00798b14 != 0 &&                             // 004865F5..004865FC
                !(get<float>(record, 0x28) > get<float>(self, 0x2C))) {             // 004865FE..00486609
                fire = record[0x2C] != 0 &&                                         // 0048660B..00486610
                       gob_vt(self)->s180_gob_is_in_view_volume(self, nullptr) != 0;         // 00486612..0048661E
            }
            if (fire) {
                const U value = arg != 0 ? arg : get<U>(record, 0x24);              // 00486620..00486628
                get<GobEventCallback>(record, 0x20)(self, const_cast<char*>(event), ptr<Part>(value)); // 0048662B..0048662E
            }
        }
        Gob* const live = guard;                                                    // 00486634
        --index;                                                                    // 00486638
        if (live == nullptr) return 0;                                              // 00486639..0048663D
    }
}

// 0043EEE0 (Gob vtbl+184; saved name Gob::GetMinimumBoundingBox): box min [+120..+128] = 10000.0f, max
// [+12C..+134] = -10000.0f, then root part [+6C] vtbl+74(&min, &max, [+1B4]); returns that call's EAX.
extern "C" U __fastcall gob_get_minimum_bounding_box_0043eee0(Gob* self, void*) {
    put<U>(self, 0x120, 0x461C4000u); put<U>(self, 0x124, 0x461C4000u); put<U>(self, 0x128, 0x461C4000u); // 0043EEEC..0043EF18
    put<U>(self, 0x12C, 0xC61C4000u); put<U>(self, 0x130, 0xC61C4000u); put<U>(self, 0x134, 0xC61C4000u); // 0043EF21..0043EF4C
    Part* const root = get<Part*>(self, 0x6C);                                      // 0043EF4F
    return part_vt(root)->bounding_box(root, nullptr, addr(at(self, 0x120)), addr(at(self, 0x12C)), get<U>(self, 0x1B4)); // 0043EF52..0043EF5D
}

namespace {
float f32(double v) { return static_cast<float>(v); }   // the original fstp dword rounding points (D30)
bool same(const unsigned char* field, const char* literal, U length) { return std::memcmp(field, literal, length) == 0; } // repe cmpsb incl. NUL
}

// 0049CAA0 PartEmitter::Initialize (vtbl+7C): render-mode word from node render string (+90); frame-blend word
// [+1F4] = word [00798B84]; [+1F7] = 0; node flags (+12C) bit0 registers SetEmitterTarget on "_EmitterTarget"
// (and, without bit1, [+D4] = [+74]); blend [+4C] from node blend string (+B0); new Material [+1E0] with
// LoadTextures(this) (emitter texture at node+D0); [+F4] by vtbl+84; [+F8] = 10000.0f; [+14C..+154] = 0, [+CC] = 0;
// copies [+164..] -> [+170..], [+1A4..+1B0] -> [+1B4..+1C0]; rotation rows [+128], [+134], [+140] from the
// quaternion at +1A4 (q0..q3 = +1A4..+1B0). Returns the last value left in EAX (row-2 element 0 bits, 0049CDF2).
extern "C" U __fastcall part_emitter_initialize_0049caa0(void* self_raw, void*) {
    Part* const self = static_cast<Part*>(self_raw);
    unsigned char* const node = get<unsigned char*>(self, 4);                       // 0049CABC
    const unsigned char* const render = node + 0x90;                                // 0049CAC1
    bool mode_set = true;
    U mode = 0;
    if (same(render, "Normal", 7)) mode = emitter_render_normal_00798b6c;                                  // 0049CAC7..0049CAD9
    else if (same(render, "Billboard_to_World_Z", 0x15)) mode = emitter_render_billboard_world_z_00798b74; // 0049CAE3..0049CAF5
    else if (same(render, "Billboard_to_Local_Z", 0x15)) mode = emitter_render_billboard_local_z_00798b70; // 0049CAFC..0049CB0E
    else if (same(render, "Linked", 7)) mode = emitter_render_linked_00798b78;                             // 0049CB15..0049CB27
    else if (same(render, "Aligned_to_World_Z", 0x13)) mode = emitter_render_aligned_world_z_00798b7c;     // 0049CB2E..0049CB40
    else if (same(render, "Aligned_to_Particle_Dir", 0x18)) mode = emitter_render_aligned_particle_dir_00798b88; // 0049CB47..0049CB59
    else if (same(render, "Motion_Blur", 0xC)) mode = emitter_render_motion_blur_00798b8c;                // 0049CB60..0049CB72
    else mode_set = false;
    if (mode_set) put<U>(self, 0x114, mode);                                        // 0049CB77
    put<U8>(self, 0x1F7, 0);                                                        // 0049CB84
    put<U16>(self, 0x1F4, static_cast<U16>(emitter_frame_blend_default_00798b84));   // 0049CB7D..0049CB8B
    if ((get<U8>(node, 0x12C) & 1u) != 0) {                                         // 0049CB92..0049CB9C
        Gob* const gob = get<Gob*>(self, 0x40);                                     // 0049CB9E
        const U handle = gob_vt(gob)->register_callback(gob, nullptr, &set_emitter_target_0049b240,
                                                        "_EmitterTarget", self, 0x461C3C00u, 0); // 0049CBA3..0049CBB4 (7413FC)
        put<U>(self, 0x1F0, handle);                                                // 0049CBB7
        if (((get<U>(node, 0x12C) >> 1) & 1u) == 0) put<U>(self, 0xD4, get<U>(self, 0x74)); // 0049CBBD..0049CBCC
    }
    const char* const blend = reinterpret_cast<const char*>(node + 0xB0);           // 0049CBD2
    if (kotor_stricmp_0070acaf(blend, "Normal") == 0) put<U>(self, 0x4C, 0);        // 0049CBD8..0049CBEA
    else if (kotor_stricmp_0070acaf(blend, "PunchThrough") == 0 ||                  // 0049CBEF..0049CBFF (7433D8)
             kotor_stricmp_0070acaf(blend, "Punch-Through") == 0) put<U>(self, 0x4C, 1); // 0049CC01..0049CC1C (7433C8)
    else put<U>(self, 0x4C, 2);                                                     // 0049CC13
    void* const block = live_operator_new(0x4C);                                    // 0049CC23..0049CC25
    void* const material = block ? static_cast<void*>(::new (block) material_owner::Material()) : nullptr; // 0049CC2D..0049CC42
    put<void*>(self, 0x1E0, material);                                              // 0049CC4F
    aurora_material_load_textures_0047b770(material, nullptr, self);                // 0049CC44..0049CC55
    if (part_vt(self)->query84(self, nullptr) != 0) put<U>(self, 0xF4, 0x461C4000u); // 0049CC5A..0049CC6E
    else put<U>(self, 0xF4, 0);                                                     // 0049CC76
    put<U>(self, 0xF8, 0x461C4000u);                                                // 0049CC7C
    put<U>(self, 0x14C, 0); put<U>(self, 0xCC, 0); put<U>(self, 0x150, 0); put<U>(self, 0x154, 0); // 0049CC82..0049CCB7
    put<U>(self, 0x170, get<U>(self, 0x164)); put<U>(self, 0x174, get<U>(self, 0x168)); put<U>(self, 0x178, get<U>(self, 0x16C)); // 0049CCBA..0049CCD3
    for (U k = 0; k < 16; k += 4) put<U>(self, 0x1B4 + k, get<U>(self, 0x1A4 + k)); // 0049CCD6..0049CCF7
    const double q0 = get<float>(self, 0x1A4), q1 = get<float>(self, 0x1A8);
    const double q2 = get<float>(self, 0x1AC), q3 = get<float>(self, 0x1B0);
    const float m00 = f32(1.0 - (q2 * q2 + q3 * q3) * 2.0);                         // 0049CCFA..0049CD18
    const float m01 = f32((q2 * q1 + q3 * q0) * 2.0);                               // 0049CD24..0049CD33
    const float m02 = f32((q3 * q1 - q2 * q0) * 2.0);                               // 0049CD37..0049CD4F
    put<float>(self, 0x128, m00); put<float>(self, 0x12C, m01); put<float>(self, 0x130, m02); // 0049CD42..0049CD57
    const float m10 = f32((q1 * q2 - q0 * q3) * 2.0);                               // 0049CD5A..0049CD69
    const float m11 = f32(1.0 - (q3 * q3 + q1 * q1) * 2.0);                         // 0049CD6D..0049CD97
    const float m12 = f32((q0 * q1 + q2 * q3) * 2.0);                               // 0049CD9F..0049CDB7
    put<float>(self, 0x134, m10); put<float>(self, 0x138, m11); put<float>(self, 0x13C, m12); // 0049CDAA..0049CDBF
    const float m20 = f32((q0 * q2 + q1 * q3) * 2.0);                               // 0049CDC2..0049CDD1
    const float m21 = f32((q3 * q2 - q1 * q0) * 2.0);                               // 0049CDD5..0049CDE4
    const float m22 = f32(1.0 - (q2 * q2 + q1 * q1) * 2.0);                         // 0049CDE8..0049CE12
    put<float>(self, 0x140, m20); put<float>(self, 0x144, m21); put<float>(self, 0x148, m22); // 0049CDF8..0049CE1A
    return get<U>(self, 0x140);                                                     // EAX = m20 bits (0049CDF2)
}

// ---------------------------------------------------------------- CreateReferenceObjects 004448C0
extern "C" void* __cdecl compat_getptd();   // task-35 heap-tls helpers.cpp (original _getptd 006FEEDF; srand route 006FC43A seeds ptd+14)
// 006FC447 rand: ptd+14 = ptd+14 * 343FDh + 269EC3h; return (state >> 16) & 7FFFh.
extern "C" I __cdecl crt_rand_006fc447() {
    void* const ptd = compat_getptd();                                              // 006FC447
    const U state = get<U>(ptd, 0x14) * 0x343FDu + 0x269EC3u;                       // 006FC44C..006FC455
    put<U>(ptd, 0x14, state);                                                       // 006FC45B
    return static_cast<I>((state >> 16) & 0x7FFFu);                                 // 006FC45E..006FC463
}

namespace {
// Per-part block shared by every unrolled level of 004448C0 (e.g. 004448CB..00444981): the reference
// node's model (name at node+50; node cast inline: byte & 11h == 11h) is instanced with NewCAurObject(name,0,0,0)
// into ref[+4C]; on success it is attached to the owning gob at the reference part's name (mode 0) and plays
// "default" at speed 1 + (rand()%100 - 50) * 0.001f with offset (rand()%100) * 0.01f.
void reference_block(Gob* gob, Part* part) {
    Part* const ref = ptr<Part>(part_vt(part)->query38(part, nullptr));            // 004448CB..004448CF
    if (ref == nullptr) return;                                                     // 004448D8..004448DA
    MdlNode* const node = get<MdlNode*>(ref, 4);                                    // 004448E0
    MdlNode* const refnode = (get<U8>(node, 0) & 0x11u) == 0x11u ? node : nullptr;  // 004448E3..004448F7
    Gob* const obj = aurora_new_aur_object_00449cc0(reinterpret_cast<const char*>(at(refnode, 0x50)), nullptr, nullptr, nullptr); // 004448ED..004448FD
    put<Gob*>(ref, 0x4C, obj);                                                      // 00444907
    if (obj == nullptr) return;                                                     // 00444905..0044490A
    const GobVTable* const table = gob_vt(obj);                                     // 0044490E (loaded before the name call)
    const char* const part_name = ptr<const char>(part_vt(ref)->name(ref, nullptr)); // 00444910..00444914 (push 0 = arg 3 below)
    table->s050_gob_attach_to_object(get<Gob*>(ref, 0x4C), nullptr, gob, part_name, 0); // 00444917..0044491C
    const I d1 = crt_rand_006fc447() % 100;                                         // 0044491F..0044492A
    const float speed = static_cast<float>((static_cast<double>(d1) - 50.0) * static_cast<double>(0.001f) + 1.0); // 00444930..00444946 (00740CC0, 0073D708, 0073D764)
    const I d2 = crt_rand_006fc447() % 100;                                         // 0044494A..00444955
    const float offset = static_cast<float>(static_cast<double>(d2) * static_cast<double>(0.01f)); // 00444962..0044496E (0073DFEC)
    Gob* const target = get<Gob*>(ref, 0x4C);                                       // 0044495B
    U speed_bits, offset_bits;
    std::memcpy(&speed_bits, &speed, 4); std::memcpy(&offset_bits, &offset, 4);
    gob_vt(target)->s018_gob_play_animation(target, nullptr, addr("default"), speed_bits, 0, offset_bits); // 00444966..00444981 (73DF6C)
}
}
// 004448C0 CreateReferenceObjects(gob, part), cdecl: the block for part, then recursively for every child
// (+30/+34 re-read); the original unrolls four levels and recurses at 00444D18.
extern "C" void __cdecl aurora_create_reference_objects_004448c0(Gob* gob, Part* part) {
    reference_block(gob, part);                                                     // 004448C7..00444981
    for (I i = 0; i < get<I>(part, 0x34); ++i)                                      // 00444984..00444D7E
        aurora_create_reference_objects_004448c0(gob, ptr<Part>(ptr<U>(get<U>(part, 0x30))[i]));
}

// ---------------------------------------------------------------- callbacks / fog arrays
// 00481A50 CallBack::CallBack(fn, name, data, time, flag), RET14h: [+20] = fn; name (null -> "") copied to +0
// (unbounded byte copy incl. NUL); [+24] = data; [+28] = time; byte [+2C] = flag. Returns this.
extern "C" void* __fastcall callback_construct_00481a50(void* self, void*, GobEventCallback fn, const char* name, Part* data, U raw_time, U flag) {
    put<GobEventCallback>(self, 0x20, fn);                                          // 00481A58..00481A5C
    const char* const text = name != nullptr ? name : "";                          // 00481A54..00481A61 (73D71C)
    std::strcpy(reinterpret_cast<char*>(self), text);                               // 00481A68..00481A78
    put<Part*>(self, 0x24, data);                                                   // 00481A7A..00481A82
    put<U>(self, 0x28, raw_time);                                                   // 00481A8A
    put<U8>(self, 0x2C, static_cast<U8>(flag));                                     // 00481A85..00481A8D
    return self;
}
// 00484E40 Gob::RegisterCallBack(fn, event, data, time, flag), RET14h: record = new(30h) CallBack(...) (0 when the
// allocation fails); appended to the callback list +190 (growth cap ? 2*cap : 8 via 005E03C0); returns the record.
extern "C" U __fastcall gob_register_callback_00484e40(Gob* self, void*, GobEventCallback fn, const char* event, Part* data, U raw_time, U flag) {
    void* const block = live_operator_new(0x30);                                    // 00484E42..00484E46
    void* const record = block ? callback_construct_00481a50(block, nullptr, fn, event, data, raw_time, flag) : nullptr; // 00484E4E..00484E76
    List* const l = list_at(self, 0x190);                                           // 00484E84
    if (l->size == l->capacity)                                                     // 00484E78..00484E8C
        menu_list_growth::allocate_005e03c0(l, nullptr, l->capacity ? static_cast<U>(l->capacity) * 2u : 8u); // 00484E8E..00484E9E
    ptr<U>(l->data)[l->size] = addr(record);                                        // 00484EA3..00484EA8
    l->size = l->size + 1;                                                          // 00484EAB
    return addr(record);                                                            // 00484EAE
}
// 00425C60 GLRender::GetFogArraysInfoRef: h = pool->vtbl[1](0, 8, 2); p = pool->vtbl[2](h, 1); p[0] = p[1] = 0;
// pool->vtbl[3](h); returns h (pool pointer 007A68A4 reloaded before each call).
extern "C" U __cdecl aurora_fog_arrays_info_ref_00425c60() {
    PoolObject* pool = kotor_vertex_pool_007a68a4;                                  // 00425C60
    const U handle = pool->vtable->request(pool, nullptr, 0, 8, 2);                 // 00425C66..00425C6F
    pool = kotor_vertex_pool_007a68a4;                                              // 00425C72
    unsigned char* const p = static_cast<unsigned char*>(pool->vtable->lock(pool, nullptr, handle, 1)); // 00425C78..00425C7F
    pool = kotor_vertex_pool_007a68a4;                                              // 00425C82
    put<U>(p, 0, 0); put<U>(p, 4, 0);                                               // 00425C88..00425C8E
    pool->vtable->unlock(pool, nullptr, handle);                                    // 00425C95..00425C98
    return handle;                                                                  // 00425C9B
}
