// Lane "dialogue-start", fork B1: client in-game GUI dialogue functions (CGuiInGame dialog entry / replies / close,
// letterbox and computer panels, bark and dialog sound), transcribed from the original swkotor.exe (read-only).
// Every statement is annotated with the original instruction range. thiscall -> __fastcall(self, unused_edx, ...).
// Quirks of the original are kept and marked "QUIRK".
#include "6bd5403233-claude_dialogue_common.hpp"
#include <initializer_list>

using namespace dlg;

// ---------------------------------------------------------------------------------------------------------------
// Additional rebuilt globals
// ---------------------------------------------------------------------------------------------------------------
namespace kotor_menu_window_cells { extern void* volatile input_007a39e4; }      // [007A39E4] CExoInput*
namespace w4 { extern const volatile U ingame_gui_single_panel_007a2370; }       // [007A2370] (.data 1, never written)
extern "C" {
extern void* kotor_cell_rules_007a3a28;                                        // [007A3A28] CSWRules*
extern unsigned kotor_rules_cell32_007a2190;                                   // [007A2190] 2DA column index
extern unsigned kotor_rules_cell32_007a2198;                                   // [007A2198] 2DA column index
extern void* kotor_cell_tlk_client_007a3a0c;                                   // [007A3A0C] CSWTlkTable*
extern void* kotor_cell_sound_007a39ec;                                        // [007A39EC] CExoSound*
extern unsigned kotor_cell_disable_sound_00832894;                             // [00832894] (read as a dword at 00631F63)
}
namespace {
// [0083390C] "disableLetterBox": .bss, no writer anywhere in the original image (only two reads, 0062B7C3 and
// 006334A2; no other reference to the address exists) -> proven constant 0.
constexpr U k_disable_letterbox_0083390c = 0;
constexpr U k_sound_mode_0074c5ec = 0;                 // .rdata [0074C5EC] = 0
constexpr U k_float_0_5 = 0x3f000000u;                 // .rdata [0073E9AC] = 0.5f
constexpr U k_float_3_7 = 0x3edb6db7u;                 // .rdata [00755788] = 0.428571492f (3/7)
}

// ---------------------------------------------------------------------------------------------------------------
// Providers (c05, real bodies) not declared by the common header
// ---------------------------------------------------------------------------------------------------------------
namespace options_video { struct String; }
namespace kotor_reconstruction {
struct StartupString { void* data; U length; int __thiscall BorrowedEquals005e5290(const char* text); };  // 0077
}
namespace client_movies {
int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*);        // CExoString::!=(char*)
int __fastcall StringFind_005e5eb0(options_video::String*, void*, options_video::String*, int); // CExoString::Find
}
namespace kotor_menu_10_19 { struct Tlk; }
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*, void*, kotor_reconstruction::StartupString*); }
namespace menu_tables_required {
void* __fastcall StreamConstruct_005d5a50(void*, void*);                                   // CExoStreamingSoundSource()
void __fastcall StreamSetPriority_005d5ba0(void*, void*, unsigned);                        // SetPriorityGroup
unsigned __fastcall StreamPlay_005d5b10(void*, void*, menu_resources::Ref*, unsigned, unsigned, unsigned, unsigned);
}
namespace kotor_runtime_reporter_dependencies { char* __cdecl string_copy_n_006fab80(char*, const char*, unsigned); } // strncpy

extern "C" {
void* __fastcall frame_LogAssign_005e5140(void* s, void*, const char* text);               // CExoString::operator=(char*)
void* __fastcall async_ref_construct_string_00406d60(void* r, void*, const void* s);      // CResRef(CExoString const&)
void* __fastcall c2da_current_construct_00413cc0(void* t, void*, ResRef ref, int flag);    // C2DA(CResRef, int)
int __fastcall c2da_current_load_004143b0(void* t, void*);                                 // Load2DArray
int __fastcall c2da_current_get_string_00413270(void* t, void*, int row, const void* col, void* out); // GetCExoStringEntry
int __fastcall c2da_current_get_int_entry_00413510(void* t, void*, int row, int col, int* out);      // GetINTEntry
void __fastcall gui_load_panel_set_alpha_00404b00(void* panel, void*, float alpha);         // CSWGuiPanel::SetPanelAlpha
void __fastcall kotor_m15_panel_set_background_0040a870(void* panel, void*, const void* ref); // SetBackground
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void* panel, void*, U visible);       // SetVisible
void __fastcall kotor_fastcall_0040bc70(void* mgr, void*, void* panel, U a, U b);          // CSWGuiManager::AddPanel
int __fastcall frame_ContainsPanel_0040bd70(void* mgr, void*, void* panel);                // PanelExists
void __fastcall kotor_fastcall_0040be00(void* mgr, void*);                                 // PopModalPanel
U __fastcall frame_RemovePanel_0040c830(void* mgr, void*, void* panel);                    // RemovePanel
void __fastcall kotor_m15_fade_set_fade_006244e0(void* fade, void*, U a, U b, float c, const void* v); // CSWGuiFade::SetFade
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                             // GetSWGuiManager
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                       // CClientExoApp::GetGameObject
void* __fastcall frame_R4_005ed5b0(void* client, void*, U id);                             // GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                             // GetClientOptions
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                             // GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U index);                 // CSWParty::GetCharacter
U __fastcall gui_m48_client_get_input_class_005eda70(void* client, void*);                 // GetInputClass
void __fastcall kotor_fastcall_005edba0(void* client, void*);                              // ResetDriveAcceleration
void __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);               // SetPausedByCombat
void __fastcall kotor_fastcall_005edc30(void* client, void*, I a, U b);                    // ChangeCharacter
void __fastcall kotor_fastcall_005edf20(void* client, void*);                              // DisableVideoEffect
void __fastcall kotor_fastcall_0061f9c0(void* internal, void*, U flags);                   // HideMouse
void __fastcall kotor_fastcall_0061fa00(void* internal, void*, U flags);                   // ShowMouse
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                             // CExoSoundInternal::SetSoundMode
void __fastcall kotor_fastcall_005d5b80(void* stream, void*);                              // CExoStreamingSoundSource::Stop
void __fastcall kotor_fastcall_005d6200(void* stream, void*);                              // ~CExoStreamingSoundSource
void __fastcall cool_down_event_005df4b0(void* input, void*, U event, U ms);               // CExoInput::CoolDownEvent
U __fastcall kotor_fastcall_00674b40(void* tlk, void*);                                    // CSWTlkTable::GetObjectGender
U __fastcall kotor_fastcall_005b4880(void* creature, void*);                               // CSWSCreature::IsDebilitated
U __fastcall recovered_creature_incapacitated_004ef890(void* creature, void*);             // GetDeadTemp
U16 __fastcall kotor_m15a_object_get_current_animation_0063c060(void* object, void*);     // GetCurrentAnimation
void* __fastcall gob_get_property_0x68_0063c070(void* object, void*);                      // CSWCObject +68 gob
void __fastcall kotor_fastcall_0063c9c0(void* object, void*);                              // RemoveTopFireForgetAnimation
void __fastcall kotor_m15a_object_abort_fire_forget_animation_0063cad0(void* object, void*, U flag);
void __fastcall kotor_fastcall_0061a670(void* creature, void*);                            // AnimateFireAndForget
U __fastcall kotor_fastcall_00614b60(void* creature, void*);                               // CSWCCreature::IsInjured
void __fastcall kotor_fastcall_006146e0(void* creature, void*, U id, float time);          // CSWCCreature::LookAt
U __fastcall recovered_client_creature_set_lock_orientation_00610950(void* c, void*, U id, U force);
void* __fastcall kotor_fastcall_00682e00(void* placeable, void*);                          // GetServerPlaceable
void* __fastcall kotor_fastcall_006a8eb0(void* computer, void*, void* manager);            // CSWGuiDialogComputer()
void __fastcall object_projectile_bark_006a9920(void* bubble, void*, U id, void* text, void* sound, U flags); // SetBark
U __fastcall object_projectile_bark_0062ab30(void* gui, void*, U id, U a, U b, U c);       // ShowBarkBubble
void __fastcall frame_R4_0062aa00(void* gui, void*, U a, U b);                             // SetSWGuiStatus
void __fastcall kotor_fastcall_0062ba30(void* gui, void*);                                 // ClearRepliesData
U __fastcall kotor_fastcall_0062d5e0(void* gui, void*, U anim);                            // IsLoopingDialogAnimation
U __fastcall kotor_fastcall_0062d700(void* gui, void*, U anim);                            // IsFireForgetDialogAnimation
void __fastcall kotor_fastcall_00630560(void* gui, void*, void* text, U speaker);          // AppendToDialogBuffer
void __fastcall kotor_fastcall_00630620(void* gui, void*, U count);                        // InitializeDialogAnimationData
void __fastcall frame_GuiDispatch_006339c0(void* gui, void*);                              // CGuiInGame::UpdateDialog

// fork B2 (claude_dialogue_camera.cpp)
U __fastcall kotor_fastcall_0062bf70(void* gui, void*, U a, U b);                          // SetAnimatedCamera
void __fastcall kotor_fastcall_0062bf30(void* gui, void*);                                 // InitializeShotCamera
void __fastcall kotor_fastcall_0062c030(void* gui, void*, U a, U b, U c);                  // SetComputerCameraShot
void __fastcall kotor_fastcall_006306e0(void* gui, void*, U a, U b, U c, U d, U e, U f);   // UpdateShotCamera
void __fastcall kotor_fastcall_006412f0(void* module, void*, U a);                         // CSWCModule::SetDialogCamera
}

