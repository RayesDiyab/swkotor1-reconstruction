// Original004AEF40 CServerExoApp::ClearAllActions, RET4; native fastcall@12.
#include <cstdint>
#include <cstring>
static_assert(sizeof(void*) == 4, "original x86 object layout");
extern "C" {
void* __fastcall kotor_fastcall_004b1790(void*, void*, unsigned);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, unsigned);
void __fastcall recovered_combat_round_remove_all_actions_004d3770(void*, void*);
}
namespace {
void* pointer(const void* object, unsigned offset) {
    void* value;
    std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, sizeof value);
    return value;
}
void store(void* object, unsigned offset, std::uint32_t value) {
    std::memcpy(static_cast<unsigned char*>(object) + offset, &value, sizeof value);
}
}
extern "C" void __fastcall kotor_fastcall_004aef40(void* server, void*, unsigned server_id) {
    void* creature = kotor_fastcall_004b1790(pointer(server, 4), nullptr, server_id); //004AEF40..4E
    if (!creature) return;                                                        //004AEF50..52
    w5_object_clear_all_actions_004ccd80(creature, nullptr, 1);                    //004AEF54..58
    void* round = pointer(creature, 0x9c8);                                        //004AEF5D
    // 004AEF5D..63: ECX=[creature+9C8] passed unchecked (004D3770 faults on null as original).
    recovered_combat_round_remove_all_actions_004d3770(round, nullptr);           //004AEF63
    round = pointer(creature, 0x9c8);                                             //004AEF68 reload after call
    // 004AEF73: [round+9CC] written unchecked as original.
    store(round, 0x9cc, 0x7f000000u);                                              //004AEF73
    store(creature, 0x50c, 0x7f000000u);                                           //004AEF79
    store(creature, 0x504, 0x7f000000u);                                           //004AEF7F
    store(creature, 0x524, 0x7f000000u);                                           //004AEF85
    store(creature, 0x528, 0x7f000000u);                                           //004AEF8B
}
