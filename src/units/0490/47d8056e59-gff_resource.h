#pragma once
// Lane gff: CResGFF (0xA0-byte resource, vtable 0073E2D0) construction,
// resource callbacks and destruction.
// Original members are __thiscall (ECX receiver). Following the M40 C2DA
// cohort (resource-successor/table, callees), these are free-function source
// ports with the project __fastcall transport: ECX receiver, explicitly unused
// EDX, original stack arguments in original order, callee cleanup
// (fastcall @N == thiscall RET N-8).
#include "91aa5306fb-resource_methods.h"   // M40 resource-successor/table: Word, DeletingDestroy,
                                // ConstantWord, ResourceCallback, 00406490/0063E7F0 leaves,
                                // resource_lower.h lower imports (00406460, 00409B10,
                                // 00409B80, 00409CF0, 007A39E8 accessor)
#include "faae54b0b1-gff_views.h"          // q05 consumed CResGFF view (+40..+70 offsets, size 0xA0)
#include <cstddef>
#include <type_traits>

namespace gff_resource {
using Word = c2da_resource_methods::Word;
using Gff = kotor_q05::CResGFF;
using Ref = menu_resources::Ref;

// Observed offsets (cited per store in gff_resource.cpp). Only offsets touched by
// the eight bodies here are named; +7C..+8B and +91..+93 are never written by the
// constructor (00410630) and stay unnamed/untouched.
enum Offset : unsigned {
    kData = 0x10,             // CRes resource data pointer (read 00410743, 00410700)
    kConst28 = 0x28, kConst2C = 0x2C, kConst30 = 0x30, kConst34 = 0x34, // 004106A0..B5
    kCapacity38 = 0x38, kCapacity3C = 0x3C,                             // 0041065E/61 (0xFD4)
    kHeader = 0x40, kStructs = 0x44, kFields = 0x4C, kLabels = 0x54,
    kFieldData = 0x5C, kFieldIndices = 0x64, kListIndices = 0x70,
    kZeroEnd = 0x78,          // last dword of the +40..+78 cleared run (0041069D, 0041091F)
    kByte8C = 0x8C,           // 004106BC
    kFileType = 0x8D,         // 4-byte tag, unaligned (004106D9, 004106DB, 004107B5..DF)
    kServiced = 0x94,         // 004106C2, 0041086D, 0041088A
    kBorrowed = 0x98,         // 004106C8, 00410867, 004108A7 (1 = section pointers alias +10 data)
    kHeld = 0x9C,             // 004106CE, 00410713, 004108E9 (1 = Demand produced data in ctor)
    kSize = 0xA0              // new(0xA0) at 0040A6AC
};
static_assert(sizeof(Gff) == kSize, "q05 view size equals observed allocation");
static_assert(offsetof(Gff, header) == kHeader && offsetof(Gff, structs) == kStructs &&
              offsetof(Gff, fields) == kFields && offsetof(Gff, labels) == kLabels &&
              offsetof(Gff, field_data) == kFieldData && offsetof(Gff, field_indices) == kFieldIndices &&
              offsetof(Gff, list_indices) == kListIndices, "q05 consumed offsets");

// Five cells at 0073E2D0 (0073E2E4 onward is unrelated "%f"/"%x" string data).
struct Methods0073e2d0 {
    c2da_resource_methods::DeletingDestroy deleting_destroy_00413010;   // +00
    c2da_resource_methods::ConstantWord constant_word_00406490;         // +04 shared CRes leaf
    c2da_resource_methods::ConstantWord constant_word_0063e7f0;         // +08 shared CRes leaf
    c2da_resource_methods::ResourceCallback freed_00410880;             // +0C OnResourceFreed
    c2da_resource_methods::ResourceCallback serviced_00410740;          // +10 OnResourceServiced
};
static_assert(std::is_standard_layout<Methods0073e2d0>::value && sizeof(Methods0073e2d0) == 20,
              "exact five-cell table");
static_assert(offsetof(Methods0073e2d0, freed_00410880) == 0x0C &&
              offsetof(Methods0073e2d0, serviced_00410740) == 0x10, "slot offsets");
}

extern "C" {
// 00410630 CResGFF::CResGFF(unsigned short type, char const* file_type, CResRef const& name) RET 0Ch.
// Symbol is the one already required by lane-gui-load (0040A6D5 consumer).
__declspec(noinline) gff_resource::Gff* __fastcall gui_load_gff_construct_00410630(
    gff_resource::Gff* self, void* unused_edx, gff_resource::Word type, const char* file_type,
    const gff_resource::Ref* name);
// 00410740 CResGFF::OnResourceServiced() RET 0 (vtable +10).
__declspec(noinline) gff_resource::Word __fastcall gff_resource_serviced_00410740(void* self, void* unused_edx);
// 00410880 CResGFF::OnResourceFreed() RET 0 (vtable +0C).
__declspec(noinline) gff_resource::Word __fastcall gff_resource_freed_00410880(void* self, void* unused_edx);
// 004108C0 CResGFF::ReleaseResource() RET 0 (direct call from 004115FF).
__declspec(noinline) void __fastcall gff_resource_release_004108c0(void* self, void* unused_edx);
// 00411570 CResGFF::~CResGFF() RET 0.
__declspec(noinline) void __fastcall gff_resource_destroy_00411570(void* self, void* unused_edx);
// 00413010 scalar deleting destructor RET 4 (vtable +00).
__declspec(noinline) void* __fastcall gff_resource_deleting_destroy_00413010(
    void* self, void* unused_edx, gff_resource::Word flags);

extern const gff_resource::Methods0073e2d0 gff_resource_method_table_0073e2d0;
// 0078D3CC: static data cell holding a pointer to 0073E2C8 "V3.2"; read live at
// 004107E5 and 00411278; no instruction in .text writes it.
extern const char gff_version_text_0073e2c8[8];
extern const char* volatile gff_version_0078d3cc;

// ---- imports (not defined here) ----
// M40 cres-base-destroy (base_destroy.obj); same declaration as callees/destruction.h.
void __fastcall c2da_resource_base_destroy_00409d40(void* self, void* unused_edx);
// 00407680 CExoResMan::SetResObject RET 0Ch; declaration identical to
// task-10 c2da-current-producer-r1 binding.h. Provider: lane-resman (not M40/M41).
void __fastcall c2da_register_00407680(menu_resources::Manager*, void*, menu_resources::Ref*,
                                       menu_resources::Word, void*);
}
