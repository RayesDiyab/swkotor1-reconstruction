#pragma once
// Lane "dialogue-start": shared helpers and provider declarations for the server conversation START chain
// (BeginConversation 0052E9F0 -> CSWSObject::StartDialog 004CF490 -> CSWSDialog::LoadDialog 005A2AE0 ...) and the
// dialogue callees that the delivered D1-D5 objects import. Transcribed from the original swkotor.exe (read-only).
// Conventions:
//   * original thiscall members -> extern "C" __fastcall(self, unused_edx, stack args...) decorated @name@(8+4*args);
//   * every provider below is a real body in the c05 candidate map (checked against the c05 trap census) or a
//     delivered dialogue object (D1-D5) or a function defined in one of the claude_dialogue_*.cpp files;
//   * game heap only (live_operator_new / live_free); no original address is ever dereferenced: globals are bound
//     by their rebuilt symbol; .rdata constants are written as their proven literal values.
#include <cstdint>
#include <cstring>

#if !defined(_M_IX86)
#error dialogue reconstruction is native x86 (32-bit guest pointers)
#endif

namespace dlg {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
constexpr U kInvalid = 0x7f000000u;     // OBJECT_INVALID
constexpr U8 kStatusOk = 0;             // .rdata [0074666C] (byte 00) compared after CGameObjectArray::GetGameObject
constexpr U8 kByte0073ecd0 = 0;         // .rdata [0073ECD0] (byte 00)
inline void* ptr(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
// Field access computes the address in integer arithmetic so a null base faults exactly where the original does
// (no null-pointer-arithmetic UB for the optimiser to exploit).
inline const void* fa(const void* b, U off) { return ptr(word(b) + off); }
inline void* fa(void* b, U off) { return ptr(word(b) + off); }
inline U rd(const void* b, U off) { U v; std::memcpy(&v, fa(b, off), 4); return v; }
inline I rdi(const void* b, U off) { I v; std::memcpy(&v, fa(b, off), 4); return v; }
inline float rdf(const void* b, U off) { float v; std::memcpy(&v, fa(b, off), 4); return v; }
inline U16 rd16(const void* b, U off) { U16 v; std::memcpy(&v, fa(b, off), 2); return v; }
inline U8 rd8(const void* b, U off) { return *static_cast<const volatile U8*>(fa(b, off)); }
inline void* rp(const void* b, U off) { return ptr(rd(b, off)); }
inline void wr(void* b, U off, U v) { std::memcpy(fa(b, off), &v, 4); }
inline void wrf(void* b, U off, float v) { std::memcpy(fa(b, off), &v, 4); }
inline void wr16(void* b, U off, U16 v) { std::memcpy(fa(b, off), &v, 2); }
inline void wr8(void* b, U off, U8 v) { *static_cast<volatile U8*>(fa(b, off)) = v; }
inline void wp(void* b, U off, const void* p) { wr(b, off, word(p)); }
inline char* at(void* b, U off) { return static_cast<char*>(fa(b, off)); }
inline U fbits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
inline float bitsf(U u) { float f; std::memcpy(&f, &u, 4); return f; }
template <class R = U, class... A> inline R vcall(void* o, U slot, A... a) {
    using F = R(__fastcall*)(void*, void*, A...);
    return reinterpret_cast<F>(rp(rp(o, 0), slot))(o, nullptr, a...);
}
}  // namespace dlg

// ---------------------------------------------------------------------------------------------------------------
// Rebuilt globals
// ---------------------------------------------------------------------------------------------------------------
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }  // [007A39FC] CAppManager*
namespace frame_control { extern unsigned message_manager_007a3a00; }          // [007A3A00] CVirtualMachine*
extern "C" int recovered_camera_reset_cell_007a2288;                         // [007A2288] (w1_native_b7)

namespace dlg {
inline void* app() { return kotor_menu_window_cells::manager_007a39fc; }
inline void* client() { return rp(app(), 4); }   // CAppManager+4 CClientExoApp*
inline void* server() { return rp(app(), 8); }   // CAppManager+8 CServerExoApp*
inline void* vm() { return ptr(frame_control::message_manager_007a3a00); }
}  // namespace dlg

