// W2 G08 native exports. Original ABI thiscall + callee cleanup, exported as
// __fastcall with an unused EDX word (project convention).
#include "46c78495cb-w2_entry_native.hpp"
using namespace w2::native;
namespace e = w2::entry;
namespace { using Scratch = unsigned char[0x200]; }

// ===== closeA: CSWSScriptVarTable create path and CExoArrayList<CSWSScriptVar> growth =====
// CSWSScriptVar (10h): +0 CExoString name (text, length), +8 type, +C data.
namespace { struct ScriptVar { Word text, length, type, data; }; static_assert(sizeof(ScriptVar) == 16, "CSWSScriptVar"); }
extern "C" {
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*);                         // CScriptLocation::CScriptLocation (claude_vm_actions_queries)

// 004C6740 CExoArrayList<CSWSScriptVar>::Allocate(int capacity) (thiscall, RET 4). list: +0 data, +4 count, +8 capacity.
void __fastcall kotor_fastcall_004c6740(void* list, void*, Word capacity) {
    Mem m;
    const Word l = word(list);
    const Word old = m.read(l);                                                    // 004C6762 / [esp+10h]
    m.write(l + 8, capacity);                                                      // 004C676F
    const Word block = word(live_operator_new(capacity * 0x10 + 4));               // 004C675E..004C6772 new[] cookie + elements
    Word data = 0;                                                                 // 004C677E ebp = 0 when new returns null
    if (block != 0) {                                                              // 004C6780..004C6786
        m.write(block, capacity);                                                  // 004C6799 element count cookie
        data = block + 4;                                                          // 004C6793
        for (Word i = 0; i < capacity; ++i) {                                      // 004C679B eh vector ctor, 0059EA90
            m.write(data + i * 0x10, 0); m.write(data + i * 0x10 + 4, 0);          // 005B3190 CExoString(): two zero stores
        }
    }
    m.write(l, data);                                                              // 004C67A3
    for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(m.read(l + 4)); ++i) { // 004C67A7..004C67ED (count re-read)
        const Word to = m.read(l) + i * 0x10, from = old + i * 0x10;               // data re-read each pass (004C67C7)
        async_assign_string_005e5c50(ptr(to), nullptr, ptr(from));                 // 004C67CE..004C67D4 operator=
        m.write(to + 8, m.read(from + 8));                                         // 004C67D9..004C67DC type
        m.write(to + 0xc, m.read(from + 0xc));                                     // 004C67DF..004C67E1 data
    }
    if (old != 0) {                                                                // 004C67F3..004C67F5
        const Word n = m.read(old - 4);                                            // 004C67F7 cookie
        for (Word i = n; i != 0; --i) frame_LogStringDtor_005e5c20(ptr(old + (i - 1) * 0x10), nullptr); // 004C6806 eh vector dtor (reverse), 0059EA30
        live_free(ptr(old - 4));                                                   // 004C680B..004C680C
    }
}