// ---------------------------------------------------------------------------------------------------------------
// Exports (this file)
// ---------------------------------------------------------------------------------------------------------------
extern "C" {
void __fastcall kotor_fastcall_005ee200(void* client, void*, U id);
void __fastcall kotor_fastcall_00628670(void* gui, void*, U type, U computer);
void __fastcall kotor_fastcall_00628750(void* gui, void*, U i, void* text, U a3, U a4, U a5, U a6, U f7, U f8, U a9,
    U w10, U f11, U a12, U b13, const void* v14, U f15, U f16, I n17, const U* p18, const U16* p19);
U __fastcall kotor_fastcall_0062abd0(void* gui, void*);
void __fastcall kotor_fastcall_0062b130(void* gui, void*);
void* __fastcall kotor_fastcall_0062add0(void* gui, void*, void* out, void* text);
void __fastcall kotor_fastcall_0062b440(void* gui, void*, U a, const void* resref);
void __fastcall kotor_fastcall_0062b730(void* gui, void*, void* text, U a2, U a3);
U __fastcall kotor_fastcall_0062bc70(void* gui, void*, U count);
void __fastcall kotor_fastcall_0062c2a0(void* gui, void*);
void __fastcall kotor_fastcall_0062c6f0(void* gui, void*);
void __fastcall kotor_fastcall_0062d510(void* gui, void*);
U __fastcall kotor_fastcall_0062d820(void* gui, void*, U anim);
void __fastcall kotor_fastcall_0062d8c0(void* gui, void*);
void __fastcall kotor_fastcall_0062d990(void* gui, void*);
void __fastcall kotor_fastcall_0062db50(void* gui, void*);
int __fastcall kotor_fastcall_0062e100(void* gui, void*, U a1, U a2, U a3, void* text, U a5, U8 a6, I a7);
int __fastcall kotor_fastcall_0062e1d0(void* gui, void*, U id, const void* sound);
U __fastcall kotor_fastcall_0062f0e0(void* gui, void*, U id);
void __fastcall kotor_fastcall_0062f8b0(void* gui, void*);
void __fastcall kotor_fastcall_006313a0(void* gui, void*);
void __fastcall kotor_fastcall_00631b70(void* gui, void*);
void __fastcall kotor_fastcall_00631c60(void* gui, void*);
U __fastcall kotor_fastcall_00631d80(void* gui, void*, U a1, U a2, U a3, U a4, U a5, U a6, void* a7, U a8,
    const U* a9, const U16* a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18, U a19, U a20, U a21,
    U a22, U a23);
U __fastcall kotor_fastcall_006332b0(void* gui, void*);
void __fastcall kotor_fastcall_006340e0(void* gui, void*, U a1, U a2);
U __fastcall kotor_fastcall_00634340(void* gui, void*, U a1, U a2, U a3, void* a4, U a5, U a6, U a7, U a8, U a9,
    U a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18, U a19, U a20, U a21, U a22, U a23, U a24, U a25);
void __fastcall kotor_fastcall_0063cab0(void* object, void*);
void __fastcall kotor_fastcall_006a6d70(void* dialog, void*);
void __fastcall kotor_fastcall_006a74b0(void* letterbox, void*);
void __fastcall kotor_fastcall_006a7540(void* letterbox, void*);
void __fastcall kotor_fastcall_006a8a60(void* letterbox, void*);
void __fastcall kotor_fastcall_006a75e0(void* letterbox, void*);
void __fastcall kotor_fastcall_006a7620(void* letterbox, void*, U on, U a, U b, const void* color);
void __fastcall kotor_fastcall_006a8400(void* computer, void*, U8 type);
void __fastcall kotor_fastcall_006a9c60(void* bubble, void*, U stop_sound);
}

namespace {
void* manager_of(void* gui) { return rp(gui, 0x38); }                                     // CGuiInGame+38 CSWGuiManager*
// CExoString storage constructed by a callee (copy constructor / hidden return) and destroyed at scope end
struct RawStr {
    void* data = nullptr;
    U length = 0;
    RawStr() = default;
    RawStr(const RawStr&) = delete;
    RawStr& operator=(const RawStr&) = delete;
    ~RawStr() { frame_LogStringDtor_005e5c20(this, nullptr); }
};
void* client_object(U id) { return kotor_fastcall_005ed580(client(), nullptr, id); }
void* client_internal() { return rp(client(), 4); }
// player's client-side game object id via server: GetClientObjectByObjectId(GetPlayerCreatureId())+38 -> client id
U player_client_id() {
    void* player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr));
    return kotor_fastcall_005eda50(client(), nullptr, rd(player, 0x38));
}
// tlk = [007A3A0C]: +58 = player object, +3C = gender, +5C &= ~1, ParseStr(text)
void tlk_parse(void* text) {
    void* tlk = kotor_cell_tlk_client_007a3a0c;
    wp(tlk, 0x58, client_object(player_client_id()));
    const U8 gender = static_cast<U8>(kotor_fastcall_00674b40(tlk, nullptr));
    wr(tlk, 0x5c, rd(tlk, 0x5c) & 0xfffffffeu);
    wr8(tlk, 0x3c, gender);
    tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(tlk), nullptr,
                               static_cast<kotor_reconstruction::StartupString*>(text));
}
// object->PlayAnimation(anim, a, b): vtable +9C
void play(void* o, U anim, U a, U b) { vcall<void>(o, 0x9c, anim, a, b); }
// gob(+68)->vtable +70(flag)
void gob70(void* o, U flag) { vcall<void>(rp(o, 0x68), 0x70, flag); }
bool anim_is_none(void* o) { return kotor_m15a_object_get_current_animation_0063c060(o, nullptr) == 0x2710; }
// client "talk" reset of one participant (ResetTalkAnimations 00631C60 body / SetDialogAnimations 00631549..)
void reset_participant(void* gui, U id) {
    void* o = client_object(id);
    if (!o) return;
    if (!vcall<void*>(o, 0xc)) return;
    if (vcall<void*>(o, 0x2c)) {
        if (!kotor_fastcall_0062f0e0(gui, nullptr, rd(vcall<void*>(o, 0x2c), 4))) return;
    }
    if (!anim_is_none(vcall<void*>(o, 0xc))) play(vcall<void*>(o, 0xc), 0x2710, 0, 1);
    gob70(vcall<void*>(o, 0xc), 0);
}
}  // namespace

extern "C" {

// 005EE200 CClientExoApp::SetLastClickedOnTarget(id): internal+2B8 = id. RET 4
void __fastcall kotor_fastcall_005ee200(void* client_app, void*, U id) { wr(rp(client_app, 4), 0x2b8, id); }

// 005F2C90 CClientExoAppInternal::BarkString is owned by claude_w5_object_actions.cpp (coordinator ruling).

// 0062ABD0 CGuiInGame::IsCameraDialog: +3C != 0 && +3C == +40
U __fastcall kotor_fastcall_0062abd0(void* gui, void*) {
    const U p = rd(gui, 0x3c);
    return (p != 0 && p == rd(gui, 0x40)) ? 1u : 0u;
}

// 0062B130 CGuiInGame::ShowDialogMessage: when +3C == +40 tail-jumps to panel vtable +80 (no arguments)
void __fastcall kotor_fastcall_0062b130(void* gui, void*) {
    void* panel = rp(gui, 0x3c);
    if (word(panel) == rd(gui, 0x40)) vcall<void>(panel, 0x80);                            // 0062B130..0062B13C
}

// 0062B440 (a, CResRef const& sound): +BDC = a; +BE0 = sound. RET 8
void __fastcall kotor_fastcall_0062b440(void* gui, void*, U a, const void* resref) {
    wr(gui, 0xbdc, a);
    resref_assign(at(gui, 0xbe0), resref);
}

// 0062C2A0 CGuiInGame::EndDialogCamera
void __fastcall kotor_fastcall_0062c2a0(void* gui, void*) {
    kotor_fastcall_00641dc0(kotor_fastcall_005ed530(client(), nullptr), nullptr);          // 0062C2A0..0062C2B2
    wr(gui, 0xb24, 0); wr(gui, 0xb2c, 0); wr(gui, 0xb20, 0); wr(gui, 0xb28, 0); wr(gui, 0xb34, 0);
    wr16(gui, 0x164, 0x2772);
    wr(gui, 0x184, kInvalid); wr(gui, 0x170, kInvalid); wr(gui, 0x174, kInvalid);
    wr(gui, 0x178, kInvalid); wr(gui, 0x17c, kInvalid);
}

// 0062D510 CGuiInGame::HideContainerGui
void __fastcall kotor_fastcall_0062d510(void* gui, void*) {
    if (!rd(gui, 0x108)) return;                                                           // 0062D513..0062D51B
    if (!frame_ContainsPanel_0040bd70(manager_of(gui), nullptr, rp(gui, 0x54))) return;   // 0062D51D..0062D52B
    kotor_fastcall_0040be00(manager_of(gui), nullptr);                                     // 0062D530 PopModalPanel
    (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, rp(gui, 0x54));            // 0062D535..0062D53C
    kotor_fastcall_005eda60(client(), nullptr, 0, 1);                                      // 0062D541..0062D54E
    kotor_fastcall_005edc20(client(), nullptr, 0, 0, 0);                                   // 0062D553..0062D561
}

// 0062D820 CGuiInGame::IsOverlayDialogAnimation(WORD anim) RET 4
U __fastcall kotor_fastcall_0062d820(void* gui, void*, U anim) {
    (void)gui;
    const U16 a = static_cast<U16>(anim);
    if (a < 0x2710) return 0;                                                              // 0062D821..0062D82A (unsigned)
    void* table = rp(rp(kotor_cell_rules_007a3a28, 0xb8), 0x40);                           // rules+B8 -> +40 C2DA
    if (static_cast<I>(a) >= rdi(table, 0x24) + 0x2710) return 0;                          // 0062D836..0062D84D (signed)
    const int row = static_cast<U16>(a - 0x2710);                                          // 0062D858..0062D85D
    int first = 0, second = 0;
    if (!c2da_current_get_int_entry_00413510(table, nullptr, row,
            static_cast<int>(kotor_rules_cell32_007a2198), &first)) return 0;              // 0062D860..0062D873
    table = rp(rp(kotor_cell_rules_007a3a28, 0xb8), 0x40);                                 // 0062D875..0062D881 re-read
    if (!c2da_current_get_int_entry_00413510(table, nullptr, row,
            static_cast<int>(kotor_rules_cell32_007a2190), &second)) return 0;             // 0062D884..0062D898
    return (first != 0 && second != 0) ? 1u : 0u;                                          // 0062D89A..0062D8AA
}

// 0062D8C0 CGuiInGame::CleanUpEntryNode / 0062D990 CleanUpReplyNode (the latter also ClearRepliesData)
namespace {
void clean_up_node(void* gui, bool reply) {
    wr(gui, 0xe8, 0); wr(gui, 0xec, 0); wr(gui, 0xf0, 0);                                  // vector at +E8 = 0
    wr(gui, 0xe4, 0); wr(gui, 0xf4, 0);
    kotor_fastcall_006a75e0(rp(gui, 0x68), nullptr);                                       // letterbox ResetFade
    if (reply) kotor_fastcall_0062ba30(gui, nullptr);                                      // 0062D9E4 ClearRepliesData
    const U panel = rd(gui, 0x3c);
    if (panel == 0 || panel != rd(gui, 0x40)) {
        if (frame_ContainsPanel_0040bd70(manager_of(gui), nullptr, rp(gui, 0x48))) {
            (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, rp(gui, 0x48));
            kotor_fastcall_0040bc70(manager_of(gui), nullptr, rp(gui, 0x3c), 2, 1);
            kotor_fastcall_005edf20(client(), nullptr);                                    // DisableVideoEffect
        }
    }
    wr(gui, 0xb2c, 0);
    cool_down_event_005df4b0(kotor_menu_window_cells::input_007a39e4, nullptr, 0x43, 0x2ee);
    cool_down_event_005df4b0(kotor_menu_window_cells::input_007a39e4, nullptr, 0xb5, 0x2ee);
}
}  // namespace
void __fastcall kotor_fastcall_0062d8c0(void* gui, void*) { clean_up_node(gui, false); }
void __fastcall kotor_fastcall_0062d990(void* gui, void*) { clean_up_node(gui, true); }

// 0062DB50 CGuiInGame::UnlockDialogParticipantOrientations: +170 and +174 creatures LockOrientation(invalid, 0)
void __fastcall kotor_fastcall_0062db50(void* gui, void*) {
    for (U off : {0x170u, 0x174u}) {
        void* o = client_object(rd(gui, off));
        if (o && vcall<void*>(o, 0x2c))
            (void)recovered_client_creature_set_lock_orientation_00610950(vcall<void*>(o, 0x2c), nullptr, kInvalid, 0);
    }
}

// 0062E100 CGuiInGame::HandleDialogReplyChosen(.., CExoString const& text, ..) RET 1Ch; returns 1
int __fastcall kotor_fastcall_0062e100(void* gui, void*, U, U, U, void* text, U, U8, I) {
    (void)gui;
    RawStr copy;
    (void)WindowCopy005e5b00(&copy, nullptr, text);                                         // 0062E11D..0062E122 copy
    if (!frame_LogIsEmpty_005e58a0(text, nullptr)) tlk_parse(&copy);                       // 0062E127..0062E19A
    // QUIRK: the parsed copy is discarded
    return 1;                                                                               // 0062E19F..0062E1B4
}

// 0062E1D0 CGuiInGame::HandleDialogSound(id, CResRef const& sound) RET 8
int __fastcall kotor_fastcall_0062e1d0(void* gui, void*, U id, const void* sound) {
    void* o = client_object(rd(gui, 0x170));                                                // 0062E1EA..0062E1FB
    if (!o || !vcall<void*>(o, 0xc)) return 0;                                              // 0062E202..0062E213
    void* speaker = vcall<void*>(o, 0xc);                                                   // 0062E219..0062E21D
    void* ref = const_cast<void*>(sound);
    if (gui_load_resref_is_valid_00406030(ref, nullptr) && speaker) {                       // 0062E224..0062E237
        if (void* old = rp(gui, 0xb40)) {                                                   // 0062E23D..0062E245
            kotor_fastcall_005d5b80(old, nullptr);                                          // Stop
            if (void* s = rp(gui, 0xb40)) { kotor_fastcall_005d6200(s, nullptr); live_free(s); } // 0062E24C..0062E25E
        }
        void* mem = live_operator_new(4);                                                   // 0062E266..0062E268
        void* stream = mem ? menu_tables_required::StreamConstruct_005d5a50(mem, nullptr) : nullptr;
        wp(gui, 0xb40, stream);                                                             // 0062E297
        menu_tables_required::StreamSetPriority_005d5ba0(stream, nullptr, 9);              // 0062E28B..0062E29D
        U ok = menu_tables_required::StreamPlay_005d5b10(rp(gui, 0xb40), nullptr,
            static_cast<menu_resources::Ref*>(ref), 8, 1, 0, 0);                           // 0062E2A2..0062E2B1
        if (!ok) ok = menu_tables_required::StreamPlay_005d5b10(rp(gui, 0xb40), nullptr,
            static_cast<menu_resources::Ref*>(ref), 4, 1, 0, 0);                           // 0062E2BA..0062E2C7
        if (!ok) { vcall<void>(speaker, 0x128, ref); return 0; }                            // 0062E2D0..0062E2DC
        kotor_fastcall_0062b440(gui, nullptr, id, ref);                                     // 0062E2F3..0062E2FB
    }
    wr(gui, 0x180, rd(speaker, 4));                                                         // 0062E300..0062E303
    return 1;
}

// 0062F0E0 CGuiInGame::CanPlayDialogAnimations(client id) RET 4
U __fastcall kotor_fastcall_0062f0e0(void* gui, void*, U id) {
    (void)gui;
    if (id == kInvalid) return 1;                                                           // 0062F0E4..0062F0EA
    void* s = kotor_fastcall_004ae750(server(), nullptr, kotor_fastcall_004aea30(server(), nullptr, id)); // ..0062F105
    if (!s) return 0;                                                                       // 0062F10C..0062F10E
    if (!vcall<void*>(s, 0x30)) return 1;                                                   // 0062F110..0062F119
    void* c = vcall<void*>(s, 0x30);                                                        // 0062F11B..0062F11F
    if (kotor_fastcall_005b4880(c, nullptr)) return 0;                                      // IsDebilitated
    if (vcall<U>(c, 0x94)) return 0;                                                        // 0062F12F..0062F13B
    return recovered_creature_incapacitated_004ef890(c, nullptr) ? 0u : 1u;                 // GetDeadTemp
}

// 0062F8B0 CGuiInGame::ClearDialogNonDependantList: drain +B4C (heap CExoStrings)
void __fastcall kotor_fastcall_0062f8b0(void* gui, void*) {
    while (rd(rp(gui, 0xb4c), 0) != 0) {                                                    // 0062F8B3..0062F8BC
        void* s = base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(rp(gui, 0xb4c)), nullptr);
        if (s) { frame_LogStringDtor_005e5c20(s, nullptr); live_free(s); }                  // 0062F8C5..0062F8D3
    }
}

