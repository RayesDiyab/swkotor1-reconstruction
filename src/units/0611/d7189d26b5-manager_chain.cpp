// lane-prepare: faithful bodies for the CAppManager 00401160 constructor callees
// reached from reconstructed WinMain 004042C9 (via client_services::dependencies).
// Native thiscall receivers use the ABI-identical __fastcall(this, unused EDX)
// form. Objects are the caller's 006FA7E6 allocations (minimum addressed views);
// only the cited bytes are written, nothing else is invented or zero-filled.
#include "2ff1299369-game_prepare_producers.h"
#include "84a5500766-startup_string.h"      // kotor_reconstruction::StartupString (005B3190 / 005E5C20 owner)
#include "a6391dda4a-string_priority.h"     // GuiAssign005e5140 (CExoString::operator=(char const*))
#include "89b65663c9-startup_views.h"       // TimerDependency::high_resolution_005e9260, unsigned_divide_006fb030, base cell
#include "3e75a3deac-d1_bodies.h"           // lane-menu-tables D1: ClientListConstruct_004b25d0, LiveContentConstruct/Destroy, cells
#include "ed8f77ad6d-allocator_live.h"      // live_operator_new / live_free = 006FA7E6 / 006FA390
#include "97d5414c83-constructors.h"        // kotor_list::Internal 005E9AA0 + kotor_list_dependencies (task-37)
#include <windows.h>
#include <cstring>
#include <new>
#include <type_traits>

