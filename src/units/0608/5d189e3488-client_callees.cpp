// lane-prepare: client_services::Dependencies callees and small CExoInput /
// CSWParty / CGuiInGame bodies on the Initialize 005F51C0 / input 005F6B10 /
// party-ctor paths. D1 form: native thiscall as __fastcall(this, unused EDX).
// Each statement cites its swkotor.exe instruction(s); objects are minimum
// addressed views (only the cited offsets are touched).
#include "2ff1299369-game_prepare_producers.h"
#include "84a5500766-startup_string.h"      // kotor_reconstruction::StartupString: sole 005B3190/005E5A90/005E5C20 owner
#include "f9de668f9a-gui_mouse.h"           // gui_mouse::Gui::activate_0040b060 (M41 service-gui_0040B060-26)
#include "df72b7ab87-window_shared_cells.h" // kotor_menu_window_cells::input_007a39e4 / manager_007a39fc
#include "ed8f77ad6d-allocator_live.h"
#include <cstring>
#include <new>

namespace {
using Word = std::uint32_t;
using kotor_reconstruction::StartupString;
inline Word get32(const void* p, unsigned off) noexcept { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void put32(void* p, unsigned off, Word v) noexcept { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline void* ptr(Word w) noexcept { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
inline Word word_of(const void* p) noexcept { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
inline float getf(const void* p, unsigned off) noexcept { float v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
static_assert(sizeof(client_services::String) == sizeof(StartupString) && sizeof(kotor_window_shell::String) == sizeof(StartupString),
              "8-byte CExoString storage (data, length) in every typed view");
}

namespace kotor_game_prepare {
// Typed transports: the client_services / window_shell String views are the
// same 8-byte CExoString storage; the sole algorithm owner StartupString runs
// in that storage (005E5A90 ctor(char const*), 005B3190 ctor(), 005E5C20 dtor).
client_services::String* __fastcall ConstructStringText005e5a90(client_services::String* s, void*, const char* text) {
    ::new(static_cast<void*>(s)) StartupString(text);
    return s;
}
void __fastcall DestroyString005e5c20(client_services::String* s, void*) {
    std::launder(reinterpret_cast<StartupString*>(s))->~StartupString();
}
kotor_window_shell::String* __fastcall kotor_shell_string_construct_005b3190(kotor_window_shell::String* s, void*) noexcept {
    ::new(static_cast<void*>(s)) StartupString();   // 005B3192/005B3198 [this]=0,[this+4]=0
    return s;
}
void __fastcall kotor_shell_string_destroy_005e5c20(kotor_window_shell::String* s, void*) noexcept {
    std::launder(reinterpret_cast<StartupString*>(s))->~StartupString(); // 005E5C23..005E5C39
}
}

extern "C" {
// 005DF4E0 CExoInput::ClearEvents: mov ecx,[ecx+4]; jmp 005E0710.
void __fastcall kotor_fastcall_005df4e0(void* input, void*) { kotor_fastcall_005e0710(ptr(get32(input, 4)), nullptr); }
// 005DF540 CExoInput::SetActive(int): mov ecx,[ecx+4]; jmp 005DFFB0 (RET 4 there).
void __fastcall kotor_fastcall_005df540(void* input, void*, int active) { kotor_fastcall_005dffb0(ptr(get32(input, 4)), nullptr, active); }
// 005DFFB0 CExoInputInternal::SetActive(int): [+154]=arg; ecx=[+140]; if non-null
// tail-jump 005E3C20 with the same stack word, else RET 4.
void __fastcall kotor_fastcall_005dffb0(void* internal, void*, int active) {
    put32(internal, 0x154, static_cast<Word>(active));                  // 005DFFB4
    void* next = ptr(get32(internal, 0x140));                            // 005DFFBA
    if (next) kotor_fastcall_005e3c20(next, nullptr, active);            // 005DFFC4/005DFFC8
}
// 005F2980 CClientExoAppInternal::TurnOffKeyboardModal.
void __fastcall kotor_fastcall_005f2980(void* internal, void*) {
    auto* b = static_cast<unsigned char*>(internal);
    unsigned i = 0;
    for (; i < 7; ++i) if (get32(b, 0x100 + i * 4) != 0) break;          // 005F2984..005F299C
    if (i < 7) {                                                          // 005F29A0
        std::memset(b + 0x100, 0, 7 * 4);                                 // 005F29A7 rep stos
        put32(b, 0xFC, 1); put32(b, 0xF4, 0); put32(b, 0xF8, 0);          // 005F29AF..005F29BF
        kotor_fastcall_00677e50(ptr(get32(b, 0x14C)), nullptr);          // 005F29A9/005F29C5
    }
    void* module = ptr(get32(b, 0x18));                                   // 005F29CA
    if (module) {
        void* area = ptr(get32(module, 0x48));                            // 005F29D1
        if (area) kotor_fastcall_00605c40(area, nullptr, 0);              // 005F29D8/005F29DA
    }
}
// 005EDA40 CClientExoApp::GetSWGuiManager: return [[ecx+4]+274h].
void* __fastcall kotor_fastcall_005eda40(void* client, void*) { return ptr(get32(ptr(get32(client, 4)), 0x274)); }

// 00634B60 CSWPartyFollowDataArray::Reset(const Vector*, value) RET 8:
// 100 entries of 28h bytes: +0..+8 = *vector, +0C = value; then [+FA8]=[+FA4]=0.
void __fastcall kotor_fastcall_00634b60(void* self, void*, const void* vector, std::uint32_t value) {
    auto* b = static_cast<unsigned char*>(self);
    for (unsigned row = 0; row < 10; ++row)                               // 00634B6B esi=0Ah ... 00634CA5
        for (unsigned k = 0; k < 10; ++k) {
            unsigned char* e = b + row * 0x190 + k * 0x28;
            put32(e, 0, get32(vector, 0)); put32(e, 4, get32(vector, 4)); put32(e, 8, get32(vector, 8));
            put32(e, 0x0C, value);
        }
    put32(b, 0xFA8, 0); put32(b, 0xFA4, 0);                               // 00634CB0/00634CB6
}
// 00635A90 CSWPartyFollowDataArray::CSWPartyFollowDataArray: zero ten words of
// each of 64h entries (stride 28h), then Reset(&{0,0,0}, 0).
void* __fastcall kotor_fastcall_00635a90(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    for (unsigned k = 0; k < 0x64; ++k) {                                 // 00635A97 esi=64h
        unsigned char* e = b + k * 0x28;
        for (unsigned off : {0x00u, 0x04u, 0x08u, 0x10u, 0x14u, 0x18u, 0x1Cu, 0x20u, 0x24u, 0x0Cu}) put32(e, off, 0); // 00635AA0..00635ABA
    }
    const Word zero_vector[3] = {0, 0, 0};                                // 00635AC9..00635AD9
    kotor_fastcall_00634b60(self, nullptr, zero_vector, 0);              // 00635AC3/00635AE1
    return self;                                                          // 00635AE6
}
// 00634760 CSWParty::SetFormation(int) RET 4 (self tail-call at 00634807).
void __fastcall kotor_fastcall_00634760(void* self, void*, int formation) {
    auto* b = static_cast<unsigned char*>(self);
    for (;;) {
        const int current = static_cast<int>(get32(b, 0x1BC));            // 00634760
        int f = formation;
        if (f == current || f >= 3) return;                               // 0063476A..00634775
        unsigned src1, src2; Word weight;
        if (f >= 0) {                                                     // 00634781 jl
            src1 = 0x1C8 + static_cast<unsigned>(f) * 36;                 // 00634783/00634786
            src2 = static_cast<unsigned>(f * 9 + 0x75) * 4;               // 006347A5/006347A9
            weight = get32(b, 0x234 + static_cast<unsigned>(f) * 4);      // 006347C4
        } else if (f == -9999) {                                          // 006347D0 (0FFFFD8F1h)
            f = current + 1;                                              // 006347D7
            if (f >= 3) f = -3;                                           // 006347DA..006347DF
            if (f < 0) {                                                  // 006347E6/006347EA loop
                unsigned at = 0x2AB - static_cast<unsigned>(f);           // edx = this - eax + 2ABh
                do { if (b[at] != 0) break; ++f; --at; } while (f < 0);   // 006347F4..006347FD
            }
            formation = f;                                                // 00634803 [esp+4]=eax; jmp 00634760
            continue;
        } else {
            if (f < -3) return;                                           // 0063480C/0063480F
            if (b[0x2AB - f] == 0) return;                                // 00634815..00634821
            src1 = static_cast<unsigned>(0x0F - f) * 36;                  // 00634827..00634831
            src2 = 0x228 - static_cast<unsigned>(f * 36);                 // 0063484C..00634856
            weight = get32(b, static_cast<unsigned>(0x8C - f) * 4);       // 00634874..0063487B
        }
        for (unsigned i = 0; i < 3; ++i) put32(b, 0xE4 + i * 4, get32(b, src1 + i * 4));  // ...0063479F / 00634849
        for (unsigned i = 0; i < 3; ++i) put32(b, 0x16C + i * 4, get32(b, src2 + i * 4)); // ...006347C1 / 00634871
        put32(b, 0x1BC, static_cast<Word>(f));                           // 0063487E
        put32(b, 0x1C0, weight);                                          // 0063488C
        if (b[0x1C4]) {                                                   // 00634884..00634892
            Word t[3]; for (unsigned i = 0; i < 3; ++i) t[i] = get32(b, 0xE4 + i * 4);    // 00634896..0063489B
            for (unsigned i = 0; i < 3; ++i) put32(b, 0xE4 + i * 4, get32(b, 0x16C + i * 4)); // 006348A0..006348B2
            for (unsigned i = 0; i < 3; ++i) put32(b, 0x16C + i * 4, t[i]);               // 006348AA..006348B5
        }
        return;
    }
}
}

namespace kotor_original {
// 00632720 CGuiInGame::InitializeGlobal (EH frame 0072963C; states 0..3 free the
// pending allocation through 006FA390 if its constructor unwinds).
namespace {
template<class Ctor> void* make(Word size, Ctor ctor) {
    void* p = live_operator_new(size);
    if (!p) return nullptr;
    try { return ctor(p); } catch (...) { live_free(p); throw; }
}
}
void __fastcall kotor_fastcall_00632720(void* in_game, void*) {
    if (get32(in_game, 0x38) == 0)                                        // 00632739..0063273F
        put32(in_game, 0x38, word_of(kotor_fastcall_005eda40(ptr(get32(kotor_menu_window_cells::manager_007a39fc, 4)), nullptr))); // 00632741..0063274E
    put32(in_game, 0x98, word_of(make(0x984, [&](void* p) {               // 00632751/00632756
        return kotor_fastcall_00626df0(p, nullptr, ptr(get32(in_game, 0x38))); })));     // 0063276E..00632789
    put32(in_game, 0x9C, word_of(make(0x24E8, [&](void* p) {              // 00632780/0063278F
        return kotor_fastcall_006ce7c0(p, nullptr, ptr(get32(in_game, 0x38))); })));     // 006327A7..006327BF
    put32(in_game, 0xA0, word_of(make(0x998, [&](void* p) {               // 006327B6/006327C5
        return kotor_fastcall_006aa100(p, nullptr, ptr(get32(in_game, 0x38))); })));     // 006327DD..006327F5
    put32(in_game, 0xA4, word_of(make(0x984, [&](void* p) {               // 006327EC/006327FB
        void* box = kotor_fastcall_00626df0(p, nullptr, ptr(get32(in_game, 0x38)));      // 0063281B
        put32(p, 0, word_of(kotor_vtable_007513f8));                       // 00632820 derived vtable
        (void)box; return p; })));                                         // 00632826 stores EDI (allocation), not EAX
}
}