// 0063CAB0 CSWCObject::RemoveAllFireForgetAnimations: (WORD)+5A times RemoveTopFireForgetAnimation
void __fastcall kotor_fastcall_0063cab0(void* object, void*) {
    for (I i = 0; i < static_cast<I>(rd16(object, 0x5a)); ++i) kotor_fastcall_0063c9c0(object, nullptr);
}

// 006A6D70 CSWGuiDialog::EndDialog: +1DF8 |= 4
void __fastcall kotor_fastcall_006a6d70(void* dialog, void*) { wr(dialog, 0x1df8, rd(dialog, 0x1df8) | 4); }

namespace {
void* letterbox_manager(void* lb) {                                                         // +18 cached GetSWGuiManager
    if (!rd(lb, 0x18)) wp(lb, 0x18, kotor_fastcall_005eda40(client(), nullptr));
    return rp(lb, 0x18);
}
// fild dword; fmul dword [k]; __ftol2 (x87 at 53-bit precision: exact product of a 32-bit int and a float)
I scaled(I v, U kbits) { return static_cast<I>(w2::ftol2_006fae8c(static_cast<double>(v) * static_cast<double>(bitsf(kbits)))); }
}  // namespace

// 006A74B0 CSWGuiDialogLetterbox::SetTop
void __fastcall kotor_fastcall_006a74b0(void* lb, void*) {
    void* m = letterbox_manager(lb);                                                        // 006A74B5..006A74CF
    const I a = scaled(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6c))), k_float_3_7); // 006A74D2..006A74E4
    const I b = scaled(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6e))) - a, k_float_0_5); // 006A74E9..006A74FD
    wr(lb, 0x80, 0); wr(lb, 0x68, 0); wr(lb, 0x64, 0);
    const I w = static_cast<std::int16_t>(rd16(m, 0x6c));
    wr(lb, 8, 0); wr(lb, 4, 0);
    wr(lb, 0x6c, static_cast<U>(w)); wr(lb, 0x70, static_cast<U>(b));
    wr(lb, 0x10, 0); wr(lb, 0xc, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6c)))));
    wr(lb, 0x84, 1);
}

// 006A7540 CSWGuiDialogLetterbox::SetBottom
void __fastcall kotor_fastcall_006a7540(void* lb, void*) {
    void* m = letterbox_manager(lb);
    const I h = static_cast<std::int16_t>(rd16(m, 0x6e));                                   // 006A7564 ebx
    const I a = scaled(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6c))), k_float_3_7);
    const I b = scaled(h - a, k_float_0_5);                                                 // 006A757B..006A758D
    wr(lb, 0x78, static_cast<U>(h));
    wr(lb, 0x68, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6e))) - b));
    wr(lb, 0x64, 0);
    wr(lb, 0x6c, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6c)))));
    wr(lb, 0x70, static_cast<U>(b));
    wr(lb, 8, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6e)))));
    wr(lb, 4, 0);
    wr(lb, 0xc, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6c)))));
    wr(lb, 0x10, static_cast<U>(b));
    wr(lb, 0x84, 2);
}

// 006A8A60 CSWGuiDialogLetterbox::SetMiddle
void __fastcall kotor_fastcall_006a8a60(void* lb, void*) {
    (void)letterbox_manager(lb);                                                            // 006A8A63..006A8A7A
    wr(lb, 0x84, 3);
    gui_load_panel_set_alpha_00404b00(lb, nullptr, 0.0f);                                   // 006A8A7D..006A8A8A
    void* m = rp(lb, 0x18);
    wr(lb, 0x94, 0); wr(lb, 0x98, 0); wr(lb, 0x9c, 0); wr(lb, 0xa0, 0); wr(lb, 8, 0); wr(lb, 4, 0);
    wr(lb, 0xc, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6c)))));
    wr(lb, 0x10, static_cast<U>(static_cast<I>(static_cast<std::int16_t>(rd16(m, 0x6e)))));
}

// 006A75E0 CSWGuiDialogLetterbox::ResetFade
void __fastcall kotor_fastcall_006a75e0(void* lb, void*) {
    gui_load_panel_set_alpha_00404b00(lb, nullptr, rd(lb, 0x84) == 3 ? 0.0f : 1.0f);       // 006A75E3..006A75F9
    wr(lb, 0x94, 0); wr(lb, 0x98, 0); wr(lb, 0x9c, 0); wr(lb, 0xa0, 0);
}

// 006A7620 CSWGuiDialogLetterbox::SetFade(on, a, b, Vector const& color) RET 10h
void __fastcall kotor_fastcall_006a7620(void* lb, void*, U on, U a, U b, const void* color) {
    std::memcpy(at(lb, 0x50), color, 12);                                                   // 006A7623..006A763E
    gui_load_panel_set_alpha_00404b00(lb, nullptr, on ? 1.0f : 0.0f);                      // 006A763C..006A764E
    wr(lb, 0xa0, on); wr(lb, 0x94, a); wr(lb, 0x98, b); wr(lb, 0x9c, 0x3dcccccdu);          // 0.1f
}

