// m46_material_donor.cpp - lane-m46-3dmodel: DONOR REUSE (no new original-address credit). Verbatim copy of
// 2026-10-02/task-17/material-ownership-r1/source/material.cpp (sha256 88c451b3...): Material::Material 0047B290 and
// Material::~Material 0047B980, closing ??0Material / ??1Material@material_owner. ONE adaptation: the absolute include of
// task-23 allocator_live.h is replaced by the staged copy (M45 root canonical-inputs/dependencies/reconstructed/task-23/
// sbh-slice2/live, same bytes - verified by sha256 in PACKET.json). Header: staged texture-epoch/include/material.h.
// Plus (end of file) the single registry storage material_owner::historical_storage_00830298 (task-17 registry.cpp slice;
// cold-zero BSS). Its original static initializer 0073B0D0 (atexit dispose 0073BC90) is NOT reproduced (shutdown-only;
// needs an allowlisted initializer, D60/D61 path).
#include "856ceecd30-material.h"
#include "d08b97bcca-allocator_live.h"   // m46: staged copy of the donor's absolute include
namespace material_owner {
static std::int32_t signed_bits(Word bits) {
    return bits&0x80000000u ? -1-static_cast<std::int32_t>(~bits) : static_cast<std::int32_t>(bits);
}
static void initialize_list(List& list) {
    list.size=0; list.capacity=0; list.data=0;
}
static void grow_if_equal(List& list) {
    const Word capacity=static_cast<Word>(list.capacity);
    const Word size=static_cast<Word>(list.size);
    if(size==capacity)
        menu_list_growth::allocate_005e03c0(&list,nullptr,capacity ? capacity*2u : 8u);
}
static void append_member(List& list,Word value) {
    grow_if_equal(list);
    const Word count=static_cast<Word>(list.size);
    const Word data=list.data;
    *reinterpret_cast<volatile Word*>(data+count*4u)=value;
    list.size=signed_bits(static_cast<Word>(list.size)+1u);
}
static void append_registry(Material* material) {
    List& list=historical_storage_00830298;
    grow_if_equal(list);
    // Registry loads data before count; nested member append loads count first.
    const Word data=list.data;
    const Word count=static_cast<Word>(list.size);
    *reinterpret_cast<volatile Word*>(data+count*4u)=reinterpret_cast<Word>(material);
    list.size=signed_bits(static_cast<Word>(list.size)+1u);
}
Material::Material() {
    // Default initialization already starts each nominal member lifetime;
    // it does not value-initialize members or write the padding bytes.
    for(unsigned i=0;i<7;++i) textures[i].target=nullptr;
    initialize_list(states);
    initialize_list(modes);
    font::Texture* const first=textures[0].target; // Original load precedes name stores.
    names[0]=nullptr; names[2]=nullptr; names[1]=nullptr;
    if(first) first->registrations.RemoveReverse32(reinterpret_cast<Word>(&textures[0]));
    textures[0].target=nullptr;
    // Preserve the original conditional unregister/re-clear sequence (six slots).
    // The ordinary fresh-construction domain has null targets here.
    for(unsigned i=1;i<6;++i) {
        font::Texture* old=textures[i].target;
        if(old) old->registrations.RemoveReverse32(reinterpret_cast<Word>(&textures[i]));
        textures[i].target=nullptr;
    }
    textures[6].target=nullptr;
    menu_list_growth::allocate_005e03c0(&states,nullptr,2);
    menu_list_growth::allocate_005e03c0(&modes,nullptr,2);
    append_member(states,0);
    append_member(modes,1);
    decal=false;
    render_type=1;
    append_registry(this);
    water_alpha=1.0f;
}
Material::~Material() noexcept(false) {
    clear_textures_0047b440(); // Must complete before touching registry/list storage.
    historical_storage_00830298.RemoveReverse32(reinterpret_cast<Word>(this));
    Word data=modes.data;
    if(data) live_free(reinterpret_cast<void*>(data));
    data=states.data; // Fresh load after the first free callback.
    if(data) live_free(reinterpret_cast<void*>(data));
    for(unsigned index=7;index>0;--index) {
        Safe& safe=textures[index-1];
        font::Texture* target=safe.target;
        if(target) target->registrations.RemoveReverse32(reinterpret_cast<Word>(&safe));
    }
    // No field reset, texture deletion, outer allocation free or name cleanup here.
    // Safe/List automatic destructors are trivial and add no payload effects.
}
}

namespace material_owner {
// task-17 registry.cpp line 5 (storage only): BSS 00830298 {data,size,capacity}; ctor appends at 0047B3DA..0047B419,
// dtor removes (RemoveReverse32). One definition.
List historical_storage_00830298{};
}
