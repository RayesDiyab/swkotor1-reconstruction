#pragma once
#include <cstdint>
#include <cstddef>
#include "0f11596b37-gui_ctor.h"
namespace menu_continuation {
using U32=std::uint32_t;
using I32=std::int32_t;
using I64=std::int64_t;
struct WorldTimer { I64 __thiscall GetSnapshotTimeDifference_004adc80(); };
struct ExoTimers { I64 __thiscall GetHighResolutionTimer_005e9260(); };
struct TimerClient { unsigned char pad[0x2c]; WorldTimer* timer; };
struct App { U32 word0; TimerClient* client; };
struct AppManager { U32 word0; App* app; };
struct ExoBase { U32 word0; ExoTimers* timers; };
struct InGame {
 unsigned char pad0[0xb4]; U32 field_b4;
 unsigned char pad1[0xb98-0xb8]; U32 field_b98;
 I32 __thiscall GetPendingStatusSummary_0062ef70();
 I32 __thiscall IsGlobalFading_0062ac60();
 I32 __thiscall IsGlobalFadeObscuring_0062ded0();
 void __thiscall ShowStatusSummary_0062ef90();
};
using GuiManager=gui_ctor::Gui; // One nominal receiver shared with the selected constructor.
// Values produced before this stage, needed by later MainLoop code. R2 does
// not retain saved_ebp_006038ab; integration must supply the actual return.
struct Carry {
 U32 saved_ebp_006038ab;
 U32 world_time_local_28;
 U32 world_time_local_2c;
};
struct Client {
 unsigned char pad0[0x40]; InGame* in_game;
 unsigned char pad1[0x9c-0x44]; U32 input_mode;
 unsigned char pad2[0x274-0xa0]; GuiManager* gui;
 unsigned char pad3[0x288-0x278]; U32 field_288;
 unsigned char pad4[0x370-0x28c]; float status_countdown;
 unsigned char pad5[0x3dc-0x374]; float mouse_countdown;
 unsigned char pad6[0x4d4-0x3e0];
 void __thiscall UpdateHostileHilite_005f7640(float);
 void __thiscall ShowMouse_0061fa00(I32);
 // New bounded-stage API, NOT the original MainLoop ABI or successful result.
 U32 __thiscall ContinueToFirstDraw_00603b6d(Carry*);
};
extern AppManager* app_manager_007a39fc;
extern ExoBase* exo_base_007a39e0;
extern float frame_delta_0078e574;
extern U32 high_resolution_low_008328f0;
extern U32 high_resolution_high_008328f4;
extern const float zero_0073d700;
extern const float scale_007439bc;
extern const float threshold_0073edf0;
static_assert(sizeof(void*)==4,"x86 only");
static_assert(sizeof(Client)==0x4d4 && sizeof(GuiManager)==0xa8,"receiver spans");
static_assert(offsetof(Client,in_game)==0x40 && offsetof(Client,input_mode)==0x9c,"client prefix");
static_assert(offsetof(Client,gui)==0x274 && offsetof(Client,field_288)==0x288,"client pointers");
static_assert(offsetof(Client,status_countdown)==0x370 && offsetof(Client,mouse_countdown)==0x3dc,"client timers");
static_assert(offsetof(InGame,field_b4)==0xb4 && offsetof(InGame,field_b98)==0xb98,"in-game spans");
static_assert(offsetof(GuiManager,bytes)==0 && sizeof(GuiManager)==0xa8,"GUI representation; saved_delta accesses +60");
}
