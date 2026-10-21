// lane-m48-b fork CLIENT, unit client-leaves-a: small client / input / dialog-buffer / posted-string bodies that close
// M47 strict-link names referenced by routes, r4-lowers and startup46 units.
// Evidence: dumpbin /disasm of swkotor.exe (GOG, sha256 9c10e045..1435) per function (lane-m48-b tools/dis/<addr>.txt);
// per-statement original instruction addresses are cited inline. Conventions follow the consumer epoch (r4_lowers.h /
// kotor_original_calls.h): original __thiscall -> extern "C" __fastcall(self, unused EDX, stack words...), same callee
// cleanup (stack words = original RET immediate / 4). No binding tables, no function-pointer casts.
#include "e6446a7354-r4_lowers.h"
#include "96c5832fd3-startup_string.h"

using namespace r4_lowers;
using kotor_reconstruction::StartupString;

// ---- genuine providers already in the M47 link (exact existing spellings) ----
namespace kotor_reconstruction { __declspec(noinline) StartupString* __fastcall StartupAssign005e5c50(StartupString*, void*, StartupString*); }
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String*, void*, const char*); }
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*, void*); }
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t); }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);                                   // 006FA7E6
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*); // 005ED4F0 (composition.h spelling)
}

// ---- names closed here ----
extern "C" {
std::uint32_t __fastcall kotor_fastcall_005dff40(void* internal, void*);
void* __fastcall kotor_fastcall_005f10f0(void* internal, void*, std::uint32_t id);
void __fastcall kotor_fastcall_005ed840(void* outer, void*, std::int32_t rate);
void __fastcall kotor_fastcall_005eeae0(void* internal, void*, std::int32_t rate);
void __fastcall kotor_fastcall_004ae170(void* timer, void*, std::uint32_t fixed, std::uint32_t rate);
void* __fastcall kotor_fastcall_0044d320(void* posted, void*, const char* text, std::uint32_t y, std::uint32_t x, std::uint32_t life_bits);
void __fastcall kotor_fastcall_0062b680(void* in_game, void*, StartupString* text, StartupString* speaker);
}

namespace {
// .rdata 0073D71C "" : frame_log::clear_0073d71c (existing owner, mainloop.h).
const char* const k_empty_0073d71c = frame_log::clear_0073d71c;
// .rdata byte 0074666C = 00h (CGameObjectArray::GetGameObject success code), same literal as r4_frame_lowers.cpp:24.
constexpr unsigned char k_object_found_0074666c = 0x00u;
void* client_of_manager() { return pw(menu_continuation::app_manager_007a39fc, 4); }
}

// 0078E368..0078E387: eight .data floats, image value 3F800000h each (dumpbin /rawdata 0078E368), copied into every new
// PostedString at +40C..+428 by 0044D42B..0044D483. No earlier owner in M47 or any lane packet (single storage here).
namespace kotor_posted_string_cells {
extern volatile std::uint32_t colors_0078e368[8];
volatile std::uint32_t colors_0078e368[8] = {0x3F800000u, 0x3F800000u, 0x3F800000u, 0x3F800000u,
                                             0x3F800000u, 0x3F800000u, 0x3F800000u, 0x3F800000u};
}

// 005DFF40 CExoInputInternal::GetActive: [ecx+140h] ? [[ecx+140h]+4] : 0; plain RET.
extern "C" std::uint32_t __fastcall kotor_fastcall_005dff40(void* internal, void*) {
    void* device = pw(internal, 0x140);                                 // 005DFF40
    if (!device) return 0;                                              // 005DFF46..005DFF48 / 005DFF4E
    return w(device, 4);                                                // 005DFF4A
}

// 005F10F0 CClientExoAppInternal::GetGameObject(id), RET 4: CGameObjectArray::GetGameObject([this+14h], id, &object)
// with object pre-zeroed (005F10FE); returns object when AL == [0074666C], else 0 (005F110B..005F111D sbb/not/and).
extern "C" void* __fastcall kotor_fastcall_005f10f0(void* internal, void*, std::uint32_t id) {
    void* object = nullptr;                                             // 005F10FE
    const unsigned char status = kotor_fastcall_004d8230(pw(internal, 0x14), nullptr, id, &object); // 005F10F1..005F1106
    return status == k_object_found_0074666c ? object : nullptr;        // 005F110B..005F111D
}
// 005ED580 CClientExoApp::GetGameObject: mov ecx,[ecx+4]; jmp 005F10F0 (RET 4 there).
extern "C" void* __fastcall kotor_fastcall_005ed580(void* client, void*, std::uint32_t id) {
    return kotor_fastcall_005f10f0(pw(client, 4), nullptr, id);         // 005ED580..005ED583
}

