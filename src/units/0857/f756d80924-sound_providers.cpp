// Lane-sound batch 4: provider selection and table loads.
// 005D66D0 LoadProviderPreferences, 005DA6D0 Enumerate3DProviders,
// 005D9140 LoadSoundProvider, 005D7BD0 LoadPriorityGroups (+005D6C10
// CPriorityGroup ctor), CExoStringList 005E94A0/005E9590/005E95F0/005E98B0.
// C2DA bodies come from lane-startup-misc (extern "C" carriers; C linkage, the
// Table is passed as an opaque pointer here).
#include "3a10bcdf21-sound_internal.h"
#include "15d7bcda8b-resources.h"
#include "39c670fc66-allocator_live.h"
#include "b2b642e2f3-base_ini.h" // PublicIni (005E6750/005E67E0)
#include "9252d3b484-warning_runtime.h"   // kotor_reconstruction::StartupFormat005e5680
#include <windows.h>
#include <cstring>
#include <new>

using String = kotor_reconstruction::StartupString;
extern "C" {
void* __fastcall c2da_current_construct_00413cc0(void* table, void*, menu_resources::Ref name, std::int32_t mode); // lane-startup-misc @28
int __fastcall c2da_current_load_004143b0(void* table, void*);                                              // @8
std::int32_t __fastcall c2da_current_get_int_00413660(void*, void*, std::int32_t, String*, std::int32_t*);   // @20
std::int32_t __fastcall c2da_current_get_float_00413430(void*, void*, std::int32_t, String*, float*);        // @20
std::int32_t __fastcall c2da_current_get_string_00413270(void*, void*, std::int32_t, String*, String*);      // @20
void __fastcall c2da_current_unload_004139e0(void*, void*);                                                  // @8
char* __fastcall async_string_cstr_005e5670(String*, void*);
__declspec(dllimport) void __stdcall AIL_set_3D_room_type(void*, std::int32_t);
__declspec(dllimport) void __stdcall AIL_set_digital_master_room_type(void*, std::int32_t);
__declspec(dllimport) std::int32_t __stdcall AIL_set_3D_provider_preference(void*, const char*, const void*);
__declspec(dllimport) std::int32_t __stdcall AIL_enumerate_3D_providers(void**, void**, char**);
}
namespace add_key_lower { int __fastcall Equal005e5200(String*, void*, String*); }          // add-key-equal-transport.obj
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t); } // viewport-list-growth.obj
namespace kotor_sound_data { extern const unsigned char eax_presets_0074c610[24 * 0x70]; }   // D13 generated

namespace {
using U = std::uint32_t;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U rd(void* p, unsigned off) { U v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, void* v) { std::memcpy(at(p, off), &v, 4); }
void* bp(U v) { void* p; std::memcpy(&p, &v, 4); return p; }
void table_delete(void* table) {                                     // C2DA vtable slot 0 (00414A30), push 1
    using Deleting = void*(__fastcall*)(void*, void*, U);
    Deleting del; std::memcpy(&del, rp(table, 0), 4); del(table, nullptr, 1);
}
struct Raw { void* p; ~Raw() { if (p) live_free(p); } };              // new-expression unwind (_free)
struct Temp {                                                        // CExoString temporaries (005E5A90/005E5C20)
    alignas(4) unsigned char b[8];
    explicit Temp(const char* t) { ::new (static_cast<void*>(b)) String(t); }
    ~Temp() { get()->~StartupString(); }
    String* get() { return std::launder(reinterpret_cast<String*>(b)); }
};
// 006FB823 _calloc on the game heap: live_malloc(n*size) zero-filled (deviation:
// the original SBH/HeapAlloc zeroing path and its new-handler loop are not split out).
void* calloc_006fb823(U n, U size) {
    const U total = n * size;
    void* p = live_malloc(total);
    if (p) std::memset(p, 0, total);
    return p;
}
int strcmp_bytes(const char* a, const char* b) {                     // inline compare at 005E9624-005E964A
    for (;; ++a, ++b) {
        const unsigned char x = static_cast<unsigned char>(*a), y = static_cast<unsigned char>(*b);
        if (x != y) return x < y ? -1 : 1;
        if (!x) return 0;
    }
}
}

