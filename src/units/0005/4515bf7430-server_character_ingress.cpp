#include "f30bc5c282-server_character_ingress.hpp"
#include "862e60d023-internal.h"
#include "5cc7361c75-menu_targets.h"
#include <cstring>
using kotor_reconstruction::StartupString;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace menu_resources { Manager* __cdecl BorrowCanonicalManager007a39e8(); }

namespace file_internal {
extern "C" unsigned __fastcall kotor_fastcall_005ebc60(Internal*, void*, const void*, unsigned, unsigned);
}
extern "C" {
void* __cdecl live_operator_new(unsigned);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*);
void* __fastcall server_get_module_004ae6b0(void*, void*);
void* __fastcall kotor_fastcall_00561080(void*, void*);
unsigned __fastcall kotor_fastcall_004d6230(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004d6240(void*, void*, unsigned);
unsigned __fastcall kotor_fastcall_004d66f0(void*, void*);
unsigned __fastcall kotor_fastcall_004d6780(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004d6200(void*, void*, unsigned);
void* __fastcall kotor_fastcall_004d6180(void*, void*, void*, unsigned);
void __fastcall c2da_ref_to_string_00405f70(const void*, void*, StartupString*);
void __fastcall kotor_fastcall_004088d0(void*, void*, void*);
unsigned __fastcall gui_load_resman_clean_directory_00409460(void*, void*, void*, unsigned, unsigned);
unsigned __fastcall recovered_player_load_bic_00561d70(void*, void*); // module lane
unsigned __fastcall recovered_load_character_00561e30(void*, void*, unsigned, unsigned, unsigned); // W2 IFO
unsigned __fastcall recovered_load_character_finish_004b5c50(void*, void*, void*, unsigned); // W2
void __fastcall recovered_create_party_00565760(void*, void*); // W2
int __fastcall kotor_fastcall_004c5c70(void* module, void*, void* player);            // claude_save_module GetPlayerIndexInPlayerList RET 4
unsigned __fastcall kotor_fastcall_004b3070(void* internal, void*, void* player);      // claude_stale_bodies_char SendCharacterQuery RET 4
unsigned __fastcall kotor_fastcall_00562380(void* player, void*, IngressRef16 name);   // claude_stale_bodies_char LoadServerCharacter RET 10h
void* __fastcall kotor_fastcall_005610b0(void* player, void*, void* out);              // claude_save_module GetPlayerName RET 4, EAX out
unsigned __fastcall kotor_fastcall_0056c7f0(void* message, void*, unsigned player);    // claude_0886_bodies_server RET 4
void __fastcall kotor_fastcall_00405920(void* array, void*);                           // 0603 CExoArrayList<CExoString> dtor
void* __fastcall async_assign_string_005e5c50(void*, void*, const void*);             // CExoString::operator=
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);                            // ~CExoString
void* __fastcall WindowCopy005e5b00(void*, void*, const void*);                        // 0078 CExoString(const CExoString&)
const char* __fastcall WindowCStr005e5670(void*, void*);                               // CExoString::CStr
}
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // 0125 CExoString::Format
namespace f200_ports { void __fastcall Enumerate005e6640(void* base, void*, void* list, kotor_reconstruction::StartupString* dir,
                                                         unsigned type, unsigned directories, unsigned order); } // 0288 GetDirectoryList
namespace options_video { struct String { char* text; unsigned length; }; }           // 8-byte CExoString by-value view
namespace client_intro { int __fastcall CountStrings_004b2640(void* list, void*, options_video::String value); } // 0371 RET 8, destroys value
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } } // 0068 [007A39E0] CExoBase
namespace {
void* ptr(const void* p, unsigned off) {
    void* result; std::memcpy(&result, static_cast<const unsigned char*>(p) + off, 4); return result;
}
unsigned word(const void* p, unsigned off) {
    unsigned result; std::memcpy(&result, static_cast<const unsigned char*>(p) + off, 4); return result;
}
void put_word(void* p, unsigned off, unsigned value) {
    std::memcpy(static_cast<unsigned char*>(p) + off, &value, 4);
}
void* server() { return ptr(kotor_menu_window_cells::manager_007a39fc, 8); }
void* party(void* internal) { return static_cast<unsigned char*>(internal) + 0x1b770; }
// These alternate saved/vault dependencies have no recovered provider in the
// inspected map/donor census. Named stops, never safe defaults.
// CExoArrayList<CExoString> local [esp+44h..4Fh]: zeroed at entry, filled only by the server-vault branch,
// destroyed (00405920 / its inline copy at 004B76AE) on every exit between vault_path and player_name.
struct NameArray {
    void* data = nullptr; unsigned count = 0; unsigned allocated = 0;
    ~NameArray() { kotor_fastcall_00405920(this, nullptr); }
};
}

