// lane-m47-core: CAuroraInterface static entries of the resource-helper family.
//  - installed raster callbacks (service-tail-8 0070D9C0 stores them; texture_cells.hpp declarations):
//    0070C6E0 GetLayeredRaster, 0070C790 GetRaster, 0070C850 ReleaseLayeredRaster, 0070C870 ReleaseRaster
//    (first frame: lane-firstframe PATH.md 7i), 0070CC60/0070CD10/0070CDD0/0070CE80 header readers,
//    0070D380 GetCompressedRaster, 0070D6B0 Get16BitRaster;
//  - GZF read-callback getters (tpc-lane read_callback.cpp, cell 007B8FC4 -> 0070C920): 0070E080 MDL,
//    0070EB50 MDX, 0070E8B0 TGA, 0070EAF0 VIS (MDL/MDX are reached whenever the 3D menu scene loads a model
//    through 007B8FC4; direct consumer 0044C680 reads the cell at 0044C68F, task-4 mdl-recovery-r1
//    consumer-policy-r1 EVIDENCE.json 673ca0d9...);
//  - GetModelResHelper 0070DE20 (requested-model cache 00834094, owner D73/D74 unit xc-0073b6d0);
//  - release internals 0070E8C0/0070E920/0070E980/0070E9E0 and ~CAuroraInterface 0070D340 (shutdown,
//    routes-shutdown 00604412).
// Evidence: lane dis/<addr>.asm (dumpbin /disasm swkotor.exe). Donor cross-check (algorithm only):
// task-4 mdl-recovery-r1 repair-r1 model_helper.cpp (0070DE20) and task-9 mdl-factory-r1 (0070E080).
// D2 (labelled): attribute outputs read after a possibly-failing getter (0070C75B.., 0070C80B.., 0070D4C0..,
// 0070D7B2..) are value-initialised to 0. D30-FINAL: float outputs copied as float values (fld/fstp pairs).
#include "76452bc32b-aurora_family_impl.h"
#include "65c9c926f6-constructors.h"
#include "708974864c-texture_cells.hpp"
#include <cstring>

using namespace aurora_family_impl;
using font_callback_read::Model;
using font_callback_read::ModelX;
using font_callback_read::Texture;
using font_callback_read::Visibility;
using font_callback_read::LayeredTexture;
using font_callback_read::CompressedTexture;
using font_callback_read::Texture16;
using font_callback_read::ProcessedTexture;
using font_callback_read::lower_construct_005e6080;
using font_callback_read::assign_string_005e5c50;
using font_callback_read::data_pointer_007107b0;

namespace kotor_requested_models { extern kotor_list::ModelList list_00834094; }   // owner: xc-0073b6d0 unit (D73/D74)
namespace alias_add { int __fastcall Greater005e5490(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*); }
extern "C" {
int __fastcall async_string_equal_005e5200(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*);
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*, void*, menu_resources::String*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
void __fastcall c2da_005e9c10(void*, void*, void*);
void __cdecl borrowed_release_processed_texture_0070ea40(void*);   // tpc-release (tpc_release.cpp)
}
namespace base_ini_reconstruction { __declspec(noinline) void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }

extern "C" {
// D21 tail entries (five-byte JMPs), cdecl one pointer, caller cleanup.
Texture16* __cdecl aurora_texture16_tail_0070e910(String*) noexcept(false);
LayeredTexture* __cdecl aurora_layered_tail_0070e970(String*) noexcept(false);
CompressedTexture* __cdecl aurora_compressed_tail_0070e9d0(String*) noexcept(false);
void __cdecl aurora_release_texture_0070e8c0(Texture*);
void __cdecl aurora_release_texture16_0070e920(Texture16*);
void __cdecl aurora_release_layered_0070e980(LayeredTexture*);
void __cdecl aurora_release_compressed_0070e9e0(CompressedTexture*);
}