namespace kotor_sound_internal {
// 005E94A0 CExoStringList::Introduce(str, index), RET 8.
void __fastcall StringListIntroduce005e94a0(void* list, void*, void* str, std::int32_t index) {
    const std::int32_t cap = static_cast<std::int32_t>(rd(list, 0x10)), count = static_cast<std::int32_t>(rd(list, 4));
    std::int32_t newcap = cap;
    if (count >= cap) newcap = cap + 5;                              // 005E94AC-005E94B1
    if (newcap > cap) {                                              // 005E94B4-005E94B6
        void* grown = calloc_006fb823(static_cast<U>(newcap), 4);    // 005E94B8-005E94BB
        for (std::int32_t i = 0; i < static_cast<std::int32_t>(rd(list, 0x10)); ++i)   // 005E94C8-005E94DE
            wp(grown, static_cast<U>(i) * 4, rp(rp(list, 0), static_cast<U>(i) * 4));
        live_free(rp(list, 0));                                      // 005E94E0-005E94E3 (006FB7B2)
        wp(list, 0, grown);                                          // 005E94EB
    }
    for (std::int32_t i = static_cast<std::int32_t>(rd(list, 4)); i > index; --i) // 005E94ED-005E950E
        wp(rp(list, 0), static_cast<U>(i) * 4, rp(rp(list, 0), static_cast<U>(i - 1) * 4));
    wp(rp(list, 0), static_cast<U>(index) * 4, str);                 // 005E9510-005E9516
    wr(list, 0x10, static_cast<U>(newcap));                          // 005E951E
    wr(list, 4, rd(list, 4) + 1);                                    // 005E9519-005E9521
}
// 005E9590 CExoStringList::Clear, RET 0.
void __fastcall StringListClear005e9590(void* list, void*) {
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(rd(list, 0x10)); ++i) { // 005E9594-005E95C8
        String* s = static_cast<String*>(rp(rp(list, 0), static_cast<U>(i) * 4));
        if (!s) continue;                                            // 005E95A5-005E95A7
        s->~StartupString(); live_free(s);                           // 005E95A9-005E95B1
        wp(rp(list, 0), static_cast<U>(i) * 4, nullptr);             // 005E95B6-005E95BB
    }
    wr(list, 4, 0);                                                  // 005E95CB
}
static int compare(void* list, U i, String* key) {
    const char* entry = async_string_cstr_005e5670(static_cast<String*>(rp(rp(list, 0), i * 4)), nullptr);
    const char* k = async_string_cstr_005e5670(key, nullptr);
    return strcmp_bytes(k, entry);                                   // key vs entry (EAX=key text, ESI=entry text)
}
static void duplicate_box(const char* text) { MessageBoxA(nullptr, text, "Error: ExoStringList", 1u); } // IAT 0073D418, uType 1 (MB_OKCANCEL) as pushed
// 005E95F0 CExoStringList::AddSorted(str), RET 4. ESI = last comparison result.
void __fastcall StringListAddSorted005e95f0(void* list, void*, String* str) {
    std::int32_t low = 0, high = 0, last_cmp = 0; U done = 0;        // [esp+18] [esp+14] ESI [esp+10]
    auto equal_case = [&](std::int32_t index) {
        if (rd(list, 8) == 0) StringListIntroduce005e94a0(list, nullptr, str, index); // 005E9668-005E967B / 005E96F3-005E9705
        else if (rd(list, 8) == 2) duplicate_box("Duplicate string detected."); // 005E970C-005E971F
        done = 1;                                                    // 005E9725
    };
    if (static_cast<std::int32_t>(rd(list, 4)) > 0) {                // 005E95FF-005E960E
        last_cmp = compare(list, 0, str);                            // 005E9610-005E964D
        if (last_cmp < 0) { StringListIntroduce005e94a0(list, nullptr, str, 0); done = 1; } // 005E964F-005E9661
        else if (last_cmp == 0) equal_case(0);
        else if (static_cast<std::int32_t>(rd(list, 4)) > 0) {       // 005E9685-005E968C
            const std::int32_t last = static_cast<std::int32_t>(rd(list, 4)) - 1; // 005E9692-005E969B
            high = last;
            last_cmp = compare(list, static_cast<U>(last), str);     // 005E969F-005E96D9
            if (last_cmp > 0) { StringListIntroduce005e94a0(list, nullptr, str, static_cast<std::int32_t>(rd(list, 4))); done = 1; } // 005E96DB-005E96EF
            else if (last_cmp == 0) equal_case(static_cast<std::int32_t>(rd(list, 4)));
        }
    }
    const std::int32_t n = static_cast<std::int32_t>(rd(list, 4));   // 005E972D
    std::int32_t step = n / 2, mid = n / 2;                          // EDI / EBP (CDQ/SUB/SAR)
    if (done) return;                                                // 005E973F
    for (;;) {
        if (high - low <= 1) {                                       // 005E9745-005E9750
            StringListIntroduce005e94a0(list, nullptr, str, last_cmp > 0 ? mid + 1 : mid); // 005E981E-005E9842
            return;
        }
        last_cmp = compare(list, static_cast<U>(mid), str);          // 005E9756-005E9799
        if (last_cmp < 0) { high = mid; step = -1; }                 // 005E979F-005E97A3
        else if (last_cmp > 0) { low = mid; step = 1; }              // 005E97A8-005E97AE
        else equal_case(mid);                                        // 005E97B5-005E97E4
        mid += step * ((high - low) / 2);                            // 005E97EC-005E9802
        if (done) return;                                            // 005E9804-005E9806
    }
}
// 005E98B0 CExoStringList::Add(str), RET 4: [+0C]==0 -> AddSorted, else unsorted append.
void __fastcall StringListAdd005e98b0(void* list, void*, String* str) {
    const std::int32_t count = static_cast<std::int32_t>(rd(list, 4)); // 005E98BA
    if (rd(list, 0x0c) == 0) { StringListAddSorted005e95f0(list, nullptr, str); return; } // 005E98B5-005E98CF
    if (rd(list, 8) == 1) {                                          // 005E98DA-005E98E1
        for (std::int32_t i = 0; i < static_cast<std::int32_t>(rd(list, 4)); ++i) // 005E98E3-005E993D
            if (compare(list, static_cast<U>(i), str) == 0) return;  // 005E992E-005E9942 (EBX=0 -> no insert)
    } else if (rd(list, 8) == 2) {                                   // 005E995C-005E995F
        duplicate_box("Duplicate string found.");                    // 005E9961-005E996F
        return;
    }
    StringListIntroduce005e94a0(list, nullptr, str, count);          // 005E9944-005E9950
}
}

