// Lane A — small load helpers (005EEAB0, 00677560, 005F4AE0).
//
// Whole-function reconstructed source.  It binds only accepted selected
// providers/globals and defines no global cell.  No trap, stub, partial body,
// byte payload or address alias is present.  The emitted ABI is a source/
// native gate: SOURCE-AUDIT.md records the expected decorations and stack
// shapes for the root post-compile readback.
#include "586d9a2fc0-small_load_helpers.h"

// ---- Selected globals (declarations only; owners are 0068/0673) ----------
namespace menu_continuation {
struct ExoBase;
extern ExoBase* exo_base_007a39e0;   // ?exo_base_007a39e0@menu_continuation@@...
struct ExoTimers {
    __int64 GetHighResolutionTimer_005e9260();  // selected 0457/0062 provider
};
}

extern "C" {
// Selected message/sound providers (see EVIDENCE/04-REUSE-REGISTRY.json).
void __fastcall kotor_fastcall_004d68a0(void* self, void* unused_edx,
                                        unsigned size, unsigned field4c,
                                        unsigned field48);          // @20
void __fastcall kotor_fastcall_004d6340(void* self, void* unused_edx,
                                        unsigned value, unsigned bits); // @16
void __fastcall kotor_fastcall_004d6960(void* self, void* unused_edx,
                                        const void* str, unsigned bits); // @16
unsigned __fastcall kotor_fastcall_004d6440(void* self, void* unused_edx,
                                            void** output,
                                            unsigned* size);        // @16
unsigned __fastcall recovered_client_send_00677410(
    void* self, void* unused_edx, unsigned major, unsigned minor,
    void* data, unsigned size);                                     // @24
void __fastcall frame_R4_005d5e80(void* outer, void* unused_edx,
                                  unsigned mode);                   // @12

// Selected cells: extern binds only, no definitions / no second cells.
extern void* volatile kotor_cell_sound_007a39ec;
extern const unsigned kotor_rdata_sound_mode_0074c5f0;
}

namespace sound_fade29_required {
void __fastcall update_005d5dd0(void* outer, void* unused_edx);     // ret 0
}

namespace kotor_menu_10_19 {
namespace dependencies {
unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64 a,
                                                    unsigned __int64 b);
}
}

namespace {
// The original reads [0x7A39E0] and then [ [0x7A39E0] + 4 ] as the timer.
// Only the +4 field is used; the ExoBase object itself is not owned here.
struct ExoBaseLayout {
    unsigned word0;
    void* timers;
};
static_assert(sizeof(ExoBaseLayout) == 8, "ExoBase +0/+4 contract");
static_assert(offsetof(ExoBaseLayout, timers) == 4, "timer pointer at +4");

void* load_exo_base() {
    return reinterpret_cast<void*>(menu_continuation::exo_base_007a39e0);
}

void* load_timer() {
    auto* base = static_cast<ExoBaseLayout*>(load_exo_base());
    return base->timers;
}
}  // namespace

// 005EEAB0: ECX = CClientExoAppInternal*, 3 stack dwords, RET 0xC.
// Null [ECX+0x14C] -> EAX=0; otherwise tail/transfer to 00677560.
extern "C" unsigned __fastcall kotor_fastcall_005eeab0(
    void* self, LoadGameArgs args) {
    void* manager = *reinterpret_cast<void**>(
        static_cast<unsigned char*>(self) + 0x14C);
    if (!manager) return 0;
    return kotor_fastcall_00677560(manager, args);
}

// 00677560: ECX = message manager, 3 stack dwords, both exits RET 0xC.
// The five-call order, constants and two local output slots are preserved.
extern "C" unsigned __fastcall kotor_fastcall_00677560(
    void* self, LoadGameArgs args) {
    // The selected message_finish provider writes both slots on every path;
    // the original frame leaves them uninitialized until that call.
    void* data;
    unsigned size;

    kotor_fastcall_004d68a0(self, nullptr, 0x80u, 0xFFFFFFFFu, 1u);
    kotor_fastcall_004d6340(self, nullptr, args.arg1, 0x20u);
    kotor_fastcall_004d6960(self, nullptr, args.arg2, 0x20u);
    kotor_fastcall_004d6960(self, nullptr, args.arg3, 0x20u);
    if (!kotor_fastcall_004d6440(self, nullptr, &data, &size)) return 0;
    return recovered_client_send_00677410(self, nullptr, 3u, 5u, data, size);
}

// 005F4AE0: ECX passed but unused, 0 stack args, RET 0.
// Timer/divide/sound/busy-wait sequence and every global read are preserved.
extern "C" void __fastcall kotor_fastcall_005f4ae0(void*) {
    auto* timer =
        static_cast<menu_continuation::ExoTimers*>(load_timer());
    const unsigned __int64 first = static_cast<unsigned __int64>(
        timer->GetHighResolutionTimer_005e9260());
    const unsigned __int64 first_q =
        kotor_menu_10_19::dependencies::unsigned_divide_006fb030(first,
                                                                 1000);
    const unsigned elapsed_base = static_cast<unsigned>(first_q);

    // Read order before the loop: 0x74C5F0, then 0x7A39EC.
    const unsigned mode = kotor_rdata_sound_mode_0074c5f0;
    void* sound_outer = kotor_cell_sound_007a39ec;
    frame_R4_005d5e80(sound_outer, nullptr, mode);

    for (;;) {
        // Per iteration: re-read 0x7A39EC, render/update, re-read
        // 0x7A39E0 and [+4], poll/divide, then 32-bit elapsed compare.
        void* outer = kotor_cell_sound_007a39ec;
        sound_fade29_required::update_005d5dd0(outer, nullptr);

        auto* timer2 =
            static_cast<menu_continuation::ExoTimers*>(load_timer());
        const unsigned __int64 now = static_cast<unsigned __int64>(
            timer2->GetHighResolutionTimer_005e9260());
        const unsigned __int64 now_q =
            kotor_menu_10_19::dependencies::unsigned_divide_006fb030(now,
                                                                     1000);
        const unsigned elapsed =
            static_cast<unsigned>(now_q) - elapsed_base;
        if (elapsed >= 0x1F4u) return;
    }
}
