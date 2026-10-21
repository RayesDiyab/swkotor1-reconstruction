// lane-m48-b fork CLIENT, unit client-sound-shutdown: CExoSound teardown referenced by routes-shutdown
// (shutdown_bodies.cpp:202 `kotor_fastcall_005d62f0(sound)`). Under the D17 run configuration (Disable Sound=1) the
// CExoSoundInternal object is never created, so 005D62F0 skips the internal destructor; the bodies are link-required.
// Evidence: dumpbin /disasm swkotor.exe 005D62F0..005D6312, 005D9400..005D958x (Ghidra ranges of ~CExoSoundInternal are
// incomplete; the full contiguous body 005D9400..005D9580 was disassembled), 005D6D30..005D6EEE. Miles imports are the
// original mss32.dll entries (IAT 0073D4C4..0073D4D0, lane-audit-r7 frozen mss32.lib, D77).
#include "e13053e600-r4_lowers.h"

using namespace r4_lowers;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { __declspec(noinline) void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*); }
namespace kotor_sound_internal { void __fastcall StringListDestroy005e9860(void* self, void*); }   // 005E9860 (sound-internal)
namespace sound_stream_play29_required { void __fastcall close_existing_005dc380(void*, void*); }   // 005DC380
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void*, void*); }
extern "C" {
__declspec(dllimport) void __stdcall AIL_release_3D_sample_handle(void*);    // [0073D4D0]
__declspec(dllimport) void __stdcall AIL_release_sample_handle(void*);       // [0073D4CC]
__declspec(dllimport) void __stdcall AIL_close_3D_provider(void*);           // [0073D4C8]
__declspec(dllimport) void __stdcall AIL_quick_shutdown();                   // [0073D4C4]
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
void __fastcall kotor_fastcall_004015a0(void*, void*);                       // 004015A0 RET-only element destructor (routes-shutdown)
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);
extern volatile std::uint32_t kotor_cell_disable_sound_00832894;            // sound-owners (D25)
void __fastcall kotor_fastcall_005d6d30(void* internal, void*);
void __fastcall kotor_fastcall_005d9400(void* internal, void*);
void __fastcall kotor_fastcall_005d62f0(void* sound, void*);
}
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }

namespace {
// .rdata 0074D208 "CExoSoundSource %s not freed" (dumpbin /rawdata 0074D208..0074D224).
const char k_not_freed_0074d208[] = "CExoSoundSource %s not freed";
}

// 005D6D30 CExoSoundInternal::ShutDownSoundSystem (Ghidra unnamed body called first by ~CExoSoundInternal).
extern "C" void __fastcall kotor_fastcall_005d6d30(void* internal, void*) {
    void* streams = pw(internal, 0x44);                                 // 005D6D53
    void* position = pw(streams, 0);                                    // 005D6D56..005D6D58
    void* head = pw(streams, 0);                                        // 005D6D5C
    void* source = head ? pw(head, 8) : nullptr;                        // 005D6D60..005D6D69
    while (position) {                                                  // 005D6D6C..005D6D70 / 005D6D99..005D6D9D
        (void)unresolved_list_next_005e9b90(pw(internal, 0x44), nullptr, &position); // 005D6D72..005D6D7A
        sound_stream_play29_required::close_existing_005dc380(source, nullptr);      // 005D6D7F..005D6D81
        if (!position) break;                                           // 005D6D86..005D6D8C
        source = at(unresolved_list_at_pos_005e9b60(pw(internal, 0x44), nullptr, position)); // 005D6D8E..005D6D97
    }
    while (w(pw(internal, 0x40), 0) != 0) {                             // 005D6D9F..005D6DA4 / 005D6E10..005D6E15
        void* first = pw(pw(internal, 0x40), 0);                        // 005D6DB0..005D6DB3
        void* sound = first ? pw(first, 8) : nullptr;                   // 005D6DB5..005D6DBD
        if (w(sound, 0x28) == 0) {                                      // 005D6DC0..005D6DC3
            // Debug message built and discarded (the original's logging call is compiled out): 005D6DC5..005D6E03.
            frame_log::String text;
            kotor_movie::string_element_construct_005b3190(&text, nullptr); // 005D6DC5..005D6DC9
            struct Guard { frame_log::String* p; ~Guard() { frame_LogStringDtor_005e5c20(p, nullptr); } } guard{&text};
            c2da_ref_to_string_00405f70(bytes(sound) + 0x0c, nullptr, &text); // 005D6DCE..005D6DDA
            const char* name = frame_LogCStr_005e5670(&text, nullptr);  // 005D6DDF..005D6DE3
            kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&text),
                                                        k_not_freed_0074d208, name); // 005D6DE8..005D6DF8
        }                                                               // 005D6DFB..005D6E03
        vcall1(sound, 0, 1);                                            // 005D6E08..005D6E0E (scalar deleting destructor)
    }
    if (kotor_cell_disable_sound_00832894 == 0) {                       // 005D6E17..005D6E1D
        if (pw(internal, 0x30)) {                                       // 005D6E23..005D6E26
            if (b8(internal, 0x29) != 0) {                              // 005D6E28..005D6E2F
                I32 i = 0;                                              // 005D6E2B
                do {
                    unsigned char* slot = bytes(pw(internal, 0x30)) + i * 16; // 005D6E33..005D6E36
                    if (void* handle = pw(slot, 4)) {                   // 005D6E3A..005D6E3C
                        AIL_release_3D_sample_handle(handle);           // 005D6E3E..005D6E3F
                        sw(bytes(pw(internal, 0x30)) + i * 16, 4, 0);    // 005D6E45..005D6E48
                    }
                    ++i;                                                // 005D6E50
                } while (i < static_cast<I32>(b8(internal, 0x29)));     // 005D6E4C / 005D6E54..005D6E56 (signed, reread)
            }
            live_free(pw(internal, 0x30));                              // 005D6E58..005D6E5C
            sw(internal, 0x30, 0);                                      // 005D6E64
        }
        if (pw(internal, 0x2c)) {                                       // 005D6E67..005D6E6A
            if (b8(internal, 0x28) != 0) {                              // 005D6E6C..005D6E73
                unsigned char i = 0;                                    // 005D6E6F
                do {
                    unsigned char* slot = bytes(pw(internal, 0x2c)) + static_cast<U32>(i) * 16; // 005D6E75..005D6E7E
                    if (void* handle = pw(slot, 4)) {                   // 005D6E82
                        AIL_release_sample_handle(handle);              // 005D6E86..005D6E87
                        sw(bytes(pw(internal, 0x2c)) + static_cast<U32>(i) * 16, 4, 0); // 005D6E8D..005D6E90
                    }
                    ++i;                                                // 005D6E97
                } while (i < b8(internal, 0x28));                       // 005D6E94 / 005D6E99..005D6E9B (unsigned)
            }
            live_free(pw(internal, 0x2c));                              // 005D6E9D..005D6EA1
            sw(internal, 0x2c, 0);                                      // 005D6EA9
        }
    }
    if (void* provider = pw(internal, 0xf8)) {                          // 005D6EAC..005D6EB4
        AIL_close_3D_provider(provider);                                // 005D6EB6..005D6EB7
        sw(internal, 0xf8, 0);                                          // 005D6EC8
        sw(internal, 8, 0);                                             // 005D6ECE
        frame_LogAssign_005e5140(reinterpret_cast<frame_log::String*>(bytes(internal) + 0xd0), nullptr, frame_log::clear_0073d71c); // 005D6EBD..005D6ED1
    }
    AIL_quick_shutdown();                                               // 005D6ED6
}

