#pragma once
// Typed private bridge from the selected 1065 Native::increment port to the
// existing 1061 increment_players_00508c20 body and the private 00507220 body.
#include "81ab9ce981-c2_priority.hpp"
#include "a7d5343046-w2_entry_native.hpp"

namespace c2 {
inline void* pointer(Word address) {
    return reinterpret_cast<void*>(address);
}

extern "C" unsigned __fastcall recovered_ai_level_004b08a0(void*, void*, void*, unsigned);
extern "C" void __fastcall kotor_fastcall_00507340(void*, void*);

struct C2AreaPorts final : kotor::area_attachment::Ports, PriorityPorts {
    explicit C2AreaPorts(w2::native::AreaMemory& memory) : memory_(memory) {}

    std::uint8_t get_game_object_004d8230(Word array, Word id, Word& output) override {
        return base_.get_game_object_004d8230(array, id, output);
    }
    Word as_area_slot_28(Word function, Word object) override {
        return base_.as_area_slot_28(function, object);
    }
    void increase_ai_priority_00507220(Word area) override {
        c2::increase_ai_priority_00507220(memory_, *this, area);
    }
    void decrease_ai_priority_00507340(Word area) override {
        // stale-stubs G3: 00507340 CSWSArea::DecreaseAILevelPriority (claude_combat_spell_mine), was the W2 trap.
        kotor_fastcall_00507340(pointer(area), nullptr);
    }

    Word object_array() override {
        const Word manager = memory_.read(0x7a39fcU);
        const Word app = memory_.read(manager + 8U);
        const Word internal = memory_.read(app + 4U);
        return memory_.read(internal + 0x1005cU);
    }
    Word ai_master() override {
        const Word manager = memory_.read(0x7a39fcU);
        const Word app = memory_.read(manager + 8U);
        const Word internal = memory_.read(app + 4U);
        return memory_.read(internal + 0x10044U);
    }
    std::uint8_t lookup(Word array, Word id, Word& object) override {
        return kotor_fastcall_004d8230(pointer(array), nullptr, id, &object);
    }
    Word as_creature(Word object) override {
        return slot(object, 0x30U);
    }
    Word as_door(Word object) override {
        return slot(object, 0x18U);
    }
    Word as_trigger(Word object) override {
        return slot(object, 0x40U);
    }
    Word as_placeable(Word object) override {
        return slot(object, 0x48U);
    }
    Word as_area_of_effect(Word object) override {
        return slot(object, 0x50U);
    }
    int string_length(Word string) override {
        return static_cast<int>(async_string_length_005e5790(pointer(string), nullptr));
    }
    void set_ai_level(Word master, Word object, Word level) override {
        (void)recovered_ai_level_004b08a0(pointer(master), nullptr,
                                          pointer(object), level);
    }

private:
    Word slot(Word object, Word offset) {
        using Function = Word(__thiscall*)(void*);
        const Word function = memory_.read(memory_.read(object) + offset);
        return reinterpret_cast<Function>(function)(pointer(object));
    }

    w2::native::AreaMemory& memory_;
    w2::native::AreaPorts base_;
};
}
