// claude_loadgame_queue.cpp (lane worker loadgame): the saved-game event / action queue loaders behind the two
// remaining 0886 save-load gates (NativeModuleLoad::messages_load_004b0a00, NativeObjectCommon::actions_nonempty),
// transcribed from the original swkotor.exe (read-only):
//   004B0A00 CServerAIMaster::LoadEventQueue(CResGFF*, CResStruct*)                  RET 8, EAX 1   kotor_fastcall_004b0a00
//   004B0290 CServerAIEventNode::LoadNode(CResGFF*, CResStruct*)                     RET 8, EAX 0/1 kotor_fastcall_004b0290
//   004CECB0 CSWSObject::LoadActionQueue(CResGFF*, CResStruct*)                      RET 8          kotor_fastcall_004cecb0
//            (+ loadgame_object_load_action_queue_tail_004ced20: the loop from 004CED20 on, for the 0886 ObjectCommon
//             port, which has already read "ActionList" into its frame at s+B4h and checked the count)
//   005D1100 CVirtualMachine::LoadScriptSituation(CVirtualMachineScript**, CResGFF*, CResStruct*) RET 0Ch
//            (thunk: ecx = [this] -> jmp 005D26F0)                                                   kotor_fastcall_005d1100
//   005D26F0 CVirtualMachineInternal::LoadScriptSituation_Internal(CVirtualMachineScript**, CResGFF*, CResStruct*)
//                                                                                    RET 0Ch, EAX 1 kotor_fastcall_005d26f0
//   005D1EF0 CVirtualMachineStack::LoadStack(CResGFF*, CResStruct*)                 RET 8, EAX 1   kotor_fastcall_005d1ef0
//   005D1740 CVirtualMachineScript::CVirtualMachineScript()                         RET 0          (file-local, see below)
//   00412380 CResGFF::ReadFieldVOID(CResStruct*, void*, ulong, char*, int*, void*)  RET 18h        kotor_fastcall_00412380
//   004D57F0 CSWSCombatAttackData::CSWSCombatAttackData()                            RET 0          kotor_fastcall_004d57f0
//   004D6B10 CSWCCMessageData::LoadData(CResGFF*, CResStruct*)                       RET 8, EAX 1   kotor_fastcall_004d6b10
//   004AF1C0 CServerAIBodyBagInfo::LoadBodyBag(CResGFF*, CResStruct*)                RET 8, EAX 1   kotor_fastcall_004af1c0
//   004AF130 CSWSSpellScriptData::CSWSSpellScriptData()                              RET 0          kotor_fastcall_004af130
//   004EE2E0 CSWSSpellScriptData::LoadData(CResGFF*, CResStruct*)                    RET 8, EAX 1   kotor_fastcall_004ee2e0
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only
// (live_operator_new / live_free); x87 float results stored as dwords exactly where the original fstp's them.
// GFF handles: CResList = 14h bytes (struct id + 16-char label), CResStruct = 4 bytes (GetListElement /
// GetStructFromStruct write one dword). Reader status out-parameters are argument slots in the original (locals here).
// Quirks kept: LoadEventQueue allocates the 18h-byte node without constructing it and frees it raw when LoadNode
// returns 0 (event 18); LoadNode leaves node+14h untouched for event ids 0, 19, 23, 25 and >= 28; the type-5 and
// type-14 (effect) arms ignore a missing "EventData" struct; LoadStack ignores a missing "Stack" list and dispatches
// unknown stack types to the command implementer's LoadGameDefinedStructure(type-10h, ...) (vtable +28h); null
// allocations are dereferenced where the original does (CExoString/stack/script results unchecked).
// LoadActionQueue keeps the original frame layout (one frame buffer, same offsets) so that the parameter arrays,
// the stale per-slot type codes used for freeing and the 13-slot capacity behave as in the original.
// CVirtualMachineScript() is rebuilt file-locally: the existing 0851 body (recovered_vm_construct::frame_005d1740)
// is only reachable through a Ports interface whose other pure virtuals (VM tables) have no provider here.
#include <cstdint>
#include <cstring>
#include <initializer_list>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline U rd8(U a) { return *static_cast<U8*>(ptr(a)); }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
inline void wr8(U a, U v) { *static_cast<U8*>(ptr(a)) = static_cast<U8>(v); }
inline void wr16(U a, U v) { const U16 x = static_cast<U16>(v); std::memcpy(ptr(a), &x, 2); }
inline void wrf(U a, float v) { std::memcpy(ptr(a), &v, 4); }
template <class R, class... A> R vcall(U object, U slot, A... a) {
    using F = R(__thiscall*)(void*, A...);
    return reinterpret_cast<F>(rd(rd(object) + slot))(ptr(object), a...);
}
}

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); } // 005B3190 CExoString() [0603]
namespace kotor_q05 { struct StructHandle; struct FieldRecord; struct CResGFF {
    FieldRecord* __thiscall GetField(StructHandle*, unsigned);                                 // 00410990 [0290]
    unsigned __thiscall GetDataField(FieldRecord*, unsigned*); }; }                            // 00410A20 [0290]
namespace frame_control { extern unsigned message_manager_007a3a00; }                        // [007A3A00] CVirtualMachine* [0456]