namespace kotor_sound_path_required {
// 005D66D0 CExoSoundInternal::LoadProviderPreferences(environment), RET 4.
void __fastcall provider_preferences_005d66d0(void* sys, void*, std::uint32_t env_word) {
    const unsigned char env = static_cast<unsigned char>(env_word);  // 005D66D1 (BL)
    *at(sys, 0x10) = env;                                            // 005D66E0
    void* provider = rp(sys, 0xf8);
    if (!provider) return;                                           // 005D66E3
    if (env >= 0x18) {                                               // 005D66E9-005D66EF
        if (rd(sys, 4)) AIL_set_3D_room_type(provider, 0);           // 005D66F1-005D66F8
        if (rd(sys, 0)) { void* dig = nullptr; AIL_quick_handles(&dig, nullptr, nullptr); AIL_set_digital_master_room_type(dig, 0); } // 005D66FE-005D671D
        return;
    }
    const unsigned char* preset = kotor_sound_data::eax_presets_0074c610 + static_cast<U>(env) * 0x70u; // 005D6732-005D6735
    if (rd(sys, 4)) {                                                // 005D6728-005D672A
        std::int32_t type; std::memcpy(&type, preset, 4);
        AIL_set_3D_room_type(rp(sys, 0xf8), type);                   // 005D6738-005D6740
        static const struct { const char* name; unsigned off; } prefs[] = {
            {"EAX2 environment size", 0x04}, {"EAX2 environment diffusion", 0x08}, {"EAX2 room", 0x0c},
            {"EAX2 room HF", 0x10}, {"EAX3 room LF ", 0x14}, {"EAX decay time", 0x18}, {"EAX2 room HF", 0x1c},
            {"EAX3 room LF", 0x20}, {"EAX2 reflections", 0x24}, {"EAX2 reflections delay", 0x28},
            {"EAX2 reverb", 0x38}, {"EAX2 reverb delay", 0x3c}, {"EAX3 echo time", 0x4c}, {"EAX3 echo depth", 0x50},
            {"EAX3 modulation time", 0x54}, {"EAX3 modulation depth", 0x58}, {"EAX2 air absorption", 0x5c},
            {"EAX3 HF reference", 0x60}, {"EAX3 LF reference", 0x64}, {"EAX2 room rolloff", 0x68}, {"EAX2 flags", 0x6c}};
        for (const auto& p : prefs)                                  // 005D6746-005D6903 (provider reloaded each call)
            (void)AIL_set_3D_provider_preference(rp(sys, 0xf8), p.name, preset + p.off);
    }
    if (rd(sys, 0)) {                                                // 005D6907-005D690A
        void* dig = nullptr; AIL_quick_handles(&dig, nullptr, nullptr); // 005D690C-005D6915
        std::int32_t type; std::memcpy(&type, preset, 4);
        AIL_set_digital_master_room_type(dig, type);                 // 005D691B-005D692D
    }
}
}