namespace {
struct CompletedString {
    String* object;
    ~CompletedString() noexcept { if (object) object->~String(); }
    void destroy() noexcept { String* p = object; object = nullptr; p->~String(); }
};
// `lea out,[temp]; call 005E6080 (lower into temp); push eax; call 005E5C50 (assign); ~temp 005E5C20`.
void lower_assign(String* destination, String* input) {
    alignas(String) unsigned char storage[sizeof(String)];
    String* const result = lower_construct_005e6080(input, nullptr, storage);
    CompletedString cleanup{static_cast<String*>(static_cast<void*>(storage))};
    assign_string_005e5c50(destination, nullptr, result);
    cleanup.destroy();
}
template <class H> struct StackHelper {   // helper object constructed in a caller frame (EH state 0 -> derived dtor)
    H* object; void (__fastcall* destroy)(H*, void*) noexcept;
    ~StackHelper() noexcept { if (object) destroy(object, nullptr); }
};
// Inline `strlen` loop of the header readers (e.g. 0070CC7D..0070CC89), unsigned compare with 10h.
bool name_too_long(const char* name) {
    const char* p = name;
    while (*p++ != 0) {}
    return static_cast<Word>(p - name - 1) > 0x10u;
}
// Common Release*Internal shape (0070E8C0 / 0070E920 / 0070E980 / 0070E9E0): demands of [this+8] read with no
// null test; info tag/owner check; unload; delete when no demand remains.
template <class H, class Info, class Unload> void release_internal(H* self, Info info, std::uint16_t tag, Unload unload) {
    if (aurora_get_demands_004064b0(self->resource, nullptr) < 1) return;        // +05..+10
    const void* const tagged = info(self, nullptr);                              // +12..+14
    if (!tagged) return;                                                         // +19..+1B
    if (load<std::uint16_t>(tagged, 0) != tag) return;                           // +1D..+21
    if (load<void*>(tagged, 2) != self) return;                                  // +23..+26
    unload(self, nullptr);                                                       // +28..+2A
    if (aurora_get_demands_004064b0(self->resource, nullptr) >= 1) return;       // +2F..+3B
    call_slot0(self, 1u);                                                        // +3D..+42
}
}

