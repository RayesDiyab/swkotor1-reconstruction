#pragma once
// lane-m15-c: shared externals and D24 call views for the char-gen flow units (m15c_maincg.cpp, m15c_flow.cpp,
// m15c_portrait_name.cpp). Every external is an existing owner (M60 functional.map object or a sibling M1.5 lane
// header); declarations are identical in linkage/ABI to the owners'.
#include "41527c6f69b3dd09_m15c_panels.h"
#include "f155d76cd432eed7_m15_l0.h"          // lane-m15-0: SetVisible 0040B9D0, CExoArrayList<WORD>::Allocate 004D2FA0
#include "b8824978385052ca_m15_l0_gui.h"      // lane-m15-0: PanelEventThunks15::OnYButtonPressed_00644720
#include "bf662019daaaa2b6_m15b_stats.h"      // lane-m15-b: CSWCCreatureStats / CSWCLevelUpStats / CSWClass
#include "8d831d5c6afcac70_m15a_creature.h"   // lane-m15-a: CSWCCreature SetName / SetAppearance / Setup3DScene / CGPauseCycle, Yaw/Pitch
#include "a5e77e84517413d8_m15d.h"            // lane-m15-d: Feats / Skills / Abilities ctors
#include "40ced824a174dc05_m15d_gui.h"        // lane-m15-d: PanelStepThunks 0067CB40 / 0067CB50
#include "525fe8bd23209b6c_m15_traps.h"       // lane-m15-f: Play boundary trap 006EB320
#include "29a305efc7265995_m1_shared.h"       // lane-m1-options: PanelEventThunks 00624BA0 / 00624BB0
#include "35e16fcac8fcb6d4_kotor_unreachable_trap.h"
#include "e1805047c9b7563b_startup_string.h"
#include "79da78e626bf1ab1_string_priority.h"
#include <cstring>
#include <new>

struct MenuColor;   // input-hover donor/matching.h: { unsigned long words[3]; }
extern "C" {
// gui-load / core47 / r4-lowers / m48 owners
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                        // PopModalPanel 0040BE00
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                                     // CClientExoApp::GetSWGuiManager
void* __fastcall gob_get_property_0x68_0063c070(void* creature, void*);                            // GetProperty0x68 (anim base)
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word a, gui_load::Word b, gui_load::Word c);
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const MenuColor* color);   // core47
void __fastcall gui_m48_listbox_add_controls_0041c2c0(gui_lb::ListBox*, void*, gui_load::Control** controls, gui_load::I32 count,
    gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false);   // m48b 0041C2C0
void __fastcall gui_m48_label_hilight_destruct_0041b610(void*, void*);                            // m48b 0041B610
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_005e5bc0(kotor_reconstruction::StartupString* self, void*,
    int value) noexcept(false);                                                                    // CExoString(int) 005E5BC0
std::int32_t __fastcall find_005e5f20(kotor_reconstruction::StartupString*, void*, gui_load::Word ch, std::int32_t start);  // 005E5F20
extern const MenuColor kotor_m1_color_007a2408;                                                    // .data 007A2408 (m1-saveload)
extern void* kotor_cell_rules_007a3a28;                                                            // [007A3A28] g_pRules (lane-prepare)
}
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*, void*,
                                                              kotor_reconstruction::StartupString*, int);   // 005E5F70
}
namespace directory_list { char __fastcall At005e5600(kotor_reconstruction::StartupString*, void*, std::int32_t); }   // 005E5600