extern "C" {
void* __cdecl live_operator_new(unsigned);                                                 // 006FA7E6
void __cdecl live_free(void*);                                                             // 006FA390
U __fastcall gff_label_00411630(void* gff, void*, void* st, const char* label);           // 00411630 GetFieldByLabel [0291]
U __fastcall gff_list_004118c0(void* gff, void*, void* out, void* parent, const char* label); // 004118C0 GetList [0291]
U __fastcall gff_list_count_00411940(void* gff, void*, void* list);                      // 00411940 GetListCount [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index); // 00411990 GetListElement [0291]
U __fastcall gui_load_gff_struct_00411a10(void* gff, void*, void* out, void* parent, const char* label); // 00411A10 [0292]
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* st, const char* label, U* ok, unsigned char def); // 00411A60 [0292]
char __fastcall kotor_fastcall_00411ad0(void* gff, void*, void* st, const char* label, U* ok, char def); // 00411AD0 ReadFieldCHAR [closeB_load_a]
unsigned short __fastcall recovered_gff_word_00411b40(void* gff, void*, void* st, const char* label, U* ok, unsigned short def); // 00411B40 (0886)
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* st, const char* label, U* ok, U def); // 00411C20 [0292]
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* st, const char* label, U* ok, int def); // 00411C90 [0292]
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* st, const char* label, U* ok, float def); // 00411D00 [0292]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* st, const char* label, U* ok, void* def); // 00411EC0 [0292]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);          // 005E5A90 CExoString(char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                             // 005E5C20 ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);               // 005E5C50 CExoString::operator= [0199]
void* __fastcall GuiAssign005e5140(void* s, void*, const char* text);                     // 005E5140 CExoString::operator=(char*) [0093]
void* __fastcall resource_append_005e9dc0(void* list, void*, void* data);                 // 005E9DC0 CExoLinkedListInternal::AddTail [0264]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);         // 00503E40 CGameEffect(int) (0886)
void __fastcall kotor_fastcall_005043a0(void* effect, void*, void* gff, void* st);        // 005043A0 LoadGameEffect [closeB_load_b]
U __fastcall kotor_fastcall_004d2450(void* attack, void*, void* gff, void* st);           // 004D2450 CSWSCombatAttackData::LoadData [closeB_load_a]
void __fastcall w5_combat_attack_clear_004d3010(void* attack, void*);                     // 004D3010 ClearAttackData [1206-07 w5t3]
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);            // 004D69F0 CSWCCMessageData() [w1_native_b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, U index, U value);    // 004D76B0 CSWCCMessageData::SetInteger [1017]
void __fastcall kotor_fastcall_00405080(void* list, void*, int size);                     // 00405080 CExoArrayList<CExoString>::Allocate [0603]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, int count, void* destroy); // 006FA852 eh_vector_destructor_iterator [0186]
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                  // 004D7540 CScriptEvent() [1017]
U __fastcall kotor_fastcall_004d79b0(void* event, void*, void* gff, void* st);            // 004D79B0 CScriptEvent::LoadEvent [0854 export]
U __fastcall recovered_object_add_action_004cea20(void* self, void*, U type, U group,
    U t0, void* v0, U t1, void* v1, U t2, void* v2, U t3, void* v3, U t4, void* v4, U t5, void* v5,
    U t6, void* v6, U t7, void* v7, U t8, void* v8, U t9, void* v9, U t10, void* v10, U t11, void* v11,
    U t12, void* v12);                                                                     // 004CEA20 CSWSObject::AddAction RET 70h [w1_native_b7]

void* __fastcall kotor_fastcall_00412380(void* gff, void*, void* st, void* dest, U size, const char* label, U* ok, void* def);
U __fastcall kotor_fastcall_005d1ef0(void* stack, void*, void* gff, void* st);
U __fastcall kotor_fastcall_005d26f0(void* internal, void*, void* out, void* gff, void* st);
U __fastcall kotor_fastcall_005d1100(void* vm, void*, void* out, void* gff, void* st);
void* __fastcall kotor_fastcall_004d57f0(void* attack, void*);
U __fastcall kotor_fastcall_004d6b10(void* data, void*, void* gff, void* st);
U __fastcall kotor_fastcall_004af1c0(void* bag, void*, void* gff, void* st);
void* __fastcall kotor_fastcall_004af130(void* data, void*);
U __fastcall kotor_fastcall_004ee2e0(void* data, void*, void* gff, void* st);
U __fastcall kotor_fastcall_004b0290(void* node, void*, void* gff, void* st);
U __fastcall kotor_fastcall_004b0a00(void* master, void*, void* gff, void* root);
void __fastcall loadgame_object_load_action_queue_tail_004ced20(void* object, void*, void* gff, void* list);
void __fastcall kotor_fastcall_004cecb0(void* object, void*, void* gff, void* root);
}

namespace {
inline void* vm_global() { return ptr(frame_control::message_manager_007a3a00); }
struct ExoString { U data, length; };                                                     // CExoString (8 bytes)
}

// ===================================================================================== 00412380
void* __fastcall kotor_fastcall_00412380(void* gff, void*, void* st, void* dest, U size, const char* label, U* ok, void* def) {
    if (st == nullptr || label == nullptr || dest == nullptr) { *ok = 0; return def; }   // 00412383..004123A3, 0041243C..0041244D
    kotor_q05::FieldRecord* const field = static_cast<kotor_q05::CResGFF*>(gff)->GetField(
        static_cast<kotor_q05::StructHandle*>(st), gff_label_00411630(gff, nullptr, st, label)); // 004123A9..004123B4
    if (field == nullptr || *reinterpret_cast<const U*>(field) != 0xd) { *ok = 0; return def; } // 004123B9..004123C0, 00412428
    U avail = 0;                                                                         // [esp+10h] (structure slot)
    const U data = static_cast<kotor_q05::CResGFF*>(gff)->GetDataField(field, &avail);   // 004123C2..004123CA
    if (data == 0 || avail < 4) { *ok = 0; return def; }                                 // 004123CF..004123DA, 00412414
    U len = rd(data);                                                                    // 004123DC
    if (avail < len + 4) { *ok = 0; return def; }                                        // 004123DE..004123E7 (jb), 0041242C
    *ok = 1;                                                                             // 004123E9
    if (len >= size) len = size;                                                         // 004123EF..004123F7 (jb skips)
    std::memcpy(dest, ptr(data + 4), len);                                               // 004123F9..0041240A rep movsd/movsb
    return dest;                                                                         // 0041240E
}

// ===================================================================================== 005D1740 (file-local)
namespace {
void* vm_script_construct_005d1740(U s) {
    kotor_movie::string_element_construct_005b3190(ptr(s + 0x18), nullptr);            // 005D175A..005D1764
    GuiAssign005e5140(ptr(s + 0x18), nullptr, "");                                      // 005D176B..005D1776 ([0073D71C])
    for (U o : {0x0u, 0x4u, 0x8u, 0xcu, 0x10u, 0x14u, 0x20u, 0x24u}) wr(s + o, 0);       // 005D177F..005D1793
    return ptr(s);
}
}

