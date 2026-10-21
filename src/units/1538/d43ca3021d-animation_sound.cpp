// Bounded original animation sound closure. Original GOG x86 swkotor.exe is read-only.
// Canonical string, resource, heap, model and audio providers retain their owners.
#include "b1e009d1c4-startup_string.h"
#include "aca6a4ceb8-resources.h"
#include "b0ef26a01c-m15_l0.h"
#include "fc01b5c3bb-animation_sound.h"
#include <cstdint>
#include <cstring>
#include <new>

using U = std::uint32_t;
using I = std::int32_t;
using String = kotor_reconstruction::StartupString;
namespace c2da_current { struct Table; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void*, void*);
void __fastcall gui_priority_005d5ba0(void*, void*, U);
U __fastcall gui_play_005d5b10(void*, void*, menu_resources::Ref*, U, U, U, U);
}
String* __fastcall GuiConcat005e5d10(String*, void*, String*, const String*);
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern void* kotor_cell_tlk_007a3a08;
extern void* kotor_cell_sound_007a39ec;
extern U kotor_rules_cell32_007a2124, kotor_rules_cell32_007a2128;
extern U kotor_rules_cell32_007a2140, kotor_rules_cell32_007a2144;
extern U kotor_rules_cell32_007a216c, kotor_rules_cell32_007a21a4;
extern U kotor_rules_cell32_007a224c, kotor_rules_cell32_007a2250;
extern U kotor_rules_cell32_007a2254, kotor_rules_cell32_007a2260;
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);
U __fastcall c2da_current_resource_demand_00409b10(void*, void*);
U __fastcall c2da_release_demand_00409b80(void*, void*);
U __fastcall gui_scene_get_property_0x10_004064a0(void*, void*);
U __fastcall tlk_fetch_0041e550(void*, void*, U, void*, U);
I __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table*, void*, I, I, I*);
I __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table*, void*, I, I, String*);
I __fastcall c2da_current_get_string_00413270(c2da_current::Table*, void*, I, String*, String*);
I __fastcall kotor_m15b_c2da_get_float_entry_00413350(void*, void*, I, I, float*);
String* __fastcall resman_string_from_int_005e5bc0(String*, void*, I);
char* __fastcall WindowCStr005e5670(String*, void*);
String* __fastcall GuiAssign005e5140(String*, void*, const char*);
U __fastcall frame_LogIsEmpty_005e58a0(void*, void*);
void* __fastcall client_get_player_creature_005ed540(void*, void*);
void* __fastcall recovered_client_get_player_by_id_005ed590(void*, void*, U);
void* __fastcall kotor_fastcall_005ed580(void*, void*, U);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*, void*);
U __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
U __fastcall recovered_effect_integer_00503690(void*, void*, U);
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void*, void*, U);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*, void*, I);
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void*, void*);
void* __fastcall gob_get_property_0x68_0063c070(void*, void*);
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void*, void*);
void __fastcall kotor_fastcall_005d6200(void*, void*);
int __cdecl crt_rand_006fc447();
void* __cdecl aurora_new_aur_object_00449cc0(const char*, const char*, void*, void*);
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void*, void*);
void __fastcall recovered_client_ai_master_add_object_00604a20(void*, void*, void*);
}