// 006A8400 CSWGuiDialogComputer::SetType(BYTE type) RET 4
void __fastcall kotor_fastcall_006a8400(void* computer, void*, U8 type) {
    void* mem = live_operator_new(0x54);                                                    // 006A841B..006A841F
    void* table = nullptr;
    if (mem) {
        ResRef ref; resref_from(&ref, "comptypes");                                         // 006A8439..006A8449 (by value)
        table = c2da_current_construct_00413cc0(mem, nullptr, ref, 0);                      // 006A844E..006A8450
    }
    // QUIRK: a null allocation is loaded through unchecked; a failed load or empty table leaks the C2DA
    if (!c2da_current_load_004143b0(table, nullptr)) return;                                // 006A845B..006A846C
    U8 row = type;                                                                          // 006A8472
    const I rows = rdi(table, 0x24);
    if (static_cast<I>(row) >= rows) {                                                      // 006A8479..006A847E
        if (rows == 0) return;                                                              // 006A8480..006A8482
        row = 0;
    }
    wr8(computer, 0x48, row);                                                               // 006A848A
    ExoString value;                                                                         // 006A8486..006A848D
    {
        ExoString column("ComputerBackground");                                             // 006A8492..006A84A3
        (void)c2da_current_get_string_00413270(table, nullptr, row, &column, &value);      // 006A84A8..006A84BD
    }
    ResRef background;
    (void)async_ref_construct_string_00406d60(&background, nullptr, &value);               // 006A84D0..006A84D9
    kotor_m15_panel_set_background_0040a870(computer, nullptr, &background);               // 006A84DE..006A84E5
    vcall<void>(table, 0, 1u);                                                              // 006A84EA..006A84F0 delete
}

// 006A9C60 CSWGuiBarkBubble::StopBark(stop_sound) RET 4
void __fastcall kotor_fastcall_006a9c60(void* bubble, void*, U stop_sound) {
    if (stop_sound && rd(bubble, 0x1b8)) {                                                  // 006A9C60..006A9C73
        kotor_fastcall_005d5b80(rp(bubble, 0x1b8), nullptr);                                // Stop
        if (void* s = rp(bubble, 0x1b8)) { kotor_fastcall_005d6200(s, nullptr); live_free(s); }
        wr(bubble, 0x1b8, 0);
    }
    U flags = rd(bubble, 0x44) & 0xfffffeffu;                                               // 006A9CA0..006A9CA3
    wr(bubble, 0x1b4, 0);
    wr(bubble, 0x44, flags);
    if ((flags & 0x600) != 0x400) wr(bubble, 0x44, (flags & 0xfffffbffu) | 0x200);          // 006A9CA8..006A9CCF
}

// 00628670 CGuiInGame::SetConversationType(type, computer type) RET 8
void __fastcall kotor_fastcall_00628670(void* gui, void*, U type, U computer) {
    if (type == 1) {                                                                        // 0062867E..0062868D
        if (!rd(gui, 0x44)) {                                                               // 0062868F..00628694
            void* mem = live_operator_new(0x3620);
            wp(gui, 0x44, mem ? kotor_fastcall_006a8eb0(mem, nullptr, manager_of(gui)) : nullptr);
        }
        wr(gui, 0x3c, rd(gui, 0x44));                                                       // 006286CD..006286D5
        kotor_fastcall_006a8400(rp(gui, 0x44), nullptr, static_cast<U8>(computer));        // (callee reads the low byte)
        return;
    }
    wr(gui, 0x3c, rd(gui, 0x40));                                                           // 006286EF..006286F6
}

// 00628750 CGuiInGame::SetReplyData(index, text, ... 19 args) RET 4Ch
void __fastcall kotor_fastcall_00628750(void* gui, void*, U i, void* text, U a3, U a4, U a5, U a6, U f7, U f8, U a9,
    U w10, U f11, U a12, U b13, const void* v14, U f15, U f16, I n17, const U* p18, const U16* p19) {
    if (i >= rd(gui, 0x114)) return;                                                        // 00628758..0062875E
    str_assign(at(rp(gui, 0x118), i * 8), text);                                            // 00628764..00628773
    // float slots are fld/fstp dword moves (bit copies for finite values)
    wr(rp(gui, 0x11c), i * 4, a3); wr(rp(gui, 0x120), i * 4, a4); wr(rp(gui, 0x124), i * 4, a5);
    wr(rp(gui, 0x128), i * 4, a6); wr(rp(gui, 0x12c), i * 4, f7); wr(rp(gui, 0x130), i * 4, f8);
    wr(rp(gui, 0x134), i * 4, a9); wr16(rp(gui, 0x138), i * 2, static_cast<U16>(w10));
    wr(rp(gui, 0x13c), i * 4, f11); wr(rp(gui, 0x140), i * 4, a12); wr8(rp(gui, 0x144), i, static_cast<U8>(b13));
    std::memcpy(at(rp(gui, 0x148), i * 12), v14, 12);                                       // 0062880D..00628830
    wr(rp(gui, 0x14c), i * 4, f15); wr(rp(gui, 0x150), i * 4, f16); wr(rp(gui, 0x154), i * 4, static_cast<U>(n17));
    if (n17) {                                                                              // 00628852
        wp(rp(gui, 0x158), i * 4, live_operator_new(static_cast<U>(n17) * 4));
        wp(rp(gui, 0x15c), i * 4, live_operator_new(static_cast<U>(n17) * 2));
    } else {
        wr(rp(gui, 0x158), i * 4, 0);
        wr(rp(gui, 0x15c), i * 4, 0);
    }
    for (I j = 0; j < n17; ++j) {                                                           // 0062889B..006288D8 (signed)
        wr(rp(rp(gui, 0x158), i * 4), static_cast<U>(j) * 4, p18[j]);
        wr16(rp(rp(gui, 0x15c), i * 4), static_cast<U>(j) * 2, p19[j]);
    }
}

// 0062ADD0 CGuiInGame::DialogNodeSpanningTextParse(CExoString* out (hidden), CExoString const& text) RET 8
void* __fastcall kotor_fastcall_0062add0(void* gui, void*, void* out, void* text) {
    const I pending = rdi(gui, 0xb50);
    if (pending > 0) {                                                                      // 0062ADF2..0062ADFE
        const U idx = rd(gui, 0xb54) - static_cast<U>(pending);
        wr(gui, 0xb50, static_cast<U>(pending - 1));
        (void)WindowCopy005e5b00(out, nullptr, at(gui, 0xb58 + idx * 8));                  // 0062AE00..0062AE1D
        return out;
    }
    bool found;
    {
        ExoString marker("##");                                                             // 0062AE29..0062AE32
        found = client_movies::StringFind_005e5eb0(static_cast<options_video::String*>(text), nullptr,
            reinterpret_cast<options_video::String*>(&marker), 0) != -1;                    // 0062AE37..0062AE53
    }
    if (!found) { (void)WindowCopy005e5b00(out, nullptr, text); return out; }               // 0062AE63 -> 0062AFD9
    wr(gui, 0xb54, 0);                                                                       // 0062AE6D
    U overflow = 0;                                                                          // [esp+14]
    char* const buffer = static_cast<char*>(live_operator_new(async_string_length_005e5790(text, nullptr) + 1));
    {
        const char* src = WindowCStr005e5670(text, nullptr);                                // 0062AE86..0062AE99 strcpy
        char* d = buffer;
        char c;
        do { c = *src++; *d++ = c; } while (c);
    }
    buffer[async_string_length_005e5790(text, nullptr)] = 0;                                // 0062AE9B..0062AEA2
    char* s = buffer;
    I i = 0;
    if (s[0] != 0) {                                                                         // 0062AEA6..0062AEB0
        for (;;) {
            char next;
            bool split;
            if (s[i] == '#' && s[i + 1] == '#') { next = '#'; split = true; }               // 0062AEB6..0062AEC2
            else { next = s[i + 1]; split = (next == 0); }                                   // 0062AEC4..0062AECA
            if (!split) { ++i; if (s[i] == 0) break; continue; }                             // 0062AF7A..0062AF7F
            if (i == 0 && s[1] != 0) { overflow = 1; break; }                                // 0062AED0..0062AF87
            const I n = next ? i - 1 : i + 1;                                                // 0062AEDF..0062AEE6
            char* piece = static_cast<char*>(live_operator_new(static_cast<U>(n + 1)));     // 0062AEE9..0062AEED
            kotor_runtime_reporter_dependencies::string_copy_n_006fab80(piece, s, static_cast<U>(n));
            piece[n] = 0;                                                                     // 0062AF04
            ExoString segment(piece);                                                         // 0062AEFF..0062AF08
            if (piece) live_free(piece);                                                      // 0062AF0D..0062AF1B
            const U k = rd(gui, 0xb54);
            if (k == 8) { overflow = 1; break; }                                              // 0062AF2D..0062AF91
            wr(gui, 0xb54, k + 1);
            str_assign(at(gui, 0xb58 + k * 8), &segment);                                     // 0062AF32..0062AF45
            if (s[i + 1] == 0) { ++i; }                                                       // 0062AF4A..0062AF62
            else if (s[i + 2] == 0) { break; }                                                // 0062AF52..0062AF58 (no flag)
            else { s = s + i + 3; i = 0; }                                                    // 0062AF5A..0062AF5E
            if (s[i] == 0) break;                                                             // 0062AF7B..0062AF7F
        }
    }
    live_free(buffer);                                                                        // 0062AFAA..0062AFAF
    if (overflow == 0) {                                                                      // 0062AFB4..0062AFBD
        wr(gui, 0xb50, rd(gui, 0xb54) - 1);                                                  // 0062AFBF..0062AFCC
        (void)WindowCopy005e5b00(out, nullptr, at(gui, 0xb58));
    } else {
        (void)WindowCopy005e5b00(out, nullptr, text);                                        // 0062AFD5..0062AFE0
    }
    return out;
}