namespace {
using Word = client_services::Word;
using kotor_reconstruction::StartupString;
inline void put32(unsigned char* b, unsigned off, Word v) noexcept { std::memcpy(b + off, &v, 4); }
inline void put8(unsigned char* b, unsigned off, unsigned char v) noexcept { b[off] = v; }
inline Word get32(const unsigned char* b, unsigned off) noexcept { Word v; std::memcpy(&v, b + off, 4); return v; }
inline void* addr(Word w) noexcept { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
inline Word word_of(const void* p) noexcept { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }

// Allocation + constructor pair with the original EH funclet: if the
// constructor unwinds, the 006FA7E6 block is released through 006FA390.
template<class Ctor>
void* allocate_construct(Word size, Ctor ctor) {
    void* p = live_operator_new(size);
    if (!p) return nullptr;                 // cmp eax,ebx / je -> xor eax,eax
    try { return ctor(p); }
    catch (...) { live_free(p); throw; }
}
}

using namespace kotor_game_prepare;
static_assert(std::is_same<decltype(&Tick0073d0f8), decltype(client_services::Dependencies::tick_0073d0f8)>::value, "tick slot");
static_assert(std::is_same<decltype(&ConstructReentrantServerStats004010c0), client_services::Constructor>::value, "stats slot");
static_assert(std::is_same<decltype(&ConstructObjectTableManager004ae5d0), client_services::Constructor>::value, "object table slot");
static_assert(std::is_same<decltype(&ConstructClientInternal005fbfe0), client_services::Constructor>::value, "internal slot");

namespace kotor_game_prepare {
// IAT 0073D0F8 kernel32!GetTickCount, called at 00401188 (stdcall, no args).
// Exact Word return type for the slot; the DWORD value is passed through.
Word __stdcall Tick0073d0f8() { return static_cast<Word>(::GetTickCount()); }

// 004010C0 CSWReentrantServerStats::CSWReentrantServerStats (0x3C allocation at
// 004011C2). Strings +18 (state -1->0), +20 (state 1), +28; then six zero words.
void* __fastcall ConstructReentrantServerStats004010c0(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    StartupString* s18 = ::new(b + 0x18) StartupString();          // 004010E1
    StartupString* s20;
    try { s20 = ::new(b + 0x20) StartupString(); }                  // 004010EF
    catch (...) { s18->~StartupString(); throw; }                   // state 0 unwind
    try { ::new(b + 0x28) StartupString(); }                        // 004010FC
    catch (...) { s20->~StartupString(); s18->~StartupString(); throw; } // state 1 unwind
    put32(b, 0x00, 0); put32(b, 0x04, 0); put32(b, 0x0C, 0);        // 00401105/07/0A
    put32(b, 0x08, 0); put32(b, 0x10, 0); put32(b, 0x14, 0);        // 0040110D/10/13
    return self;                                                    // 00401117 EAX=this
}

// 004AE5D0 CObjectTableManager::CObjectTableManager (0x184 allocations at
// 004011F1/00401220): [this+180h]=0, then rep stosd 60h zero dwords from this.
void* __fastcall ConstructObjectTableManager004ae5d0(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    put32(b, 0x180, 0);                                             // 004AE5DC
    std::memset(b, 0, 0x60 * 4);                                    // 004AE5E6 rep stos
    return self;                                                    // 004AE5E8
}

// 0066BF10 CSWMiniGameObjectArray::CSWMiniGameObjectArray (0x400 allocation at
// 005FC463): rep stosd FFh zero dwords from this+4, then [this]=0.
void* __fastcall ConstructMiniGameObjectArray0066bf10(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    std::memset(b + 4, 0, 0xFF * 4);                                // 0066BF1D
    put32(b, 0, 0);                                                 // 0066BF1F
    return self;
}

// 005FBFE0 CClientExoAppInternal::CClientExoAppInternal (0x4D4 allocation at
// 005EE314). Transcribed store-for-store; offsets/values cite the instruction.
void* __fastcall ConstructClientInternal005fbfe0(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    const Word big = 0x7F000000u;                                   // 005FC1E6 mov ebp,7F000000h
    const Word invalid = menu_tables_m41::cells::invalid_object_00746644; // .rdata 00746644
    ConstructPlayerList005f4f40(b + 0x1C, nullptr);                 // 005FC005
    ::new(b + 0x74) StartupString();                                // 005FC013
    put32(b, 0x70, 0); put32(b, 0x80, 0); put32(b, 0x7C, 0); put32(b, 0x84, 0);
    ConstructPlayerList005f4f40(b + 0x98, nullptr);                 // 005FC035
    put32(b, 0xB8, 0); put32(b, 0xBC, 0); put32(b, 0xC0, 0);
    put32(b, 0xC4, 0); put32(b, 0xC8, 0); put32(b, 0xCC, 0);
    ::new(b + 0x13C) StartupString();                               // 005FC069
    ConstructPlayerList005f4f40(b + 0x168, nullptr);                // 005FC079
    menu_tables_m41::ClientListConstruct_004b25d0(b + 0x190, nullptr);                // 005FC089
    put32(b, 0x1DC, 0); put32(b, 0x1E0, 0); put32(b, 0x1D8, 0);
    ::new(b + 0x1E4) StartupString();                               // 005FC0AB
    put32(b, 0x20C, 0); put32(b, 0x210, 0); put32(b, 0x214, 0);
    ::new(b + 0x294) StartupString();                               // 005FC0CD
    for (unsigned off : {0x2ACu, 0x2B0u, 0x2A8u, 0x2DCu, 0x2E0u, 0x2D8u, 0x2E8u, 0x2ECu, 0x2E4u, 0x2F4u, 0x2F8u, 0x2F0u})
        put32(b, off, 0);                                           // 005FC0D2..005FC114
    ::new(b + 0x2FC) StartupString();                               // 005FC125
    ::new(b + 0x304) StartupString();                               // 005FC135
    put32(b, 0x318, 0); put32(b, 0x31C, 0); put32(b, 0x314, 0);
    ::new(b + 0x330) StartupString();                               // 005FC157
    for (unsigned off : {0x344u, 0x348u, 0x340u, 0x350u, 0x354u, 0x34Cu, 0x35Cu, 0x360u, 0x358u})
        put32(b, off, 0);                                           // 005FC15C..005FC18C
    {   // 005FC1AC `eh vector constructor iterator'(this+418h, 10h, 7, 0059EA90, 0059EA30)
        unsigned built = 0;
        try { for (; built < 7; ++built) menu_tables_m41::LiveContentConstruct_0059ea90(b + 0x418 + built * 0x10, nullptr); }
        catch (...) { while (built) { --built; menu_tables_m41::LiveContentDestroy_0059ea30(b + 0x418 + built * 0x10, nullptr); } throw; } // __ArrayUnwind 006FA7F4
    }
    for (unsigned off : {0x4A8u, 0x4ACu, 0x4B0u, 0x4CCu, 0x4D0u, 0x4C8u}) put32(b, off, 0);
    put32(b, 0x1EC, 1); put32(b, 0x1D4, 1);                         // 005FC1DA/005FC1E0
    for (unsigned off : {0x4C0u, 0x26Cu, 0x204u, 0x194u, 0x19Cu}) put32(b, off, 0);
    put32(b, 0x20, big); put32(b, 0x28C, big);                      // 005FC216/005FC219
    for (unsigned off : {0x44u, 0x280u, 0x284u, 0x48u, 0x4Cu, 0x50u, 0x18u, 0x54u, 0x68u, 0x14Cu, 0x38u, 0x3Cu,
                         0x6Cu, 0x188u, 0x18Cu, 0x274u, 0x278u, 0x27Cu, 0x08u, 0x9Cu})
        put32(b, off, 0);                                           // 005FC21F..005FC270
    put8(b, 0xA8, 0);                                               // 005FC276
    put32(b, 0x2BC, 0); put32(b, 0x320, 0); put32(b, 0x324, 0); put32(b, 0x328, 0);
    put8(b, 0x414, 0);                                              // 005FC294
    put32(b, 0xA4, 0);                                              // 005FC29A
    std::memset(b + 0x100, 0, 7 * 4);                               // 005FC2A0 rep stos (edi=+100h, ecx=7)
    for (unsigned off : {0xECu, 0xF0u, 0xE4u, 0xE8u, 0xF4u, 0xF8u, 0x11Cu, 0x120u, 0x124u, 0x128u, 0xD0u, 0xD4u,
                         0xDCu, 0xE0u, 0x150u, 0x154u, 0x88u, 0x8Cu, 0x90u})
        put32(b, off, 0);                                           // 005FC2A7..005FC313
    put32(b, 0x94, 0x1E);                                           // 005FC319
    put32(b, 0x40, 0);                                              // 005FC323
    put32(b, 0x58, invalid);                                        // 005FC326/005FC32B
    put32(b, 0x170, invalid);                                       // 005FC32E/005FC336
    put32(b, 0x174, 0); put32(b, 0x178, 0); put8(b, 0x17C, 0); put32(b, 0x16C, 0);
    put32(b, 0x1A0, 0xFFFFFFFFu); put8(b, 0x1A4, 0xFF);             // 005FC354/005FC35E
    put32(b, 0x1B4, 0); put32(b, 0x1B8, 0); put32(b, 0x1BC, 0); put32(b, 0x1C4, 0); put32(b, 0x1C8, 0);
    put8(b, 0x1CC, 0);                                              // 005FC383
    put32(b, 0x1D0, word_of(allocate_construct(4, [](void* p) {     // 005FC334 push 4 / 005FC389
        return menu_tables_m41::ClientListConstruct_004b25d0(p, nullptr); }))); // 005FC3A0
    for (unsigned off : {0x24u, 0x28u, 0x2Cu, 0x30u, 0x12Cu, 0x200u, 0x60u, 0x208u, 0x220u, 0x1F0u, 0xACu, 0xB0u, 0xB4u})
        put32(b, off, 0);                                           // 005FC3AF..005FC3E8
    put32(b, 0x224, invalid);                                       // 005FC3EE/005FC3F4
    for (unsigned off : {0x238u, 0x23Cu, 0x244u, 0x240u, 0x248u, 0x24Cu, 0x250u, 0x254u, 0x258u, 0x25Cu, 0x1A8u})
        put32(b, off, 0);
    put32(b, 0x1AC, 1);                                             // 005FC441
    put32(b, 0x260, 0); put32(b, 0x264, 0); put32(b, 0x268, 0);     // 005FC447/005FC44D/005FC45D
    put32(b, 0x000, word_of(allocate_construct(0x400, [](void* p) { // 005FC453/005FC463
        return ConstructMiniGameObjectArray0066bf10(p, nullptr); })));        // 005FC47A, store 005FC48D
    put32(b, 0x270, word_of(allocate_construct(0x1290, [](void* p) { // 005FC483/005FC48F
        return ConstructParty00636170(p, nullptr); })));                       // 005FC4A6, store 005FC4AF
    put32(b, 0x288, 1);                                             // 005FC4B5 (edi=1)
    put32(b, 0x2A4, 0xBF800000u);                                   // 005FC4C0 (-1.0f)
    put32(b, 0x2A0, 0); put32(b, 0x29C, 1); put32(b, 0x2C0, 0); put32(b, 0x2C4, 1);
    {   // 005FC4E3..005FC502: GetHighResolutionTimer on Base+4, __aulldiv by 1000, low word.
        auto* base = static_cast<unsigned char*>(kotor_menu_10_19::dependencies::base_007a39e0);
        auto* timer = static_cast<kotor_menu_10_19::TimerDependency*>(addr(get32(base, 4)));
        const kotor_menu_10_19::Wide ticks = timer->high_resolution_005e9260();
        const kotor_menu_10_19::Wide q = kotor_menu_10_19::dependencies::unsigned_divide_006fb030(ticks, 1000);
        put32(b, 0x2C8, static_cast<Word>(q));
    }
    put32(b, 0x2CC, 0); put32(b, 0x2D0, 0);
    put32(b, 0x2D4, big); put32(b, 0x2B4, big); put32(b, 0x2B8, big); // 005FC514..005FC520
    put32(b, 0x228, 0); put32(b, 0x22C, 0); put32(b, 0x230, 0); put32(b, 0x234, 0);
    GuiAssign005e5140(reinterpret_cast<StartupString*>(b + 0x2FC), nullptr, ""); // 005FC538 push 0073D71C ("") / 005FC549
    for (unsigned off : {0x30Cu, 0x310u, 0x32Cu, 0x1B0u, 0x33Cu}) put32(b, off, 0);
    put32(b, 0x338, big); put32(b, 0x364, big);                     // 005FC56C/005FC572
    put32(b, 0x368, 0); put32(b, 0x36C, 0); put32(b, 0x370, 0); put32(b, 0x374, 0);
    put32(b, 0x37C, get32(b, 0x37C) & 0xFFFFFFFBu);                 // 005FC590
    put32(b, 0x380, 0);
    put32(b, 0x384, get32(b, 0x384) & 0xFFFFFFFEu);                 // 005FC59D..005FC5AA
    put32(b, 0x38C, 0); put32(b, 0x390, 0); put32(b, 0x394, 0); put32(b, 0x3A0, 0);
    put8(b, 0x398, 0xFF);                                           // 005FC5C8
    put32(b, 0x39C, 0); put32(b, 0x3A4, 0);
    put32(b, 0x3A8, 0xFFFFFFFEu);                                   // 005FC5DB (eax)
    put32(b, 0x3AC, big); put32(b, 0x3B0, big); put32(b, 0x3B4, 0);
    put8(b, 0x3B8, 0xFF);                                           // 005FC5F3
    put32(b, 0x3BC, big); put32(b, 0x3C0, 0);
    for (unsigned off : {0x3D4u, 0x3D0u, 0x3CCu, 0x3C8u, 0x3D8u}) put32(b, off, 0);
    put32(b, 0x3DC, 0xBF800000u);                                   // 005FC624 (edi)
    put32(b, 0x3E0, 0);
    put32(b, 0x3E4, 0); put8(b, 0x3E8, 0);                          // 005FC638/005FC63A
    std::memset(b + 0x418, 0, 0x1C * 4);                            // 005FC64A rep stos (overwrites vector words)
    put32(b, 0x488, 0); put32(b, 0x48C, 0); put8(b, 0x490, 0); put8(b, 0x491, 0); put32(b, 0x494, 0);
    put32(b, 0x37C, get32(b, 0x37C) & 0xFFFFFFFCu);                 // 005FC66A
    put32(b, 0x378, 0);                                             // 005FC676
    put32(b, 0x004, word_of(allocate_construct(0xA0, [](void* p) {  // 005FC671/005FC67C
        return client_options::CClientOptions_0061db60(p, nullptr); })));  // lane-config               // 005FC693, store 005FC6A0
    put8(b, 0x498, 0); put32(b, 0x49C, 0); put32(b, 0x4A0, 0); put32(b, 0x3C4, 0);
    put32(b, 0x4A4, big);                                           // 005FC6BB
    put32(b, 0x4B4, 0); put32(b, 0x4B8, 0); put32(b, 0x4BC, 0); put32(b, 0x4C4, 0);
    return self;                                                    // 005FC6DA EAX=this
}

// 005F4F40 CExoLinkedList<CSWCPlayer>::CExoLinkedList (own EH funclet 0072656B):
// alloc 0Ch (005F4F57/005F4F5B); non-null -> CExoLinkedListInternal 005E9AA0
// (005F4F75); state-0 unwind frees the block; [this]=result (005F4F7A); EAX=this.
void* __fastcall ConstructPlayerList005f4f40(void* self, void*) {
    kotor_list::Internal* p = nullptr;
    void* storage = kotor_list_dependencies::allocate(0x0C);
    if (storage) {
        try { p = (::new(storage) kotor_list::Internal)->Construct(); }
        catch (...) { kotor_list_dependencies::release(storage); throw; }
    }
    put32(static_cast<unsigned char*>(self), 0, word_of(p));
    return self;
}

// 004EAE80 CSWPartyMemberData::Reset (thiscall RET, no stack args).
void __fastcall ResetPartyMemberData004eae80(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    put32(b, 0x34, 0); put32(b, 0x44, 0); put32(b, 0x48, 0); put32(b, 0x68, 0); // 004EAE95..004EAE9E
    put32(b, 0x0C, 0); put32(b, 0x10, 0); put32(b, 0x14, 0);                     // 004EAEA4..004EAEB1
    put32(b, 0x18, 0); put32(b, 0x1C, 0); put32(b, 0x20, 0);                     // 004EAEC3..004EAEE0
    put32(b, 0x28, 0); put32(b, 0x2C, 0); put32(b, 0x30, 0);                     // 004EAEF2..004EAF0F
    put32(b, 0x4C, 0); put32(b, 0x50, 0x3F800000u);                               // 004EAF21/004EAF2F
    put32(b, 0x04, 0xFFFFFFFFu);                                                  // 004EAF3E
    put32(b, 0x54, 0);                                                            // 004EAF45
    put32(b, 0x58, 0x3F800000u); put32(b, 0x80, 0x7F000000u); put32(b, 0x84, 0xBC50u); // 004EAF48..004EAF59
}

// 00636170 CSWParty::CSWParty (0x1290 allocation at 005FC48F). No EH frame.
void* __fastcall ConstructParty00636170(void* self, void*) {
    auto* b = static_cast<unsigned char*>(self);
    put32(b, 0x0C, 0); put32(b, 0x10, 0); put32(b, 0x14, 0);                     // 0063617E..00636184
    for (unsigned i = 0; i < 3; ++i) {                                            // 0063618A count 3, stride 88h
        const unsigned m = 0x24 + i * 0x88;                                       // ebp
        const unsigned e = m + 0x14;                                              // esi
        for (unsigned off : {e - 8, e - 4, e, e + 4, e + 8, e + 0x0Cu, e + 0x14u, e + 0x18u, e + 0x1Cu,
                             e + 0x24u, e + 0x28u, e + 0x2Cu, e + 0x38u, e + 0x3Cu, e + 0x40u, e + 0x48u,
                             e + 0x4Cu, e + 0x50u})
            put32(b, off, 0);                                                     // 00636192..006361C4
        put32(b, e + 0x60, 0); put32(b, e + 0x64, 0); put32(b, e + 0x68, 0);      // 006361C9..006361CF
        ResetPartyMemberData004eae80(b + m, nullptr);                             // 006361D2
        put32(b, e + 0x10, 0);                                                    // 006361DB
        put32(b, m, 0x7F000000u);                                                 // 006361DE
        put32(b, e - 0x0C, 0x3F800000u);                                          // 006361E5
        put32(b, e + 0x5C, 0);                                                    // 006361EC
    }
    for (unsigned i = 0; i < 9; ++i) {                                            // 00636202..0063621C
        const unsigned o = 0x1C8 + i * 0x0C; put32(b, o, 0); put32(b, o + 4, 0); put32(b, o + 8, 0);
    }
    for (unsigned i = 0; i < 9; ++i) {                                            // 0063621E..0063623C
        const unsigned o = 0x240 + i * 0x0C; put32(b, o, 0); put32(b, o + 4, 0); put32(b, o + 8, 0);
    }
    put32(b, 0x2B0, 0); put32(b, 0x2B4, 0); put32(b, 0x2B8, 0);                   // 0063623E..0063624A
    put32(b, 0x2C0, 0); put32(b, 0x2C4, 0); put32(b, 0x2C8, 0);                   // 00636250..0063625C
    put32(b, 0x2D8, 0); put32(b, 0x2D4, 0); put32(b, 0x2D0, 0);                   // 00636268..00636274
    put32(b, 0x2CC, 0x3F800000u);                                                 // 0063627A
    kotor_fastcall_00635a90(b + 0x2DC, nullptr);                         // 00636262/00636284
    const Word f15 = 0x3FC00000u;                                                 // 00636289 ebp (1.5f)
    put32(b, 0x1C8, f15); put32(b, 0x1CC, 0xBF333333u); put32(b, 0x1D0, 0);       // 0063629C..006362AD
    put32(b, 0x1D4, 0xBFC00000u); put32(b, 0x1D8, 0x3F4CCCCDu); put32(b, 0x1DC, 0); // 006362C6..006362E3
    put32(b, 0x1EC, 0); put32(b, 0x1F0, 0); put32(b, 0x1F4, 0);                   // 006362F8..00636315
    put32(b, 0x1F8, 0); put32(b, 0x1FC, 0); put32(b, 0x200, 0);                   // 0063632A..00636347
    put32(b, 0x210, 0xC0000000u); put32(b, 0x214, f15); put32(b, 0x218, 0);       // 0063635C..0063636F
    put32(b, 0x21C, 0x40000000u); put32(b, 0x220, 0x40900000u);                   // 00636384/00636396
    put32(b, 0x234, f15); put32(b, 0x238, f15); put32(b, 0x23C, f15);             // 006363A8..006363B4
    put32(b, 0x224, 0);                                                           // 006363BA
    put8(b, 0x1C4, 0);                                                            // 006363BD
    put32(b, 0x1BC, 0xFFFFD8F1u);                                                 // 006363C3
    kotor_fastcall_00634760(b, nullptr, 0);                                     // 006363A5 push ebx / 006363CD
    put32(b, 0x1C0, 0x3F800000u);                                                 // 006363D2
    put8(b, 0x2AC, 0); put8(b, 0x2AD, 0); put8(b, 0x2AE, 0);                      // 006363DC..006363E8
    for (unsigned i = 0; i < 3; ++i) put32(b, 0x24 + i * 0x88, 0x7F000000u);      // 006363EE..0063640C
    put32(b, 0x0C, 0); put32(b, 0x10, 0); put32(b, 0x14, 0);                      // 00636426..00636438
    put32(b, 0x2B0, 0); put32(b, 0x2B4, 0); put32(b, 0x2B8, 0);                   // 00636453..0063646B
    put32(b, 0x00, 0); put32(b, 0x04, 0); put32(b, 0x18, 0); put32(b, 0x1C, 0);   // 00636471..00636479
    put32(b, 0x2BC, 0); put32(b, 0x2C, 0);                                        // 0063647C/00636482
    put32(b, 0xB4, f15);                                                          // 00636485
    put32(b, 0x08, 1);                                                            // 0063648B
    put32(b, 0x20, 0x3F000000u);                                                  // 00636492
    put32(b, 0x127C, word_of(self));                                              // 00636499 [edi+127Ch]=edi
    put32(b, 0x13C, 0x40400000u);                                                 // 0063649F
    return self;                                                                  // 006364A9 EAX=this
}
}
