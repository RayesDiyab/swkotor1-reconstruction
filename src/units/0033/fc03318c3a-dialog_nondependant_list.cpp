// Lane W4 t2: CGuiInGame dialog non-dependant list (+B4C), W4-owned per coordinator. Bodies adopted from W7
// lane-w7\tiers\t6\source\vm_actions_t6.cpp (review confirmed they match the original), exported under W4's
// kotor_fastcall_* names; W7 binds to these. The matching free 0062F8B0 is in src/dialog/dialog_animations.cpp.
#include "21665bcd5a-w4_native.hpp"
#include "f99027b04a-w4_providers.hpp"
using namespace w4;
namespace {
// CExoLinkedListInternal::GetNext 005E9B90 returns the data in EAX; w4_providers.hpp declares it void (callers there
// ignore it), so read EAX through an ABI-identical pointer type.
U32 list_next(void* list, void** pos) {
    using Fn = U32(__fastcall*)(void*, void*, void*);
    void* const target = reinterpret_cast<void*>(&unresolved_list_next_005e9b90);
    return reinterpret_cast<Fn>(target)(list, nullptr, pos);
}
}

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*); }
extern "C" {
unsigned __fastcall frame_LogIsEmpty_005e58a0(void*, void*);                    // CExoString::IsEmpty
const char* __fastcall WindowCStr005e5670(void*, void*);                        // CExoString::CStr
void* __fastcall async_assign_string_005e5c50(void*, void*, const void*);       // CExoString::operator=
void* __fastcall c2da_005e9d70(void*, void*, void*);                            // CExoLinkedListInternal::AddHead
unsigned __fastcall kotor_fastcall_0062f840(void* gui, void*, void* name);
void __fastcall kotor_fastcall_00632300(void* gui, void*, void* name);
}

// 0062F840 CGuiInGame::IsOnDialogNonDependantList(CExoString name) (thiscall, RET 4): case-insensitive search ("" -> 0).
extern "C" unsigned __fastcall kotor_fastcall_0062f840(void* gui, void*, void* name) {
    auto* s = static_cast<kotor_reconstruction::StartupString*>(name);
    if (ini_comparison::CompareNC005e58c0(s, nullptr, "")) return 0;
    void* pos = lp(lp(gui, 0xB4C), 0);                                                    // GetHeadPos (inlined)
    U32 data = pos != nullptr ? lw(pos, 8) : 0;
    while (pos != nullptr) {
        if (ini_comparison::CompareNC005e58c0(s, nullptr, WindowCStr005e5670(pt(data), nullptr))) return 1;
        data = list_next(lp(gui, 0xB4C), &pos);                                 // 0062F884..0062F88F (list re-read)
    }
    return 0;
}

// 00632300 CGuiInGame::AddToDialogNonDependantList(CExoString name) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00632300(void* gui, void*, void* name) {
    if (ini_comparison::CompareNC005e58c0(static_cast<kotor_reconstruction::StartupString*>(name), nullptr, "")) return;
    if (frame_LogIsEmpty_005e58a0(name, nullptr)) return;
    if (kotor_fastcall_0062f840(gui, nullptr, name)) return;
    void* mem = live_operator_new(8);                                           // 00632345..00632347
    void* str = nullptr;                                                        // 00632353 ESI = 0
    if (mem != nullptr)                                                         // 00632355..0063235B
        str = kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(mem), nullptr); // 0063235D..00632364 ESI = ctor EAX
    // Original quirk: on allocation failure operator= runs on a null CExoString (faults inside 005E5C50).
    (void)async_assign_string_005e5c50(str, nullptr, name);                     // 00632366..00632371
    (void)c2da_005e9d70(lp(gui, 0xB4C), nullptr, str);                          // 00632376..0063237D AddHead
}