// 005E69A0: mov ecx,[ecx]; jmp005EBC60. No duplicate internal writer.
extern "C" unsigned __fastcall recovered_file_write_005e69a0(
    void* file, void*, const void* bytes, unsigned size, unsigned count) {
    return file_internal::kotor_fastcall_005ebc60(static_cast<tlk_file::File*>(file)->internal, nullptr, bytes, size, count);
}

// 004AE700..004AE73C: real public->internal thunk, original RET20h.
extern "C" unsigned __fastcall recovered_server_character_004ae700(
    void* outer, void*, unsigned minor, void* player, IngressRef16 reference,
    const void* bytes, unsigned length) {
    return recovered_server_character_internal_004b7470(ptr(outer, 4), nullptr, minor, player, reference, bytes, length);
}

// 004B7470..004B78EA, RET20h. No GUI/device initialization or fabricated PC.
extern "C" unsigned __fastcall recovered_server_character_internal_004b7470(
    void* internal, void*, unsigned minor, void* player, IngressRef16 reference,
    const void* bytes, unsigned length) {
    void* module = server_get_module_004ae6b0(server(), nullptr); // cached before strings, original004B74A5
    
    StartupString player_name;                                     // [esp+24h]
    NameArray names;                                               // [esp+44h]
    StartupString vault_path;                                      // [esp+1Ch]
    const unsigned mode = minor & 0xffu;
    if (mode == 2u) {
        if (kotor_fastcall_00561080(player, nullptr) == nullptr)
            (void)kotor_fastcall_00562380(player, nullptr, reference);    // 004B74F3..11 LoadServerCharacter(ref), result ignored
    } else if (mode == 0x0eu) {
        if (kotor_fastcall_00561080(player, nullptr) == nullptr) {
            StartupString index_text;
            c2da_ref_to_string_00405f70(&reference, nullptr, &index_text);
            const int index = reinterpret_cast<kotor_menu20::String*>(&index_text)->as_int_005e5630();
            (void)recovered_load_character_00561e30(player, nullptr, static_cast<unsigned>(index), 0, 1);
        }
    } else if (mode == 0x11u) {
        if (kotor_fastcall_00561080(player, nullptr) == nullptr) {
            {
                alignas(4) unsigned char name[8];                  // [esp+3Ch] constructed by GetPlayerName
                void* const returned = kotor_fastcall_005610b0(player, nullptr, name);    // 004B7584..8B
                try { (void)async_assign_string_005e5c50(&player_name, nullptr, returned); } // 004B7590..9B
                catch (...) { frame_LogStringDtor_005e5c20(name, nullptr); throw; }
                frame_LogStringDtor_005e5c20(name, nullptr);       // 004B75A0..A9
            }
            {
                StartupString directory("SERVERVAULT:");           // 004B75AE..B7 [esp+2Ch]
                f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &names,
                                              &directory, 0xffff, 1, 0);   // 004B75BC..DA
            }                                                      // 004B75DF..E8
            options_video::String copy;                            // by-value argument built in place
            (void)WindowCopy005e5b00(&copy, nullptr, &player_name);   // 004B75ED..FB
            if (client_intro::CountStrings_004b2640(&names, nullptr, copy) != 0) {     // 004B7600..0B
                kotor_reconstruction::StartupFormat005e5680(&vault_path, "SERVERVAULT:%s",
                    WindowCStr005e5670(&player_name, nullptr));    // 004B7611..2A
                (void)menu_resources::BorrowCanonicalManager007a39e8()->directory_00408800(&vault_path); // 004B762D..38
                (void)kotor_fastcall_00562380(player, nullptr, reference);  // 004B763D..5B result ignored
                { const unsigned char vault_state = 4; std::memcpy(static_cast<unsigned char*>(player) + 0x64, &vault_state, 1); } // 004B7664 byte [esi+64h] = BL (4)
                kotor_fastcall_004088d0(menu_resources::BorrowCanonicalManager007a39e8(), nullptr, &vault_path); // 004B7660..6E
            }
        }
    } else if (mode == 0x0fu) {
        (void)recovered_load_character_00561e30(player, nullptr, 0xffffffffu, 1, 1);
        recovered_create_party_00565760(party(internal), nullptr);
        (void)recovered_load_character_finish_004b5c50(internal, nullptr, player, 0);
        return 1;
    } else if (kotor_fastcall_00561080(player, nullptr) == nullptr) {
        void* allocation = live_operator_new(4u);
        tlk_file::File* file = nullptr;
        if (allocation != nullptr) {
            try {
                StartupString file_mode("wb");
                StartupString filename("TEMP:temp_char");
                file = file_internal::exofile_construct_005e68d0(static_cast<tlk_file::File*>(allocation), nullptr,
                    &filename, 0x7dfu, &file_mode);
            } catch (...) { live_free(allocation); throw; }
        }
        // Unlike00678080, the original receiver handles null allocation by0.
        if (file == nullptr) return 0;
        if (tlk_file::file_opened_005e6a10(file, nullptr) == 0) {
            tlk_file::file_destroy_005e69e0(file, nullptr); live_free(file); return 0;
        }
        (void)recovered_file_write_005e69a0(file, nullptr, bytes, 1, length); // original ignores short write
        
        tlk_file::file_destroy_005e69e0(file, nullptr);
        live_free(file);
        StartupString temporary("TEMP:");
        auto* resources = menu_resources::BorrowCanonicalManager007a39e8();
        (void)resources->directory_00408800(&temporary); // result intentionally ignored
        if (mode == 1u || mode == 0x13u) {
            (void)recovered_player_load_bic_00561d70(player, nullptr); // original ignores status
            
        }
        // The original reloads the global resource manager for each operation.
        kotor_fastcall_004088d0(menu_resources::BorrowCanonicalManager007a39e8(), nullptr, &temporary);
        (void)gui_load_resman_clean_directory_00409460(menu_resources::BorrowCanonicalManager007a39e8(), nullptr,
            &temporary, 0, 0);
        recovered_create_party_00565760(party(internal), nullptr);
        
    }
    if (word(module, 0x1c8) == 0 || mode == 0x0eu) {
        (void)recovered_load_character_finish_004b5c50(internal, nullptr, player, 0);
        
    } else {
        const unsigned index = static_cast<unsigned>(kotor_fastcall_004c5c70(module, nullptr, player)); // 004B78D0..D1
        if (index == 0xffffffffu)
            (void)recovered_load_character_finish_004b5c50(internal, nullptr, player, 0);
        else {
            put_word(player, 0x44, index);
            (void)kotor_fastcall_004b3070(internal, nullptr, player);   // 004B78E1..E5 SendCharacterQuery
        }
    }
    return 1;
}