namespace kotor_sound_internal {
// 005D9140 CExoSoundInternal::LoadSoundProvider, RET 0.
void __fastcall LoadSoundProvider005d9140(void* sys, void*) {
    void* raw = live_operator_new(0x54);                             // 005D9164-005D916A
    void* table = nullptr;
    if (raw) {
        Raw guard{raw};                                              // state 0 -> free
        menu_resources::Ref name; name.construct_00406d80("SoundProvider"); // 005D9184-005D9194
        table = c2da_current_construct_00413cc0(raw, nullptr, name, 0); // 005D9199-005D919B
        guard.p = nullptr;
    }
    if (!c2da_current_load_004143b0(table, nullptr)) {               // 005D91A6-005D91B7
        if (table) table_delete(table);                              // 005D91B9-005D91C8
        return;
    }
    alignas(4) unsigned char value_bytes[8];
    ::new (static_cast<void*>(value_bytes)) String();                // 005D91DD-005D91E1 (state 1)
    String* value = std::launder(reinterpret_cast<String*>(value_bytes));
    struct V { String* s; ~V() { s->~StartupString(); } } value_guard{value}; // 005D93CF-005D93DB
    auto one_provider = [&](unsigned char i) {
        unsigned char row = 0;                                       // 005D920B
        if (static_cast<std::int32_t>(rd(table, 0x24)) <= 0) return; // 005D9208-005D9213
        bool found = false;
        do {
            std::int32_t ok;
            { Temp col("Provider"); ok = c2da_current_get_string_00413270(table, nullptr, row, col.get(), value); } // 005D9220-005D9250
            if (ok) {
                String* listed = static_cast<String*>(StringListGet005e95e0(at(sys, 0xd8), nullptr, i)); // 005D9259-005D9269
                if (add_key_lower::Equal005e5200(listed, nullptr, value)) { found = true; break; } // 005D926E-005D9277
            }
            ++row;                                                   // 005D927C
        } while (static_cast<std::int32_t>(row) < static_cast<std::int32_t>(rd(table, 0x24))); // 005D927E-005D9283
        if (!found) return;
        void* entry = live_operator_new(16);                         // 005D928E-005D9294 (no null test, as observed)
        wr(entry, 4, 0x3f800000u);                                   // 005D92AA
        wr(entry, 0, i);                                             // 005D92B0
        { Temp col("EAX"); (void)c2da_current_get_int_00413660(table, nullptr, row, col.get(), reinterpret_cast<std::int32_t*>(at(entry, 0x0c))); } // 005D92A1-005D92DF
        { Temp col("Hardware"); (void)c2da_current_get_int_00413660(table, nullptr, row, col.get(), reinterpret_cast<std::int32_t*>(at(entry, 8))); } // 005D92E4-005D9311
        { Temp col("2D3DBias"); (void)c2da_current_get_float_00413430(table, nullptr, row, col.get(), reinterpret_cast<float*>(at(entry, 4))); } // 005D9316-005D9340
        void* list = at(sys, 0x108);                                 // {data,count,capacity}
        if (rd(list, 4) == rd(list, 8)) {                            // 005D9349-005D935D
            const U cap = rd(list, 8);
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, cap ? cap + cap : 16u); // 005D935F-005D936F
        }
        wp(rp(list, 0), rd(list, 4) * 4, entry);                     // 005D9374-005D937D
        wr(list, 4, rd(list, 4) + 1);                                // 005D9380
        const std::int32_t eax = static_cast<std::int32_t>(rd(entry, 0x0c)), best = static_cast<std::int32_t>(rd(sys, 0x104));
        wr(sys, 0x104, static_cast<U>(best > eax ? best : eax));     // 005D9383-005D9391 (JG)
    };
    if (static_cast<std::int32_t>(rd(at(sys, 0xd8), 4)) > 0) {      // 005D91F4-005D9200
        unsigned char i = 0;
        do { one_provider(i); ++i; }                                 // 005D939F
        while (static_cast<std::int32_t>(i) < static_cast<std::int32_t>(rd(at(sys, 0xd8), 4))); // 005D93AB-005D93B5 (re-read)
    }
    c2da_current_unload_004139e0(table, nullptr);                    // 005D93BB-005D93BD
    if (table) table_delete(table);                                  // 005D93C2-005D93CD
}
}