extern "C" {
// ======================================================================= tail entries
namespace font_callback_read {
// 0070E8B0: JMP 0070E130.
__declspec(naked) Texture* __cdecl texture_0070e8b0(String*) noexcept(false) { __asm { jmp aurora_texture_target_0070e130 } }
// 0070EAF0: JMP 0070E6D0.
__declspec(naked) Visibility* __cdecl visibility_0070eaf0(String*) noexcept(false) { __asm { jmp aurora_visibility_target_0070e6d0 } }
// 0070EB50: JMP 0070E7C0.
__declspec(naked) ModelX* __cdecl modelx_0070eb50(String*) noexcept(false) { __asm { jmp aurora_modelx_target_0070e7c0 } }
}
// 0070E910: JMP 0070E220.
__declspec(naked) Texture16* __cdecl aurora_texture16_tail_0070e910(String*) noexcept(false) { __asm { jmp aurora_texture16_target_0070e220 } }
// 0070E970: JMP 0070E310.
__declspec(naked) LayeredTexture* __cdecl aurora_layered_tail_0070e970(String*) noexcept(false) { __asm { jmp aurora_layered_target_0070e310 } }
// 0070E9D0: JMP 0070E400.
__declspec(naked) CompressedTexture* __cdecl aurora_compressed_tail_0070e9d0(String*) noexcept(false) { __asm { jmp aurora_compressed_target_0070e400 } }

// ======================================================================= model cache helper
// 0070DE20 CAuroraInterface::GetModelResHelper(name, mode, int* comparison, node** position) (cdecl).
__declspec(noinline) Model* __cdecl aurora_model_helper_0070de20(String* name, Signed mode, void* comparison_out,
                                                                 void* position_out) noexcept(false) {
    String requested;                                                            // 0070DE40..44 (state 0)
    String candidate;                                                            // 0070DE49..55 (state 1)
    lower_assign(&requested, name);                                              // 0070DE5A..84 (states 2 -> 1)
    kotor_list::Internal* const cache = kotor_requested_models::list_00834094.internal;   // 0070DE89
    void* position = cache->head;                                                // 0070DE8E..90 [esp+8]
    void* const head = cache->head;                                              // 0070DE94
    Model* model = head ? load<Model*>(head, 8) : nullptr;                       // 0070DE96..A2 (ESI)
    if (position_out) store<void*>(position_out, 0, nullptr);                    // 0070DE9D..A6 (EDI)
    bool allocate = false;
    if (position) {                                                              // 0070DEAC..AE
        for (;;) {
            if (model) {                                                         // 0070DEB0..B2
                c2da_ref_to_string_00405f70(&model->reference, nullptr, &candidate);   // 0070DEB4..BC
                lower_assign(&candidate, &candidate);                            // 0070DEC1..E7 (states 3 -> 1)
                if (async_string_equal_005e5200(&candidate, nullptr, &requested)) {     // 0070DEEC..FC
                    if (comparison_out) store<Word>(comparison_out, 0, 0u);      // 0070DF6E..75
                    if (position_out) store<void*>(position_out, 0, nullptr);    // 0070DF7B..7F
                    if (mode != 0) return model;                                 // 0070DF85..8A -> 0070E03F (no coalesce)
                    c2da_005e9c10(kotor_requested_models::list_00834094.internal, nullptr, position); // 0070DF90..9B
                    break;                                                       // 0070DFA0 -> 0070DF2E
                }
                if (alias_add::Greater005e5490(&candidate, nullptr, &requested)) {      // 0070DEFE..0E
                    if (comparison_out) store<Word>(comparison_out, 0, 1u);      // 0070DFA2..A9
                    if (position_out) store<void*>(position_out, 0, position);   // 0070DFAF..B7
                    allocate = true;                                             // 0070DFB9 -> 0070DF36
                    break;
                }
            }
            model = reinterpret_cast<Model*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(
                kotor_requested_models::list_00834094.internal, nullptr, &position)));      // 0070DF14..24
            if (!position) break;                                                // 0070DF26..2C
        }
    }
    if (allocate || model == nullptr) {                                          // 0070DF2E..30
        void* const raw = live_operator_new(0x1c);                               // 0070DF36..3D
        {
            Allocation guard{raw};                                               // state 4 (0070DF48)
            if (raw) {
                menu_resources::Ref outgoing;
                (void)async_ref_construct_string_00406d60(&outgoing, nullptr, name);   // 0070DF4F..60
                model = aurora_model_construct_00710300(static_cast<Model*>(raw), nullptr, outgoing, mode); // 0070DF65..67
            } else {
                model = nullptr;                                                 // 0070DFBE
            }
            guard.raw = nullptr;                                                 // state 1 (0070DFC2)
        }
        if (!model) return nullptr;                                              // 0070DFC0..E9 (both strings destroyed)
    }
    // 0070DFFC: coalesce with an already-loaded owner of the same resource.
    if (void* resource = model->resource) {                                      // 0070DFFC..E001
        if (aurora_get_demands_004064b0(resource, nullptr) > 0) {                // 0070E003..0A
            const void* const tagged = aurora_helper_info_0070faf0(model, nullptr); // 0070E00C..0E
            if (tagged && load<std::uint16_t>(tagged, 0) == 0x07d2) {            // 0070E013..1C
                Model* const owner = load<Model*>(tagged, 2);                    // 0070E01E
                if (owner) {                                                     // 0070E024..26
                    if (comparison_out) store<Word>(comparison_out, 0, 0u);      // 0070E028..2F
                    call_slot0(model, 1u);                                       // 0070E035..3B
                    model = owner;                                               // 0070E03D
                }
            }
        }
    }
    return model;                                                                // 0070E03F..70
}

// 0070E080 CAuroraInterface::GetModelInternal(name, mode, int*, node**) (cdecl).
namespace font_callback_read {
__declspec(noinline) Model* __cdecl model_0070e080(String* name, Signed mode, void* comparison_out, void* position_out) noexcept(false) {
    Model* const model = aurora_model_helper_0070de20(name, mode, comparison_out, position_out); // 0070E080..96 (args forwarded)
    if (!model) return nullptr;                                                  // 0070E0A0..A2
    if (mode != 0) return model;                                                 // 0070E0A4..A6
    if (aurora_model_load_007100d0(model, nullptr)) return model;                // 0070E0A8..AF
    void* const resource = model->resource;                                      // 0070E0B3
    if (resource && aurora_get_demands_004064b0(resource, nullptr) >= 1) return nullptr; // 0070E0B6..C2
    call_slot0(model, 1u);                                                       // 0070E0C4..CA
    return nullptr;
}
}

