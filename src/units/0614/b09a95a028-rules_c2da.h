#pragma once
// lane-prepare: CSWRules::CSWRules 00552C50 and C2DA::GetINTEntry 00413660
// (swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000), D1 form:
// original __thiscall -> __fastcall(self, unused EDX, stack words...).
// Every callee below without a definition in the M41 strict-link set is an
// explicit still-missing genuine provider (link UNDEF, never a stub).
#include "a767de49a4-services.h"          // client_services::String / Dependencies slot shapes
#include "c534281817-startup_string.h"    // kotor_reconstruction::StartupString (005E5A90/005B3190/005E5C20 owner)
#include "c30aaa70eb-resources.h"         // menu_resources::Ref (16-byte CResRef) + construct_00406d80
#include <cstdint>
// lane-rules: typed CSWRules vtable 00748BB4 + C++-linkage subtree entry points (rules_api.h).
#include "fe4d299c89-rules_api.h"

namespace kotor_rules_c2da { using Word = std::uint32_t; }
#include "10d1301a0a-table_contract.h"   // lane-startup-misc donor-c2da: c2da_current::Table/String/Ref + 00413CC0/00414A30
#include "f430e94c06-load.h"             // lane-startup-misc donor-c2da: c2da_current_load_004143b0
// D38: C2DA 00413100/00413430/00413660/00413CC0/004143B0/00414A30 are owned by
// lane-startup-misc (src/c2da/c2da_entries.cpp + donor-c2da); identical declarations:
extern "C" {
__declspec(noinline) std::int32_t __fastcall c2da_current_get_float_00413430(c2da_current::Table*, void*, std::int32_t, c2da_current::String*, float*);
__declspec(noinline) std::int32_t __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*, std::int32_t, c2da_current::String*, std::int32_t*);
}
namespace kotor_rules_c2da {
// client_services::Dependencies::get_int_00413660 slot adapter (typed transport only;
// the 00413660 algorithm is lane-startup-misc's c2da_current_get_int_00413660).
int __fastcall ClientGetIntEntry00413660(void* table, void* unused_edx, int row, client_services::String* column, int* value);
}
extern "C" {
// ---- still-MISSING genuine providers (UNDEF) ----
// 005BFB60 CTwoDimArrays::CTwoDimArrays, RET (00552EAD) - [lane-prepare] rules_small.cpp.
void* __fastcall kotor_fastcall_005bfb60(void* self, void* unused_edx);
// 005C4C20/0059BA20/005515E0/005B31D0/00550590/0054F2C0/0054FEC0: lane-rules (rules_api.h, namespace kotor_rules).
}

// ---- [lane-prepare] defined in rules_c2da.cpp: original has an EH frame
// (handler 0071D8AF, states 0..9), so C++ linkage preserves the unwinds. ----
namespace kotor_original {
// 00552C50 CSWRules::CSWRules(), RET; receiver = 0xDC allocation (005F5238).
void* __fastcall kotor_fastcall_00552c50(void* self, void* unused_edx);
// 00550C50 CSWRules::LoadDifficultyInfo, RET (00553000); EH frame 0071D64B -> C++ linkage (rules_small.cpp).
void __fastcall kotor_fastcall_00550c50(void* rules, void* unused_edx);
}
