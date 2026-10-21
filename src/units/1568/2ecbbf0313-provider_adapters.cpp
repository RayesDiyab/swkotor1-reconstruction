// Provider adapters: the exact selected portable bodies are called through their
// designed ports.  These are not second semantic bodies: kt_get_class and
// kt_feat_remaining_uses are argument/ABI facades only.
#include "30d695bcc8-talent_cohort.hpp"
#include <cstdlib>

namespace kotor_talent {
namespace {

using Word = U;
using Memory = ::kotor::server_mainloop::Memory;

struct DirectMemory final : Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, pv(a), 4); return v; }
    void write(Word a, Word v) override { std::memcpy(pv(a), &v, 4); }
    Word read16(Word a) override { unsigned short v; std::memcpy(&v, pv(a), 2); return v; }
    void write16(Word a, Word v) override { const unsigned short s = static_cast<unsigned short>(v); std::memcpy(pv(a), &s, 2); }
    Word read8(Word a) override { return *static_cast<unsigned char*>(pv(a)); }
};

struct Ports final : w2::g07::AppearancePorts {
    Word item_in_slot_005a4c20(Word inventory, Word slot) override {
        return static_cast<Word>(reinterpret_cast<std::uintptr_t>(
            recovered_equipment_item_005a4c20(pv(inventory), nullptr, slot)));
    }
    Word server_info_004aec90(Word app) override {
        return static_cast<Word>(reinterpret_cast<std::uintptr_t>(
            recovered_server_info_004aec90(pv(app), nullptr)));
    }
    void set_last_instant_equip_004efe00(Word creature, Word slot, Word value) override {
        recovered_creature_set_last_instant_equip_004efe00(pv(creature), nullptr, slot, value);
    }
    Word base_item_005b31a0(Word array, Word type) override {
        return static_cast<Word>(reinterpret_cast<std::uintptr_t>(
            kotor_m15b_base_item_array_get_base_item_005b31a0(pv(array), nullptr, static_cast<I>(type))));
    }
    Word rules_007a3a28() override { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(kotor_cell_rules_007a3a28)); }
    Word item_base_item_005b4790(Word item) override {
        return static_cast<Word>(reinterpret_cast<std::uintptr_t>(
            kotor_m15b_item_get_base_item_005b4790(pv(item), nullptr)));
    }
    Word stats_level_005a5fd0(Word stats, Word flag) override {
        return recovered_stats_level_005a5fd0(pv(stats), nullptr, flag);
    }
    Word item_property_exists_00555c90(Word item, Word type, Word subtype) override {
        return recovered_item_property_has_00555c90(pv(item), nullptr, type, subtype);
    }
    Word item_property_by_type_005539c0(Word item, Word out, Word type, Word subtype) override {
        return static_cast<Word>(reinterpret_cast<std::uintptr_t>(
            recovered_item_property_query_005539c0(pv(item), nullptr, reinterpret_cast<void**>(pv(out)), type, subtype)));
    }
    Word stats_has_feat_005a6630(Word stats, Word feat) override {
        return recovered_stats_has_feat_005a6630(pv(stats), nullptr, feat);
    }
    Word rules_feat_00550c00(Word rules, Word feat) override {
        return static_cast<Word>(reinterpret_cast<std::uintptr_t>(
            kotor_rules::kotor_fastcall_00550c00(pv(rules), nullptr, static_cast<unsigned short>(feat))));
    }
    Word stats_class_005a4e90(Word stats, Word index) override {
        return kt_get_class_005a4e90(pv(stats), nullptr, index);
    }
    void list_add_00671c00(Word list, Word value) override {
        gui_load_array_add_00671c00(pv(list), nullptr, value);
    }
    // Original 005A6680 wraps its 16-bit feat index; when the 32-bit count exceeds
    // 0xFFFF and the feat is absent the original loops forever (the provider's
    // "trap" port exists only to surface that wrap).  This port reproduces the
    // original non-terminating loop rather than substituting a trap or abort.
    // It is unreachable for a count <= 0xFFFF; closure for the degenerate
    // count > 0xFFFF case is explicitly NOT claimed (see RECEIPT/REVIEW-REQUEST).
    [[noreturn]] void trap(Word, const char*) override {
        volatile unsigned spin = 0;
        for (;;) spin = spin + 1u;
    }
};

}  // namespace

extern "C" unsigned __fastcall kt_get_class_005a4e90(void* stats, void*, unsigned index) {
    DirectMemory memory;
    return ::kotor::server_mainloop::creature_get_class_005a4e90(memory,
                                                               static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(stats)),
                                                               index) & 0xffu;
}

extern "C" unsigned __fastcall kt_is_jedi_005be4a0(void*, void*, unsigned classId) {
    return ::kotor::server_mainloop::creature_force_class_005be4a0(classId);
}

extern "C" unsigned __fastcall kt_feat_remaining_uses_005a6680(void* stats, void*, unsigned feat) {
    DirectMemory memory;
    Ports ports;
    return w2::g07::feat_remaining_uses_005a6680(memory, ports,
                                                 static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(stats)),
                                                 feat);
}

}  // namespace kotor_talent

