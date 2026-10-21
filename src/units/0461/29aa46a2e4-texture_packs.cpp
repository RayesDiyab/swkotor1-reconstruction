// lane-resman: texture-pack key-source registration (swpc_tex_tpa/gui.erf).
// CClientExoAppInternal::SetTexturePack 005F14A0 (M40 service-options-18, reached
// from start_services 005F8550 call site 005F8E3F) calls set_pack_0070d800(1, tpa)
// and (2, gui). This TU authors the CAuroraInterface pack registry bodies and the
// one-line CExoResMan wrapper they need. Static evidence only; never executed.
#include "0d6cdf91cb-resources.h"   // menu_resources::{Manager,String}; Manager::encapsulated_004087a0
#include <cstdint>
#include <cstdio>
#include <cstring>

using Word = menu_resources::Word;
using String = menu_resources::String;

extern "C" {
// M40-defined:
menu_resources::Manager* __cdecl c2da_manager_007a39e8();
const char* __cdecl c2da_empty_0073d71c();
Word __fastcall async_remove_key_00407830(menu_resources::Manager*, void*, String*, Word);
int __fastcall async_string_equal_005e5200(String*, void*, String*);
String* __fastcall WindowCopy005e5b00(String*, void*, const String* volatile);
String* __fastcall GuiAssign005e5140(String*, void*, const char*);
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);
// MISSING in M40: four CExoString slots at 00834098 (stride 8), storage owner =
// 2026-10-02\task-34\cpp-roots-80-82 root 82 TexturePacks (0073B6F0/0073C190), unreviewed.
extern unsigned char Receiver_00834098[32];

// Lane exports.
Word __fastcall resman_remove_encapsulated_00408820(menu_resources::Manager*, void*, String*);
String* __fastcall resman_string_from_int_005e5bc0(String*, void*, int);
String* __cdecl aurora_current_texture_pack_0070d060(String* out, std::uint8_t index);
Word __cdecl aurora_unload_texture_pack_0070cf80(std::uint8_t index);
Word __cdecl aurora_load_texture_pack_0070cf30(std::uint8_t index, String* name);
Word __cdecl aurora_set_texture_pack_0070d800(std::uint8_t index, String* name);
}

namespace {
String* pack_slot(std::uint8_t index) {
    // lea reg,[index*8+00834098h] (0070CF69 / 0070CFD7 / 0070D087)
    return reinterpret_cast<String*>(Receiver_00834098 + static_cast<unsigned>(index) * 8u);
}
}

// 00408820 CExoResMan::RemoveEncapsulatedResourceFile(CExoString const&) RET4:
// push 3; push name; call 00407830 RemoveKeyTable; EAX forwarded.
extern "C" __declspec(noinline) Word __fastcall resman_remove_encapsulated_00408820(
    menu_resources::Manager* manager, void*, String* name) {
    return async_remove_key_00407830(manager, nullptr, name, 3u);
}

// 005E5BC0 CExoString::CExoString(int) RET4: sprintf(local[0x24],"%i") (literal 0073E2EC),
// capacity=strlen+1 -> +4, new(capacity) -> +0, byte copy including NUL; returns receiver.
extern "C" __declspec(noinline) String* __fastcall resman_string_from_int_005e5bc0(String* self, void*, int value) {
    char text[0x24];
    std::sprintf(text, "%i", value);                                      // 005E5BC8-005E5BD5
    const Word capacity = static_cast<Word>(std::strlen(text)) + 1u;     // 005E5BE1-005E5BED
    std::memcpy(reinterpret_cast<unsigned char*>(self) + 4, &capacity, 4); // 005E5BEF
    char* data = static_cast<char*>(c2da_directory_new_006fa7e6(capacity)); // 005E5BF2
    std::memcpy(reinterpret_cast<unsigned char*>(self), &data, 4);      // 005E5BFA
    std::memcpy(data, text, capacity);                                    // 005E5C02-005E5C0A
    return self;
}