// 0059A2A0 CExoArrayList<CSWSScriptVar>::Add(CSWSScriptVar) (thiscall, by-value 10h, RET 10h; destroys the argument).
void __fastcall kotor_fastcall_0059a2a0(void* list, void*, ScriptVar value) {
    Mem m;
    const Word l = word(list);
    const Word capacity = m.read(l + 8);                                           // 0059A2B9
    if (m.read(l + 4) == capacity)                                                 // 0059A2BC..0059A2C7
        kotor_fastcall_004c6740(list, nullptr, capacity != 0 ? capacity + capacity : 0x10u); // 0059A2C9..0059A2D9
    const Word count = m.read(l + 4);                                              // 0059A2DE
    const Word element = m.read(l) + count * 0x10;                                 // 0059A2E1..0059A2E8
    m.write(l + 4, count + 1);                                                     // 0059A2EA..0059A2EB
    async_assign_string_005e5c50(ptr(element), nullptr, &value);                   // 0059A2EE..0059A2F5
    m.write(element + 8, value.type);                                              // 0059A2FA..0059A2FE
    m.write(element + 0xc, value.data);                                            // 0059A301..0059A309
    frame_LogStringDtor_005e5c20(&value, nullptr);                                 // 0059A305..0059A314 ~argument
}
}
namespace w2::native {
// 0059A3EF..0059A4BD: CSWSScriptVar temp at [esp+10h]; type 3 owns a new CExoString (8), type 5 a new
// CScriptLocation (18h); a failed allocation leaves data 0. The temp is copy-passed by value to Add 0059A2A0,
// the result is the last element, then the temp name is destroyed.
Word var_create_0059a3ef_native(Word table, Word name, Word type) {
    Mem m;
    ScriptVar temp;
    temp.text = 0; temp.length = 0;                                                // 0059A3EF..0059A3F3 005B3190
    async_assign_string_005e5c50(&temp, nullptr, ptr(name));                       // 0059A3F8..0059A407 operator=
    temp.type = type;                                                              // 0059A40F
    temp.data = 0;                                                                 // 0059A413
    if (type == 3) {                                                               // 0059A40C..0059A417
        const Word s = word(live_operator_new(8));                                 // 0059A419..0059A41B
        if (s != 0) { m.write(s, 0); m.write(s + 4, 0); }                          // 0059A430..0059A432 005B3190
        temp.data = s;                                                             // 0059A46B..0059A470
    } else if (type == 5) {                                                        // 0059A444..0059A447
        const Word loc = word(live_operator_new(0x18));                            // 0059A449..0059A44B
        temp.data = loc != 0 ? word(kotor_fastcall_004ca7a0(ptr(loc), nullptr)) : 0; // 0059A457..0059A470
    }
    ScriptVar argument;                                                            // 0059A474..0059A477 sub esp,10h
    (void)r8_copy_construct_005e5b00(&argument, nullptr, &temp);                   // 0059A479..0059A484
    argument.type = temp.type;                                                     // 0059A489..0059A48D
    argument.data = temp.data;                                                     // 0059A490..0059A496
    kotor_fastcall_0059a2a0(ptr(table), nullptr, argument);                        // 0059A494..0059A499
    const Word result = m.read(table + 4) * 0x10 + m.read(table) - 0x10;           // 0059A49E..0059A4A6
    frame_LogStringDtor_005e5c20(&temp, nullptr);                                  // 0059A4AA..0059A4B6
    return result;                                                                 // 0059A4BB
}
}
extern "C" {
// 0056CD20 CSWSMessage::SendServerToPlayerArea_ClientArea (RET 1Ch)
unsigned __fastcall recovered_area_client_area_0056cd20(void* self, void*, void* player, void* area, unsigned x, unsigned y, unsigned z, void* orientation, unsigned flag) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {};
    return e::area_client_area_0056cd20(m, p, word(self), word(player), word(area), x, y, z, word(orientation), flag, word(s));
}
// 0056D2D0 SendServerToPlayerSetCustomTokenList (RET 4)
unsigned __fastcall recovered_custom_token_list_0056d2d0(void* self, void*, unsigned recipient) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[8] = {}; return e::custom_token_list_0056d2d0(m, p, word(self), recipient, word(s));
}
// 00567EF0 SendServerToPlayerModule_Info (RET 4)
unsigned __fastcall recovered_module_info_00567ef0(void* self, void*, unsigned recipient) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::module_info_00567ef0(m, p, word(self), recipient, word(s));
}
// 004C3170 CSWSModule::PackModuleIntoMessage (RET 4; argument unused)
void __fastcall recovered_pack_module_004c3170(void* module, void*, unsigned) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; e::pack_module_004c3170(m, p, word(module), word(s));
}
// 0056FDE0 Login_Confirm / 0056CB10 Module_Loading (RET 4)
unsigned __fastcall recovered_login_confirm_0056fde0(void* self, void*, unsigned recipient) { Mem m; EntryPorts p(m); return e::login_confirm_0056fde0(m, p, word(self), recipient); }
unsigned __fastcall recovered_module_loading_0056cb10(void* self, void*, void* player) { Mem m; EntryPorts p(m); return e::module_loading_0056cb10(m, p, word(self), word(player)); }
// 0056CC50 SendServerToPlayerModule_EndStartNewModule(flag) RET 4
unsigned __fastcall recovered_end_start_new_module_0056cc50(void* self, void*, unsigned flag) { Mem m; EntryPorts p(m); alignas(8) unsigned char s[8] = {}; return e::end_start_new_module_0056cc50(m, p, word(self), flag, word(s)); }
// 0056CBF0 SendServerToPlayerModule_StartStartNewModule (RET 0)
unsigned __fastcall recovered_start_start_new_module_0056cbf0(void* self, void*) { Mem m; EntryPorts p(m); return e::start_start_new_module_0056cbf0(m, p, word(self)); }
unsigned __fastcall recovered_module_update_time_0056a5f0(void* self, void*, void* player, unsigned flags, unsigned d10, unsigned b8, unsigned b4, unsigned b2, unsigned kind, unsigned value) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[8] = {}; return e::module_update_time_0056a5f0(m, p, word(self), word(player), flags, d10, b8, b4, b2, kind, value, word(s));
}
unsigned __fastcall recovered_area_weather_0056d1e0(void* self, void*, void* player, unsigned weather, unsigned flag) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[8] = {}; return e::area_weather_0056d1e0(m, p, word(self), word(player), weather, flag, word(s));
}
// 00568030 PlayerList_Add (RET 8) / 0056FE40 PlayerList_All (RET 4)
unsigned __fastcall recovered_player_list_add_00568030(void* self, void*, unsigned recipient, void* player) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::player_list_add_00568030(m, p, word(self), recipient, word(player), word(s));
}
unsigned __fastcall recovered_player_list_all_0056fe40(void* self, void*, void* player) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::player_list_all_0056fe40(m, p, word(self), word(player), word(s));
}
// 00560CD0 CSWSPlayer::SetAreaTransitionBMP (RET 8)
void __fastcall recovered_area_transition_bmp_00560cd0(void* player, void*, unsigned value, void* str) { Mem m; EntryPorts p(m); e::set_area_transition_bmp_00560cd0(m, p, word(player), value, word(str)); }
// 00561C40 CSWSPlayer::SetGameObject (RET 4)
void __fastcall recovered_set_game_object_00561c40(void* player, void*, void* object) { Mem m; EntryPorts p(m); e::set_game_object_00561c40(m, p, word(player), word(object)); }
// 004B3A90 CServerExoAppInternal::InitiateModuleForPlayer (RET 4)
void __fastcall recovered_initiate_module_004b3a90(void* internal, void*, void* player) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; e::initiate_module_004b3a90(m, p, word(internal), word(player), word(s));
}
// 004B3D10 CServerExoAppInternal::MovePlayerToArea (RET 4)
void __fastcall recovered_move_player_to_area_004b3d10(void* internal, void*, void* player) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x20] = {}; e::move_player_to_area_004b3d10(m, p, word(internal), word(player), word(s));
}
// 004C5D90 CSWSModule::GetPrimaryPlayerIndex (RET)
unsigned __fastcall recovered_primary_player_index_004c5d90(void* module, void*) { Mem m; EntryPorts p(m); Word slot = 0; return e::primary_player_index_004c5d90(m, p, word(module), word(&slot)); }
// 004B5C50 CServerExoAppInternal::LoadCharacterFinish (RET 8)
unsigned __fastcall recovered_load_character_finish_004b5c50(void* internal, void*, void* player, unsigned flag) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::load_character_finish_004b5c50(m, p, word(internal), word(player), flag, word(s));
}
// 004B5F50 CServerExoAppInternal::LoadPrimaryPlayer (RET 4)
unsigned __fastcall recovered_load_primary_player_004b5f50(void* internal, void*, void* player) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::load_primary_player_004b5f50(m, p, word(internal), word(player), word(s));
}
// 00524800 HandlePlayerToServerModuleMessage (RET 8)
unsigned __fastcall recovered_module_message_00524800(void* self, void*, void* player, unsigned minor) {
    Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x400] = {}; return e::module_message_00524800(m, p, word(self), word(player), minor, word(s));
}
// 00524B80 HandlePlayerToServerAreaMessage (RET 8)
unsigned __fastcall recovered_area_message_00524b80(void* self, void*, void* player, unsigned minor) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::area_message_00524b80(m, p, word(self), word(player), minor, word(s));
}
// 0050AAA0 CSWSArea::PackAreaIntoMessage (RET 10h)
unsigned __fastcall recovered_pack_area_0050aaa0(void* area, void*, unsigned a, unsigned b, unsigned c, void* player) {
    Mem m; EntryPorts p(m); alignas(8) Scratch s = {}; return e::pack_area_0050aaa0(m, p, word(area), a, b, c, word(player), word(s));
}
// 00508820 CSWSArea::EnforceNoPCHeadCloning(head) RET 4
void __fastcall recovered_no_pc_head_cloning_00508820(void* area, void*, unsigned head) { Mem m; EntryPorts p(m); alignas(8) unsigned char s[0x10] = {}; e::no_pc_head_cloning_00508820(m, p, word(area), head, word(s)); }
// 004C4100 CSWSModule::GetTime (RET 18h)
void __fastcall recovered_module_time_004c4100(void* module, void*, void* a, void* b, void* c, void* d, void* byte_out, void* f) {
    Mem m; EntryPorts p(m); e::module_time_004c4100(m, p, word(module), word(a), word(b), word(c), word(d), word(byte_out), word(f));
}
// 0059A390 / 0059A530 / 0059A560 / 0059A590 / 0059A620 CSWSScriptVarTable lookups
void* __fastcall recovered_var_match_0059a390(void* table, void*, void* name, unsigned type, unsigned create) { Mem m; EntryPorts p(m); return ptr(e::var_match_0059a390(m, p, word(table), word(name), type, create)); }
unsigned __fastcall recovered_var_int_0059a530(void* table, void*, void* name) { Mem m; EntryPorts p(m); return e::var_int_0059a530(m, p, word(table), word(name)); }
float __fastcall recovered_var_float_0059a560(void* table, void*, void* name) { Mem m; EntryPorts p(m); const Word v = e::var_float_bits_0059a560(m, p, word(table), word(name)); float f; std::memcpy(&f, &v, 4); return f; }
void* __fastcall recovered_var_string_0059a590(void* table, void*, void* result, void* name) { Mem m; EntryPorts p(m); alignas(8) unsigned char s[8] = {}; return ptr(e::var_string_0059a590(m, p, word(table), word(result), word(name), word(s))); }
unsigned __fastcall recovered_var_object_0059a620(void* table, void*, void* name) { Mem m; EntryPorts p(m); return e::var_object_0059a620(m, p, word(table), word(name)); }
// 005C9780 / 005C89C0 sound packers (RET 4)
void __fastcall recovered_ambient_pack_005c9780(void* ambient, void*, void* message) { Mem m; EntryPorts p(m); e::ambient_pack_005c9780(m, p, word(ambient), word(message)); }
void __fastcall recovered_sound_pack_005c89c0(void* sound, void*, void* message) { Mem m; EntryPorts p(m); alignas(8) unsigned char s[16] = {}; e::sound_pack_005c89c0(m, p, word(sound), word(message), word(s)); }
}
