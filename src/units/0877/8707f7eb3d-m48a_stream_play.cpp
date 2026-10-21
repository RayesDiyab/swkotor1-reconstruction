// lane-m48-a: CExoStreamingSoundSourceInternal::Play 005DCA30 (thiscall(ref, second, third, fourth, fifth) RET 14h,
// EAX = the ms position the stream reports into the `second` argument slot, or 0).
// Re-authored from the original onto current canonical headers and the existing single providers (the M35-epoch donor
// sound/donors/donor-stream-play/stream_play.cpp is not used: absolute M35 includes / StartupString C2011, mutant
// macros, inline-asm float copy). Consumer: sound_stream_play29::Play005dca30 (sound-path, sound-render).
// Evidence: swkotor.exe GOG K1 dumpbin /disasm 005DCA30..005DCD48 (lane-m48-a/d/005DCA30.txt); EH FuncInfo 00724340:
// state 0 = the CExoString temporary at [esp+28h] (005DCA79 ctor 005B3190 .. 005DCC38 dtor 005E5C20), state 1 = the
// resolved String at [esp+18h] around 005DCBD6 Assign (005DCBE4 dtor). Both are C++ RAII here (emitted cleanup).
// Providers (existing M47 symbols): 00406DC0 gui_load_resref_not_equal_00406dc0; 005DC380/005D7BA0/005D80A0/005DC4D0/
// 005D8470/005D6BC0/005DBD30/005DC560 sound_stream_play29_required (sound-lists/-stream-open/-volume); 00405F70
// c2da_ref_to_string_00405f70; 005E5670 WindowCStr005e5670; 00406120 resource_ref_assignment::assign_00406120 (D48);
// 005E5680 kotor_reconstruction::StartupFormat005e5680; 005E5600 directory_list::At005e5600; 005E5790/005E5C50
// StartupString members; 005E68A0 path_resolution::resolve_005e68a0; 005D7AB0/005D7A00 kotor_sound_path_required;
// 005DC930 sound_lower29_required::volume_005dc930; 005DC3C0 lane-m48-a stream_pause.cpp; cells 007A39E0
// kotor_menu_10_19::dependencies::base_007a39e0, 00832890 sound_pause29_required::word_00832890; format strings
// 0074C304/0074A808/0074A7F4/0074D33C (sound-data, D13). Miles IAT 0073D570 AIL_pause_stream, 0073D538 AIL_close_stream,
// 0073D56C AIL_set_stream_loop_count, 0073D588 AIL_stream_ms_position, 0073D584 AIL_start_stream.
// Not reached with Disable Sound=1 (D17).
#include <cstdint>
#include <new>
#include <cstring>
#include "ab5e650e56-startup_string.h"
#include "f1dcaa377b-resources.h"