namespace {
// /EHsc otherwise assumes direct extern-C calls cannot throw. Original callers
// retain their active string states around these calls. A volatile C++ function
// pointer preserves that EH edge without changing the provider or machine ABI.
template<class F, class... A> auto opaque(F fn, A... args) {
    F volatile target = fn;
    return target(args...);
}
template<class T> T read(const void* p, U off) {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v;
}
template<class T> void write(void* p, U off, T v) {
    std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v));
}
void* ptr(const void* p, U off) { return read<void*>(p, off); }
void* manager() { return kotor_menu_window_cells::manager_007a39fc; }
c2da_current::Table* table(U off) { return static_cast<c2da_current::Table*>(ptr(ptr(kotor_cell_rules_007a3a28, 0xB8), off)); }
template<class F> F slot(void* p, U off) { return read<F>(ptr(p, 0), off); }
using Getter = void* (__fastcall*)(void*, void*);
using Predicate = U (__fastcall*)(void*, void*);
using Delete = U (__fastcall*)(void*, void*, U);
// Constructor-style return temporaries use raw storage; activate only after a
// successful original constructor/concat. Destruction order is explicit at the
// observed instructions; exceptional cleanup follows the original unwind map.
struct TemporaryString {
    alignas(String) unsigned char storage[sizeof(String)];
    bool active = false;
    String* get() { return reinterpret_cast<String*>(storage); }
    ~TemporaryString() { destroy(); }
    void destroy() { if (active) { active = false; get()->~StartupString(); } }
};
// STR_RES is 28 bytes: CExoString + CResRef + duration. TLK writes all three.
struct StringResource { String text; menu_resources::Ref sound; U duration; };
static_assert(sizeof(StringResource) == 28, "TLK output frame");
static_assert(sizeof(menu_resources::Ref) == 16 && sizeof(String) == 8, "original carriers");
void* server_creature(void* self) {
    if (!opaque(kotor_m15a_object_get_server_object_0063d4b0, self, nullptr)) return nullptr;
    void* s = opaque(kotor_m15a_object_get_server_object_0063d4b0, self, nullptr);
    return slot<Getter>(s, 0x30)(s, nullptr);
}
void dispose_stream(void* p) {
    if (p) { opaque(kotor_fastcall_005d6200, p, nullptr); live_free(p); }
}
I random_mod2() { return opaque(crt_rand_006fc447) % 2; } // original signed remainder sequence
}

