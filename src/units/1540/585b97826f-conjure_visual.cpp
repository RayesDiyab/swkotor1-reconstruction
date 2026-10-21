#include "b1751895fd-w4_conjure.h"
#include "ead2dcbdd7-m15e.h"
#include "fe0daa935e-scene3d.h"
#include "07d4e464a9-vtables_gen.h"
#include "b0e1e980f2-m15_l0.h"
#include <cstdio>
#include <cstring>

namespace gui_load { struct Object; }
extern "C" {
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*, void*, menu_resources::String*);
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*, const char*, std::FILE*, std::FILE*);
gui_load::Object* __fastcall gui_load_slot_self_00641db0(gui_load::Object*, void*);
void __fastcall kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(void*, void*);
extern void* kotor_cell_sound_007a39ec;
}
namespace {
using U = std::uint32_t;
template<class T> T load(const void* p, U off = 0) {
    T value; std::memcpy(&value, static_cast<const unsigned char*>(p) + off, sizeof value); return value;
}
template<class T> void store(void* p, U off, T value) {
    std::memcpy(static_cast<unsigned char*>(p) + off, &value, sizeof value);
}
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
const aurora_model::GobVTable* table(void* model) { return load<const aurora_model::GobVTable*>(model); }
const KotorM15CreatureObjectMethods* object_table(void* object) { return load<const KotorM15CreatureObjectMethods*>(object); }
}

// 00695EF0: event callback; the third argument is the registered data pointer.
extern "C" void __cdecl recovered_spell_conjure_done_00695ef0(void* model, void*, aurora_model::Part* data) {
    if (data != nullptr) {
        if (model == load<void*>(data, 0)) store(data, 0x24, U{1});
        if (model == load<void*>(data, 4)) store(data, 0x28, U{1});
        if (model == load<void*>(data, 8)) store(data, 0x2C, U{1});
    }
}

// 00695F30: five CResRef values and the creature pointer occupy 54h stack bytes.
extern "C" U __fastcall recovered_spell_load_conjure_00695f30(void* self, void*,
    menu_resources::Ref head, menu_resources::Ref hand, menu_resources::Ref ground,
    menu_resources::Ref sound_first, menu_resources::Ref sound_second, void* creature) {
    kotor_reconstruction::StartupString text;
    kotor_m15e_spell_cast_visual_unload_conjure_visual_00695d10(self, nullptr);
    menu_resources::Ref* references[3] = {&head, &hand, &ground};
    const char* hooks[3] = {"headconjure", "handconjure", "root"};
    for (U i = 0; i < 3; ++i) {
        c2da_ref_to_string_00405f70(references[i], nullptr, &text);
        if (text.BorrowedLength005e5790() <= 0) continue;
        const char* resource = WindowCStr005e5670(&text, nullptr);
        aurora_model::Gob* model = aurora_new_aur_object_00449cc0(resource,
            i == 2 ? "conjuregroundvisual" : "conjurevisual", nullptr, nullptr);
        store(self, i * 4, model);
        if (model == nullptr) continue;
        store(self, 0x24 + i * 4, U{0});
        table(model)->register_callback(model, nullptr, &recovered_spell_conjure_done_00695ef0,
            "donefade", static_cast<aurora_model::Part*>(self), 0x461C3C00u, 0);
        // Original caches this table before GetModel, then reloads the receiver.
        const aurora_model::GobVTable* attach_table = table(load<void*>(self, i * 4));
        void* parent = object_table(creature)->s098_get_model(creature, nullptr, 0xFF, 1);
        attach_table->s050_gob_attach_to_object(load<void*>(self, i * 4), nullptr, parent, hooks[i], 0);
        model = load<aurora_model::Gob*>(self, i * 4);
        table(model)->s018_gob_play_animation(model, nullptr, word("conjure01"), 0x3F800000u, 0, 0);
    }
    U position[3] = {0, 0, 0};
    U orientation[4] = {0x3F800000u, 0, 0, 0};
    auto* model = static_cast<aurora_model::Gob*>(object_table(creature)->s098_get_model(creature, nullptr, 0xFF, 1));
    if (model == nullptr || table(model)->s098_gob_get_part_position(model, nullptr,
            word("handconjure"), word(position), word(orientation)) == 0) {
        std::memcpy(position, static_cast<unsigned char*>(creature) + 0x24, 12);
    }
    const char* first = static_cast<const char*>(static_cast<const void*>(gui_load_slot_self_00641db0(
        static_cast<gui_load::Object*>(static_cast<void*>(&sound_first)), nullptr)));
    if (*first != 0) {
        KotorM15VectorWords copied; std::memcpy(&copied, position, sizeof copied);
        kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr,
            &sound_first, copied, 1.5f, 0xD, 0, 0, 0.0f, 0.0f);
    }
    const char* second = static_cast<const char*>(static_cast<const void*>(gui_load_slot_self_00641db0(
        static_cast<gui_load::Object*>(static_cast<void*>(&sound_second)), nullptr)));
    if (*second != 0) {
        KotorM15VectorWords copied; std::memcpy(&copied, position, sizeof copied);
        kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr,
            &sound_second, copied, 1.5f, 0xD, 0, 0, 0.0f, 0.0f);
    }
    return 1;
}
