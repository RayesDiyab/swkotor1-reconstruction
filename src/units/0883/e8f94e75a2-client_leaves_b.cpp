// lane-m48-b fork CLIENT, unit client-leaves-b: area-transition label and rumble pattern bodies (r4-lowers consumers).
// Evidence: dumpbin /disasm swkotor.exe per function (tools/dis); addresses cited per statement. Conventions per
// r4_lowers.h (original __thiscall -> extern "C" __fastcall(self, unused EDX, stack words), same callee cleanup).
// D30-FINAL: x87 intermediates are doubles; floats only at the original fstp dword / fst dword stores.
#include "e6446a7354-r4_lowers.h"
#include "96c5832fd3-startup_string.h"

using namespace r4_lowers;
using kotor_reconstruction::StartupString;

namespace kotor_reconstruction { __declspec(noinline) StartupString* __fastcall StartupAssign005e5c50(StartupString*, void*, StartupString*); }
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringFind_005e5eb0(options_video::String*, void*, options_video::String*, int); }
namespace base_ini_reconstruction { StartupString* __fastcall right_005e6170(StartupString*, void*, StartupString*, int); }
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*, void*); }
namespace gui_load { struct String; }
extern "C" {
std::uint32_t __fastcall async_string_length_005e5790(StartupString*, void*);
void __fastcall gui_load_text_params_set_text_00415e00(void*, void*, const gui_load::String*) noexcept(false);
StartupString* __fastcall WindowCopy005e5b00(StartupString*, void*, const StartupString* volatile);
StartupString* __fastcall kotor_fastcall_006c7d20(void* door, void*, StartupString* out);
double __fastcall kotor_fastcall_0068fcb0(void* pattern, void*, float time, std::uint32_t loop);
}

namespace {
struct LogString {                      // CExoString local with the original destructor 005E5C20 on scope exit
    frame_log::String s{};
    LogString() = default;
    LogString(const LogString&) = delete;
    ~LogString() { frame_LogStringDtor_005e5c20(&s, nullptr); }
    StartupString* p() { return reinterpret_cast<StartupString*>(&s); }
};
// .rdata 0073ED40 "-" (bytes 2D 00); 0073D700 = 0.0f.
const char k_dash_0073ed40[] = "-";
constexpr float k_zero_0073d700 = 0.0f;
float fbits(U32 b) { float v; std::memcpy(&v, &b, 4); return v; }
}

// 006C7D20 CSWCDoor::GetModelName(out), RET 4: copy-construct out from [this+13Ch] (005E5B00); return out.
extern "C" StartupString* __fastcall kotor_fastcall_006c7d20(void* door, void*, StartupString* out) {
    WindowCopy005e5b00(out, nullptr, reinterpret_cast<StartupString*>(bytes(door) + 0x13c)); // 006C7D22..006C7D37
    return out;                                                         // 006C7D3C
}

