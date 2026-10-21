#pragma once
#include <cstdint>
#include <cstddef>
namespace kotor_client_normal {
using Word=std::uint32_t;
using Allocate=void* (__cdecl*)(Word);
using Construct=void* (__thiscall*)(void*);
// Caller transport: ECX constructor-return instance, one zero stack DWORD;
// ignored EAX. The actual 0063E7C0 leaf is XOR EAX/EAX; RET4, ignoring ECX.
using MovieInitialize=Word (__thiscall*)(void*,Word);
using LoadOptions=int (__thiscall*)(void*);
using SetDefaults=void (__thiscall*)(void*);
using GetOptions=void* (__thiscall*)(void*);
// An opaque callable entry transported by explicit x86 assembly: ECX input,
// no stack args, result ST0 extended80. Never called using its nominal C type.
using X87Entry=void (__cdecl*)();
using SetGamma=void (__thiscall*)(void*,float,Word);
struct Providers {
    Allocate allocate;
    Construct world_construct,quest_construct,movie_construct;
    MovieInitialize movie_initialize;
    LoadOptions load_options;
    SetDefaults set_defaults;
    GetOptions get_options;
    X87Entry gamma_st0;
    void* const* app_slot_007a39fc;
    void* const* renderer_slot_007a39f8;
};
extern Providers providers; // Caller binds metadata; owns all real providers/views.
// Only the minimal borrowed byte prefix used by these four entries. This is
// not a complete ClientExoAppInternal class, canonical storage or constructor.
struct alignas(4) ClientPrefix {
    unsigned char observed[0x130];
    __declspec(noinline) void __thiscall CreateWorldJournal();
    __declspec(noinline) void __thiscall CreateQuestJournal();
    __declspec(noinline) void __thiscall InitializeMoviePlayer();
    __declspec(noinline) int __thiscall LoadClientOptions();
};
bool WorldReady() noexcept;
bool QuestReady() noexcept;
bool MovieReady() noexcept;
bool OptionsReady() noexcept;
}