// ===================================================================================== 005D1EF0
U __fastcall kotor_fastcall_005d1ef0(void* stack, void*, void* gff, void* st) {
    const U k = word(stack);
    U ok = 0;                                                                            // [esp+4Ch] (structure slot)
    wr(k + 4, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "BasePointer", &ok, 0)));  // 005D1F13..005D1F38
    wr(k, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "StackPointer", &ok, 0)));     // 005D1F29..005D1F4F
    const I total = gui_load_gff_int_00411c90(gff, nullptr, st, "TotalSize", &ok, 0);   // 005D1F40..005D1F51
    wr(k + 8, static_cast<U>(total));                                                    // 005D1F58
    if (total <= 0) return 1;                                                            // 005D1F56..005D1F5B (jle)
    wr(k + 0xc, word(live_operator_new(static_cast<U>(total))));                         // 005D1F61..005D1F67 type bytes
    wr(k + 0x10, word(live_operator_new(rd(k + 8) << 2)));                               // 005D1F6A..005D1F86 values
    alignas(4) unsigned char list[0x14] = {};                                            // [esp+24h]
    gff_list_004118c0(gff, nullptr, list, st, "Stack");                                  // 005D1F79..005D1F89 (result unchecked)
    alignas(4) unsigned char element[4] = {};                                            // gff argument slot
    for (I i = 0; i < static_cast<I>(rd(k)); ++i) {                                      // 005D1F8E..005D1F94, 005D2106..005D210B (jl)
        gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i));       // 005D1FA0..005D1FAD
        U eok = 0;
        wr8(rd(k + 0xc) + static_cast<U>(i),
            static_cast<U8>(kotor_fastcall_00411ad0(gff, nullptr, element, "Type", &eok, 0))); // 005D1FB2..005D1FCE
        const I type = static_cast<signed char>(rd8(rd(k + 0xc) + static_cast<U>(i)));   // 005D1FD1..005D1FD4 movsx
        const U slot = rd(k + 0x10) + static_cast<U>(i) * 4;
        switch (type) {                                                                  // 005D1FD8..005D1FE4 (table 005D212C)
        case 3:                                                                          // 005D1FEB integer
            wr(slot, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, element, "Value", &eok, 0)));
            break;
        case 4: {                                                                        // 005D200D float
            const float f = gui_load_gff_float_00411d00(gff, nullptr, element, "Value", &eok, 0.0f);
            wrf(slot, f);                                                                // 005D2024..005D202F
            break;
        }
        case 5: {                                                                        // 005D2037 string
            void* const memory = live_operator_new(8);                                   // 005D2037..005D2039
            U string = 0;                                                                // ebx (0 on null)
            if (memory != nullptr) string = word(kotor_movie::string_element_construct_005b3190(memory, nullptr)); // 005D2045..005D2054
            ExoString def, out;
            frame_LogStringCtor_005e5a90(&def, nullptr, "");                             // 005D2056..005D2067 ([0073D71C])
            void* const text = gui_load_gff_string_00411ec0(gff, nullptr, &out, element, "Value", &eok, &def); // 005D206C..005D208F
            async_assign_string_005e5c50(ptr(string), nullptr, text);                    // 005D2094..005D209C (null unchecked)
            frame_LogStringDtor_005e5c20(&out, nullptr);                                 // 005D20A1..005D20AA
            frame_LogStringDtor_005e5c20(&def, nullptr);                                 // 005D20AF..005D20BB
            wr(rd(k + 0x10) + static_cast<U>(i) * 4, string);                            // 005D20C0..005D20C3
            break;
        }
        case 6:                                                                          // 005D20C8 object
            wr(slot, gui_load_gff_dword_00411c20(gff, nullptr, element, "Value", &eok, 0));
            break;
        default: {                                                                       // 005D20E7..005D2103 engine structure
            const U implementer = rd(rd(k + 0x14) + 0x3d4);
            vcall<U>(implementer, 0x28, static_cast<int>(type - 0x10), ptr(slot), gff, static_cast<void*>(element));
            break;
        }
        }
    }
    return 1;                                                                            // 005D2118
}