// 0062B730 CGuiInGame::ShowDialogEntry(CExoString* text, a2, a3) RET 0Ch (a3 unused)
void __fastcall kotor_fastcall_0062b730(void* gui, void*, void* text, U a2, U) {
    if (rd(gui, 0x114) <= 1) kotor_fastcall_0061f9c0(client_internal(), nullptr, 0x10);   // 0062B733..0062B749 HideMouse
    if (rd(gui, 0xb0)) {
        vcall<void>(rp(gui, 0x3c), 0x70);                                                    // 0062B758..0062B75D
    } else {
        wr(gui, 0xb0, 1);                                                                    // 0062B762
        kotor_fastcall_005eda60(client(), nullptr, 3, 1);                                    // 0062B76C..0062B778
        kotor_fastcall_005edba0(client(), nullptr);                                          // ResetDriveAcceleration
        kotor_fastcall_006a9c60(rp(gui, 0x4c), nullptr, 1);                                  // 0062B78B..0062B790 StopBark
    }
    vcall<void>(rp(gui, 0x3c), 0x78, a2);                                                    // 0062B795..0062B79F
    if (!frame_ContainsPanel_0040bd70(manager_of(gui), nullptr, rp(gui, 0x3c))) {           // 0062B7A2..0062B7B0
        U mode = 0;
        if (rd(gui, 0x3c) == rd(gui, 0x40)) {                                                // 0062B7B6..0062B7C1
            if (k_disable_letterbox_0083390c == 0) {                                         // 0062B7C3..0062B7CA
                kotor_fastcall_006a8a60(rp(gui, 0x68), nullptr);
                kotor_fastcall_0040bc70(manager_of(gui), nullptr, rp(gui, 0x68), 0, 1);
                kotor_fastcall_006a74b0(rp(gui, 0x60), nullptr);
                kotor_fastcall_0040bc70(manager_of(gui), nullptr, rp(gui, 0x60), 0, 1);
                kotor_fastcall_006a7540(rp(gui, 0x64), nullptr);
                kotor_fastcall_0040bc70(manager_of(gui), nullptr, rp(gui, 0x64), 0, 1);
            }
        } else if (rd(gui, 0x3c) == rd(gui, 0x44)) {
            mode = 2;                                                                         // 0062B813..0062B818
        }
        kotor_fastcall_0040bc70(manager_of(gui), nullptr, rp(gui, 0x3c), mode, 1);          // 0062B81D..0062B827
        frame_R4_0062aa00(gui, nullptr, 3, 1);                                               // SetSWGuiStatus(3, 1)
    }
    vcall<void>(rp(gui, 0x3c), 0x7c, text);                                                  // 0062B838..0062B842
}

// 0062BC70 CGuiInGame::InitializeRepliesData(count) RET 4
U __fastcall kotor_fastcall_0062bc70(void* gui, void*, U count) {
    kotor_fastcall_0062ba30(gui, nullptr);                                                   // ClearRepliesData
    wr(gui, 0x114, count);
    void* mem = live_operator_new(count * 8 + 4);                                            // 0062BC96..0062BCA4
    void* strings = nullptr;
    if (mem) {
        wr(mem, 0, count);
        strings = at(mem, 4);
        crt_eh_vector_constructor_006fb55f(strings, 8, static_cast<I>(count),
            reinterpret_cast<void*>(&kotor_menu20::ConstructString005b3190),
            reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));                         // 0062BCB8..0062BCCB
    }
    wp(gui, 0x118, strings);
    auto alloc = [&](U off, U scale) { wp(gui, off, live_operator_new(rd(gui, 0x114) * scale)); };
    alloc(0x11c, 4); alloc(0x120, 4); alloc(0x124, 4); alloc(0x128, 4); alloc(0x12c, 4); alloc(0x130, 4);
    alloc(0x134, 4); alloc(0x138, 2); alloc(0x13c, 4); alloc(0x140, 4); alloc(0x144, 1);
    {
        const U n = rd(gui, 0x114);                                                          // 0062BDBF..0062BDD2
        void* v = live_operator_new(n * 12);
        if (v) for (I k = 0; k <= static_cast<I>(n) - 1; ++k) {                              // 0062BDDE..0062BDFC
            wr(v, static_cast<U>(k) * 12, 0); wr(v, static_cast<U>(k) * 12 + 4, 0); wr(v, static_cast<U>(k) * 12 + 8, 0);
        }
        wp(gui, 0x148, v);
    }
    alloc(0x14c, 4); alloc(0x150, 4); alloc(0x154, 4); alloc(0x158, 4); alloc(0x15c, 4);
    for (U off = 0x118; off <= 0x154; off += 4) if (!rd(gui, off)) return 0;                 // 0062BE6B..0062BEFC
    // QUIRK: the first elements of the fresh (uninitialised) +158/+15C arrays are tested
    if (!rd(rp(gui, 0x158), 0)) return 0;                                                    // 0062BEFE..0062BF06
    if (!rd(rp(gui, 0x15c), 0)) return 0;                                                    // 0062BF08..0062BF0A
    return 1;
}

// 0062C6F0 CGuiInGame::UnloadStuntModelsForDialog
void __fastcall kotor_fastcall_0062c6f0(void* gui, void*) {
    U anim = word(gui);  // edi starts as `this` (0062C72C); only read after one of the branches assigns it
    for (U k = 0; k < rd(gui, 0x18c); ++k) {                                                 // 0062C712..0062C95A
        void* o = client_object(rd(rp(gui, 0x190), k * 4));                                  // 0062C730..0062C742
        if (!o) continue;
        void* obj = vcall<void*>(o, 0xc);                                                    // 0062C74F..0062C753
        if (!obj) continue;
        void* stunt_a = vcall<void*>(obj, 0x98, 0xffu, 0u);                                  // 0062C760..0062C76B
        if (stunt_a) { vcall<void>(stunt_a, 0x18, 0u, 1.0f, 0u, 0u); vcall<void>(stunt_a, 0x24); }
        void* stunt_b = vcall<void*>(obj, 0x98, 0xfeu, 0u);                                  // 0062C794..0062C79F
        if (stunt_b) { vcall<void>(stunt_b, 0x18, 0u, 1.0f, 0u, 0u); vcall<void>(stunt_b, 0x24); }
        ExoString name;                                                                       // 0062C7C6..0062C7CA
        if (vcall<void*>(obj, 0x2c)) {                                                       // 0062C7CF..0062C7E0 creature
            RawStr tmp;  // hidden-return temporary constructed by the callee
            void* g = gob_get_property_0x68_0063c070(obj, nullptr);
            str_assign(&name, vcall<void*>(g, 0x40, static_cast<void*>(&tmp), 0x2710u));    // 0062C7E9..0062C804
            anim = 0x2710;
        }
        if (vcall<void*>(obj, 0x4c)) {                                                       // 0062C81C..0062C825 placeable
            void* server_placeable = kotor_fastcall_00682e00(vcall<void*>(obj, 0x4c), nullptr);
            if (server_placeable) {                                                          // 0062C839..0062C83F
                anim = rd(server_placeable, 0xd4);                                           // 0062C841
                RawStr tmp;
                void* g = gob_get_property_0x68_0063c070(vcall<void*>(obj, 0x4c), nullptr);
                str_assign(&name, vcall<void*>(g, 0x40, static_cast<void*>(&tmp), anim));
            } else {
                RawStr tmp;
                void* g = gob_get_property_0x68_0063c070(vcall<void*>(obj, 0x4c), nullptr);
                str_assign(&name, vcall<void*>(g, 0x40, static_cast<void*>(&tmp), 0x2710u));
                anim = 0x2710;
            }
        }
        if (stunt_a) vcall<void>(stunt_a, 0x18, WindowCStr005e5670(&name, nullptr), 1.0f, 2u, 0u); // 0062C8BA..0062C8D7
        if (stunt_b) vcall<void>(stunt_b, 0x18, WindowCStr005e5670(&name, nullptr), 1.0f, 2u, 0u); // 0062C8DA..0062C8F6
        if (vcall<void*>(obj, 0x2c)) play(obj, anim, 0, 0);                                  // 0062C8F9..0062C90D
        if (vcall<void*>(obj, 0x4c)) play(vcall<void*>(obj, 0x4c), anim, 0, 0);             // 0062C913..0062C92E
    }
    if (void* p = rp(gui, 0x190)) { live_free(p); wr(gui, 0x190, 0); }                      // 0062C960..0062C975
    if (void* p = rp(gui, 0x194)) { live_free(p); wr(gui, 0x194, 0); }                      // 0062C97B..0062C98E
    wr(gui, 0x18c, 0);
}

// 00631B70 CGuiInGame::ResetDialogAnimations
void __fastcall kotor_fastcall_00631b70(void* gui, void*) {
    for (U k = 0; k < rd(gui, 0x19c); ++k) {                                                 // 00631B73..00631C55
        void* o = client_object(rd(rp(gui, 0x1a8), k * 4));
        if (!o) continue;
        if (vcall<void*>(o, 0x2c)) {                                                         // 00631BB2..00631BBB
            void* c = vcall<void*>(o, 0x2c);
            if (kotor_fastcall_0062f0e0(gui, nullptr, rd(c, 4))) {                          // 00631BC6..00631BD3
                wr(c, 0x54, 0);
                if (!anim_is_none(c)) play(c, 0x2710, 0, 1);                                 // 00631BDE..00631BF6
                gob70(c, 0);                                                                 // 00631BFC..00631C03
            }
        }
        if (vcall<void*>(o, 0x4c)) {                                                         // 00631C06..00631C0F
            void* p = vcall<void*>(o, 0x4c);
            void* sp = kotor_fastcall_00682e00(p, nullptr);
            if (!p) continue;  // QUIRK: tests the client placeable, then reads the unchecked server placeable
            wr(p, 0x54, 0);
            play(p, rd16(sp, 0xd4), 0, 0);                                                   // 00631C25..00631C3C
            gob70(p, 0);
        }
    }
}

// 00631C60 CGuiInGame::ResetTalkAnimations: participants +178 and +17C
void __fastcall kotor_fastcall_00631c60(void* gui, void*) {
    reset_participant(gui, rd(gui, 0x178));
    reset_participant(gui, rd(gui, 0x17c));
}