namespace kotor_sound_path_required {
// 005DA6D0 CExoSoundInternal::Enumerate3DProviders, RET 0.
void __fastcall enumerate_providers_005da6d0(void* sys, void*) {
    void* next = nullptr;                                            // [esp+14h]
    String s1c, s28;                                                 // 005DA705 / 005DA715 (005B3190)
    base_ini_reconstruction::PublicIni ini;                          // [esp+18h]
    ini.construct_005e6750();                                        // 005DA726
    struct IniGuard { base_ini_reconstruction::PublicIni* i; ~IniGuard() { i->destroy_005e67e0(); } } ini_guard{&ini}; // 005DA85F
    wr(sys, 0xe4, 1);                                                // 005DA739 (StringList sorted flag)
    kotor_sound_internal::StringListClear005e9590(at(sys, 0xd8), nullptr); // 005DA743
    void* list = at(sys, 0xec);                                      // EDI
    wr(list, 4, 0);                                                  // 005DA760
    char* names[64]; void* providers[64];                            // [esp+30h] / [esp+130h] (no bound check, as observed)
    U k = 0;
    while (AIL_enumerate_3D_providers(&next, &providers[k], &names[k])) { // 005DA748-005DA76B / 005DA82C-005DA842
        for (std::int32_t i = 0; i != -1; i = -1)                   // 005DA780-005DA7A2 (single pass)
            kotor_reconstruction::StartupFormat005e5680(&s1c, "%s%d", "3D Provider Filter", i);
        if (AIL_open_3D_provider(providers[k]) == 0) {               // 005DA7A4-005DA7AF
            AIL_close_3D_provider(providers[k]);                     // 005DA7B1-005DA7B4
            void* raw = live_operator_new(8);                        // 005DA7BA-005DA7BC
            String* named = nullptr;
            if (raw) { Raw guard{raw}; named = ::new (raw) String(names[k]); guard.p = nullptr; } // 005DA7C8-005DA7DA (state 3)
            kotor_sound_internal::StringListAdd005e98b0(at(sys, 0xd8), nullptr, named); // 005DA7E3-005DA7F6
            void* provider = providers[k];                           // 005DA801
            if (rd(list, 4) == rd(list, 8)) {                        // 005DA7FB-005DA803
                const U cap = rd(list, 8);
                menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, cap ? cap + cap : 16u); // 005DA805-005DA815
            }
            wp(rp(list, 0), rd(list, 4) * 4, provider);              // 005DA81A-005DA81F
            wr(list, 4, rd(list, 4) + 1);                            // 005DA822
        }
        ++k;                                                         // 005DA825
    }
    kotor_sound_internal::LoadSoundProvider005d9140(sys, nullptr);   // 005DA84C-005DA84E
}
}

namespace kotor_sound_internal {
// 005D6C10 CPriorityGroup::CPriorityGroup, RET 0.
void* __fastcall PriorityGroupConstruct005d6c10(void* g, void*) {
    *at(g, 6) = 0; *at(g, 7) = 0x7f; *at(g, 4) = 0xff;              // 005D6C14-005D6C1B
    wr(g, 0, 1); wr(g, 0x0c, 0x41200000u); wr(g, 8, 0x41a00000u);   // 005D6C1F-005D6C2C (10.0f, 20.0f)
    *reinterpret_cast<std::uint16_t*>(at(g, 0x14)) = 0;             // 005D6C33
    *at(g, 5) = 0; wr(g, 0x10, 0);                                   // 005D6C37-005D6C3A
    return g;
}
}