// 006C7EC0 CSWGuiInGameAreaTransition::SetTransitionObject(id, Vector* position), RET 8.
extern "C" void __fastcall kotor_fastcall_006c7ec0(void* transition, void*, std::uint32_t id, void* position) {
    if (w(transition, 0x430) != id) {                                   // 006C7EDE..006C7EEC
        void* client = pw(menu_continuation::app_manager_007a39fc, 4);  // 006C7EF2..006C7EF8
        void* object = kotor_fastcall_005ed580(client, nullptr, id);    // 006C7EFB..006C7EFC
        void* inner = object ? at(vcall0(object, 0x0c)) : nullptr;      // 006C7F03..006C7F0F
        if (inner) {                                                    // 006C7F12..006C7F16
            LogString name;                                             // 006C7F1C..006C7F20 (005B3190), EH state 0
            kotor_movie::string_element_construct_005b3190(&name.s, nullptr);
            const unsigned char kind = b8(inner, 8);                    // 006C7F25
            if (kind == 0x0a) {                                         // 006C7F28..006C7F2E (door)
                void* door = at(vcall0(inner, 0x14));                   // 006C7F30..006C7F39
                LogString temp;                                         // [esp+18] (EH state 1)
                kotor_fastcall_006c7d20(door, nullptr, temp.p());       // 006C7F32..006C7F3E
                kotor_reconstruction::StartupAssign005e5c50(name.p(), nullptr, temp.p()); // 006C7F43..006C7F4D
            } else if (kind == 0x07) {                                  // 006C7F61..006C7F63 (placeable)
                unsigned char* base = bytes(at(vcall0(inner, 0x3c)));    // 006C7F65..006C7F69
                kotor_reconstruction::StartupAssign005e5c50(name.p(), nullptr, reinterpret_cast<StartupString*>(base + 0x140)); // 006C7F6C..006C7F76
            }
            int found;
            {
                LogString dash;                                         // 006C7F7B..006C7F84 (005E5A90 "-"), EH state 2
                frame_LogStringCtor_005e5a90(&dash.s, nullptr, k_dash_0073ed40);
                found = client_movies::StringFind_005e5eb0(reinterpret_cast<options_video::String*>(&name.s), nullptr,
                                                           reinterpret_cast<options_video::String*>(&dash.s), 0); // 006C7F89..006C7F98
            }                                                           // 006C7F9D..006C7FA7
            if (found != -1) {                                          // 006C7FAC..006C7FAF
                const int length = static_cast<int>(async_string_length_005e5790(name.p(), nullptr)); // 006C7FB1..006C7FB5
                const int count = length + (-2 - found);                // 006C7FBA..006C7FC1
                LogString right;                                        // EH state 3
                base_ini_reconstruction::right_005e6170(name.p(), nullptr, right.p(), count); // 006C7FC3..006C7FCD
                kotor_reconstruction::StartupAssign005e5c50(name.p(), nullptr, right.p()); // 006C7FD2..006C7FDC
            }                                                           // 006C7FE1..006C7FE9
            gui_load_text_params_set_text_00415e00(bytes(transition) + 0x3cc, nullptr,
                                                   reinterpret_cast<const gui_load::String*>(&name.s)); // 006C7FEE..006C7FF9
        }                                                               // 006C7FFE..006C800A
        sw(transition, 0x430, id);                                      // 006C800F..006C8012 (also when no object)
    }
    sw(transition, 0x424, w(position, 0));                              // 006C8018..006C8023
    sw(transition, 0x428, w(position, 4));                              // 006C8025..006C802C
    sw(transition, 0x42c, w(position, 8));                              // 006C802F..006C8032
}