// ===================================================================================== 005D26F0 / 005D1100
U __fastcall kotor_fastcall_005d26f0(void* internal, void*, void* out, void* gff, void* st) {
    void* const memory = live_operator_new(0x28);                                        // 005D270B..005D2711
    void* const script = memory != nullptr ? vm_script_construct_005d1740(word(memory)) : nullptr; // 005D2719..005D2730
    const U o = word(out);
    wr(o, word(script));                                                                 // 005D2740
    U ok = 0;                                                                            // [esp+10h] local
    wr(rd(o) + 0x14, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "CodeSize", &ok, 0))); // 005D273F..005D275E
    const U size = rd(rd(o) + 0x14);                                                     // 005D2761..005D2763
    if (size != 0) wr(rd(o) + 0x10, word(live_operator_new(size)));                      // 005D2766..005D2775
    else wr(rd(o) + 0x10, 0);                                                            // 005D277A
    kotor_fastcall_00412380(gff, nullptr, st, ptr(rd(rd(o) + 0x10)), rd(rd(o) + 0x14), "Code", &ok, nullptr); // 005D277D..005D2795
    wr(rd(o) + 0x24, gui_load_gff_dword_00411c20(gff, nullptr, st, "CRC", &ok, 0));     // 005D279A..005D27AF
    if (ok == 0) wr(rd(o) + 0x24, 0);                                                    // 005D27B2..005D27BA
    wr(rd(o) + 0x20, 1);                                                                 // 005D27C6..005D27D0
    wr(rd(o) + 8, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "InstructionPtr", &ok, 0))); // 005D27BF..005D27E9
    wr(rd(o) + 0xc, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "SecondaryPtr", &ok, 0))); // 005D27DD..005D27F6
    {
        ExoString def, text_out;
        frame_LogStringCtor_005e5a90(&def, nullptr, "");                                 // 005D27F9..005D2802 ([0073D71C])
        void* const text = gui_load_gff_string_00411ec0(gff, nullptr, &text_out, st, "Name", &ok, &def); // 005D2807..005D2822
        async_assign_string_005e5c50(ptr(rd(o) + 0x18), nullptr, text);                  // 005D2827..005D2832
        frame_LogStringDtor_005e5c20(&text_out, nullptr);                                // 005D2837..005D283F
        frame_LogStringDtor_005e5c20(&def, nullptr);                                     // 005D2844..005D2850
    }
    wr(rd(o) + 4, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "StackSize", &ok, 0))); // 005D2855..005D2875
    alignas(4) unsigned char stack_struct[4] = {};                                       // [esp+14h]
    gui_load_gff_struct_00411a10(gff, nullptr, stack_struct, st, "Stack");               // 005D286B..005D287B (result unchecked)
    void* const stack_memory = live_operator_new(0x18);                                  // 005D2880..005D2882
    if (stack_memory != nullptr) {                                                       // 005D288A..005D289C (+14h not cleared)
        const U s = word(stack_memory);
        wr(s, 0); wr(s + 4, 0); wr(s + 8, 0); wr(s + 0xc, 0); wr(s + 0x10, 0);
    }
    wr(rd(o), word(stack_memory));                                                       // 005D28A3..005D28A5
    wr(rd(rd(o)) + 0x14, word(internal));                                                // 005D28A7..005D28AF stack->VM internal
    kotor_fastcall_005d1ef0(ptr(rd(rd(o))), nullptr, gff, stack_struct);                 // 005D28B2..005D28BC
    return 1;                                                                            // 005D28C7 (ebx = 1)
}
U __fastcall kotor_fastcall_005d1100(void* vm, void*, void* out, void* gff, void* st) {
    return kotor_fastcall_005d26f0(ptr(rd(word(vm))), nullptr, out, gff, st);            // 005D1100..005D1102 mov ecx,[ecx]; jmp
}

// ===================================================================================== 004D57F0
void* __fastcall kotor_fastcall_004d57f0(void* attack, void*) {
    const U a = word(attack);
    wr(a + 0x68, 0); wr(a + 0x6c, 0); wr(a + 0x70, 0);                                   // 004D5814..004D581A
    kotor_movie::string_element_construct_005b3190(ptr(a + 0x78), nullptr);             // 004D580D..004D581D
    kotor_movie::string_element_construct_005b3190(ptr(a + 0x80), nullptr);             // 004D5822..004D582C
    for (U o : {0x8cu, 0x90u, 0x88u, 0x98u, 0x9cu, 0x94u, 0xa4u, 0xa8u, 0xa0u, 0xb0u, 0xb4u, 0xacu}) wr(a + o, 0); // 004D5831..004D5873
    for (U o = 0xc0; o <= 0xc7; ++o) wr8(a + o, 0);                                      // 004D5879..004D58A3
    wr(a + 0xc8, 0);                                                                     // 004D58A9
    for (U o = 0xcc; o <= 0xd5; ++o) wr8(a + o, 0);                                      // 004D58AF..004D58E5
    wr(a + 0xd8, 0); wr(a + 0xdc, 0); wr8(a + 0xe0, 0); wr(a + 0xe4, 0); wr8(a + 0xe8, 0); wr(a + 0xec, 0); // 004D58EB..004D5909
    wr8(a + 0xf0, 0); wr8(a + 0xf1, 0);                                                  // 004D590F..004D5915
    for (U o = 0xf4; o <= 0xfc; ++o) wr8(a + o, 0);                                      // 004D591B..004D594B (+FDh untouched)
    for (U o = 0xfe; o <= 0x109; ++o) wr8(a + o, 0);                                     // 004D5956..004D5998
    std::memset(ptr(a + 0x10a), 0xff, 0x1e);                                             // 004D599E..004D59AE rep stosd x7 + stosw
    wr(a + 0x128, 0x7f000000);                                                           // 004D59B0
    for (U o = 0x12c; o <= 0x133; ++o) wr8(a + o, 0);                                    // 004D59BA..004D59E4
    wr8(a + 0x134, 0xff);                                                                // 004D59EA
    wr(a + 0x138, 0); wr(a + 0x13c, 0);                                                  // 004D59F1..004D59F7
    for (U o = 0x140; o <= 0x148; ++o) wr8(a + o, 0);                                    // 004D59FF..004D5A2F
    w5_combat_attack_clear_004d3010(attack, nullptr);                                    // 004D59FD..004D5A35
    return attack;                                                                       // 004D5A3F
}