// 004AE170 CWorldTimer::SetFixedSnapshotRate(fixed, rate), RET 8: +0C..+18 = 0, +0 = fixed, +4 = rate.
extern "C" void __fastcall kotor_fastcall_004ae170(void* timer, void*, std::uint32_t fixed, std::uint32_t rate) {
    sw(timer, 0x0c, 0); sw(timer, 0x10, 0); sw(timer, 0x14, 0); sw(timer, 0x18, 0); // 004AE174..004AE17F
    sw(timer, 0, fixed);                                                // 004AE182..004AE186
    sw(timer, 4, rate);                                                 // 004AE170 / 004AE188
}
// 005EEAE0 CClientExoAppInternal::SetCaptureEventRate(rate), RET 4.
extern "C" void __fastcall kotor_fastcall_005eeae0(void* internal, void*, std::int32_t rate) {
    sw(internal, 0x94, static_cast<U32>(rate));                         // 005EEAE0..005EEAE4
    void* timer = frame_GetWorldTimer_005ed4f0(client_of_manager(), nullptr); // 005EEAEA..005EEAF6 ([[007A39FC]+4])
    kotor_fastcall_004ae170(timer, nullptr, 1u, static_cast<U32>(rate)); // 005EEAF3/005EEAF4 pushes, 005EEAFD
}
// 005ED840 CClientExoApp::setCaptureEventRate: mov ecx,[ecx+4]; jmp 005EEAE0.
extern "C" void __fastcall kotor_fastcall_005ed840(void* outer, void*, std::int32_t rate) {
    kotor_fastcall_005eeae0(pw(outer, 4), nullptr, rate);               // 005ED840..005ED843
}

// 0044D320 PostedString::PostedString(text, y, x, life), RET 10h, returns this.
extern "C" void* __fastcall kotor_fastcall_0044d320(void* posted, void*, const char* text, std::uint32_t y, std::uint32_t x,
                                                    std::uint32_t life_bits) {
    unsigned char* self = bytes(posted);
    std::size_t n = 0;
    do { self[n] = static_cast<unsigned char>(text[n]); } while (text[n++] != '\0'); // 0044D330..0044D338 (byte copy incl. NUL)
    sw(posted, 0x404, life_bits);                                       // 0044D340
    sw(posted, 0x408, life_bits);                                       // 0044D34A
    sw(posted, 0x430, x);                                               // 0044D354
    sw(posted, 0x42c, y);                                               // 0044D35B
    sw(posted, 0x400, static_cast<U32>(std::strlen(text)));             // 0044D367..0044D37B
    auto& list = r4_lowers_cells::posted_list_007b90fc;
    U32 i = list.count;                                                 // 0044D361
    while (i != 0) {                                                    // 0044D379 / 0044D3EC
        void* other = at(w(at(list.data), (i - 1) * 4));                // 0044D390..0044D395
        const I32 oy = static_cast<I32>(w(other, 0x42c));               // 0044D399
        const I32 sy = static_cast<I32>(w(posted, 0x42c));              // 0044D39F
        --i;                                                            // 0044D3A5
        bool overlap;
        if (sy < oy) {                                                  // 0044D3A6..0044D3A8
            overlap = !(static_cast<I32>(w(posted, 0x400)) + sy < oy);  // 0044D3BA..0044D3C4
        } else if (static_cast<I32>(w(other, 0x400)) + oy >= sy) {      // 0044D3AA..0044D3B4
            overlap = true;
        } else if (sy > oy) {                                           // 0044D3B6..0044D3B8
            overlap = false;
        } else {
            overlap = !(static_cast<I32>(w(posted, 0x400)) + sy < oy);  // 0044D3BA..0044D3C4
        }
        if (overlap && w(other, 0x430) == w(posted, 0x430) && other) {  // 0044D3C6..0044D3D6
            kotor_fastcall_00445080(&list, nullptr, other);             // 0044D3D8..0044D3DE (ECX = 007B90FC)
            live_free(other);                                           // 0044D3E3..0044D3E9 (006FA390)
        }
    }
    if (list.count == list.alloc) {                                     // 0044D3F0..0044D3FC
        const U32 grow = list.alloc ? list.alloc + list.alloc : 8u;     // 0044D3FE..0044D406
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr, grow); // 0044D40B..0044D411
    }
    sw(at(list.data), list.count * 4, static_cast<U32>(reinterpret_cast<std::uintptr_t>(posted))); // 0044D416..0044D422
    list.count = list.count + 1;                                        // 0044D425
    for (U32 k = 0; k < 8; ++k)                                         // 0044D42B..0044D483
        sw(posted, 0x40c + k * 4, kotor_posted_string_cells::colors_0078e368[k]);
    return posted;                                                      // 0044D489
}
// 0044D490 AurPostString(text, y, x, life) cdecl: new(434h) then PostedString ctor when non-null.
// EH state 0 (0044D4B9) would delete the block if the ctor threw; the ctor has no throwing callee (D30-FINAL).
extern "C" void __cdecl kotor_cdecl_0044d490(const char* text, std::uint32_t y, std::int32_t x, std::uint32_t life_bits) {
    void* block = live_operator_new(0x434);                             // 0044D4A6..0044D4AB
    if (block)                                                          // 0044D4B7..0044D4C1
        kotor_fastcall_0044d320(block, nullptr, text, y, static_cast<U32>(x), life_bits); // 0044D4C3..0044D4D9
}