namespace kotor_sound_path_required {
// 005D7BD0 CExoSoundInternal::LoadPriorityGroups, RET 0.
void __fastcall load_priority_groups_005d7bd0(void* sys, void*) {
    void* raw = live_operator_new(0x54);                             // 005D7BF4-005D7BFA
    void* table = nullptr;
    if (raw) {
        Raw guard{raw};
        menu_resources::Ref name; name.construct_00406d80("PriorityGroups"); // 005D7C14-005D7C24
        table = c2da_current_construct_00413cc0(raw, nullptr, name, 0); // 005D7C29-005D7C2B
        guard.p = nullptr;
    }
    if (!c2da_current_load_004143b0(table, nullptr)) {               // 005D7C36-005D7C47
        if (table) table_delete(table);
        return;
    }
    const unsigned char rows = static_cast<unsigned char>(rd(table, 0x24)); // 005D7C6C-005D7C6F
    *at(sys, 0x2a) = rows;                                           // 005D7C7D
    void* block = live_operator_new(static_cast<U>(rows) * 24u + 4u); // 005D7C72-005D7C80
    void* groups = nullptr;
    if (block) {                                                     // 005D7C8C-005D7C96 (state 1)
        Raw guard{block};
        wr(block, 0, rows);                                          // 005D7CA9
        for (U i = 0; i < rows; ++i)                                 // 005D7CAB eh_vector_constructor_iterator(+4, 24, n, 005D6C10, 004015A0)
            (void)kotor_sound_internal::PriorityGroupConstruct005d6c10(at(block, 4 + i * 24), nullptr);
        guard.p = nullptr;
        groups = at(block, 4);
    }
    wp(sys, 0x4c, groups);                                           // 005D7CB6
    std::int32_t value = 0;                                          // [esp+14h] (shared across reads)
    float max_dist = 0.0f, min_dist = 0.0f, variance = 0.0f;         // [esp+24h] / [esp+20h] / [esp+1Ch]
    for (unsigned char i = 0; i < *at(sys, 0x2a); ++i) {             // 005D7CC6-005D7FB4
        auto group = [&]() { return at(rp(sys, 0x4c), static_cast<U>(i) * 24u); };
        { Temp c("Priority"); if (c2da_current_get_int_00413660(table, nullptr, i, c.get(), &value)) *(group() + 6) = static_cast<unsigned char>(value); } // 005D7CE0-005D7D34
        { Temp c("Volume"); if (c2da_current_get_int_00413660(table, nullptr, i, c.get(), &value)) *(group() + 7) = static_cast<unsigned char>(value); }   // 005D7D37-005D7D86
        { Temp c("MaxPlaying"); if (c2da_current_get_int_00413660(table, nullptr, i, c.get(), &value)) *(group() + 4) = static_cast<unsigned char>(value); } // 005D7D89-005D7DD8
        { Temp c("Interrupt"); if (c2da_current_get_int_00413660(table, nullptr, i, c.get(), &value)) wr(group(), 0, static_cast<U>(value)); }        // 005D7DDB-005D7E2A
        { Temp c("FadeTime"); if (c2da_current_get_int_00413660(table, nullptr, i, c.get(), &value)) *reinterpret_cast<std::uint16_t*>(group() + 0x14) = static_cast<std::uint16_t>(value); } // 005D7E2C-005D7E7C
        bool both = false;
        {
            Temp c("MaxVolumeDist");                                 // 005D7E80-005D7EA6 (state 7)
            if (c2da_current_get_float_00413430(table, nullptr, i, c.get(), &max_dist)) {
                Temp c2("MinVolumeDist");                            // 005D7EAF-005D7ED2 (state 8)
                both = c2da_current_get_float_00413430(table, nullptr, i, c2.get(), &min_dist) != 0; // 005D7EE7-005D7EED
            }
        }                                                            // 005D7EF1-005D7EFD
        if (both) { std::memcpy(group() + 8, &min_dist, 4); std::memcpy(group() + 0x0c, &max_dist, 4); } // 005D7F0A-005D7F1E
        {
            Temp c("PlaybackVariance");                              // 005D7F21-005D7F5A
            if (c2da_current_get_float_00413430(table, nullptr, i, c.get(), &variance)) {
                float stored;
                if (0.0f > variance) stored = 0.0f;                  // 005D7F63-005D7F7B / 005D7F96
                else if (1.0f >= variance || variance != variance) stored = variance; // 005D7F7D-005D7F8C / 005D7F9E
                else stored = 1.0f;                                  // 005D7F8E
                std::memcpy(group() + 0x10, &stored, 4);             // 005D7FA2
            }
        }
    }
    table_delete(table);                                             // 005D7FBA-005D7FC0 (no Unload, no null test)
}
}