// ===================================================================================== 004D6B10
namespace {
// CExoArrayList<4-byte>::SetSize(index + 1) inline (004D6C1F..004D6C83 / 004D6D0F..004D6D73)
void grow4(U list, I index) {
    if (index < static_cast<I>(rd(list + 4))) return;                                    // cmp ebp,[size]; jl
    const U n = static_cast<U>(index + 1);
    if (n == 0) {
        const U data = rd(list);
        if (data != 0) { live_free(ptr(data)); wr(list, 0); wr(list + 8, 0); }
    } else {
        const U old = rd(list);
        wr(list + 8, n);
        wr(list, word(live_operator_new(static_cast<U>(index) * 4 + 4)));
        for (I j = 0; j < static_cast<I>(rd(list + 4)); ++j) wr(rd(list) + static_cast<U>(j) * 4, rd(old + static_cast<U>(j) * 4));
        if (old != 0) live_free(ptr(old));
    }
    wr(list + 4, n);
}
}
U __fastcall kotor_fastcall_004d6b10(void* data, void*, void* gff, void* st) {
    const U d = word(data);
    U ok = 0;
    alignas(4) unsigned char list[0x14] = {}, element[4] = {};
    wr8(d, gui_load_gff_byte_00411a60(gff, nullptr, st, "Type", &ok, 0));               // 004D6B34..004D6B57
    if (gff_list_004118c0(gff, nullptr, list, st, "IntList") == 1) {                     // 004D6B4A..004D6B61
        for (U i = 0; i < gff_list_count_00411940(gff, nullptr, list); ++i) {            // 004D6B63..004D6BB7
            gff_list_element_00411990(gff, nullptr, element, list, i);                   // 004D6B75..004D6B82
            const int v = gui_load_gff_int_00411c90(gff, nullptr, element, "IntegerValue", &ok, 0); // 004D6B87..004D6B9A
            recovered_event_integer_004d76b0(data, nullptr, i, static_cast<U>(v));       // 004D6B9F..004D6BA3
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "FloatList") == 1) {                   // 004D6BB9..004D6BCE
        for (U i = 0; i < gff_list_count_00411940(gff, nullptr, list); ++i) {            // 004D6BD4..004D6C9F
            gff_list_element_00411990(gff, nullptr, element, list, i);                   // 004D6BF0..004D6BFD
            const float f = gui_load_gff_float_00411d00(gff, nullptr, element, "FloatValue", &ok, 0.0f); // 004D6C02..004D6C1B
            grow4(d + 0x10, static_cast<I>(i));                                          // 004D6C1F..004D6C83
            wrf(rd(d + 0x10) + i * 4, f);                                                // 004D6C86..004D6C91
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "ObjectIDList") == 1) {                // 004D6CA9..004D6CBE
        for (U i = 0; i < gff_list_count_00411940(gff, nullptr, list); ++i) {            // 004D6CC4..004D6D8F
            gff_list_element_00411990(gff, nullptr, element, list, i);                   // 004D6CE0..004D6CED
            const U v = gui_load_gff_dword_00411c20(gff, nullptr, element, "ObjectValue", &ok, 0); // 004D6CF2..004D6D0B
            grow4(d + 0x28, static_cast<I>(i));                                          // 004D6D0F..004D6D73
            wr(rd(d + 0x28) + i * 4, v);                                                 // 004D6D76..004D6D7D
        }
    }
    if (gff_list_004118c0(gff, nullptr, list, st, "StringList") == 1) {                  // 004D6D99..004D6DAE
        const U strings = d + 0x1c;                                                      // ebx
        for (U i = 0; i < gff_list_count_00411940(gff, nullptr, list); ++i) {            // 004D6DB4..004D6EB2
            gff_list_element_00411990(gff, nullptr, element, list, i);                   // 004D6DD0..004D6DDD
            ExoString def, out;
            frame_LogStringCtor_005e5a90(&def, nullptr, "");                             // 004D6DE2..004D6DEB ([0073D71C])
            void* const text = gui_load_gff_string_00411ec0(gff, nullptr, &out, element, "StringValue", &ok, &def); // 004D6DF0..004D6E13
            if (static_cast<I>(i) >= static_cast<I>(rd(d + 0x20))) {                     // 004D6E1C..004D6E24 (jl)
                const U n = i + 1;
                if (n == 0) {                                                            // 004D6E29..004D6E2B
                    const U array = rd(strings);
                    if (array != 0) {                                                    // 004D6E2D..004D6E31
                        c2da_destroy_array_006fa852(ptr(array), 8, static_cast<int>(rd(array - 4)),
                                                    reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 004D6E33..004D6E46
                        live_free(ptr(array - 4));                                       // 004D6E4B..004D6E50
                        wr(strings, 0); wr(strings + 8, 0);                              // 004D6E58..004D6E5E
                    }
                } else {
                    kotor_fastcall_00405080(ptr(strings), nullptr, static_cast<int>(n)); // 004D6E67..004D6E6A
                }
                wr(strings + 4, n);                                                      // 004D6E6F..004D6E72
            }
            async_assign_string_005e5c50(ptr(rd(strings) + i * 8), nullptr, text);      // 004D6E75..004D6E7F
            frame_LogStringDtor_005e5c20(&out, nullptr);                                 // 004D6E84..004D6E8D
            frame_LogStringDtor_005e5c20(&def, nullptr);                                 // 004D6E92..004D6E9E
        }
    }
    return 1;                                                                            // 004D6EBF
}

// ===================================================================================== 004AF1C0 / 004AF130 / 004EE2E0
U __fastcall kotor_fastcall_004af1c0(void* bag, void*, void* gff, void* st) {
    const U b = word(bag);
    U ok = 0;
    wr(b, gui_load_gff_dword_00411c20(gff, nullptr, st, "BodyBagId", &ok, 0));          // 004AF1CB..004AF1F0
    wrf(b + 4, gui_load_gff_float_00411d00(gff, nullptr, st, "PositionX", &ok, 0.0f));   // 004AF1E1..004AF1F7
    wrf(b + 8, gui_load_gff_float_00411d00(gff, nullptr, st, "PositionY", &ok, 0.0f));   // 004AF1FA..004AF20E
    wrf(b + 0xc, gui_load_gff_float_00411d00(gff, nullptr, st, "PositionZ", &ok, 0.0f)); // 004AF211..004AF225
    return 1;
}
void* __fastcall kotor_fastcall_004af130(void* data, void*) {
    const U d = word(data);
    wr(d + 0x10, 0); wr(d + 0x14, 0); wr(d + 0x18, 0);                                   // 004AF133..004AF13E
    kotor_movie::string_element_construct_005b3190(ptr(d + 0x1c), nullptr);             // 004AF135..004AF141
    return data;
}
U __fastcall kotor_fastcall_004ee2e0(void* data, void*, void* gff, void* st) {
    const U d = word(data);
    U ok = 0;                                                                            // [esp+34h] (gff slot)
    wr(d, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, st, "SpellId", &ok, 0))); // 004EE303..004EE328
    wr(d + 4, gui_load_gff_dword_00411c20(gff, nullptr, st, "CasterId", &ok, 0));       // 004EE319..004EE33E
    wr(d + 8, gui_load_gff_dword_00411c20(gff, nullptr, st, "TargetId", &ok, 0));       // 004EE32F..004EE348
    wr(d + 0x24, gui_load_gff_dword_00411c20(gff, nullptr, st, "AreaId", &ok, 0));      // 004EE346..004EE36C
    wr(d + 0xc, gui_load_gff_dword_00411c20(gff, nullptr, st, "ItemId", &ok, 0));       // 004EE35D..004EE37D
    {
        ExoString def, out;
        frame_LogStringCtor_005e5a90(&def, nullptr, "");                                 // 004EE374..004EE380 ([0073D71C])
        void* const text = gui_load_gff_string_00411ec0(gff, nullptr, &out, st, "Script", &ok, &def); // 004EE385..004EE3A4
        async_assign_string_005e5c50(ptr(d + 0x1c), nullptr, text);                      // 004EE3A9..004EE3B2
        frame_LogStringDtor_005e5c20(&out, nullptr);                                     // 004EE3B7..004EE3C0
        frame_LogStringDtor_005e5c20(&def, nullptr);                                     // 004EE3C5..004EE3D1
    }
    wrf(d + 0x10, gui_load_gff_float_00411d00(gff, nullptr, st, "TargetPosX", &ok, 0.0f)); // 004EE3D6..004EE3EA
    wrf(d + 0x14, gui_load_gff_float_00411d00(gff, nullptr, st, "TargetPosY", &ok, 0.0f)); // 004EE3ED..004EE401
    wrf(d + 0x18, gui_load_gff_float_00411d00(gff, nullptr, st, "TargetPosZ", &ok, 0.0f)); // 004EE404..004EE418
    return 1;
}