// ---------------------------------------------------------------------------------------------------------------
// Providers (c05 map, real bodies)
// ---------------------------------------------------------------------------------------------------------------
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString()
namespace kotor_reconstruction { struct StartupString; }
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }  // CExoString::!=(char*)
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*); }  // CompareNoCase(char*)
namespace menu_resources { struct Ref { char bytes[16]; Ref* __thiscall construct_00406d80(const char*); }; }
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);                               // RemoveHead
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);                               // ~Internal
}
namespace recovered_server_create { void* __cdecl construct_list_internal_005e9aa0(void*); }       // Internal()
namespace m47_gob2 { double x87_pow_006fc1f0(double base, double exponent); }                        // CRT pow (x87)
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                        // __ftol2 low dword
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }           // GetPartyTable
namespace kotor_game_prepare { void __fastcall SetGlobalUnpauseableState0062b410(void*, void*, unsigned); }

extern "C" {
void* __cdecl live_operator_new(dlg::U bytes);
void __cdecl live_free(void* memory);

// CExoString
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);          // CExoString(char const*)
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                             // ~CExoString
void* __fastcall WindowCopy005e5b00(void* s, void*, const void* src);                     // CExoString(CExoString const&)
void* __fastcall async_assign_string_005e5c50(void* s, void*, const void* src);           // operator=(CExoString const&)
unsigned __fastcall frame_LogIsEmpty_005e58a0(void* s, void*);                            // IsEmpty
int __fastcall exostring_compare_no_case_005e6450(void* s, void*, const void* other);     // CompareNoCase(CExoString&)
unsigned __fastcall async_string_length_005e5790(void* s, void*);                         // GetLength
char __fastcall c2da_current_string_index_005e5600(void* s, void*, int index);            // operator[]
const char* __fastcall WindowCStr005e5670(void* s, void*);                                // CStr
// CResRef
void* __fastcall kotor_fastcall_00405ed0(void* r, void*);                                 // CResRef()
void* __fastcall ref_assign_copy_00406120(void* r, void*, void* out, const void* src);   // operator=(CResRef&) by value
void* __fastcall unresolved_ref_assign_00406290(void* r, void*, void* out, const char*); // operator=(char*) by value
void* __fastcall async_ref_assign_string_004061f0(void* r, void*, void* out, const void* s); // operator=(CExoString&)
int __fastcall c2da_equal_text_004060d0(void* r, void*, const char* text);               // operator==(char const*)
void __fastcall c2da_ref_to_string_00405f70(void* r, void*, void* out_string);           // CopyToString(CExoString&)
int __fastcall gui_load_resref_is_valid_00406030(void* r, void*);                         // IsValid
// CExoLocString
void* __fastcall exo_locstring_construct_005e9e10(void* l, void*);
void __fastcall exo_locstring_destroy_005ea000(void* l, void*);
void* __fastcall recovered_localized_assign_005e9e70(void* l, void*, const void* src);
// CExoLinkedListInternal
void* __fastcall unresolved_list_next_005e9b90(void* list, void*, void** cursor);         // GetNext(CExoLinkedListPosition&)
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);     // GetAtPos
void* __fastcall resource_append_005e9dc0(void* list, void*, void* data);                // AddTail
// CResGFF
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, unsigned type, const char* ext, const void* ref);
unsigned char __fastcall gui_load_gff_byte_00411a60(void*, void*, void* s, const char* label, dlg::U* ok, unsigned char d);
std::uint16_t __fastcall recovered_gff_word_00411b40(void*, void*, void* s, const char* label, dlg::U* ok, unsigned d);
dlg::U __fastcall gui_load_gff_dword_00411c20(void*, void*, void* s, const char* label, dlg::U* ok, dlg::U d);
dlg::I __fastcall gui_load_gff_int_00411c90(void*, void*, void* s, const char* label, dlg::U* ok, dlg::I d);
float __fastcall gui_load_gff_float_00411d00(void*, void*, void* s, const char* label, dlg::U* ok, float d);
void* __fastcall gui_load_gff_resref_00411e10(void*, void*, void* out, void* s, const char* label, dlg::U* ok, const void* d);
void* __fastcall gui_load_gff_string_00411ec0(void*, void*, void* out, void* s, const char* label, dlg::U* ok, const void* d);
void* __fastcall recovered_gff_localized_00411fd0(void*, void*, void* out, void* s, const char* label, dlg::U* ok, const void* d);
void* __fastcall gui_load_gff_vector_004122a0(void*, void*, void* out, void* s, const char* label, dlg::U* ok, const void* d);
dlg::U __fastcall gff_list_004118c0(void*, void*, void* list, void* s, const char* label);
dlg::U __fastcall gff_list_count_00411940(void*, void*, void* list);
dlg::U __fastcall gff_list_element_00411990(void*, void*, void* out, void* list, dlg::U index);
// CRT array iterators (thiscall element ctor/dtor pointers)
void __stdcall crt_eh_vector_constructor_006fb55f(void* dst, dlg::U size, dlg::I count, void* ctor, void* dtor);
void __stdcall c2da_destroy_array_006fa852(void* objects, dlg::U size, dlg::I count, void* dtor);
void __stdcall vector_constructor_iterator_00401130(void* dst, dlg::U size, dlg::I count, void* ctor);
// app / object access
void* __fastcall frame_R4_005ed690(void* client, void*);                                   // CClientExoApp::GetInGameGui
dlg::U __fastcall kotor_fastcall_004aea40(void* server, void*);                            // GetPlayerCreatureId
void* __fastcall kotor_fastcall_004ae910(void* server, void*, dlg::U id);                  // GetClientObjectByObjectId
void* __fastcall recovered_object_array_004aed70(void* server, void*);                     // GetObjectArray
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, dlg::U id, void** out); // CGameObjectArray::GetGameObject
void* __fastcall kotor_fastcall_004ae750(void* server, void*, dlg::U id);                  // CServerExoApp::GetGameObject
dlg::U __fastcall kotor_fastcall_005eda50(void* client_app, void*, dlg::U id);             // ServerToClientObjectId (ECX = CClientExoApp)
dlg::U __fastcall kotor_fastcall_004aea30(void* server, void*, dlg::U id);                 // ClientToServerObjectId
void* __fastcall kotor_fastcall_005ed530(void* client, void*);                             // CClientExoApp::GetModule
void __fastcall kotor_fastcall_00641dc0(void* module, void*);                              // CSWCModule::RestoreCamera
void __fastcall kotor_fastcall_005eda60(void* client, void*, dlg::U a, dlg::U b);          // SetInputClass
dlg::U __fastcall kotor_fastcall_004b4050(void* server_internal, void*, dlg::U player);   // Internal::GetPlayerLanguage
// creatures / objects
void __fastcall recovered_set_activity_004f00a0(void* creature, void*, dlg::U a, dlg::U b, dlg::U c);
void __fastcall w5_creature_action_manager_004f8770(void* creature, void*, dlg::U mode);
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, dlg::U flag);
void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void* creature, void*, dlg::U id, dlg::U force);
void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, const void* vector);
void* __fastcall recovered_last_update_object_00561820(void* player, void*, dlg::U id);    // CSWSPlayer::GetLastUpdateObject
void __fastcall recovered_unstealth_party_00563c60(void* party, void*, dlg::U flag);
// VM
dlg::U __fastcall recovered_vm_outer_pop_string_005d1080(void* vm, void*, void* out);
dlg::U __fastcall recovered_vm_outer_pop_object_005d10c0(void* vm, void*, dlg::U* out);
dlg::U __fastcall recovered_vm_outer_push_integer_005d1010(void* vm, void*, dlg::U value);
dlg::U __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, dlg::U object, dlg::U flag); // CVirtualMachine::RunScript
// GUI
void __fastcall kotor_fastcall_0062ec60(void* gui, void*, dlg::U state);                   // SetGlobalDialogState
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void* gui, void*, int a, float b, float c, const void* v); // StartGlobalFade

