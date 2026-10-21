#pragma once
// Source-only owner proposal. No native ABI/layout/lifetime admission yet.
#include "b9e590c34c-font.hpp"
#include "83031bbd33-growth.h" // E1 edit TC2: stage header_root <stage>/viewport-integration/dependencies/list-growth (same bytes)
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <limits>

namespace material_owner {
using Word=std::uint32_t;
using List=kotor_reconstruction::ArrayMutations;
using Safe=font::Safe; // Same nominal Texture target/registrations prefix; no copied type.
// Same scalar function-pointer types as the existing Task34 ModelExitCallback/registrar.
using ExitCallback=void (__cdecl*)();
using Register=int (__cdecl*)(ExitCallback);

// One source-owned cold-zero object, linked once by Main6 after admission.
// Original VAs are provenance tags, never dereferenced original addresses.
extern List historical_storage_00830298;
struct RegistryRuntime { Register register_exit_006fb332; };
// Existing startup/exit owner must define this reference; no default success table.
extern const RegistryRuntime& registry_runtime;

struct Material {
    Safe textures[7];                   // 00..18, seven live nominal Safe subobjects
    char* volatile names[3];             // 1C,20,24
    List states;                        // 28
    List modes;                         // 34
    bool volatile decal;                // 40
    unsigned char untouched_41_43[3];    // Lifetime starts; no initializer/store
    std::int32_t volatile render_type;   // 44
    float volatile water_alpha;         // 48

    Material();                         // 0047B290 normal nonthrowing body proposal
    ~Material() noexcept(false);        // 0047B980 normal nonthrowing body proposal
    void __thiscall clear_textures_0047b440();
    void __thiscall initialize_textures_0047b560(const char*,const char*,const char*,const char*);
    void __thiscall init_0047b110(const char*); // Task17 named follow-on resource/parser dependency
};

void __cdecl dispose_0073bc90();
// Forwards the genuine registrar's full signed EAX result; no success fabrication.
int __cdecl initialize_0073b0d0();

static_assert(sizeof(void*)==4 && sizeof(Word)==4 && sizeof(float)==4 && sizeof(bool)==1,
              "Supported x86 scalar representation required");
static_assert(std::is_same<List,font::List>::value &&
              std::is_same<List,menu_list_growth::List>::value,"Single existing List type");
static_assert(std::is_standard_layout<Material>::value && alignof(Material)==4 &&
              sizeof(Material)==76,"Proposed ordinary MSVC x86 Material layout");
static_assert(offsetof(Material,textures)==0 && offsetof(Material,names)==0x1c &&
              offsetof(Material,states)==0x28 && offsetof(Material,modes)==0x34 &&
              offsetof(Material,decal)==0x40 && offsetof(Material,untouched_41_43)==0x41 &&
              offsetof(Material,render_type)==0x44 && offsetof(Material,water_alpha)==0x48,
              "All observed Material offsets; compiler verification remains held");
static_assert(std::numeric_limits<float>::is_iec559,"IEEE32 one representation required");
static_assert(std::is_trivially_default_constructible<Safe>::value &&
              std::is_trivially_default_constructible<List>::value,"No hidden member initialization stores");
static_assert(std::is_trivially_destructible<Safe>::value &&
              std::is_trivially_destructible<List>::value,"No automatic member cleanup");
}