// 0062B680 CGuiInGame::AppendToDialogBuffer(text, speaker): 64-entry ring of {text, speaker} CExoString pairs at
// [this+FCh], count at [this+104h]; RET 8.
extern "C" void __fastcall kotor_fastcall_0062b680(void* in_game, void*, StartupString* text, StartupString* speaker) {
    if (client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(text), nullptr, k_empty_0073d71c) == 0)
        return;                                                         // 0062B683..0062B693
    if (w(in_game, 0x104) >= 0x40u) {                                   // 0062B699..0062B6A1 (jb)
        sw(in_game, 0x104, 0x3f);                                       // 0062B6A7
        std::uint16_t i = 0;                                            // 0062B6A4 (bp) / 0062B6B1
        U32 index = 0;
        do {
            unsigned char* entry = bytes(pw(in_game, 0xfc)) + index * 16; // 0062B6B3..0062B6BC
            kotor_reconstruction::StartupAssign005e5c50(reinterpret_cast<StartupString*>(entry), nullptr,
                                                        reinterpret_cast<StartupString*>(entry + 0x10)); // 0062B6BF..0062B6C5
            kotor_reconstruction::StartupAssign005e5c50(reinterpret_cast<StartupString*>(entry + 8), nullptr,
                                                        reinterpret_cast<StartupString*>(entry + 0x18)); // 0062B6CA..0062B6D1
            ++i; index = i;                                             // 0062B6DC..0062B6DD (movzx bp)
        } while (index < w(in_game, 0x104));                            // 0062B6D6 / 0062B6E0..0062B6E2
    }
    unsigned char* slot = bytes(pw(in_game, 0xfc)) + w(in_game, 0x104) * 16; // 0062B6E6..0062B6FA
    kotor_reconstruction::StartupAssign005e5c50(reinterpret_cast<StartupString*>(slot), nullptr, text); // 0062B6FC
    slot = bytes(pw(in_game, 0xfc)) + w(in_game, 0x104) * 16;           // 0062B701..0062B711 (reloaded)
    kotor_reconstruction::StartupAssign005e5c50(reinterpret_cast<StartupString*>(slot + 8), nullptr, speaker); // 0062B714..0062B719
    sw(in_game, 0x104, w(in_game, 0x104) + 1);                          // 0062B71E
}

// 00630560 CGuiInGame::AppendToDialogBuffer(text, speaker id), RET 8: resolves the speaker's name through the client
// game object (vtable +0Ch -> object, then +70h(out) -> CExoString name), then 0062B680.
extern "C" void __fastcall kotor_fastcall_00630560(void* in_game, void*, void* entry, std::uint32_t speaker) {
    StartupString* text = static_cast<StartupString*>(entry);
    if (client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(entry), nullptr, k_empty_0073d71c) == 0)
        return;                                                         // 00630580..0063058E
    void* object = kotor_fastcall_005ed580(client_of_manager(), nullptr, speaker); // 00630590..0063059E
    if (!object) return;                                                // 006305A5..006305A7
    frame_log::String name;
    kotor_movie::string_element_construct_005b3190(&name, nullptr);     // 006305A9..006305AD (EH state 0 from 006305B6)
    struct NameGuard { frame_log::String* p; ~NameGuard() { frame_LogStringDtor_005e5c20(p, nullptr); } } guard{&name}; // 006305FB..00630603 / unwind
    void* inner = at(vcall0(object, 0x0c));                             // 006305B2..006305BE
    {
        frame_log::String temp;
        vcall1(inner, 0x70, static_cast<U32>(reinterpret_cast<std::uintptr_t>(&temp))); // 006305C1..006305CA (hidden out)
        struct TempGuard { frame_log::String* p; ~TempGuard() { frame_LogStringDtor_005e5c20(p, nullptr); } } temp_guard{&temp}; // EH state 1 (006305D2)
        kotor_reconstruction::StartupAssign005e5c50(reinterpret_cast<StartupString*>(&name), nullptr,
                                                    reinterpret_cast<StartupString*>(&temp)); // 006305CD..006305D7
    }                                                                   // 006305DC..006305E5 temporary destroyed
    kotor_fastcall_0062b680(in_game, nullptr, text, reinterpret_cast<StartupString*>(&name)); // 006305EA..006305F2
}