// delivered dialogue objects (D1-D5) imported here
dlg::U __fastcall kotor_fastcall_004cd460(void* object, void*, dlg::I entry);              // CSWSObject::SendDialogEntry
void __fastcall kotor_fastcall_005edcb0(void* client, void*, dlg::U flag);                 // SetDialogModelsSent
dlg::U __fastcall kotor_fastcall_004cb4f0(void* object, void*);                            // CSWSObject::StopDialog
void __fastcall kotor_fastcall_005edbd0(void* client, void*);                              // D4 CClientExoApp::StopSoundPlayingInDialog
}

// ---------------------------------------------------------------------------------------------------------------
// RAII locals mirroring the original EH frames (destructor at the original scope end and on unwind)
// ---------------------------------------------------------------------------------------------------------------
namespace dlg {
struct ExoString {                     // CExoString {char* data; U length}
    void* data = nullptr;
    U length = 0;
    ExoString() { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(this), nullptr); }
    explicit ExoString(const char* text) { (void)frame_LogStringCtor_005e5a90(this, nullptr, text); }
    ExoString(const ExoString&) = delete;
    ExoString& operator=(const ExoString&) = delete;
    ~ExoString() { frame_LogStringDtor_005e5c20(this, nullptr); }
};
static_assert(sizeof(ExoString) == 8, "CExoString");
struct ExoLocString {                  // CExoLocString
    U a = 0, b = 0;
    ExoLocString() { (void)exo_locstring_construct_005e9e10(this, nullptr); }
    ExoLocString(const ExoLocString&) = delete;
    ExoLocString& operator=(const ExoLocString&) = delete;
    ~ExoLocString() { exo_locstring_destroy_005ea000(this, nullptr); }
};
static_assert(sizeof(ExoLocString) == 8, "CExoLocString");
struct ResRef { char bytes[16]; };     // CResRef (no destructor)
inline void resref_default(void* r) { (void)kotor_fastcall_00405ed0(r, nullptr); }
inline void resref_from(void* r, const char* text) {
    (void)reinterpret_cast<menu_resources::Ref*>(r)->construct_00406d80(text);
}
inline void resref_assign(void* dst, const void* src) { ResRef tmp; (void)ref_assign_copy_00406120(dst, nullptr, &tmp, src); }
inline void resref_assign_text(void* dst, const char* src) { ResRef tmp; (void)unresolved_ref_assign_00406290(dst, nullptr, &tmp, src); }
inline void str_assign(void* dst, const void* src) { (void)async_assign_string_005e5c50(dst, nullptr, src); }
// void*-typed wrappers for the typed free-function providers
inline int str_not_equal(const void* s, const char* text) {
    return client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(const_cast<void*>(s)), nullptr, text);
}
inline int str_compare_nc(const void* s, const char* text) {
    return ini_comparison::CompareNC005e58c0(static_cast<kotor_reconstruction::StartupString*>(const_cast<void*>(s)), nullptr, text);
}
inline void* in_game_gui() { return frame_R4_005ed690(client(), nullptr); }
inline void* object_array() { return recovered_object_array_004aed70(server(), nullptr); }
// CGameObjectArray::GetGameObject(id, &out) == status 0 (the [0074666C] comparison)
inline bool get_object(U id, void** out) { return kotor_fastcall_004d8230(object_array(), nullptr, id, out) == kStatusOk; }
}  // namespace dlg