// 005244E0..005246EC: branches on low-byte minor, forwards original full word.
extern "C" unsigned __fastcall recovered_server_login_005244e0(
    void* message, void*, void* player, unsigned minor) {
    (void)server_get_module_004ae6b0(server(), nullptr);
    if (kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;
    const unsigned mode = minor & 0xffu;
    if (mode == 2u || mode == 0x0eu || mode == 0x11u) {
        IngressRef16 reference;
        kotor_fastcall_004d6180(message, nullptr, &reference, 16);
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0 || kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;
        (void)recovered_server_character_004ae700(server(), nullptr, minor, player, reference, nullptr, 0);
        return 1;
    }
    if (mode == 0x0fu) {
        // Original order here is underrun then overrun (opposite other branches).
        if (kotor_fastcall_004d6240(message, nullptr, 1) != 0 || kotor_fastcall_004d6230(message, nullptr, 1) != 0) return 0;
        if (word(player, 0x80) != 0xffffffffu) {
            IngressRef16 empty; empty.construct_00406d80("");
            (void)recovered_server_character_004ae700(server(), nullptr, 0x0fu, player, empty, nullptr, 0);
        } else {
            (void)kotor_fastcall_0056c7f0(message, nullptr, word(player, 4));   // 0052458F..95 ECX = message
        }
        return 1;
    }
    if (mode == 1u || mode == 0x13u) {
        const unsigned count = kotor_fastcall_004d6780(message, nullptr, 32);
        const void* bytes = kotor_fastcall_004d6200(message, nullptr, count);
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0 || kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;
        
        IngressRef16 empty; empty.construct_00406d80("");
        (void)recovered_server_character_004ae700(server(), nullptr, minor, player, empty, bytes, count);
        return 1;
    }
    if (mode == 0x0bu) {
        const unsigned flag = kotor_fastcall_004d66f0(message, nullptr);
        if (kotor_fastcall_004d6230(message, nullptr, 1) != 0 || kotor_fastcall_004d6240(message, nullptr, 1) != 0) return 0;
        // Existing004AE740 thunk is only ECX=[outer+4];JMP004B5C50.
        // Reuse W2 target directly, no new004AE740 body/export/address claim.
        (void)recovered_load_character_finish_004b5c50(ptr(server(), 4), nullptr, player, flag);
        return 1;
    }
    return 0;
}


