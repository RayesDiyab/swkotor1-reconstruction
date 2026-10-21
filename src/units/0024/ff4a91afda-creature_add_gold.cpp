// Original004F3DB0, RET8. Lower providers retain their existing owners.
#include <cstdint>
#include <cstring>
static_assert(sizeof(void*) == 4, "original x86 layout");
using U32 = std::uint32_t;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }
extern "C" {
void* __cdecl live_operator_new(unsigned);
void __cdecl live_free(void*);
void* __fastcall recovered_message_data_construct_004d69f0(void*, void*);
void __fastcall recovered_event_integer_004d76b0(void*, void*, unsigned, unsigned);
void __fastcall recovered_creature_feedback_004ede10(void*, void*, unsigned, void*);
}
namespace {
U32 word(const void* object, unsigned offset) {
    U32 value; std::memcpy(&value, static_cast<const unsigned char*>(object)+offset, 4); return value;
}
void* pointer(const void* object, unsigned offset) {
    void* value; std::memcpy(&value, static_cast<const unsigned char*>(object)+offset, 4); return value;
}
void store(void* object, unsigned offset, U32 value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset, &value, 4);
}
std::int32_t signed_word(U32 value) {
    std::int32_t result; std::memcpy(&result, &value, 4); return result;
}
void* party_table() {
    return lane_config_required::GetPartyTable_004aee70(pointer(kotor_menu_window_cells::manager_007a39fc, 8), nullptr);
}
// The original inlines these reads twice; each invocation reloads A88 and the manager/party.
U32 current_gold(void* creature) {
    return word(creature, 0xa88) ? word(party_table(), 0xfc) : word(creature, 0x9d0);
}
}
extern "C" void __fastcall recovered_creature_add_gold_004f3db0(void* creature, void*, U32 amount, U32 notify) {
    U32 value = current_gold(creature) + amount;   // 004F3DC8 reads [ECX+A88] unchecked (null creature faults as original)                                     //004F3DC8..3DF3 wrapping LEA
    if (signed_word(value) > 999999999) value = 999999999;                            //004F3DF6 signed JLE
    const U32 delta = value - current_gold(creature);                                //004F3E03..3E2B reload, wrapping SUB
    if (signed_word(value) > 999999999) value = 999999999;                            //004F3E2D repeated signed cap
    if (word(creature, 0xa88)) store(party_table(), 0xfc, value);                      //004F3E3A..3E51 third lookup
    else store(creature, 0x9d0, value);                                              //004F3E59
    if (notify == 1 && delta != 0) {                                                 //004F3E5F..3E68
        void* allocation = live_operator_new(0x34);                                  //004F3E6A..3E6C
        void* data = nullptr;
        if (allocation) {
            try { data = recovered_message_data_construct_004d69f0(allocation, nullptr); } //004F3E86
            catch (...) { live_free(allocation); throw; }                            //0071B270..B27A constructor-only cleanup
        }
        // 004F3E8F..3E9E: a failed allocation leaves ESI=0 and the original calls 004D76B0 with ECX=0 unchecked (quirk kept).
        recovered_event_integer_004d76b0(data, nullptr, 0, delta);                    //004F3E91..3E9E
        recovered_creature_feedback_004ede10(creature, nullptr, 0x94, data);           //004F3EA3..3EAB transfers destruction/free
    }
}