// 0070D060 CAuroraInterface::GetCurrentTexturePack(u8) cdecl, hidden CExoString return.
extern "C" __declspec(noinline) String* __cdecl aurora_current_texture_pack_0070d060(String* out, std::uint8_t index) {
    if (index >= 4u) {                                                    // 0070D065/0070D074
        (void)resman_string_from_int_005e5bc0(out, nullptr, 0);           // 0070D076-0070D07A
        return out;
    }
    (void)WindowCopy005e5b00(out, nullptr, pack_slot(index));             // 0070D084-0070D091
    return out;
}

// 0070CF80 CAuroraInterface::UnloadTexturePack(u8) cdecl.
extern "C" __declspec(noinline) Word __cdecl aurora_unload_texture_pack_0070cf80(std::uint8_t index) {
    String current;                                                       // 0070CF98-0070CF9C
    if (index >= 4u) return 0;                                            // 0070CFA5-0070CFC2
    String* const slot = pack_slot(index);
    current.Assign005e5c50(slot);                                         // 0070CFDE-0070CFDF
    if (current.BorrowedLength005e5790() != 0) {                          // 0070CFE4-0070CFEF
        if (!resman_remove_encapsulated_00408820(c2da_manager_007a39e8(), nullptr, &current)) // 0070CFF1-0070D003
            return 0;
        (void)GuiAssign005e5140(slot, nullptr, c2da_empty_0073d71c());     // 0070D028-0070D02F
    }
    return 1;                                                             // 0070D034-0070D049
}

// 0070CF30 CAuroraInterface::LoadTexturePack(u8, CExoString const&) cdecl.
// Texture packs register as encapsulated key tables with priority 2
// (0070CF59 push 2 -> 004087A0 -> AddKeyTable(name, 3, 2)); GetKeyEntry 00407230
// searches them 4th: dirs(+14) -> ERF prio1(+18) -> images(+1C) -> ERF prio2(+18) -> KEY/BIF(+10).
extern "C" __declspec(noinline) Word __cdecl aurora_load_texture_pack_0070cf30(std::uint8_t index, String* name) {
    if (index >= 4u) return 0;                                            // 0070CF35-0070CF3A
    if (name->BorrowedLength005e5790() == 0) return 0;                    // 0070CF43-0070CF51
    if (!c2da_manager_007a39e8()->encapsulated_004087a0(name, 2u)) return 0; // 0070CF53-0070CF63
    (void)pack_slot(index)->Assign005e5c50(name);                         // 0070CF65-0070CF70
    return 1;
}

// 0070D800 CAuroraInterface::SetTexturePack(u8, CExoString const&) cdecl.
// Consumer: options_video::TextureBindings::set_pack_0070d800 (U32 (__cdecl*)(U8,String*)).
extern "C" __declspec(noinline) Word __cdecl aurora_set_texture_pack_0070d800(std::uint8_t index, String* name) {
    if (index >= 4u) return 0;                                            // 0070D81D-0070D822
    bool same;
    {
        // Hidden-return temp; EH state 0 (0070D84B) destroys it on unwind too.
        struct Temporary {
            alignas(String) unsigned char storage[sizeof(String)];
            String* live = nullptr;
            ~Temporary() { if (live) live->~StartupString(); }
        } temporary;
        temporary.live = aurora_current_texture_pack_0070d060(reinterpret_cast<String*>(temporary.storage), index); // 0070D836-0070D83C
        same = async_string_equal_005e5200(name, nullptr, temporary.live) != 0; // 0070D844-0070D853
    }                                                                     // 0070D858-0070D866 ~CExoString
    if (same) return 1;                                                   // 0070D86B-0070D871
    if (!aurora_unload_texture_pack_0070cf80(index)) return 0;            // 0070D886-0070D891
    return aurora_load_texture_pack_0070cf30(index, name);                // 0070D8A5-0070D8A7
}