// 006313A0 CGuiInGame::SetDialogAnimations (replaces the trap @kotor_fastcall_006313a0@8 in 0783-populate-removed)
void __fastcall kotor_fastcall_006313a0(void* gui, void*) {
    for (U k = 0; k < 3; ++k) {                                                              // 006313A9..006313FC
        const U off = 0xbf0 + k * 4;
        void* o = client_object(rd(gui, off));
        if (o && vcall<void*>(o, 0x2c)) { gob70(vcall<void*>(o, 0x2c), 0); wr(gui, off, kInvalid); }
    }
    U keep_17c = 0, keep_178 = 0;                                                            // [esp+14] / [esp+18]
    for (I k = 0; k < rdi(gui, 0x19c); ++k) {                                                // 006313FE..00631537 (signed)
        const U id = rd(rp(gui, 0x1a8), static_cast<U>(k) * 4);
        void* o = client_object(id);
        if (!o) continue;
        U still = 0;
        for (I j = 0; j < rdi(gui, 0x198); ++j)                                              // 00631442..00631464
            if (id == rd(rp(gui, 0x1a0), static_cast<U>(j) * 4)) { still = 1; break; }
        if (id == rd(gui, 0x17c)) keep_17c = 1;                                              // 00631469..00631471
        if (id == rd(gui, 0x178)) keep_178 = 1;                                              // 00631479..00631481
        if (still) continue;
        if (vcall<void*>(o, 0x2c)) {                                                         // 00631491..0063149A
            void* c = vcall<void*>(o, 0x2c);
            if (kotor_fastcall_0062f0e0(gui, nullptr, rd(c, 4))) {
                wr(c, 0x54, 0);
                if (!anim_is_none(c)) play(c, 0x2710, 0, 1);
                gob70(c, 0);
            }
        }
        if (vcall<void*>(o, 0x4c)) {                                                         // 006314E0..006314E9
            void* p = vcall<void*>(o, 0x4c);
            void* sp = kotor_fastcall_00682e00(p, nullptr);
            if (!p) continue;  // QUIRK as in 00631C21
            wr(p, 0x54, 0);
            play(p, rd16(sp, 0xd4), 0, 0);
            gob70(p, 0);
        }
    }
    if (!keep_17c) reset_participant(gui, rd(gui, 0x17c));                                   // 0063153D..006315CC
    if (!keep_178) reset_participant(gui, rd(gui, 0x178));                                   // 006315CF..0063165E
    U keep_174 = 0, keep_170 = 0;                                                            // [esp+18] / [esp+1C]
    for (I k = 0; k < rdi(gui, 0x198); ++k) {                                                // 00631661..0063193C (signed)
        const U id = rd(rp(gui, 0x1a0), static_cast<U>(k) * 4);
        const U anim = rd16(rp(gui, 0x1a4), static_cast<U>(k) * 2);                          // di
        void* o = client_object(id);
        if (!o) continue;
        U was = 0;
        U16 previous = 0;                                                                     // [esp+20] (only read when found)
        for (I j = 0; j < rdi(gui, 0x19c); ++j)                                              // 006316BD..006316EE
            if (id == rd(rp(gui, 0x1a8), static_cast<U>(j) * 4)) {
                previous = rd16(rp(gui, 0x1ac), static_cast<U>(j) * 2); was = 1; break;
            }
        if (id == rd(gui, 0x174)) keep_174 = 1;                                              // 006316F3..006316FB
        if (id == rd(gui, 0x170)) keep_170 = 1;                                              // 00631703..0063170B
        if (was && static_cast<U16>(anim) == previous) continue;                             // 00631713..0063171C
        if (vcall<void*>(o, 0x2c)) {                                                         // 00631722..0063172D
            void* c = vcall<void*>(o, 0x2c);
            if (c && kotor_fastcall_0062f0e0(gui, nullptr, rd(c, 4))) {                     // 0063173C..00631755
                if (kotor_fastcall_0062d5e0(gui, nullptr, anim)) {                          // 0063175B..00631765 looping
                    wr(c, 0x54, 0); gob70(c, 1);                                             // 00631767..00631771
                    kotor_fastcall_006146e0(c, nullptr, kInvalid, 0.0f);                     // LookAt(invalid, 0)
                    if (!anim_is_none(c)) {                                                  // 00631781..0063178C
                        if (kotor_fastcall_0062d820(gui, nullptr, anim))
                            kotor_m15a_object_abort_fire_forget_animation_0063cad0(c, nullptr, 0);
                        play(c, 0x2710, 0, 1);                                               // 006317A2..006317AE
                    }
                    play(c, anim, 0, 0);                                                     // 006317B4..006317BB
                }
                if (kotor_fastcall_0062d700(gui, nullptr, anim)) {                          // 006317C1..006317CB fire&forget
                    wr(c, 0x54, 0); gob70(c, 1);
                    kotor_fastcall_006146e0(c, nullptr, kInvalid, 0.0f);
                    if (!anim_is_none(c)) play(c, 0x2710, 0, 1);                             // 006317E7..00631800
                    if (static_cast<U16>(anim) >= 0x3e8 && static_cast<U16>(anim) <= 0x52f) { // 00631806..00631812
                        kotor_m15a_object_abort_fire_forget_animation_0063cad0(c, nullptr, 0);
                        kotor_fastcall_0063cab0(c, nullptr);
                        vcall<void>(c, 0xa0, anim, 0u);                                      // 00631823..00631829
                        kotor_fastcall_0061a670(c, nullptr);                                 // AnimateFireAndForget
                    } else {
                        vcall<void>(c, 0xa0, anim, 0u);                                      // 00631838..0063183E
                    }
                }
            }
        }
        if (vcall<void*>(o, 0x4c)) {                                                         // 00631844..0063184F
            void* p = vcall<void*>(o, 0x4c);
            if (!p) continue;
            if (kotor_fastcall_0062d5e0(gui, nullptr, anim)) {                              // 00631868..00631872
                wr(p, 0x54, 0); gob70(p, 1);
                if (!anim_is_none(p)) play(p, 0x2710, 0, 0);                                 // 00631885..0063189F
                play(p, anim, 0, 0);
            }
            if (kotor_fastcall_0062d700(gui, nullptr, anim)) {                              // 006318B4..006318BE
                wr(p, 0x54, 0); gob70(p, 1);
                if (!anim_is_none(p)) play(p, 0x2710, 0, 0);
                if (static_cast<U16>(anim) >= 0x3e8 && static_cast<U16>(anim) <= 0x52f) {   // 006318F1..006318FD
                    kotor_m15a_object_abort_fire_forget_animation_0063cad0(p, nullptr, 0);
                    kotor_fastcall_0063cab0(p, nullptr);
                    vcall<void>(p, 0xa0, anim, 0u);
                } else {
                    vcall<void>(p, 0xa0, anim, 0u);
                }
            }
        }
    }
    // talk idle for the current participants not driven by an explicit animation
    auto talk = [&](U id, U healthy, U injured) {                                            // 0063194E.. / 00631A3C..
        void* o = client_object(id);
        if (!o || !vcall<void*>(o, 0xc) || !vcall<void*>(o, 0x2c)) return;
        if (!kotor_fastcall_0062f0e0(gui, nullptr, rd(vcall<void*>(o, 0x2c), 4))) return;
        gob70(vcall<void*>(o, 0xc), 1);
        if (!anim_is_none(vcall<void*>(o, 0xc))) {
            kotor_m15a_object_abort_fire_forget_animation_0063cad0(vcall<void*>(o, 0xc), nullptr, 0);
            play(vcall<void*>(o, 0xc), 0x2710, 0, 1);
        }
        const U hurt = kotor_fastcall_00614b60(vcall<void*>(o, 0x2c), nullptr);              // IsInjured
        play(vcall<void*>(o, 0xc), hurt ? injured : healthy, 0, 0);
    };
    if (!keep_174) talk(rd(gui, 0x174), 0x272e, 0x27ab);                                      // 00631942..00631A2A
    if (!keep_170) talk(rd(gui, 0x170), 0x2736, 0x27aa);                                      // 00631A30..00631B18
    for (U k = 0; k < 3; ++k) {                                                              // 00631B1E..00631B5D
        void* pc = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr, k);
        if (pc) { gob70(pc, 1); wr(gui, 0xbf0 + k * 4, rd(pc, 4)); }
        else wr(gui, 0xbf0 + k * 4, kInvalid);
    }
}

// 00631D80 CGuiInGame::HandleDialogEntry(23 args) RET 5Ch; returns 1
U __fastcall kotor_fastcall_00631d80(void* gui, void*, U a1, U a2, U a3, U a4, U a5, U a6, void* a7, U a8,
    const U* a9, const U16* a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18, U a19, U a20, U a21,
    U a22, U a23) {
    (void)a1; (void)a6;
    if (rd(in_game_gui(), 0xb98)) {                                                          // 00631DA5..00631DB2
        wr(in_game_gui(), 0xb98, 0);                                                         // 00631DBD..00631DC2
        U color[3] = {0, 0, 0};                                                              // 00631DD2..00631DE2
        void* g = in_game_gui();                                                             // 00631DED..00631DF2
        if (rd(g, 0x6c)) {                                                                   // 00631DF4..00631DF9
            kotor_fastcall_0040bc70(rp(g, 0x38), nullptr, rp(g, 0x6c), 4, 1);               // 00631DFB..00631E07
            kotor_m15_panel_set_visible_0040b9d0(rp(g, 0x6c), nullptr, 1);                  // 00631E0C..00631E10
            kotor_m15_fade_set_fade_006244e0(rp(g, 0x6c), nullptr, 1, 0, 1.0f, color);      // 00631E15..00631E25
        }
    }
    kotor_fastcall_00630620(gui, nullptr, a8);                                               // 00631E31..00631E38
    for (U k = 0; k < a8; ++k) {                                                             // 00631E3D..00631E87
        const U16 anim = a10[k];
        const U id = a9[k];
        if (k < rd(gui, 0x198) && rd(gui, 0x1a0) && rd(gui, 0x1a4)) {
            wr(rp(gui, 0x1a0), k * 4, id);
            wr16(rp(gui, 0x1a4), k * 2, anim);
        }
    }
    RawStr text;
    (void)WindowCopy005e5b00(&text, nullptr, a7);                                            // 00631E8F..00631E98
    tlk_parse(&text);                                                                        // 00631E9D..00631F05
    {
        RawStr parsed;  // hidden return of DialogNodeSpanningTextParse
        str_assign(&text, kotor_fastcall_0062add0(gui, nullptr, &parsed, &text));            // 00631F0A..00631F25
    }
    kotor_fastcall_00630560(gui, nullptr, &text, a3);                                        // 00631F38..00631F44
    kotor_fastcall_0062d990(gui, nullptr);                                                   // CleanUpReplyNode
    if (!(rd8(kotor_fastcall_005ed700(client(), nullptr), 0x14) & 4)) {                      // 00631F50..00631F61
        if (kotor_cell_disable_sound_00832894 != 0) {
            wr(gui, 0xbb0, 1);                                                               // 00631F91
        } else if (rd(gui, 0xc0) == 0) {
            const U panel = rd(gui, 0x3c);
            if (panel != 0 && panel == rd(gui, 0x40)) wr(gui, 0xbb0, a23);                   // 00631F82..00631F89
            else wr(gui, 0xbb0, 1);
        }
    }
    kotor_fastcall_0062b730(gui, nullptr, &text, a2, a3);                                    // 00631F97..00631FA4
    kotor_fastcall_0062db50(gui, nullptr);                                                   // UnlockDialogParticipantOrientations
    if (rd(gui, 0x184) == kInvalid) {                                                        // 00631FB0..00631FBB
        wr(gui, 0x184, a5);
        if (a4 == kInvalid) { wr(gui, 0x17c, rd(gui, 0x174)); wr(gui, 0x174, a5); }          // 00632022..0063202E
        else { wr(gui, 0x17c, rd(gui, 0x174)); wr(gui, 0x174, a4); }                         // 00631FD3..00631FDF
    } else if (a4 != kInvalid) {
        wr(gui, 0x17c, rd(gui, 0x174)); wr(gui, 0x174, a4);                                  // 00632022..0063202E
    } else {
        const U current = rd(gui, 0x170);
        if (a3 == current) { wr(gui, 0x17c, rd(gui, 0x174)); wr(gui, 0x174, a5); }          // 00631FFC..0063200C
        else { wr(gui, 0x17c, rd(gui, 0x174)); wr(gui, 0x174, current); }                    // 00632014..0063202E
    }
    wr(gui, 0x178, rd(gui, 0x170));                                                          // 00632034..00632042
    wr(gui, 0x170, a3);
    kotor_fastcall_006313a0(gui, nullptr);                                                   // SetDialogAnimations
    const U8 fade = static_cast<U8>(a19);                                                    // 0063204D..00632056
    if (fade != 0) {
        const U on = (fade == 4 || fade == 1) ? 0u : 1u;
        if (rd(gui, 0x68))
            kotor_fastcall_006a7620(rp(gui, 0x68), nullptr, on, a21, a22, ptr(a20));         // 00632064..00632084
    }
    const U panel = rd(gui, 0x3c);
    if (panel != 0 && panel == rd(gui, 0x40)) {                                              // 00632089..00632093
        if (!kotor_fastcall_0062bf70(gui, nullptr, a16, a17)) {                              // 00632095..006320A8
            if (!rd(gui, 0xb20)) {                                                           // 006320AE..006320B6
                if (void* m = kotor_fastcall_005ed530(client(), nullptr))
                    kotor_fastcall_006412f0(m, nullptr, rd(gui, 0xc4));
                wr(gui, 0xb20, 1);
            }
            kotor_fastcall_006306e0(gui, nullptr, a11, a12, a13, a14, a15, a18);             // 006320DD..006320FD
        }
    } else if (a11 == 6) {                                                                   // 00632104..00632109
        if (!rd(gui, 0xb20)) {
            if (void* m = kotor_fastcall_005ed530(client(), nullptr))
                kotor_fastcall_006412f0(m, nullptr, rd(gui, 0xc4));
            wr(gui, 0xb20, 1);
        }
        kotor_fastcall_0062c030(gui, nullptr, a12, a15, a18);                                // 0063213B..0063214C
    }
    return 1;
}