// ===================================================================================== 004B0290
U __fastcall kotor_fastcall_004b0290(void* node, void*, void* gff, void* st) {
    const U n = word(node);
    U ok = 0;                                                                            // gff argument slot
    wr(n, gui_load_gff_dword_00411c20(gff, nullptr, st, "Day", &ok, 0));                // 004B02B4..004B02D7
    wr(n + 4, gui_load_gff_dword_00411c20(gff, nullptr, st, "Time", &ok, 0));           // 004B02C9..004B02EC
    wr(n + 0xc, gui_load_gff_dword_00411c20(gff, nullptr, st, "ObjectId", &ok, 0));     // 004B02DE..004B02F5
    wr(n + 8, gui_load_gff_dword_00411c20(gff, nullptr, st, "CallerId", &ok, 0));       // 004B02F4..004B0318
    const U id = gui_load_gff_dword_00411c20(gff, nullptr, st, "EventId", &ok, 0);       // 004B030A..004B031B
    wr(n + 0x10, id);                                                                    // 004B0320
    alignas(4) unsigned char data[4] = {};                                               // structure argument slot
    // jump table 004B0714 indexed by byte table 004B0750[id-1] (ids 1..27; anything else -> 004B06F8)
    switch (id) {
    case 5: case 14: {                                                                   // 004B033B effect
        void* const memory = live_operator_new(0x8c);
        void* const effect = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 004B0348..004B035F
        wr(n + 0x14, word(effect));                                                      // 004B036B
        if (effect == nullptr) break;                                                    // 004B0361..004B036E
        gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData");               // 004B0374..004B0381 (unchecked)
        kotor_fastcall_005043a0(ptr(rd(n + 0x14)), nullptr, gff, data);                  // 004B0386..004B038F
        break;
    }
    case 2: case 3: case 6: case 7: case 11: case 12: case 13: case 16:                 // 004B0399
        wr(n + 0x14, 0);
        break;
    case 15: case 21: {                                                                  // 004B03A1 combat attack data
        wr(n + 0x14, 0);                                                                 // 004B03AE
        if (gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData") == 0) break; // 004B03A1..004B03B8
        void* const memory = live_operator_new(0x14c);                                   // 004B03BE..004B03C3
        void* const attack = memory != nullptr ? kotor_fastcall_004d57f0(memory, nullptr) : nullptr; // 004B03CB..004B03E4
        wr(n + 0x14, word(attack));                                                      // 004B03F6
        kotor_fastcall_004d2450(attack, nullptr, gff, data);                             // 004B03E6..004B03F9 (null unchecked)
        break;
    }
    case 4:                                                                              // 004B0403 byte value
        wr(n + 0x14, 0);
        if (gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData") == 0) break; // 004B0403..004B041A
        wr(n + 0x14, gui_load_gff_byte_00411a60(gff, nullptr, data, "Value", &ok, 0));  // 004B0420..004B043A movzx
        break;
    case 20:                                                                             // 004B0442 dword value
        wr(n + 0x14, 0);
        if (gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData") == 0) break; // 004B0442..004B0459
        wr(n + 0x14, gui_load_gff_dword_00411c20(gff, nullptr, data, "Value", &ok, 0)); // 004B045F..004B0476 -> 004B06F5
        break;
    case 22: {                                                                           // 004B047B message data
        wr(n + 0x14, 0);
        if (gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData") == 0) break; // 004B047B..004B0492
        void* const memory = live_operator_new(0x34);                                    // 004B0498..004B049A
        void* const message = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr; // 004B04A2..004B04BB
        wr(n + 0x14, word(message));                                                     // 004B04CD
        kotor_fastcall_004d6b10(message, nullptr, gff, data);                            // 004B04BD..004B04D0 (null unchecked)
        break;
    }
    case 10: case 24: case 26: {                                                         // 004B04DA / 004B0612 / 004B066F script event
        void* const memory = live_operator_new(0x34);
        void* const event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
        wr(n + 0x14, word(event));
        if (event == nullptr) break;                                                     // 004B04FF..004B050C
        gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData");               // 004B0512..004B051F (unchecked)
        kotor_fastcall_004d79b0(ptr(rd(n + 0x14)), nullptr, gff, data);                  // 004B0524..004B052D
        break;
    }
    case 18:                                                                             // 004B0537
        return 0;
    case 17: {                                                                           // 004B053E body bag
        void* const memory = live_operator_new(0x10);
        if (memory != nullptr) { const U m = word(memory); wr(m + 4, 0); wr(m + 8, 0); wr(m + 0xc, 0); } // 004B0548..004B0552
        wr(n + 0x14, word(memory));                                                      // 004B055B
        if (memory == nullptr) break;                                                    // 004B0559..004B055E
        gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData");               // 004B0564..004B0571 (unchecked)
        kotor_fastcall_004af1c0(ptr(rd(n + 0x14)), nullptr, gff, data);                  // 004B0576..004B057F
        break;
    }
    case 8: {                                                                            // 004B0589 spell script data
        void* const memory = live_operator_new(0x2c);
        void* const spell = memory != nullptr ? kotor_fastcall_004af130(memory, nullptr) : nullptr; // 004B0593..004B05AC
        wr(n + 0x14, word(spell));                                                       // 004B05B8
        if (spell == nullptr) break;                                                     // 004B05AE..004B05BB
        gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData");               // 004B05C1..004B05CE (unchecked)
        kotor_fastcall_004ee2e0(ptr(rd(n + 0x14)), nullptr, gff, data);                  // 004B05D3..004B05DC
        break;
    }
    case 1:                                                                              // 004B05E6 script situation
        gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData");               // 004B05E6..004B05F3 (unchecked)
        kotor_fastcall_005d1100(vm_global(), nullptr, ptr(n + 0x14), gff, data);         // 004B05F8..004B0608
        break;
    case 9: case 27:                                                                     // 004B06C5 int value
        wr(n + 0x14, 0);
        if (gui_load_gff_struct_00411a10(gff, nullptr, data, st, "EventData") == 0) break; // 004B06C5..004B06DC
        wr(n + 0x14, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, data, "Value", &ok, 0))); // 004B06DE..004B06F5
        break;
    default:                                                                             // 0, 19, 23, 25, >= 28: 004B06F8
        break;
    }
    return 1;                                                                            // 004B06F8
}

// ===================================================================================== 004B0A00
U __fastcall kotor_fastcall_004b0a00(void* master, void*, void* gff, void* root) {
    alignas(4) unsigned char list[0x14] = {};                                            // [esp+0Ch]
    if (gff_list_004118c0(gff, nullptr, list, root, "EventQueue") == 0) return 1;        // 004B0A0E..004B0A24
    if (gff_list_count_00411940(gff, nullptr, list) == 0) return 1;                      // 004B0A26..004B0A36 (jbe)
    alignas(4) unsigned char element[4] = {};                                            // root argument slot
    U i = 0;                                                                             // ebx
    do {
        void* const node = live_operator_new(0x18);                                      // 004B0A40..004B0A4A (not constructed)
        gff_list_element_00411990(gff, nullptr, element, list, i);                       // 004B0A4C..004B0A59
        if (kotor_fastcall_004b0290(node, nullptr, gff, element) != 0)                   // 004B0A5E..004B0A6E
            resource_append_005e9dc0(ptr(rd(word(master) + 0x54)), nullptr, node);       // 004B0A70..004B0A73
        else
            live_free(node);                                                             // 004B0A7A..004B0A7F
        ++i;                                                                             // 004B0A89
    } while (i < gff_list_count_00411940(gff, nullptr, list));                           // 004B0A82..004B0A91 (jb)
    return 1;                                                                            // 004B0A96
}

// ===================================================================================== 004CECB0
// Frame offsets are relative to the original's ESP after the prologue (B): +10 status, +14 parameter struct,
// +18 action struct, +1C ActionId, +20 NumParams, +24 action index, +28 value struct, +2C this, +30 GroupActionId,
// +34 loaded script, +38 values[13], +6C default CExoString, +74 raw types[13], +A8 new CExoString, +AC read
// CExoString, +B4 "ActionList" CResList, +C8 free codes[13] (not cleared per action), +FC "Paramaters" CResList.
namespace {
struct ActionFrame { alignas(8) unsigned char b[0x120]; U at(U o) { return word(b) + o; } };
void load_actions(ActionFrame& f, U object, void* gff, U list) {
    U index = 0;
    wr(f.at(0x24), index);                                                               // 004CED07 / 004CF10E
    do {
        wr(f.at(0x2c), object);
        index = rd(f.at(0x24));
        if (gff_list_element_00411990(gff, nullptr, ptr(f.at(0x18)), ptr(list), index) != 0) { // 004CED20..004CED37
            std::memset(ptr(f.at(0x38)), 0, 13 * 4);                                     // 004CED3D..004CED49
            std::memset(ptr(f.at(0x74)), 0, 13 * 4);                                     // 004CED50..004CED59
            void* const st = ptr(f.at(0x18));
            U* const ok = reinterpret_cast<U*>(f.b + 0x10);
            wr(f.at(0x1c), gui_load_gff_dword_00411c20(gff, nullptr, st, "ActionId", ok, 0)); // 004CED4B..004CED7F
            wr(f.at(0x30), recovered_gff_word_00411b40(gff, nullptr, st, "GroupActionId", ok, 0)); // 004CED6C..004CED8E
            const U count = recovered_gff_word_00411b40(gff, nullptr, st, "NumParams", ok, 0); // 004CED88..004CED9F
            wr(f.at(0x20), count);                                                       // 004CEDA9
            if ((count & 0xffff) != 0) {                                                 // 004CEDA6..004CEDAD
                gff_list_004118c0(gff, nullptr, ptr(f.at(0xfc)), st, "Paramaters");      // 004CEDB3..004CEDC7 (unchecked)
                for (U i = 0; i < (rd(f.at(0x20)) & 0xffff); ++i) {                      // 004CEDCC..004CEDD3, 004CEFC2..004CEFCA
                    gff_list_element_00411990(gff, nullptr, ptr(f.at(0x14)), ptr(f.at(0xfc)), i); // 004CEDE0..004CEDF0
                    void* const pst = ptr(f.at(0x14));
                    const U type = gui_load_gff_dword_00411c20(gff, nullptr, pst, "Type", ok, 0); // 004CEDF5..004CEE09
                    wr(f.at(0x74) + i * 4, type);                                        // 004CEE0E
                    switch (type) {                                                      // 004CEE12..004CEE1C (table 004CF138)
                    case 1: {                                                            // 004CEE23 int
                        void* const v = live_operator_new(4);
                        wr(word(v), static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, pst, "Value", ok, 0))); // 004CEE2D..004CEE47
                        wr(f.at(0x38) + i * 4, word(v)); wr(f.at(0xc8) + i * 4, 1);      // 004CEE49..004CEE4D
                        break;
                    }
                    case 2: {                                                            // 004CEE5D float
                        void* const v = live_operator_new(4);
                        wrf(word(v), gui_load_gff_float_00411d00(gff, nullptr, pst, "Value", ok, 0.0f)); // 004CEE67..004CEE81
                        wr(f.at(0x38) + i * 4, word(v)); wr(f.at(0xc8) + i * 4, 2);      // 004CEE83..004CEE87
                        break;
                    }
                    case 4: {                                                            // 004CEE97 string
                        void* const memory = live_operator_new(8);
                        wr(f.at(0xa8), word(memory));                                    // 004CEEA1
                        U string = 0;                                                    // edi (0 from the Type read)
                        if (memory != nullptr) string = word(kotor_movie::string_element_construct_005b3190(memory, nullptr)); // 004CEEA8..004CEEBA
                        frame_LogStringCtor_005e5a90(ptr(f.at(0x6c)), nullptr, "");      // 004CEEBC..004CEED0 ([0073D71C])
                        void* const text = gui_load_gff_string_00411ec0(gff, nullptr, ptr(f.at(0xac)), pst, "Value", ok,
                                                                        ptr(f.at(0x6c))); // 004CEED5..004CEEFE
                        async_assign_string_005e5c50(ptr(string), nullptr, text);        // 004CEF03..004CEF0E (null unchecked)
                        frame_LogStringDtor_005e5c20(ptr(f.at(0xac)), nullptr);          // 004CEF13..004CEF22
                        frame_LogStringDtor_005e5c20(ptr(f.at(0x6c)), nullptr);          // 004CEF27..004CEF36
                        wr(f.at(0x38) + i * 4, string); wr(f.at(0xc8) + i * 4, 4);       // 004CEF3B..004CEF3F
                        break;
                    }
                    case 3: {                                                            // 004CEF4C object id
                        void* const v = live_operator_new(4);
                        wr(word(v), gui_load_gff_dword_00411c20(gff, nullptr, pst, "Value", ok, 0)); // 004CEF56..004CEF70
                        wr(f.at(0x38) + i * 4, word(v)); wr(f.at(0xc8) + i * 4, 3);      // 004CEF72..004CEF76
                        break;
                    }
                    case 5:                                                              // 004CEF83 script situation
                        gui_load_gff_struct_00411a10(gff, nullptr, ptr(f.at(0x28)), pst, "Value"); // 004CEF83..004CEF94 (unchecked)
                        kotor_fastcall_005d1100(vm_global(), nullptr, ptr(f.at(0x34)), gff, ptr(f.at(0x28))); // 004CEF99..004CEFAA
                        wr(f.at(0x38) + i * 4, rd(f.at(0x34))); wr(f.at(0xc8) + i * 4, 5); // 004CEFAF..004CEFB7
                        break;
                    default:                                                             // 004CEFC2: value stays 0, code stale
                        break;
                    }
                }
            }
            auto t = [&](U k) { return rd(f.at(0x74) + k * 4); };
            auto v = [&](U k) { return ptr(rd(f.at(0x38) + k * 4)); };
            recovered_object_add_action_004cea20(ptr(rd(f.at(0x2c))), nullptr, rd(f.at(0x1c)), rd(f.at(0x30)),
                t(0), v(0), t(1), v(1), t(2), v(2), t(3), v(3), t(4), v(4), t(5), v(5), t(6), v(6),
                t(7), v(7), t(8), v(8), t(9), v(9), t(10), v(10), t(11), v(11), t(12), v(12)); // 004CEFD4..004CF0A3
            const U params = rd(f.at(0x20)) & 0xffff;                                    // 004CF0A8 movzx eax,di
            for (U i = 0; i < params; ++i) {                                             // 004CF0AB..004CF0FD
                const U value = rd(f.at(0x38) + i * 4);
                switch (rd(f.at(0xc8) + i * 4)) {                                        // table 004CF14C
                case 1: case 2: case 3:                                                  // 004CF0C9 / 004CF0D0 / 004CF0E9
                    live_free(ptr(value));
                    break;
                case 4:                                                                  // 004CF0D7
                    if (value != 0) { frame_LogStringDtor_005e5c20(ptr(value), nullptr); live_free(ptr(value)); }
                    break;
                default:                                                                 // 5 and others: kept
                    break;
                }
            }
        }
        index = rd(f.at(0x24)) + 1;                                                      // 004CF0FF..004CF10A
        wr(f.at(0x24), index);
    } while (index < gff_list_count_00411940(gff, nullptr, ptr(list)));                  // 004CF103..004CF119 (jb)
}
}
void __fastcall loadgame_object_load_action_queue_tail_004ced20(void* object, void*, void* gff, void* list) {
    ActionFrame f;
    std::memset(f.b, 0, sizeof f.b);                                                     // uninitialised in the original
    load_actions(f, word(object), gff, word(list));                                      // the 0886 caller's list (its s+B4h)
}
void __fastcall kotor_fastcall_004cecb0(void* object, void*, void* gff, void* root) {
    ActionFrame f;
    std::memset(f.b, 0, sizeof f.b);
    wr(f.at(0x2c), word(object));                                                        // 004CECDF
    if (gff_list_004118c0(gff, nullptr, ptr(f.at(0xb4)), root, "ActionList") == 0) return; // 004CECDA..004CECF5
    if (gff_list_count_00411940(gff, nullptr, ptr(f.at(0xb4))) == 0) return;             // 004CECFB..004CED12 (jbe)
    load_actions(f, word(object), gff, f.at(0xb4));                                      // 004CED20..
}