extern "C" {
// 0060D430..0060D455. Reads no receiver/register input; four stack words, RET10h.
U __stdcall kotor_animation_sound_select_0060d430(U kind, U a, U b, U c) {
    if (kind == 2) return a;
    if (kind == 3) return b;
    if (kind == 4) return c;
    return 0;
}

// 00678820..00678870, CSoundSet lookup. RET8, low WORD index, ignored TLK result.
U __fastcall kotor_animation_sound_soundset_lookup_00678820(void* self, void*, U index, void* out) {
    void* data = reinterpret_cast<void*>(opaque(gui_scene_get_property_0x10_004064a0, ptr(self, 8), nullptr));
    const U n = static_cast<std::uint16_t>(index);
    if (n == 0 || n >= 0x1D) return 0;
    const U strref = read<U>(data, read<U>(data, 8) + n * 4 - 4);
    if (strref == 0xFFFFFFFFu) return 0;
    (void)opaque(tlk_fetch_0041e550, kotor_cell_tlk_007a3a08, nullptr, strref, out, 0); // byte0073ECD0 ==0
    return 1;
}

// 0060B8A0..0060B8F5. Demand/release balanced only on the original normal path.
U __fastcall kotor_animation_sound_get_soundset_0060b8a0(void* self, void*, U index, void* out) {
    if (read<std::uint16_t>(self, 0x2FC) == 0xFFFF) return 0;
    void* soundset = ptr(self, 0x36C);
    if (!soundset || !opaque(c2da_current_resource_demand_00409b10, ptr(soundset, 8), nullptr)) return 0;
    (void)kotor_animation_sound_soundset_lookup_00678820(ptr(self, 0x36C), nullptr, index, out);
    (void)opaque(c2da_release_demand_00409b80, ptr(ptr(self, 0x36C), 8), nullptr);
    return 1; // not the lookup result
}

// 00616890..006168FB. Sorted effect list, signed early-exit comparison to107.
U __fastcall kotor_animation_sound_damage_effect_00616890(void* self, void*) {
    void* server = server_creature(self);
    if (!server) return 0;
    const I count = read<I>(server, 0x128);
    if (count <= 0) return 0;
    void* const list = ptr(server, 0x124);
    for (I n = 0; n < count; ++n) {
        void* effect = read<void*>(list, static_cast<U>(n) * 4);
        const I kind = static_cast<I>(read<std::uint16_t>(effect, 8));
        if (kind == 107) return opaque(recovered_effect_integer_00503690, read<void*>(list, static_cast<U>(n) * 4), nullptr, 0);
        if (kind > 107) return 0;
    }
    return 0;
}

// 0063C4F0..0063C54F. Optional model animation mapping then parry table predicate.
U __fastcall kotor_animation_sound_animation_parry_0063c4f0(void* self, void*, U animation) {
    void* anim = ptr(self, 0x68);
    U row = animation;
    if (anim) {
        using Map = U (__fastcall*)(void*, void*, U);
        row = slot<Map>(anim, 0xE0)(anim, nullptr, animation);
    }
    I result = 0;
    if (!opaque(c2da_current_get_int_entry_00413510, table(0x3C), nullptr, static_cast<I>(row & 0xFFFFu),
                                         static_cast<I>(kotor_rules_cell32_007a216c), &result)) return 0;
    return result != 0;
}

// 00646880..00646927. Item auxiliary model replacement; no sound suppression.
void __fastcall kotor_animation_sound_item_refresh_00646880(void* self, void*, String* model, String* node) {
    void* old = ptr(self, 0x194);
    if (old) {
        (void)slot<Delete>(old, 0)(old, nullptr, 1);
        write<void*>(self, 0x194, nullptr);
    }
    const char* model_text = opaque(WindowCStr005e5670, model, nullptr);
    void* created = opaque(aurora_new_aur_object_00449cc0, model_text, "specialattack", nullptr, nullptr);
    write<void*>(self, 0x194, created);
    if (created) {
        void* const created_table = ptr(created, 0); // original caches EBX across node getter
        void* const object = static_cast<unsigned char*>(self) + 0x10;
        const char* node_text = opaque(WindowCStr005e5670, node, nullptr);
        // +98 is GetModel(part,force), RET8. The earlier name and mode pushes
        // survive it and feed the following Aurora attachment call, RET0Ch.
        using GetModel = void* (__fastcall*)(void*, void*, U, U);
        void* attach = slot<GetModel>(object, 0x98)(object, nullptr, 0xFF, 1);
        using Attach = void* (__fastcall*)(void*, void*, void*, const char*, I);
        (void)read<Attach>(created_table, 0x50)(ptr(self, 0x194), nullptr, attach, node_text, 0);
        using Animate = U (__fastcall*)(void*, void*, const char*, float, U, U);
        void* current = ptr(self, 0x194);
        slot<Animate>(current, 0x18)(current, nullptr, "impact", 1.0f, 1, 0);
    }
    void* master = opaque(kotor_m15e_client_get_client_ai_master_005ed6b0, ptr(manager(), 4), nullptr);
    opaque(recovered_client_ai_master_add_object_00604a20, master, nullptr, static_cast<unsigned char*>(self) + 0x10);
}

// 00611470..00611695. RET8. Priority uses a byte write into the original index
// argument, so its upper24 bits survive. Local STR_RES lifetime follows78594.
void __fastcall recovered_client_creature_play_sound_set_sound_00611470(void* self, void*, U index, U spatial) {
    StringResource resource;
    (void)opaque(c2da_default_ref_00405ed0, &resource.sound, nullptr);
    if (!opaque(kotor_animation_sound_get_soundset_0060b8a0, self, nullptr, index, &resource)) return;
    if (opaque(kotor_m15a_object_get_server_object_0063d4b0, self, nullptr)) {
        void* object = opaque(kotor_m15a_object_get_server_object_0063d4b0, self, nullptr);
        void* server = slot<Getter>(object, 0x30)(object, nullptr);
        if (server) {
            void* again = server_creature(self);
            if (opaque(&slot<Predicate>, again, U{0x94})(again, nullptr) ||
                opaque(recovered_creature_incapacitated_004ef890, server_creature(self), nullptr)) {
                if (static_cast<std::uint16_t>(index) != 0x10) return;
            }
        }
    }
    U priority;
    if (opaque(client_get_player_creature_005ed540, ptr(manager(), 4), nullptr) == self) priority = 8;
    else if (opaque(recovered_client_get_player_by_id_005ed590, ptr(manager(), 4), nullptr, read<U>(self, 4))) priority = 9;
    else {
        I size; // original output slot is not initialized before table query
        (void)opaque(c2da_current_get_int_entry_00413510, table(0xC), nullptr,
            read<std::uint16_t>(ptr(self, 0x21C), 0x18), static_cast<I>(kotor_rules_cell32_007a2124), &size);
        priority = size <= 3 ? 0x11u : 0x10u;
    }
    priority |= index & 0xFFFFFF00u;
    write<U>(self, 0xD0, 0);
    void* old = ptr(self, 0xCC);
    if (old) { dispose_stream(old); write<void*>(self, 0xCC, nullptr); }
    void* raw = opaque(live_operator_new, 4u);
    void* stream;
    try { stream = raw ? table29_required::gui_sound_construct_005d5a50(raw, nullptr) : nullptr; }
    catch (...) { live_free(raw); throw; } // unwind state2 ->1, 00727CF0
    write<void*>(self, 0xCC, stream);
    if (!stream) return;
    opaque(kotor_m15_stream_source_set_3d_005d6290, stream, nullptr, spatial);
    table29_required::gui_priority_005d5ba0(ptr(self, 0xCC), nullptr, priority);
    if (spatial) opaque(kotor_m15_stream_source_set_position_005d62c0, ptr(self, 0xCC), nullptr,
                                                            static_cast<unsigned char*>(self) + 0x24);
    if (!table29_required::gui_play_005d5b10(ptr(self, 0xCC), nullptr, &resource.sound, 4, 0, 0, 0)) {
        dispose_stream(ptr(self, 0xCC));
        write<void*>(self, 0xCC, nullptr);
    }
}

// 00617470..006179BD, RET8. EAX0 for absent victim; EAX1 after valid victim even
// when a2/data+20 suppress audio. Every branch preserves original table queries.
U __fastcall kotor_fastcall_00617470(void* self, void*, void* data, U suppress) {
    if (!data) return 0;
    void* victim = opaque(kotor_fastcall_005ed580, ptr(manager(), 4), nullptr, read<U>(data, 0x14));
    if (!victim || !slot<Getter>(victim, 0xC)(victim, nullptr)) return 0;
    victim = slot<Getter>(victim, 0xC)(victim, nullptr); // original second read
    I appearance = 0, sound_type = 0;
    float pitch = 0.0f;
    String sound;
    if (suppress || read<U>(data, 0x20)) return 1;
    (void)opaque(c2da_current_get_int_entry_00413510, table(0xC), nullptr,
        read<std::uint16_t>(ptr(self, 0x21C), 0x18), static_cast<I>(kotor_rules_cell32_007a2128), &appearance);
    (void)opaque(c2da_current_get_int_entry_00413510, table(0x5C), nullptr, appearance,
                                          static_cast<I>(kotor_rules_cell32_007a2140), &sound_type);
    if (sound_type == 0) {
        U equip = 0;
        const unsigned char attack = read<unsigned char>(data, 0x28);
        if (attack == 1 || attack == 6) equip = 0x10;
        else if (read<unsigned char>(opaque(gob_get_property_0x68_0063c070, self, nullptr), 0xC5) == 3) equip = 0x10;
        else if (read<unsigned char>(data, 0x28) == 2) equip = 0x20;
        void* const client = ptr(manager(), 4);
        const U item_id = opaque(kotor_m15a_creature_get_equipped_item_id_0060ca80, self, nullptr, equip);
        void* item = opaque(kotor_m15_client_get_item_by_id_005ed5a0, client, nullptr, item_id);
        if (item) {
            void* base = opaque(kotor_m15b_base_item_array_get_base_item_005b31a0, ptr(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                      read<I>(item, 0xC));
            sound_type = read<unsigned char>(base, 0x8C);
        }
    }
    TemporaryString material;
    const unsigned char result = read<unsigned char>(data, 0xC);
    if ((result == 8 || result == 4) && opaque(kotor_animation_sound_animation_parry_0063c4f0,
            victim, nullptr, opaque(kotor_m15a_object_get_current_animation_0063c060, victim, nullptr))) {
        TemporaryString number, prefix;
        (void)opaque(resman_string_from_int_005e5bc0, number.get(), nullptr, random_mod2()); number.active = true;
        ::new (static_cast<void*>(prefix.get())) String("Parry"); prefix.active = true;
        (void)GuiConcat005e5d10(prefix.get(), nullptr, material.get(), number.get()); material.active = true;
        prefix.destroy(); number.destroy(); //0061763B/00617649
    } else {
        ::new (static_cast<void*>(material.get())) String(); material.active = true;
        const unsigned char type = read<unsigned char>(victim, 8);
        if (type == 5) { // original byte00746634
            victim = slot<Getter>(victim, 0x2C)(victim, nullptr);
            if (opaque(kotor_animation_sound_damage_effect_00616890, victim, nullptr)) {
                (void)opaque(GuiAssign005e5140, material.get(), nullptr, "forcefield");
            } else if (read<U>(victim, 0xC) || read<U>(victim, 0x14)) {
                (void)opaque(GuiAssign005e5140, material.get(), nullptr, "stone");
            } else if (read<U>(victim, 0x10)) {
                (void)opaque(GuiAssign005e5140, material.get(), nullptr, "wood");
            } else {
                (void)opaque(c2da_current_get_int_entry_00413510, table(0xC), nullptr,
                    read<std::uint16_t>(ptr(victim, 0x21C), 0x18), static_cast<I>(kotor_rules_cell32_007a2128), &appearance);
                (void)opaque(c2da_current_get_string_entry_00413190, table(0x5C), nullptr, appearance,
                    static_cast<I>(kotor_rules_cell32_007a2144), material.get());
                if (opaque(frame_LogIsEmpty_005e58a0, material.get(), nullptr)) {
                    void* const client = ptr(manager(), 4);
                    U id = opaque(kotor_m15a_creature_get_equipped_item_id_0060ca80, victim, nullptr, 2);
                    void* item = opaque(kotor_m15_client_get_item_by_id_005ed5a0, client, nullptr, id);
                    if (item) material.get()->Assign005e5c50(reinterpret_cast<String*>(
                        static_cast<unsigned char*>(opaque(kotor_m15b_item_get_base_item_005b4790, item, nullptr)) + 0xBC));
                }
                if (opaque(frame_LogIsEmpty_005e58a0, material.get(), nullptr))
                    (void)opaque(GuiAssign005e5140, material.get(), nullptr, "leather");
            }
        } else if (type == 10) { // original byte00746639
            void* door = slot<Getter>(victim, 0x14)(victim, nullptr);
            const U generic = read<unsigned char>(door, 0x124);
            if (!generic) (void)opaque(c2da_current_get_int_entry_00413510, table(0x28), nullptr,
                read<unsigned char>(door, 0x125), static_cast<I>(kotor_rules_cell32_007a2250), &appearance);
            else (void)opaque(c2da_current_get_int_entry_00413510, table(0x24), nullptr, static_cast<I>(generic),
                static_cast<I>(kotor_rules_cell32_007a224c), &appearance);
            (void)opaque(c2da_current_get_string_entry_00413190, table(0x70), nullptr, appearance,
                static_cast<I>(kotor_rules_cell32_007a2260), material.get());
        } else if (type == 9) { // original byte00746638
            void* placeable = slot<Getter>(victim, 0x4C)(victim, nullptr);
            (void)opaque(c2da_current_get_int_entry_00413510, table(0x2C), nullptr, read<unsigned char>(placeable, 0x10C),
                static_cast<I>(kotor_rules_cell32_007a2254), &appearance);
            (void)opaque(c2da_current_get_string_entry_00413190, table(0x70), nullptr, appearance,
                static_cast<I>(kotor_rules_cell32_007a2260), material.get());
        }
        TemporaryString number, joined;
        (void)opaque(resman_string_from_int_005e5bc0, number.get(), nullptr, random_mod2()); number.active = true;
        (void)GuiConcat005e5d10(material.get(), nullptr, joined.get(), number.get()); joined.active = true;
        material.get()->Assign005e5c50(joined.get());
        joined.destroy(); number.destroy(); //006178B0/006178BD
    }
    (void)opaque(c2da_current_get_string_00413270, table(0x60), nullptr, sound_type, material.get(), &sound);
    (void)opaque(kotor_m15b_c2da_get_float_entry_00413350, table(0x60), nullptr, sound_type,
                                               static_cast<I>(kotor_rules_cell32_007a21a4), &pitch);
    material.destroy(); //00617916..0061791F before CStr
    (void)opaque(WindowCStr005e5670, &sound, nullptr); //00617924 original discarded call
    menu_resources::Ref ref;
    ref.construct_00406d80(opaque(WindowCStr005e5670, &sound, nullptr));
    const KotorM15VectorWords position = read<KotorM15VectorWords>(self, 0x24);
    (void)opaque(kotor_m15_sound_play_3d_one_shot_005d5e10, kotor_cell_sound_007a39ec, nullptr, &ref,
                                                 position, 1.5f, 0xF, 0, 0, 0.0f, pitch);
    return 1;
}
}