namespace alias_path { struct AliasListView; }
namespace path_resolution {
kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*, void*,
    kotor_reconstruction::StartupString*, kotor_reconstruction::StartupString*, std::uint32_t);
}
namespace resource_ref_assignment {
__declspec(noinline) menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*,
                                                                     const menu_resources::Ref*);
}
namespace directory_list { __declspec(noinline) char __fastcall At005e5600(kotor_reconstruction::StartupString*, void*, std::int32_t); }
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }
namespace sound_stream_play29_required {
// lane-m56-soundplay: the four format strings are .rdata CHAR ARRAYS (sound-data defines `const char format_...[N]`;
// the original pushes their ADDRESSES: 005DCAC7 push 74C304h, 005DCB80 push 74A808h, 005DCB9A push 74A7F4h,
// 005DCBA7 push 74D33Ch). MSVC mangles `const char x[N]` and `const char* const x` identically (3QBDB), so the old
// pointer declarations linked silently and loaded the first 4 string bytes ('HD0:' = 3A304448h) as the format pointer.
extern const char format_0074c304[];
extern const char format_0074a808[];
extern const char format_0074a7f4[];
extern const char format_0074d33c[];
void __fastcall close_existing_005dc380(void*, void*);
void __fastcall attach_005d7ba0(void*, void*, void*);
void __fastcall remove_group_005d80a0(void*, void*, std::uint32_t);
std::uint32_t __fastcall attachment_query_005dc4d0(void*, void*);
std::uint32_t __fastcall start_005d8470(void*, void*, void*);
std::uint32_t __fastcall pause_query_005d6bc0(void*, void*, void*);
void __fastcall open_005dbd30(void*, void*);
void __fastcall refresh_005dc560(void*, void*);
}
namespace sound_pause29_required {
extern volatile std::uint32_t* const word_00832890;
std::uint32_t __fastcall stream_pause_005dc3c0(void*, void*);
}
namespace kotor_sound_path_required {
std::uint32_t __fastcall remove_streaming_005d7ab0(void*, void*, void*);
std::uint32_t __fastcall remove_nonplaying_streaming_005d7a00(void*, void*, void*);
}
namespace sound_lower29_required { void __fastcall volume_005dc930(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t); }
extern "C" {
std::uint32_t __fastcall gui_load_resref_not_equal_00406dc0(const menu_resources::Ref*, void*, const menu_resources::Ref*);
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*, void*, menu_resources::String*);
char* __fastcall WindowCStr005e5670(menu_resources::String*, void*);
__declspec(dllimport) void __stdcall AIL_pause_stream(void*, std::int32_t);
__declspec(dllimport) void __stdcall AIL_close_stream(void*);
__declspec(dllimport) void __stdcall AIL_set_stream_loop_count(void*, std::int32_t);
__declspec(dllimport) void __stdcall AIL_stream_ms_position(void*, std::int32_t*, std::int32_t*);
__declspec(dllimport) void __stdcall AIL_start_stream(void*);
}

namespace {
using U = std::uint32_t; using I = std::int32_t;
using String = kotor_reconstruction::StartupString;
using Ref = menu_resources::Ref;
template<class T> T& field(void* base, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(base) + offset); }
// EH state 1: the String the resolver constructed in the frame storage at [esp+18h] (005DCBE4 call 005E5C20).
struct ResolvedString {
    String* storage;
    ~ResolvedString() { storage->~String(); }
};
// fld dword [system+14h]; fstp dword [esp]: x87 float->float copy (SNaN quieted). SSE2 float->double->float has the
// same effect; the volatile double keeps the conversion pair.
U pushed_float_bits(float value) {
    volatile double wide = value;
    const float narrow = static_cast<float>(wide);
    U bits; static_assert(sizeof(bits) == sizeof(narrow), "dword");
    std::memcpy(&bits, &narrow, 4);
    return bits;
}
}