// ======================================================================= raster getters
// 0070C790 CAuroraInterface::GetRaster(name, w, h, bytes) (cdecl, SEH 0073A898).
__declspec(noinline) char* __cdecl aurora_get_raster_0070c790(const char* filename, acquisition::Word* width,
                                                              acquisition::Word* height, acquisition::Word* bytes) {
    Texture* object;
    {
        String name(filename);                                                   // 0070C7AD..B2 005E5A90 (state 0)
        object = font_callback_read::texture_0070e8b0(&name);                    // 0070C7B7..C4
    }                                                                            // 0070C7CC..DA 005E5C20
    if (!object) return nullptr;                                                 // 0070C7DF..E3
    Word w = 0, h = 0; unsigned char bits = 0;                                   // D2 (0070C80B reads)
    (void)aurora_texture_attrib_0070ebb0(object, nullptr, &w, &h, &bits);        // 0070C7F5..806
    *width = w;                                                                  // 0070C80B..17
    *height = h;                                                                 // 0070C81E..22
    *bytes = Word{bits} >> 3;                                                    // 0070C819 / 0070C828..2B
    return font_callback_read::txi_pointer_raw_0070f3e0(object, nullptr);        // 0070C82D..2F (shared 0070F3E0)
}
// 0070C6E0 CAuroraInterface::GetLayeredRaster(name, w, h, layers) (cdecl).
__declspec(noinline) char* __cdecl aurora_get_layered_raster_0070c6e0(const char* filename, acquisition::Signed* width,
                                                                      acquisition::Signed* height, acquisition::Signed* third) {
    LayeredTexture* object;
    {
        String name(filename);                                                   // 0070C6FD..702
        object = aurora_layered_tail_0070e970(&name);                            // 0070C707..714
    }                                                                            // 0070C71C..72A
    if (!object) return nullptr;                                                 // 0070C72F..33
    Word a = 0, b = 0, c = 0;                                                    // D2 (0070C75B reads)
    (void)aurora_layered_attrib_0070da60(object, nullptr, &a, &b, &c);           // 0070C745..756
    *width = static_cast<Signed>(a);                                             // 0070C75B..67
    *height = static_cast<Signed>(b);                                            // 0070C769..71
    *third = static_cast<Signed>(c);                                             // 0070C773..77
    return font_callback_read::processed_pointer_0070f380(reinterpret_cast<ProcessedTexture*>(object), nullptr); // 0070C779..7B (shared 0070F380)
}
namespace {
// Shared precedence of 0070D5D4.. (TPC) as used by 0070D421..59 / 0070D72B..50: primary wins at 2, 4, 3 (in that
// order) unless another candidate has that code first; otherwise the primary must be 1.
bool primary_selected(Word primary, Word second, Word third) {
    if (primary == 2) return true;
    if (second == 2 || third == 2) return false;
    if (primary == 4) return true;
    if (second == 4 || third == 4) return false;
    if (primary == 3) return true;
    if (second == 3 || third == 3) return false;
    return primary == 1;
}
}
// 0070D380 CAuroraInterface::GetCompressedRaster(name, w, h, format, size, alpha) (cdecl, SEH 0073A878).
__declspec(noinline) char* __cdecl aurora_get_compressed_raster_0070d380(const char* filename, acquisition::Signed* width,
        acquisition::Signed* height, acquisition::Signed* format, acquisition::Signed* size, float* alpha) {
    menu_resources::Ref name;
    name.construct_00406d80(filename);                                           // 0070D39E..A3
    Word dds = 0, fourpc = 0, tga = 0;                                           // 0070D3BF..C7
    if (!c2da_manager_007a39e8()->exists_00408bc0(&name, 0x07f1, &dds)) dds = 0;          // 0070D3A8..D8
    if (!c2da_manager_007a39e8()->exists_00408bc0(&name, 0x080b, &fourpc)) fourpc = 0;    // 0070D3DC..FA
    const Word third = c2da_manager_007a39e8()->exists_00408bc0(&name, 0x0003, &tga) ? tga : 0u; // 0070D3FE..21 (ECX)
    if (!primary_selected(dds, fourpc, third)) return nullptr;                   // 0070D421..59 -> 0070D48D
    CompressedTexture* object;
    {
        String text(filename);                                                   // 0070D45B..60
        object = aurora_compressed_tail_0070e9d0(&text);                         // 0070D465..6E
    }                                                                            // 0070D476..84
    if (!object) return nullptr;                                                 // 0070D489..8B
    Word w = 0, h = 0, s = 0; unsigned char f = 0; float a = 0.0f;               // D2 (0070D4C0 reads)
    (void)aurora_compressed_attrib_007103c0(object, nullptr, &w, &h, &f, &s, &a); // 0070D4A0..BB
    *width = static_cast<Signed>(w);                                             // 0070D4C4..D0
    *height = static_cast<Signed>(h);                                            // 0070D4D2..DB
    *format = static_cast<Signed>(f);                                            // 0070D4D6..E5 (movzx)
    *size = static_cast<Signed>(s);                                              // 0070D4E7..EF
    *alpha = a;                                                                  // 0070D4C0 fld / 0070D4F3 fstp
    return data_pointer_007107b0(object, nullptr);                               // 0070D4F1..F5 (shared 007107B0)
}
// 0070D6B0 CAuroraInterface::Get16BitRaster(name, w, h, alpha, format) (cdecl).
__declspec(noinline) char* __cdecl aurora_get_16bit_raster_0070d6b0(const char* filename, acquisition::Signed* width,
        acquisition::Signed* height, float* alpha, acquisition::Signed* format) {
    menu_resources::Ref name;
    name.construct_00406d80(filename);                                           // 0070D6CE..D3
    Word fourpc = 0, tga = 0;                                                    // 0070D6EF..F7
    if (!c2da_manager_007a39e8()->exists_00408bc0(&name, 0x080b, &fourpc)) fourpc = 0;    // 0070D6D8..708
    const Word second = c2da_manager_007a39e8()->exists_00408bc0(&name, 0x0003, &tga) ? tga : 0u; // 0070D70C..27 (EAX)
    if (!primary_selected(fourpc, second, 0u)) return nullptr;                   // 0070D72B..50 -> 0070D784
    Texture16* object;
    {
        String text(filename);                                                   // 0070D752..57
        object = aurora_texture16_tail_0070e910(&text);                          // 0070D75C..65
    }                                                                            // 0070D76D..7B
    if (!object) return nullptr;                                                 // 0070D780..82
    Word w = 0, h = 0; float a = 0.0f; Signed f = 0;                             // D2 (0070D7B2 reads)
    (void)aurora_texture16_attrib_00710770(object, nullptr, &w, &h, &a, &f);     // 0070D797..AD
    *width = static_cast<Signed>(w);                                             // 0070D7B6..C2
    *height = static_cast<Signed>(h);                                            // 0070D7C4..CC
    *alpha = a;                                                                  // 0070D7B2 fld / 0070D7CE fstp
    *format = f;                                                                 // 0070D7D0..DA
    return data_pointer_007107b0(object, nullptr);                               // 0070D7D8..DC
}