// ---- CreateCaptureEventFile (routes winmain_bodies 00401781, command-line capture option only) ----
namespace kotor_menu_10_19 { namespace dependencies { std::uint64_t __stdcall unsigned_divide_006fb030(std::uint64_t, std::uint64_t); } }
extern "C" {
void __fastcall kotor_fastcall_005f5720(void* internal, void*, StartupString* name);
void __fastcall kotor_fastcall_005ed820(void* outer, void*, void* name);
}
namespace {
const char k_capture_format_0074e9f8[] = "HD0:%s";       // .rdata 0074E9F8
const char k_write_binary_0073d8a4[] = "wb";             // .rdata 0073D8A4
const char k_start_format_0074e9ec[] = "Start: %d\n";    // .rdata 0074E9EC
struct LogStr {                                          // CExoString local, ctor 005B3190 / dtor 005E5C20
    frame_log::String s{};
    LogStr() { kotor_movie::string_element_construct_005b3190(&s, nullptr); }
    ~LogStr() { frame_LogStringDtor_005e5c20(&s, nullptr); }
    LogStr(const LogStr&) = delete;
};
void destroy_capture_file(void* internal) {              // 005F57A0..005F57BA / 005F58BC..005F58D6
    if (void* file = pw(internal, 0x88)) {
        frame_LogFileDtor_005e69e0(static_cast<frame_log::File*>(file), nullptr);
        live_free(file);
    }
    sw(internal, 0x88, 0);
}
}
// 005F5720 CClientExoAppInternal::CreateCaptureEventFile(name), RET 4.
extern "C" void __fastcall kotor_fastcall_005f5720(void* internal, void*, StartupString* name) {
    sw(internal, 0x8c, 1);                                              // 005F5748..005F574F
    if (w(internal, 0x90) == 1) { sw(internal, 0x8c, 0); return; }     // 005F5742..005F5757
    if (name->BorrowedEquals005e5290(k_empty_0073d71c) != 0) { sw(internal, 0x8c, 0); return; } // 005F5775..005F5786
    if (pw(internal, 0x88)) destroy_capture_file(internal);             // 005F57A0..005F57BA
    LogStr path;                                                        // [esp+18] 005F57C0..005F57C4 (EH state 0)
    frame_LogFormat_005e5680(&path.s, k_capture_format_0074e9f8, frame_LogCStr_005e5670(reinterpret_cast<frame_log::String*>(name), nullptr)); // 005F57C9..005F57DF
    void* memory = live_operator_new(4);                                // 005F57E4..005F57E6 (EH state 1)
    void* file = nullptr;
    if (memory) {                                                       // 005F57F4..005F57FB
        frame_log::String mode{};
        frame_LogStringCtor_005e5a90(&mode, nullptr, k_write_binary_0073d8a4); // 005F57FD..005F5806 (EH state 2)
        struct ModeGuard { frame_log::String* p; ~ModeGuard() { frame_LogStringDtor_005e5c20(p, nullptr); } } guard{&mode}; // 005F5835..005F5844
        frame_LogFileCtor_005e68d0(static_cast<frame_log::File*>(memory), nullptr, &path.s, 0x0a, &mode); // 005F580B..005F5826
        file = memory;
    }
    sw(internal, 0x88, static_cast<U32>(reinterpret_cast<std::uintptr_t>(file))); // 005F582F
    if (!pw(internal, 0x88)) return;                                    // 005F5849..005F5851
    if (frame_LogFileOpened_005e6a10(static_cast<frame_log::File*>(pw(internal, 0x88)), nullptr) == 0) { // 005F5857..005F585E
        destroy_capture_file(internal);                                 // 005F58BC..005F58D6
        return;
    }
    const std::uint64_t now = static_cast<std::uint64_t>(menu_continuation::exo_base_007a39e0->timers->GetHighResolutionTimer_005e9260()); // 005F5860..005F5868
    const std::uint32_t start_ms = static_cast<std::uint32_t>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(now, 1000)); // 005F586D..005F5875
    LogStr line;                                                        // [esp+10] 005F587A..005F5880 (EH state 4)
    frame_LogFormat_005e5680(&line.s, k_start_format_0074e9ec, start_ms); // 005F5885..005F5895
    frame_LogWriteString_005e6a70(static_cast<frame_log::File*>(pw(internal, 0x88)), nullptr, &line.s); // 005F589A..005F58A8
}
// 005ED820 CClientExoApp::CreateCaptureEventFile: mov ecx,[ecx+4]; jmp 005F5720.
extern "C" void __fastcall kotor_fastcall_005ed820(void* outer, void*, void* name) {
    kotor_fastcall_005f5720(pw(outer, 4), nullptr, static_cast<StartupString*>(name)); // 005ED820..005ED823
}