namespace sound_stream_play29 {
U __fastcall Play005dca30(void* stream, void* /*unused_edx*/, Ref* reference, U second, U third, U fourth, U /*fifth*/) {
    Ref* const current = reinterpret_cast<Ref*>(static_cast<unsigned char*>(stream) + 9);           // esi+9
    if (field<void*>(stream, 0) != nullptr &&                                                        // 005DCA51..005DCA58
        gui_load_resref_not_equal_00406dc0(reference, nullptr, current) == 0) {                      // 005DCA5A..005DCA67
        sound_stream_play29_required::close_existing_005dc380(stream, nullptr);                      // 005DCA69..005DCA6B
    } else {
        String name;                                                                                 // 005DCA75..005DCA79 (state 0)
        c2da_ref_to_string_00405f70(reference, nullptr, &name);                                      // 005DCA7E..005DCA89
        (void)WindowCStr005e5670(&name, nullptr);                                                    // 005DCA8E..005DCA92 (EAX unused)
        alignas(4) unsigned char frame[0x10];                                                        // [esp+18h] (copied Ref / resolved String)
        resource_ref_assignment::assign_00406120(current, nullptr, reinterpret_cast<Ref*>(frame), reference); // 005DCA97..005DCAA0
        field<std::uint16_t>(stream, 0x44) = static_cast<std::uint16_t>(second);                     // 005DCAA5..005DCAB0
        String* const persistent = &field<String>(stream, 0x24);                                     // 005DCAAA lea edi,[esi+24h]
        c2da_ref_to_string_00405f70(reference, nullptr, persistent);                                 // 005DCAAD..005DCAB4
        if (fourth != 0) {                                                                           // 005DCAB9..005DCABF
            kotor_reconstruction::StartupFormat005e5680(persistent, sound_stream_play29_required::format_0074c304,
                WindowCStr005e5670(persistent, nullptr));                                            // 005DCAC1..005DCAC7, 005DCBAC
        } else if (third != 0) {                                                                     // 005DCAD1..005DCAD5
            if ((directory_list::At005e5600(persistent, nullptr, 0) == 'n' ||                        // 005DCADB..005DCAE3
                 directory_list::At005e5600(persistent, nullptr, 0) == 'N') &&                       // 005DCAE5..005DCAEF
                persistent->BorrowedLength005e5790() == 0x10 &&                                      // 005DCAF5..005DCAFF
                directory_list::At005e5600(persistent, nullptr, 1) != '_') {                         // 005DCB05..005DCB10
                char* const first = reinterpret_cast<char*>(frame);                                  // [esp+18h]
                char latter[8];                                                                      // [esp+10h]
                for (I i = 0; i < 5; ++i) first[i] = directory_list::At005e5600(persistent, nullptr, i + 1); // 005DCB16..005DCB3A
                first[5] = '\0';                                                                     // 005DCB3C
                for (I i = 0; i < 6; ++i) latter[i] = directory_list::At005e5600(persistent, nullptr, i + 6); // 005DCB41..005DCB67
                latter[6] = '\0';                                                                    // 005DCB6B
                kotor_reconstruction::StartupFormat005e5680(persistent, sound_stream_play29_required::format_0074a808,
                    first, latter, WindowCStr005e5670(persistent, nullptr));                         // 005DCB69..005DCB86
            } else {
                kotor_reconstruction::StartupFormat005e5680(persistent, sound_stream_play29_required::format_0074a7f4,
                    WindowCStr005e5670(persistent, nullptr));                                        // 005DCB92..005DCBAD
            }
        } else {
            kotor_reconstruction::StartupFormat005e5680(persistent, sound_stream_play29_required::format_0074d33c,
                WindowCStr005e5670(persistent, nullptr));                                            // 005DCBA1..005DCBAD
        }
        {
            void* const resolver = field<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 0x0C);   // 005DCBB9..005DCBC6
            String* const storage = reinterpret_cast<String*>(frame);                                // lea ecx,[esp+20h] (= [esp+18h])
            String* const resolved = path_resolution::resolve_005e68a0(
                static_cast<alias_path::AliasListView*>(resolver), nullptr, storage, persistent, second); // 005DCBB5..005DCBC9
            ResolvedString lifetime{storage};                                                        // state 1 (005DCBD1)
            persistent->Assign005e5c50(resolved);                                                    // 005DCBCE..005DCBD6
        }                                                                                            // 005DCBDB..005DCBE4 dtor
        if (void* const handle = field<void*>(stream, 0)) AIL_pause_stream(handle, 1);               // 005DCBE9..005DCBF2
        const U attached = field<U>(stream, 0x1C);                                                   // 005DCBF8
        void* const system = field<void*>(stream, 0x20);                                             // 005DCBFB
        field<U>(stream, 0x2C) = 0; field<U>(stream, 0x34) = 0; field<U>(stream, 0x40) = 0;          // 005DCBFE..005DCC04
        if (attached != 0) kotor_sound_path_required::remove_streaming_005d7ab0(system, nullptr, stream); // 005DCC07..005DCC0A
        else kotor_sound_path_required::remove_nonplaying_streaming_005d7a00(system, nullptr, stream);   // 005DCC11
        if (void* const handle = field<void*>(stream, 0)) {                                          // 005DCC16..005DCC1A
            AIL_close_stream(handle);                                                                // 005DCC1C..005DCC1D
            field<void*>(stream, 0) = nullptr;                                                       // 005DCC23
        }
        sound_stream_play29_required::open_005dbd30(stream, nullptr);                                // 005DCC25..005DCC27
    }                                                                                                // 005DCC2C..005DCC38 (state -1)
    void* system = field<void*>(stream, 0x20);                                                       // 005DCC3D
    if (field<U>(system, 8) == 0) return 0;                                                          // 005DCC40..005DCC43
    if (field<void*>(stream, 0) == nullptr) return 0;                                                // 005DCC49..005DCC4B
    sound_lower29_required::volume_005dc930(stream, nullptr, field<std::uint8_t>(stream, 0x46), 1u,
        pushed_float_bits(field<float>(system, 0x14)));                                              // 005DCC51..005DCC62
    const U loop_forever = field<U>(stream, 4) == 0 ? 1u : 0u;                                       // 005DCC67..005DCC71 sete
    sound_stream_play29_required::refresh_005dc560(stream, nullptr);                                 // 005DCC73..005DCC75
    AIL_set_stream_loop_count(field<void*>(stream, 0), static_cast<I>(loop_forever));                // 005DCC7A..005DCC7E
    void* const handle = field<void*>(stream, 0);                                                    // 005DCC84
    field<U>(stream, 0x40) = 0;                                                                      // 005DCC8D
    AIL_stream_ms_position(handle, reinterpret_cast<I*>(&second), nullptr);                          // 005DCC86..005DCC90 (out = [esp+44h] slot)
    const std::uint8_t type = field<std::uint8_t>(stream, 8);                                        // 005DCC96 mov dl,[esi+8]
    system = field<void*>(stream, 0x20);                                                             // 005DCC99
    unsigned char* const group = field<unsigned char*>(system, 0x4C) + static_cast<U>(type) * 0x18u; // 005DCC9C..005DCCAD
    if (!(group[5] < group[4])) {                                                                    // 005DCCA5..005DCCB0 jb
        if (*reinterpret_cast<U*>(group) == 0) {                                                     // 005DCCB2..005DCCB5
            if (field<U>(stream, 4) == 0) return 0;                                                  // 005DCCB7..005DCCBC
            sound_stream_play29_required::attach_005d7ba0(system, nullptr, stream);                  // 005DCCBE..005DCCBF (ecx = system)
            return second;                                                                           // 005DCCC4
        }
        sound_stream_play29_required::remove_group_005d80a0(system, nullptr, type);                  // 005DCCCA..005DCCCB (low byte read)
    }
    if (field<U>(stream, 4) != 0 &&                                                                  // 005DCCD0..005DCCD5
        sound_stream_play29_required::attachment_query_005dc4d0(stream, nullptr) == 0) {             // 005DCCD7..005DCCE0
        sound_stream_play29_required::attach_005d7ba0(field<void*>(stream, 0x20), nullptr, stream);  // 005DCCE2..005DCCE6
        return second;                                                                               // 005DCCEB
    }
    if (sound_stream_play29_required::start_005d8470(field<void*>(stream, 0x20), nullptr, stream) == 0) return 0; // 005DCCF1..005DCCFC
    AIL_start_stream(field<void*>(stream, 0));                                                       // 005DCCFE..005DCD01
    system = field<void*>(stream, 0x20);                                                             // 005DCD07
    if (field<U>(system, 0x144) != 0 ||                                                              // 005DCD0A..005DCD12
        (*sound_pause29_required::word_00832890 != 0 &&                                              // 005DCD14..005DCD1B
         sound_stream_play29_required::pause_query_005d6bc0(system, nullptr, stream) != 0))          // 005DCD1D..005DCD25
        sound_pause29_required::stream_pause_005dc3c0(stream, nullptr);                              // 005DCD27..005DCD29
    return second;                                                                                   // 005DCD2E
}
}