// ======================================================================= header readers
// 0070CC60 ReadRasterHeader(name, w, h, bytes): stack CAuroraTexture(Ref(name), 0); ReadTextureHeader; ~.
__declspec(noinline) acquisition::Word __cdecl aurora_read_raster_header_0070cc60(const char* filename, acquisition::Signed* a,
        acquisition::Signed* b, acquisition::Signed* c) {
    if (name_too_long(filename)) return 0;                                       // 0070CC7D..90
    menu_resources::Ref ref;
    ref.construct_00406d80(filename);                                            // 0070CCA2..AE (outgoing area)
    Texture helper;
    aurora_texture_construct_0070efa0(&helper, nullptr, ref, 0);                 // 0070CCB3..B7 (state 0)
    StackHelper<Texture> guard{&helper, &aurora_texture_destroy_0070f030};
    const Signed result = aurora_texture_read_header_0070ece0(&helper, nullptr, a, b, c);  // 0070CCBC..D7
    return static_cast<acquisition::Word>(result);                               // 0070CCDC..EA ~ 0070F030
}
// 0070CD10 ReadProcessedRasterHeader(name, 7 outputs): stack CAuroraProcessedTexture; 0070F590; ~0070FA10.
__declspec(noinline) acquisition::Word __cdecl aurora_read_processed_raster_header_0070cd10(const char* filename,
        acquisition::Signed* a, acquisition::Signed* b, char* c, acquisition::Signed* d, acquisition::Signed* e, float* f,
        char* g) {
    if (name_too_long(filename)) return 0;                                       // 0070CD2D..40
    menu_resources::Ref ref;
    ref.construct_00406d80(filename);                                            // 0070CD52..5E
    ProcessedTexture helper;
    aurora_processed_construct_0070f980(&helper, nullptr, ref, 0);               // 0070CD63..67
    StackHelper<ProcessedTexture> guard{&helper, &aurora_processed_destroy_0070fa10};
    const Signed result = aurora_processed_read_header_0070f590(&helper, nullptr, a, b, c, d, e, f, g); // 0070CD6C..9B
    return static_cast<acquisition::Word>(result);                               // 0070CDA0..AE ~ 0070FA10
}
// 0070CDD0 ReadCompressedRasterHeader(name, 5 outputs): stack CAuroraCompressedTexture; 00710430; ~00710740.
__declspec(noinline) acquisition::Word __cdecl aurora_read_compressed_raster_header_0070cdd0(const char* filename,
        acquisition::Signed* a, acquisition::Signed* b, acquisition::Signed* c, acquisition::Signed* d, float* e) {
    if (name_too_long(filename)) return 0;                                       // 0070CDED..00
    menu_resources::Ref ref;
    ref.construct_00406d80(filename);                                            // 0070CE12..1E
    CompressedTexture helper;
    aurora_compressed_construct_007106b0(&helper, nullptr, ref, 0);              // 0070CE23..27
    StackHelper<CompressedTexture> guard{&helper, &aurora_compressed_destroy_00710740};
    const Signed result = aurora_compressed_read_header_00710430(&helper, nullptr, a, b, c, d, e); // 0070CE2C..51
    return static_cast<acquisition::Word>(result);                               // 0070CE56..64 ~ 00710740
}
// 0070CE80 Get16BitRasterHeader(name, 4 outputs): stack CAurora16BitTexture; 00710810; ~00710B20.
__declspec(noinline) acquisition::Word __cdecl aurora_get_16bit_raster_header_0070ce80(const char* filename,
        acquisition::Signed* a, acquisition::Signed* b, float* c, acquisition::Signed* d) {
    if (name_too_long(filename)) return 0;                                       // 0070CE9D..B0
    menu_resources::Ref ref;
    ref.construct_00406d80(filename);                                            // 0070CEC2..CE
    Texture16 helper;
    aurora_texture16_construct_00710a90(&helper, nullptr, ref, 0);               // 0070CED3..D7
    StackHelper<Texture16> guard{&helper, &aurora_texture16_destroy_00710b20};
    const Signed result = aurora_texture16_read_header_00710810(&helper, nullptr, a, b, c, d);  // 0070CEDC..FC
    return static_cast<acquisition::Word>(result);                               // 0070CF01..0F ~ 00710B20
}