// ---------------------------------------------------------------------------------------------------------------
// Functions reconstructed by the claude_dialogue_*.cpp files (exact exported names; see each file)
// ---------------------------------------------------------------------------------------------------------------
extern "C" {
// claude_dialogue_start.cpp
int __fastcall recovered_vm_begin_conversation_0052e9f0(void* commands, void*, int command, unsigned argc);
dlg::U __fastcall kotor_fastcall_004cf490(void* object, void*, dlg::U with, void* dialog_resref_string, dlg::U flag);
void* __fastcall kotor_fastcall_005a0e80(void* dialog, void*);                              // CSWSDialog::CSWSDialog
void* __fastcall kotor_fastcall_005a4860(void* dialog, void*, dlg::U flags);                // scalar deleting dtor
void __fastcall kotor_fastcall_005a4880(void* dialog, void*);                               // ~CSWSDialog
dlg::U __fastcall kotor_fastcall_005a2ae0(void* dialog, void*, void* gff, dlg::U load_all); // LoadDialog
void __fastcall kotor_fastcall_0059f5f0(void* dialog, void*, void* base, void* gff, void* s, dlg::U* delay, dlg::U* ok);
void __fastcall kotor_fastcall_0059eaa0(void* dialog, void*, void* camera, void* gff, void* s, dlg::U* ok);
void __fastcall kotor_fastcall_0059ec10(void* dialog, void*, void* link, void* gff, void* s, dlg::U* ok, dlg::U count);
void __fastcall kotor_fastcall_0059f220(void* dialog, void*, void* gff);
void __fastcall kotor_fastcall_0059f430(void* dialog, void*, void* object);                 // CSWSDialog::SetDialogOwner
dlg::I __fastcall kotor_fastcall_005a3640(void* dialog, void*, void* object);               // GetStartEntry
void __fastcall kotor_fastcall_005a27b0(void* dialog, void*);                               // Cleanup
void __fastcall kotor_fastcall_0059e9b0(void* dialog, void*, dlg::U id);                    // ClearDialogOwnerInObject
int __fastcall kotor_fastcall_0059ec90(void* dialog, void*, void* object, const void* script); // CheckScript
int __fastcall kotor_fastcall_0059ed70(void* dialog, void*, void* object, const void* script); // RunScript
void* __fastcall kotor_fastcall_0059fc30(void* dialog, void*, void* object, void* speaker);    // GetSpeaker
void __fastcall kotor_fastcall_005a1050(void* dialog, void*, dlg::U entry);                 // BarkOneLine
void* __fastcall kotor_fastcall_0059e860(void* base, void*);                                // CSWSDialogBase()
void __fastcall kotor_fastcall_0059e920(void* base, void*);                                 // ~CSWSDialogBase
void* __fastcall kotor_fastcall_0059f4e0(void* node, void*);                                // CSWSDialogNode()
void __fastcall kotor_fastcall_0059f550(void* node, void*);                                 // ~CSWSDialogNode
void* __fastcall dialog_reply_ctor_0059f5b0(void* reply, void*);                            // CSWSDialogReply()
void __fastcall dialog_node_dtor_0059f5e0(void* node, void*);                               // ~CSWSDialogNode thunk
void* __fastcall kotor_fastcall_005a0e60(void* entry, void*);                               // CSWSDialogEntry()
void* __fastcall kotor_fastcall_0059ea40(void* stunt, void*);                               // CSWSDialogStuntModel()
void* __fastcall kotor_fastcall_0059ea90(void* anim, void*);                                // CLiveContentStruct()
void* __fastcall kotor_fastcall_0059e900(void* link, void*);                                // CSWSDialogLinkEntry()
void __fastcall kotor_fastcall_0059ea20(void* speaker, void*);                              // speaker element dtor
void* __fastcall kotor_fastcall_0059f470(void* list, void*);                                // CExoLinkedList<CSWSDialogPlayer>()
void __fastcall kotor_fastcall_004cc490(void* dialog, void*, void* player);                 // add player (AddTail)
void __fastcall kotor_fastcall_004cb7a0(void* object, void*, dlg::U id);                    // CSWSObject::SetDialogOwner
void __fastcall kotor_fastcall_004cb5a0(void* object, void*);                               // CSWSObject::StopSoundPlayingInDialog
void __fastcall kotor_fastcall_004cc250(void* object, void*, dlg::U bit);                   // CSWSObject::ClearDirtyFlag
void __fastcall kotor_fastcall_004064c0(void* res, void*, void* out_ref, void* out_type);   // CRes::GetResRef
void __fastcall kotor_fastcall_004af010(void* server, void*, dlg::U value);                 // SetActionConversation
dlg::U __fastcall kotor_fastcall_004af020(void* server, void*);                             // GetActionConversation
dlg::U __fastcall kotor_fastcall_004af040(void* server, void*);                             // GetIgnoreConversationStartRange
dlg::U __fastcall kotor_fastcall_004ae810(void* server, void*, dlg::U player_id);           // GetPlayerLanguage
dlg::U __fastcall kotor_fastcall_005d0fe0(void* vm, void*, dlg::U* type, dlg::U* value);    // GetRunScriptReturnValue
dlg::U __fastcall kotor_fastcall_005d1820(void* vm_internal, void*, dlg::U* type, dlg::U* value);
void __fastcall kotor_fastcall_005edca0(void* client, void*, dlg::U flag);                  // SetDialogSent
void __fastcall kotor_fastcall_005ee010(void* client, void*);                               // ClearEnemySighted
}