// 006332B0 CGuiInGame::CloseDialog; returns +B0 (0) or 0 when no dialog is open
U __fastcall kotor_fastcall_006332b0(void* gui, void*) {
    if (!rd(gui, 0x108)) return 0;                                                           // 006332B7..006332C1
    kotor_fastcall_005edf20(client(), nullptr);                                              // DisableVideoEffect
    kotor_fastcall_0062f8b0(gui, nullptr);                                                   // ClearDialogNonDependantList
    wr(gui, 0xbd8, 0);
    if (rd(gui, 0xba4) != kInvalid) {                                                        // 006332DD..006332F0
        if (!frame_ContainsPanel_0040bd70(manager_of(gui), nullptr, rp(gui, 0x78))) {       // 006332F6..00633304
            for (I k = 0; k < rdi(kotor_fastcall_005ed8b0(client(), nullptr), 0); ++k) {     // 00633311..00633374
                void* pc = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr, 0);
                if (pc && kotor_fastcall_004aea30(server(), nullptr, rd(pc, 4)) == rd(gui, 0xba4)) break;
                kotor_fastcall_005edc30(client(), nullptr, -2, 0);                           // 00633353..0063335F
            }
        }
        wr(gui, 0xba4, kInvalid);                                                            // 00633376
    }
    for (U off : {0x170u, 0x174u}) {                                                         // 0063337C..006333E3
        void* o = client_object(rd(gui, off));
        if (o && vcall<void*>(o, 0x2c)) kotor_fastcall_006146e0(vcall<void*>(o, 0x2c), nullptr, kInvalid, 0.0f);
    }
    wr(gui, 0x178, rd(gui, 0x170)); wr(gui, 0x170, kInvalid);                                // 006333E8..00633408
    wr(gui, 0x17c, rd(gui, 0x174)); wr(gui, 0x174, kInvalid);
    kotor_fastcall_00631b70(gui, nullptr);                                                   // ResetDialogAnimations
    kotor_fastcall_00631c60(gui, nullptr);                                                   // ResetTalkAnimations
    wr(gui, 0xb0, 0);
    kotor_fastcall_0062ec60(gui, nullptr, 0);                                                // SetGlobalDialogState(0)
    void* panel = rp(gui, 0x3c);
    if (word(panel) == rd(gui, 0x44)) {                                                      // 00633428..0063342E computer
        U f = rd(panel, 0x44);
        if (w4::ingame_gui_single_panel_007a2370 == 0) {                                     // 00633430..00633436
            wr(panel, 0x44, (f & 0xfffffdffu) | 0x500);
        } else {
            f |= 0x100; wr(panel, 0x44, f);                                                  // 0063344F..00633466
            if ((f & 0x600) != 0x400) wr(panel, 0x44, (f & 0xfffffbffu) | 0x200);
        }
        wr(gui, 0x44, 0);                                                                    // 0063344A / 0063347A
    } else {
        (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, panel);                   // 0063347F..00633483
    }
    (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, rp(gui, 0x48));              // 00633488..0063348F
    const bool camera = rd(gui, 0x3c) == rd(gui, 0x40);                                      // 00633494..00633497
    wr(gui, 0xbb4, 0);
    if (camera) {
        if (k_disable_letterbox_0083390c == 0) {                                             // 006334A2..006334A8
            (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, rp(gui, 0x60));
            (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, rp(gui, 0x64));
            (void)frame_RemovePanel_0040c830(manager_of(gui), nullptr, rp(gui, 0x68));
        }
        for (U off : {0x60u, 0x64u, 0x68u}) {                                                // 006334CE..006334F3
            wr(rp(gui, off), 0x88, 0);
            kotor_fastcall_006a75e0(rp(gui, off), nullptr);
        }
    }
    kotor_fastcall_006a6d70(rp(gui, 0x3c), nullptr);                                         // 006334F8..006334FB EndDialog
    wr(gui, 0x3c, rd(gui, 0x40));
    kotor_fastcall_0062c2a0(gui, nullptr);                                                   // EndDialogCamera
    kotor_fastcall_0062c6f0(gui, nullptr);                                                   // UnloadStuntModelsForDialog
    kotor_fastcall_0062ba30(gui, nullptr);                                                   // ClearRepliesData
    wr(gui, 0x198, 0);
    for (U off : {0x1a0u, 0x1a4u}) if (void* p = rp(gui, off)) { live_free(p); wr(gui, off, 0); }
    wr(gui, 0x19c, 0);
    for (U off : {0x1a8u, 0x1acu}) if (void* p = rp(gui, off)) { live_free(p); wr(gui, off, 0); }
    wr(gui, 0xb50, 0); wr(gui, 0xb54, 0);                                                    // 0063358B..00633591
    if (gui_m48_client_get_input_class_005eda70(client(), nullptr) == 3) {                   // 00633597..006335A7
        kotor_fastcall_005eda60(client(), nullptr, 0, 1);
        frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, k_sound_mode_0074c5ec);        // 006335BA..006335C7
    }
    kotor_fastcall_005edba0(client(), nullptr);                                              // ResetDriveAcceleration
    if (rd(gui, 0xb14)) {                                                                    // 006335D9..006335DF
        ResRef sound; resref_default(&sound);                                                // 006335E1..006335E5
        if (rd(gui, 0x108)) {                                                                // 006335EA..006335F0
            object_projectile_bark_006a9920(rp(gui, 0x4c), nullptr, kInvalid, at(gui, 0xb18), &sound, 0);
            kotor_fastcall_0040bc70(manager_of(gui), nullptr, rp(gui, 0x4c), 4, 1);          // 00633608..00633613
        }
        wr(gui, 0xb14, 0);
        (void)frame_LogAssign_005e5140(at(gui, 0xb18), nullptr, "");                         // 00633618..00633629
    }
    kotor_fastcall_005ee200(client(), nullptr, kInvalid);                                    // SetLastClickedOnTarget
    return rd(gui, 0xb0);                                                                    // 0063363D
}

// 006340E0 CGuiInGame::ShowDialogReplies(a1, a2) RET 8
void __fastcall kotor_fastcall_006340e0(void* gui, void*, U a1, U a2) {
    const U n = rd(gui, 0x114);
    if (n > 1 || (n != 0 && client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(rp(gui, 0x118)), nullptr, "")))
        kotor_fastcall_0061fa00(client_internal(), nullptr, 0xffff);                         // 006340E3..00634116 ShowMouse
    if (!rd(gui, 0x3c)) return;                                                              // 0063411B..00634120
    const U me = player_client_id();                                                         // 00634126..00634157
    void* strings = rp(gui, 0x118);
    if (rd(gui, 0x114) == 1 &&
        static_cast<kotor_reconstruction::StartupString*>(strings)->BorrowedEquals005e5290("")) { // 00634159..00634178
        vcall<void>(rp(gui, 0x3c), 0x74, rd(gui, 0x114), rd(gui, 0x118), rd(gui, 0x11c), a1, 0u); // 0063417E..0063419F
        wr(rp(gui, 0x3c), 0x64, a2);                                                         // 006341A2..006341A9
        vcall<void>(rp(gui, 0x3c), 0x7c, rp(gui, 0x118));                                    // 006341AC..006341BD
        const U has_text = frame_LogIsEmpty_005e58a0(rp(gui, 0x118), nullptr) ? 0u : 1u;    // 006341C0..006341CF
        wr(gui, 0xc8, 0); wr(gui, 0xcc, has_text); wr(gui, 0xd4, 0); wr(gui, 0xd0, 1);       // 006341D1..006341E7
        frame_GuiDispatch_006339c0(gui, nullptr);                                            // UpdateDialog
        vcall<void>(rp(gui, 0x3c), 8, 0u, 0u);                                               // 006341F6..006341FD
        return;
    }
    vcall<void>(rp(gui, 0x3c), 0x74, rd(gui, 0x114), rd(gui, 0x118), rd(gui, 0x11c), a1, 1u); // 00634206..00634227
    wr(rp(gui, 0x3c), 0x64, a2);
    kotor_fastcall_0062db50(gui, nullptr);                                                   // UnlockDialogParticipantOrientations
    U solo = 0;                                                                               // edi
    void* c = frame_R4_005ed5b0(client(), nullptr, me);                                       // 0063423B..00634247
    if (c && rd(c, 0x3a4)) solo = 1;                                                          // 0063424C..0063425A
    wr(gui, 0x17c, rd(gui, 0x174));                                                          // 0063425F..0063426B
    wr(gui, 0x174, rd(gui, 0x170));
    void* pc = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(), nullptr), nullptr, 0); // 00634277..00634289
    if (solo && rd(gui, 0xbd8) == 0) {                                                       // 0063428E..0063429A
        wr(gui, 0x178, rd(gui, 0x170)); wr(gui, 0x170, me);                                  // 0063429C..006342A2 / 006342D1
    } else if (pc) {
        const U pc_id = rd(pc, 4);                                                           // 006342AE..006342BD
        wr(gui, 0x178, rd(gui, 0x170)); wr(gui, 0x170, pc_id);
    } else {
        wr(gui, 0x178, rd(gui, 0x170)); wr(gui, 0x170, me);                                  // 006342C5..006342D1
    }
    const U panel = rd(gui, 0x3c);
    if (panel != 0 && panel == rd(gui, 0x40)) {                                              // 006342D7..006342E1
        if (!rd(gui, 0xb20)) kotor_fastcall_0062bf30(gui, nullptr);                          // InitializeShotCamera
        kotor_fastcall_006306e0(gui, nullptr, rd(rp(gui, 0x124), 0), rd(rp(gui, 0x128), 0), rd(rp(gui, 0x12c), 0),
            rd(rp(gui, 0x130), 0), rd(rp(gui, 0x134), 0), rd(rp(gui, 0x140), 0));            // 006342F4..0063432C
    }
}

