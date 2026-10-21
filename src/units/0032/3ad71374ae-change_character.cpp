// Original005F6B60 and004F76C0; existing W4 outer005EDC30 packaging.
#include <cstdint>
#include <cstring>
static_assert(sizeof(void*) == 4, "original x86 layout");
using U32 = std::uint32_t;
extern "C" {
void* __fastcall kotor_fastcall_00635460(void*, void*);
void __fastcall kotor_fastcall_00635480(void*, void*, std::int32_t);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
void __fastcall recovered_reset_all_follow_data_00637890(void*, void*, void*, float, unsigned);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);
unsigned __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void __fastcall kotor_fastcall_0063f9d0(void*, void*, U32, U32);
void* __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
void __fastcall c2da_005e9c10(void*, void*, void*);
void __fastcall recovered_action_node_destroy_004cc4d0(void*, void*);
void __cdecl live_free(void*);
void __fastcall kotor_nested_005f4d60(void*, void*, void*);  // 005F4D60 thiscall RET 4 (camera_nested_providers.obj)
}
namespace {
void* pointer(const void* object, unsigned offset) {
    void* value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, 4); return value;
}
U32 word(const void* object, unsigned offset) {
    U32 value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, 4); return value;
}
void store(void* object, unsigned offset, U32 value) {
    std::memcpy(static_cast<unsigned char*>(object) + offset, &value, 4);
}
template<class R, class... A> R vcall(void* object, unsigned slot, A... args) {
    return reinterpret_cast<R(__thiscall*)(void*, A...)>(pointer(pointer(object, 0), slot))(object, args...);
}
}
// Original RET4, EAX0/1. Only the first matching action accepted by slot74 is removed.
extern "C" unsigned __fastcall kotor_fastcall_004f76c0(void* object, void*, U32 action_type) {
    void* list = pointer(object, 0xfc);                                                //004F76C6
    // 004F76CC: unchecked read through [object+FCh] as original (null faults there).
    void* position = pointer(list, 0);                                               //004F76CC
    while (position) {
        void* node = unresolved_list_at_pos_005e9b60(pointer(object, 0xfc), nullptr, position); //004F76E0..EC
        // 004F76EE: unchecked node read as original.
        if (word(node, 0) == action_type && vcall<U32>(object, 0x74, node, U32{1}) == 1) {
            c2da_005e9c10(pointer(object, 0xfc), nullptr, position);                   //004F7723..2E
            recovered_action_node_destroy_004cc4d0(node, nullptr);                    //004F7733..35
            live_free(node);                                                        //004F773A..3B
            return 1;                                                               //004F7746
        }
        (void)unresolved_list_next_005e9b90(pointer(object, 0xfc), nullptr, &position); //004F7701..0C
    }
    return 0;                                                                       //004F76CE/771B
}
// Original RET8, no return value consumed.
extern "C" void __fastcall kotor_fastcall_005f6b60(void* internal, void*, U32 selection, U32 voice) {
    void* old_leader = kotor_fastcall_00635460(pointer(internal, 0x270), nullptr);       //005F6B66..6D
    std::int32_t signed_selection; std::memcpy(&signed_selection, &selection, 4);
    kotor_fastcall_00635480(pointer(internal, 0x270), nullptr, signed_selection);       //005F6B72..7F
    if (old_leader == kotor_fastcall_00635460(pointer(internal, 0x270), nullptr)) return; //005F6B84..91
    void* leader = kotor_fastcall_00635460(pointer(internal, 0x270), nullptr);           //005F6B97..A3
    // 005F6BA8: unchecked leader read as original.
    U32 position[3] = {word(leader, 0x24), word(leader, 0x28), word(leader, 0x2c)};       //005F6BA5..BE
    const float yaw = kotor_m15a_yaw_004aa0f0(static_cast<unsigned char*>(leader) + 0x30); //005F6BC2/FSTP32
    recovered_reset_all_follow_data_00637890(pointer(internal, 0x270), nullptr, position, yaw, 1); //005F6BCA..D5
    void* server_creature = kotor_m15a_creature_get_server_creature_0060fb20(leader, nullptr); //005F6BDC
    if (server_creature) {
        void* pathfind = pointer(server_creature, 0x340);                             //005F6BE7, NOT combat-round
        // 005F6BED: unchecked pathfind write as original.
        store(pathfind, 0x34, 0x7f000000u);                                           //005F6BED
        (void)kotor_fastcall_004f76c0(server_creature, nullptr, 0x3d);                  //005F6BF4..F8
        if (vcall<U32>(server_creature, 0x94) != 0 ||
            recovered_creature_incapacitated_004ef890(server_creature, nullptr) != 0) voice = 0; //005F6BFD..6C18
    }
    kotor_fastcall_0063f9d0(pointer(internal, 0x18), nullptr, word(leader, 4), 1);        //005F6C1E..27
    if (voice) kotor_nested_005f4d60(internal, nullptr, leader);                       //005F6C2C..34 PlaySelectSound(leader), RET 4
    if (void* camera = pointer(internal, 0x2a0)) vcall<void>(camera, 0x2c);              //005F6C39..45
    store(internal, 0x4b4, 1);                                                        //005F6C48
}
// Existing W4 outer wrapper: client+4, unchanged two original stack words.
extern "C" void __fastcall kotor_fastcall_005edc30(void* client, void*, U32 a, U32 b) {
    kotor_fastcall_005f6b60(pointer(client, 4), nullptr, a, b);
}