// 005D9400 CExoSoundInternal::~CExoSoundInternal (EH states 9..0 only re-run the remaining member destructors).
extern "C" void __fastcall kotor_fastcall_005d9400(void* internal, void*) {
    kotor_fastcall_005d6d30(internal, nullptr);                         // 005D9427
    if (void* first = pw(internal, 0x4c)) {                             // 005D942C..005D9433
        unsigned char* block = bytes(first) - 4;                        // 005D9438
        c2da_destroy_array_006fa852(first, 0x18, static_cast<std::int32_t>(w(block, 0)), &kotor_fastcall_004015a0); // 005D9435..005D9444
        live_free(block);                                               // 005D9449..005D944A
        sw(internal, 0x4c, 0);                                          // 005D9452
    }
    sw(internal, 0x10c, 0);                                             // 005D945D
    if (void* data = pw(internal, 0x108)) {                             // 005D9455..005D9463
        live_free(data);                                                // 005D9465..005D9466
        sw(internal, 0x108, 0);                                         // 005D946E
        sw(internal, 0x110, 0);                                         // 005D9474
    }
    sw(internal, 0x10c, 0);                                             // 005D947A
    live_free(pw(internal, 0x108));                                     // 005D9480..005D948C
    sw(internal, 0x108, 0);                                             // 005D9491
    live_free(pw(internal, 0xec));                                      // 005D9497..005D94A3
    sw(internal, 0xec, 0);                                              // 005D94B1
    kotor_sound_internal::StringListDestroy005e9860(bytes(internal) + 0xd8, nullptr); // 005D94AB..005D94BC
    frame_LogStringDtor_005e5c20(reinterpret_cast<frame_log::String*>(bytes(internal) + 0xd0), nullptr); // 005D94C1..005D94CC
    static const U32 k_lists[6] = {0x48u, 0x44u, 0x40u, 0x3cu, 0x38u, 0x34u};
    for (U32 off : k_lists) {                                           // 005D94D1..005D9578
        if (void* list = pw(internal, off)) {
            base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(list), nullptr);
            live_free(list);
        }
    }
}

// 005D62F0 CExoSound::~CExoSound: internal [this]; ~CExoSoundInternal then free.
extern "C" void __fastcall kotor_fastcall_005d62f0(void* sound, void*) {
    if (void* internal = pw(sound, 0)) {                                // 005D62F2..005D62F8
        kotor_fastcall_005d9400(internal, nullptr);                     // 005D62FA..005D62FC
        live_free(internal);                                            // 005D6301..005D6302
        sw(sound, 0, 0);                                                // 005D630A (lane-m58-exitsound: [this]=0 restored)
    }
}