// 00634340 CGuiInGame::HandleDialogReplies(25 args) RET 64h; returns 1
// Arrays: a4 CExoString[], a5..a11/a13/a14/a17/a18/a22/a23/a24 DWORD[], a12 WORD[], a15 BYTE[], a16 Vector[];
// reply count = a19 + a20; a1..a3 unused; a20/a21 forwarded to ShowDialogReplies.
U __fastcall kotor_fastcall_00634340(void* gui, void*, U a1, U a2, U a3, void* a4, U a5, U a6, U a7, U a8, U a9,
    U a10, U a11, U a12, U a13, U a14, U a15, U a16, U a17, U a18, U a19, U a20, U a21, U a22, U a23, U a24, U a25) {
    (void)a1; (void)a2; (void)a3; (void)a25;
    const U total = a19 + a20;                                                               // 00634358..00634365
    if (total != 0) {
        ExoString text;                                                                       // 0063437A..0063437E
        (void)kotor_fastcall_0062bc70(gui, nullptr, total);                                  // InitializeRepliesData
        auto e = [](U base, U k) { return rd(ptr(base), k * 4); };
        for (U k = 0; k < total; ++k) {                                                      // 00634440..00634584
            str_assign(&text, at(a4, k * 8));                                                // 00634440..00634449
            if (!frame_LogIsEmpty_005e58a0(&text, nullptr)) tlk_parse(&text);                // 0063444E..006344BC
            kotor_fastcall_00628750(gui, nullptr, k, &text, e(a5, k), e(a6, k), e(a7, k), e(a8, k), e(a9, k),
                e(a10, k), e(a11, k), rd16(ptr(a12), k * 2), e(a13, k), e(a14, k), rd8(ptr(a15), k),
                ptr(a16 + k * 12), e(a17, k), e(a18, k), static_cast<I>(e(a22, k)),
                static_cast<const U*>(ptr(e(a23, k))), static_cast<const U16*>(ptr(e(a24, k)))); // 006344C5..0063456D
        }
    }
    kotor_fastcall_0062d8c0(gui, nullptr);                                                   // CleanUpEntryNode
    kotor_fastcall_006340e0(gui, nullptr, a20, a21);                                         // 006345A7..006345B3
    return 1;
}

}  // extern "C"

// ---------------------------------------------------------------------------------------------------------------
// 00633660 CGuiInGame::HandleDialogSelection(int selection, int animate) RET 8 (review X2; replaces the 0783 trap)
// ---------------------------------------------------------------------------------------------------------------
namespace services_tail5 { struct Message { Message* __thiscall Construct(); }; }      // 0064B510 CSWCMessage()
extern "C" {
void __fastcall kotor_fastcall_0064b530(void* message, void*);                           // ~CSWCMessage (0597)
U __fastcall kotor_fastcall_006a6d80(void* panel, void*, U selection);                   // CSWGuiDialog::GetReplyIndex
void __fastcall kotor_fastcall_004cb480(void* object, void*, U player, U reply, U close, U panel_value, U zero); // ReplyDialog
}
namespace {
struct ClientMessage {                                     // CSWCMessage frame block [esp+1C]..[esp+73] (constructed, unused)
    unsigned char storage[0x58];
    ClientMessage() { (void)reinterpret_cast<services_tail5::Message*>(storage)->Construct(); }   // 006336B6..006336C0
    ClientMessage(const ClientMessage&) = delete;
    ClientMessage& operator=(const ClientMessage&) = delete;
    ~ClientMessage() { kotor_fastcall_0064b530(storage, nullptr); }                    // 00633975..00633981
};
}
extern "C" void __fastcall kotor_fastcall_00633660(void* gui, void*, I selection, U animate) {
    wr(gui, 0xd0, 0);                                                                    // 00633686
    if (selection == -2) {                                                               // 00633681..0063368C
        kotor_fastcall_006332b0(gui, nullptr);                                           // 0063399B..0063399D CloseDialog
        return;
    }
    U reply_index;                                                                       // the selection argument slot
    if (selection >= 0)                                                                  // 00633692..00633694 (signed jl)
        reply_index = kotor_fastcall_006a6d80(rp(gui, 0x3c), nullptr, static_cast<U>(selection));   // 00633696..0063369F
    else
        reply_index = 0xffffffffu;                                                       // 006336A5
    const U close = selection == -3 ? 1u : 0u;                                           // 006336AD..006336BC (ebp)
    ClientMessage message;                                                               // 006336B6..006336C0
    void* player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr));  // 006336C5..006336EF
    void* speaker = kotor_fastcall_004ae750(server(), nullptr,
        kotor_fastcall_004aea30(server(), nullptr, rd(rp(gui, 0x3c), 0x19c0)));         // 006336E6..00633711 ebx
    const U sel = static_cast<U>(selection);                                             // edi; negative indices are used as-is (QUIRK)
    vcall<void>(rp(gui, 0x3c), 0x7c, static_cast<void*>(at(rp(gui, 0x118), sel * 8)));    // 00633711..00633722 panel +7C
    if (animate != 0)                                                                    // 00633725..0063372E
        kotor_fastcall_00630620(gui, nullptr, rd(rp(gui, 0x154), sel * 4));              // 00633730..0063373C
    for (I i = 0; i < rdi(rp(gui, 0x154), sel * 4); ++i) {                               // 00633741..0063379D (signed, re-read)
        const U anim = rd(rp(rp(gui, 0x158), sel * 4), static_cast<U>(i) * 4);          // 00633759..00633762
        const U16 flags = rd16(rp(rp(gui, 0x15c), sel * 4), static_cast<U>(i) * 2);     // 00633750..0063376B
        if (static_cast<U>(i) < rd(gui, 0x198) && rd(gui, 0x1a0) != 0 && rd(gui, 0x1a4) != 0) {   // 00633765..00633782
            wr(rp(gui, 0x1a0), static_cast<U>(i) * 4, anim);                             // 00633784
            wr16(rp(gui, 0x1a4), static_cast<U>(i) * 2, flags);                          // 00633787..0063378D
        }
    }
    const U8 fade = rd8(rp(gui, 0x144), sel);                                            // 006337A3..006337A9
    if (fade != 0) {                                                                     // 006337AC..006337AE
        const U on = (fade == 4 || fade == 1) ? 0u : 1u;                                 // 006337B0..006337BD
        void* color = at(rp(gui, 0x148), sel * 12);                                      // 006337BF..006337C8
        const U length = fbits(rdf(rp(gui, 0x150), sel * 4));                            // 006337CB..006337DA fld/fstp dword
        const U delay = fbits(rdf(rp(gui, 0x14c), sel * 4));                             // 006337D4..006337E6 fld/fstp dword
        if (rd(gui, 0x68) != 0)                                                          // 006337E1..006337EA
            kotor_fastcall_006a7620(rp(gui, 0x68), nullptr, on, delay, length, color);   // 006337EC..006337F8 SetFade
    }
    if (animate != 0) {                                                                  // 006337FD..00633806
        kotor_fastcall_0062db50(gui, nullptr);                                           // 0063380C..0063380E
        U listener = rd(rp(gui, 0x120), sel * 4);                                        // 00633813..00633819
        if (listener == kInvalid) {                                                      // 0063381C..00633821
            wr(gui, 0x17c, rd(gui, 0x174));                                              // 00633823..0063382F
            listener = rd(gui, 0x170);                                                   // 00633829
        } else {
            wr(gui, 0x17c, rd(gui, 0x174));                                              // 00633837..0063383D
        }
        const U previous = rd(gui, 0x170);                                               // 00633843
        wr(gui, 0x174, listener);                                                        // 00633849
        wr(gui, 0x178, previous);                                                        // 00633857
        wr(gui, 0x170, rd(gui, 0x184));                                                  // 0063384F / 0063385D
        kotor_fastcall_006313a0(gui, nullptr);                                           // 00633855..00633863 SetDialogAnimations
        void* panel = rp(gui, 0x3c);                                                     // 00633868
        if (panel != nullptr && word(panel) == rd(gui, 0x40)) {                          // 0063386B..00633876
            if (kotor_fastcall_0062bf70(gui, nullptr, rd16(rp(gui, 0x138), sel * 2), rd(rp(gui, 0x13c), sel * 4)) == 0) {  // 00633878..00633898
                if (rd(gui, 0xb20) == 0) kotor_fastcall_0062bf30(gui, nullptr);         // 0063389E..006338AA
                kotor_fastcall_006306e0(gui, nullptr, rd(rp(gui, 0x124), sel * 4), rd(rp(gui, 0x128), sel * 4),
                                        rd(rp(gui, 0x12c), sel * 4), rd(rp(gui, 0x130), sel * 4),
                                        rd(rp(gui, 0x134), sel * 4), rd(rp(gui, 0x140), sel * 4));   // 006338AF..006338ED
            }
        } else if (rd(rp(gui, 0x124), sel * 4) == 6) {                                   // 006338F4..006338FE
            if (rd(gui, 0xb20) == 0) kotor_fastcall_0062bf30(gui, nullptr);             // 00633900..0063390C
            kotor_fastcall_0062c030(gui, nullptr, rd(rp(gui, 0x128), sel * 4), rd(rp(gui, 0x134), sel * 4),
                                    rd(rp(gui, 0x140), sel * 4));                       // 00633911..00633931
        }
    }
    if (close != 0) kotor_fastcall_006332b0(gui, nullptr);                               // 00633936..0063393C
    if (speaker != nullptr && vcall<void*>(speaker, 0x10) != nullptr) {                  // 00633941..0063394E
        const U panel_value = rd(rp(gui, 0x3c), 0x64);                                   // 00633950..00633953
        kotor_fastcall_004cb480(vcall<void*>(speaker, 0x10), nullptr, rd(player, 4), reply_index, close, panel_value, 0);  // 00633956..00633970
    }
}                                                                                        // 00633975..00633981 ~CSWCMessage