namespace kotor_m15c {
using String = kotor_reconstruction::StartupString;
inline String* str(void* raw) { return reinterpret_cast<String*>(raw); }
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
template <class T> inline T ld(const void* p, unsigned off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(T)); return v; }
template <class T> inline void st(void* p, unsigned off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(T)); }
// Method-table slot read through the object's vptr (D24 view; the stored entry is the original slot function, whose
// thiscall ABI equals the declared D1 fastcall type).
template <class F> inline F slot(const void* object, unsigned offset) {
    const void* table;
    std::memcpy(&table, object, sizeof(table));
    F fn;
    std::memcpy(&fn, static_cast<const unsigned char*>(table) + offset, sizeof(fn));
    return fn;
}
// String temporaries returned by value into raw storage.
struct Fresh {
    alignas(String) unsigned char raw[sizeof(String)];
    String* object = nullptr;
    String* storage() { return reinterpret_cast<String*>(raw); }
    ~Fresh() { if (object) object->~StartupString(); }
};
struct V3 { float x, y, z; };          // Vector passed by value (3 words)
struct Q4 { float x, y, z, w; };       // Quaternion passed by value (4 words)
struct ResRef16 { unsigned char bytes[16]; };
// CSWCCreature virtuals (table 0074F5F8; slots audited by lane-m15-f SLOT-AUDIT.json, bodies L15-E):
//   +070 GetName(CExoString* out) RET 4 -> out    +08C SetPosition(const Vector*) RET 4
//   +094 AttachToScene(CAurScene*) RET 4          +0D8 GetHeadHeightOffset() -> float (ST0)
//   +0EC SetPortrait(const CResRef*) RET 4         +0F4 SetPortraitId(WORD) RET 4
//   +148 GetPortrait(CResRef* out, int) RET 8 -> out
using CreatureGetName = String* (__fastcall*)(void*, void*, String*);
using CreatureSetPosition = void (__fastcall*)(void*, void*, const V3*);
using CreatureAttach = void (__fastcall*)(void*, void*, void*);
using CreatureHeadHeight = float (__fastcall*)(void*, void*);
using CreatureSetPortrait = void (__fastcall*)(void*, void*, const ResRef16*);
using CreatureSetPortraitId = void (__fastcall*)(void*, void*, Word);
using CreatureGetPortrait = ResRef16* (__fastcall*)(void*, void*, ResRef16*, Word);
// CSWCAnimBase +08 GetModel(part) RET 4 -> CAurObject*; Gob +18 PlayAnimation(name, speed, a, b) RET 10h (as
// gui_load::AurModelVtbl), +104 GetProperty0x68 (scene) RET, +128 AddAnimationFlag(flag) RET 4.
using AnimGetModel = void* (__fastcall*)(void*, void*, Word);
using GobPlay = void (__fastcall*)(void*, void*, const char*, float, Word, Word);
using GobScene = void* (__fastcall*)(void*, void*);
using GobAddFlag = void (__fastcall*)(void*, void*, Word);
// CAurCamera (vtables2_gen.h 0045C030 / 0045C060 / 0045C220): +18 SetPosition(Vector) -> Vector* (hidden out, RET 10h),
// +1C SetOrientation(Quaternion) -> hidden out (RET 14h), +80 GetBehavior(id) RET 4.
using CameraSetPosition = V3* (__fastcall*)(void*, void*, V3*, V3);
using CameraSetOrientation = void* (__fastcall*)(void*, void*, void*, Q4);
using CameraGetBehavior = unsigned char* (__fastcall*)(void*, void*, Word);
using SceneSpawnRoom = void (__fastcall*)(void*, void*, const char*, const float*, const float*);   // CAurScene +70

inline void* stats_of(void* creature) { return ld<void*>(creature, 0x2F8); }   // CSWCCreature+2F8 CSWCLevelUpStats*
inline void* creature_model(void* creature, Word part) {                      // GetProperty0x68 ->+08 GetModel(part)
    void* const anim = gob_get_property_0x68_0063c070(creature, nullptr);
    return slot<AnimGetModel>(anim, 0x08)(anim, nullptr, part);
}
inline float head_height(void* creature) { return slot<CreatureHeadHeight>(creature, 0xD8)(creature, nullptr); }
inline kih::Manager* manager_of(Panel* panel) { return reinterpret_cast<kih::Manager*>(panel->manager); }
inline Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* client_app() { return ld<void*>(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
inline void* message_box() { return ld<void*>(frame_R4_005ed690(client_app(), nullptr), 0x98); } // CGuiInGame+98
// CSWGuiPanel state 400h (flags (x & ~300h) | 400h): "remove me" (e.g. 006F07DF..EC).
inline void mark_done(Panel* panel) { panel->flags = (panel->flags & 0xFFFFFCFFu) | 0x400u; }
// Text params of a label / button (CSWGuiText +18 inside the control: label +E8, button +16C).
inline void* label_text(Label* l) { return bytes(l) + 0xE8; }
inline void* button_text(Button* b) { return bytes(b) + 0x16C; }
inline void set_text(void* params, String* s) { gui_load_text_params_set_text_00415e00(params, nullptr, s); }
// "new" + ctor with the caller-side EH funclet freeing the block when the ctor throws; null block -> null.
template <class Ctor> inline void* new_panel(Word size, Ctor ctor) {
    void* const memory = c2da_directory_new_006fa7e6(size);
    if (!memory) return nullptr;
    try { return ctor(memory); }
    catch (...) { live_free(memory); throw; }
}
// Initialise a control from the open layout: CExoString temp + InitControl(c, &s, 1) + ~CExoString.
inline void init_control(Panel* panel, gui_load::Control* control, const char* tag) {
    String name(tag);
    gui_load_panel_init_control_0040b930(panel, nullptr, control, &name, 1);
}
// Behaviour 3EAh of the scene camera (CAurCamera +80 GetBehavior(3EAh)); callers then store [+48] = mode and
// [+3C..+44] = offset (006EBE7B..C1 / 006EAAF2..3F / 006EB3AE..F9 / 006F8C0A..55 / 006F8D5A..A5).
inline unsigned char* camera_behavior(void* camera) {
    return slot<CameraGetBehavior>(camera, 0x80)(camera, nullptr, 0x3EA);
}
inline void store_offset(unsigned char* behavior, float x, float y, float z) {
    st<float>(behavior, 0x3C, x);
    st<float>(behavior, 0x40, y);
    st<float>(behavior, 0x44, z);
}
}