// ======================================================================= release
// 0070E8C0 ReleaseTextureInternal: tag 0003 via 0070ED40, unload 0070EC70.
__declspec(noinline) void __cdecl aurora_release_texture_0070e8c0(Texture* self) {
    release_internal(self, &aurora_texture_info_0070ed40, 0x0003, &aurora_texture_unload_0070ec70);
}
// 0070E920 Release16BitTextureInternal: tag 080B via 00710870, unload 00710100.
__declspec(noinline) void __cdecl aurora_release_texture16_0070e920(Texture16* self) {
    release_internal(self, &aurora_texture16_info_00710870, 0x080b, &aurora_unload_00710100);
}
// 0070E980 ReleaseLayeredTextureInternal: tag 0006 via 0070DB10, unload 00710100.
__declspec(noinline) void __cdecl aurora_release_layered_0070e980(LayeredTexture* self) {
    release_internal(self, &aurora_layered_info_0070db10, 0x0006, &aurora_unload_00710100);
}
// 0070E9E0 ReleaseCompressedTextureInternal: tag 07F1 via 00710490, unload 00710100.
__declspec(noinline) void __cdecl aurora_release_compressed_0070e9e0(CompressedTexture* self) {
    release_internal(self, &aurora_compressed_info_00710490, 0x07f1, &aurora_unload_00710100);
}
// 0070C850 ReleaseLayeredRaster(pixels, w, h): owner DWORD at pixels + w*h*2 + 6 (no tag check).
__declspec(noinline) void __cdecl aurora_release_layered_raster_0070c850(void* pixels, texture::I width, texture::I height) {
    const Word cells = static_cast<Word>(width) * static_cast<Word>(height);    // 0070C850..54 (imul)
    aurora_release_layered_0070e980(load<LayeredTexture*>(pixels, cells * 2u + 6u)); // 0070C859..62
}
// 0070C870 ReleaseRaster(pixels, w, h, bytes, size, size2, txi, processed).
__declspec(noinline) void __cdecl aurora_release_raster_0070c870(char* pixels, acquisition::Signed width,
        acquisition::Signed height, acquisition::Signed bytes, acquisition::Word size, acquisition::Signed size2,
        acquisition::Signed txi, acquisition::Word processed) {
    unsigned char* const base = reinterpret_cast<unsigned char*>(pixels);
    if (static_cast<unsigned char>(processed) != 0) {                            // 0070C870..76 (AL)
        const Word skip = size != 0 ? size : static_cast<Word>(size2);           // 0070C878..88
        unsigned char* const tag = base + skip + static_cast<Word>(txi) + 4u;    // 0070C88C..95
        if (load<std::uint16_t>(tag, 0) != 0x0bbf) return;                       // 0070C88E..99
        borrowed_release_processed_texture_0070ea40(load<void*>(tag, 2));        // 0070C89B..A4
        return;
    }
    if (size != 0) {                                                             // 0070C8A8..AE
        if (load<std::uint16_t>(base - 0x1a, 0) != 0x07f1) return;               // 0070C8B0..BA
        aurora_release_compressed_0070e9e0(load<CompressedTexture*>(base - 0x18, 0)); // 0070C8BC..C5
        return;
    }
    if (bytes == 2) {                                                            // 0070C8C9..D0
        if (load<std::uint16_t>(base - 0x16, 0) != 0x080b) return;               // 0070C8D2..DC
        aurora_release_texture16_0070e920(load<Texture16*>(base - 0x14, 0));     // 0070C8DE..E7
        return;
    }
    const Word offset = static_cast<Word>(width) * static_cast<Word>(height) * static_cast<Word>(bytes); // 0070C8EB..F8
    unsigned char* const tag = base + offset + 4u;                               // 0070C8FB..901
    if (load<std::uint16_t>(tag, 0) != 0x0003) return;                           // 0070C8FB..905
    aurora_release_texture_0070e8c0(load<Texture*>(tag, 2));                     // 0070C907..90B
}

// ======================================================================= ~CAuroraInterface
// 0070D340: delete every requested-model helper still cached (RemoveHead, Unload, slot0(1)), then the shared
// tail 0041EBD0 (reached only from 0070D376): free palette image 007A4704, zero 007A4704/007A4708.
__declspec(noinline) void __fastcall kotor_fastcall_0070d340(void*, void*) {
    while (kotor_requested_models::list_00834094.internal->head != nullptr) {    // 0070D340..49 / 0070D36A..73
        void* const helper = base_ini_reconstruction::remove_head_005e9c80(
            kotor_requested_models::list_00834094.internal, nullptr);            // 0070D350..55
        if (helper) {                                                            // 0070D357..59
            aurora_unload_00710100(helper, nullptr);                             // 0070D35B..5D
            call_slot0(helper, 1u);                                              // 0070D362..68
        }
    }
    live_free(const_cast<unsigned char*>(texture_cells::palette_image_007a4704)); // 0041EBD0..D6 006FA390
    texture_cells::palette_image_007a4704 = nullptr;                             // 0041EBE0
    texture_cells::palette_height_007a4708 = 0;                                  // 0041EBE5
}
}