// 0068FCB0 CSWRumblePattern::GetMagnitude(time, loop), RET 8, ST0 result. Pattern view: +0 magnitudes float*,
// +4 times float*, +8 count, +0C cursor. FCOM C0 = below/unordered, C3 = equal/unordered, C2 = unordered.
extern "C" double __fastcall kotor_fastcall_0068fcb0(void* pattern, void*, float time, std::uint32_t loop) {
    const float* times = static_cast<const float*>(pw(pattern, 4));     // 0068FCB1
    if (!times) return k_zero_0073d700;                                 // 0068FCB4..0068FCB9 / 0068FDBF
    const float* values = static_cast<const float*>(pw(pattern, 0));    // 0068FCBF
    if (!values) return k_zero_0073d700;                                // 0068FCC1..0068FCC3
    const I32 count = static_cast<I32>(w(pattern, 8));                  // 0068FCCD
    const float last = times[count - 1];
    const bool above = static_cast<double>(time) > last;                // 0068FCC9..0068FCDA (C0|C3 clear: ordered, greater)
    if (loop != 0) {                                                    // 0068FCD4..0068FCDC
        if (above) {                                                    // 0068FCDE..0068FCE1
            double t;
            do {
                t = static_cast<double>(time) - last;                   // 0068FCF0..0068FCF4
                time = static_cast<float>(t);                           // 0068FCF8 fst dword (compare uses ST)
            } while (t > last);                                         // 0068FCFC..0068FD03
        }
    } else if (above) {
        return k_zero_0073d700;                                         // 0068FD07..0068FD0A
    }
    U32 found = 0;                                                      // 0068FD13
    I32 i = static_cast<I32>(w(pattern, 0x0c));                         // 0068FD10
    if (i < count - 1) {                                                // 0068FD15..0068FD18
        do {
            if (found) break;                                           // 0068FD20..0068FD22
            if (static_cast<double>(time) >= times[i]) {                // 0068FD24..0068FD2F (C0 clear: ordered, not below)
                if (static_cast<double>(time) < times[i + 1]) {         // 0068FD31..0068FD3D (jp not taken: C0=1,C2=0)
                    found = 1; sw(pattern, 0x0c, static_cast<U32>(i)); // 0068FD3F..0068FD44
                }
            }
            ++i;                                                        // 0068FD4A
        } while (i < static_cast<I32>(w(pattern, 8)) - 1);              // 0068FD47..0068FD51
    }
    if (static_cast<I32>(w(pattern, 0x0c)) > 0 && !found) {             // 0068FD53..0068FD5A
        I32 j = 0;                                                      // 0068FD56
        do {
            if (found) break;                                           // 0068FD60..0068FD62
            if (static_cast<double>(time) >= times[j]) {                // 0068FD64..0068FD6F
                if (static_cast<double>(time) < times[j + 1]) {         // 0068FD71..0068FD7D
                    found = 1; sw(pattern, 0x0c, static_cast<U32>(j));  // 0068FD7F..0068FD84
                }
            }
            ++j;                                                        // 0068FD8A
        } while (j < static_cast<I32>(w(pattern, 0x0c)));               // 0068FD87..0068FD90
    }
    if (!found) return k_zero_0073d700;                                 // 0068FD92..0068FD94
    const U32 c = w(pattern, 0x0c);                                     // 0068FD96
    const double a = static_cast<double>(time) - times[c];              // 0068FD99..0068FDA0
    const double b = static_cast<double>(times[c + 1]) - times[c];      // 0068FDA4..0068FDA9
    const double v = static_cast<double>(values[c + 1]) - values[c];    // 0068FDAE..0068FDB2
    return a / b * v + values[c];                                       // 0068FDAC fdivp, 0068FDB5 fmulp, 0068FDB7 fadd
}

// 0068FDD0 CSWRumblePattern::GetMagnitudes(time, left*, right*), RET 0Ch: EAX 1 while inside either channel's span.
extern "C" std::uint32_t __fastcall kotor_fastcall_0068fdd0(void* pattern, void*, std::uint32_t time_bits, std::uint32_t* left,
                                                            std::uint32_t* right) {
    const float time = fbits(time_bits);
    const float l = static_cast<float>(kotor_fastcall_0068fcb0(pattern, nullptr, time, w(pattern, 0x20))); // 0068FDD9..0068FDE7
    std::memcpy(left, &l, 4);
    void* second = bytes(pattern) + 0x10;                               // 0068FDED
    const float r = static_cast<float>(kotor_fastcall_0068fcb0(second, nullptr, time, w(pattern, 0x20))); // 0068FDE9..0068FDFC
    std::memcpy(right, &r, 4);
    if (w(pattern, 0x20) != 0) return 1;                                // 0068FDFE..0068FE03 / 0068FE60
    const I32 n0 = static_cast<I32>(w(pattern, 8));                     // 0068FE05
    const I32 i0 = n0 - 1;                                              // 0068FE08
    const float end0 = (i0 >= 0 && i0 < n0) ? static_cast<const float*>(pw(pattern, 4))[i0] : k_zero_0073d700; // 0068FE0B..0068FE1B
    if (static_cast<double>(time) < end0) return 1;                     // 0068FE21..0068FE2E (jnp: C0 set, C2 clear)
    const I32 n1 = static_cast<I32>(w(second, 8));                      // 0068FE36 ([ebx+8])
    const I32 i1 = static_cast<I32>(w(pattern, 0x18)) - 1;              // 0068FE30..0068FE33
    const float end1 = (i1 >= 0 && i1 < n1) ? static_cast<const float*>(pw(second, 4))[i1] : k_zero_0073d700; // 0068FE34..0068FE43
    if (static_cast<double>(time) < end1) return 1;                     // 0068FE49..0068FE56
    return 0;                                                           // 0068FE5A
}
