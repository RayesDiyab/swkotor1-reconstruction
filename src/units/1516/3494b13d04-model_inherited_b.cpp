// Eight previously trapped CSWCAnimBase inherited methods. Original GOG K1 x86.
// Reuse the actual m15e, Gob, CResRef, CExoString and math provider contracts.
#include "2ceb145d9f-model_inherited_b.h"
#include "628decf93e-m15e_animbase_internal.h"

using namespace kotor_m15e_ab;

extern "C" {
double __cdecl quaternion_yaw_004a9f40(const Quaternion*);
double __cdecl quaternion_pitch_004aa020(const Quaternion*);
}

namespace {
struct Name { char text[20]; };
// The original's MOVSD loop followed by MOVSB is a forward copy, including aliases.
void copy_attributes(void* to, const void* from, U count) {
    U offset = 0;
    for (U words = count >> 2; words != 0; --words, offset += 4)
        st<U>(to, offset, ld<U>(from, offset));
    for (U bytes = count & 3u; bytes != 0; --bytes, ++offset)
        st<U8>(to, offset, ld<U8>(from, offset));
}
struct StringSlot {
    alignas(StartupString) unsigned char raw[sizeof(StartupString)];
    bool live = false;
    StartupString* get() noexcept { return reinterpret_cast<StartupString*>(raw); }
    ~StringSlot() { if (live) get()->~StartupString(); }
};
}

extern "C" {
// 0069CFD0 RET 18h: unlike 0069CEF0 there is no part/FF argument.
U __fastcall kotor_m15e_anim_base_replace_texture_0069cfd0(void* self, void*, U from, U to,
    U count, U attrs, U keep, U first) noexcept(false) {
    if (pv(self, 0xB8) == nullptr) return 0;                              // 0069CFD6..E6
    Name a, b;
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(from), nullptr, a.text);
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(to), nullptr, b.text);
    c2da_current_strlwr_007135df(a.text);                                // 0069D00A..0F
    c2da_current_strlwr_007135df(b.text);                                // 0069D014..19
    void* const body = pv(self, 0xB8);                                   // reload 0069D02A
    gvt(body)->s030_gob_replace_texture(gob(body), nullptr, wd(a.text), wd(b.text), count, attrs, first);
    if (keep != 0) {
        Ref scratch;
        c2da_copy_00406120(ref_at(self, 0x2C), nullptr, &scratch, reinterpret_cast<Ref*>(to));
        st<U>(self, 0x3C, 1);
        st<U8>(self, 0x28, static_cast<U8>(count));
        st<U8>(self, 0x29, static_cast<U8>(first));
        const U source = attrs + first * 2u;                            // 32-bit LEA, 0069D077
        copy_attributes(at(self, 0x14), reinterpret_cast<const void*>(source), count * 2u);
    }
    return 1;
}

// 0069D0A0 RET 0: no kept attributed replacement means no Gob call.
U __fastcall kotor_m15e_anim_base_restore_texture_0069d0a0(void* self, void*) noexcept(false) {
    if (pv(self, 0xB8) == nullptr) return 0;
    if (ld<U>(self, 0x3C) != 0) {
        Ref empty;
        empty.construct_00406d80(kEmpty_0073d71c);
        avt(self)->s014(self, nullptr, 0xFF, wd(&empty), wd(at(self, 0x2C)), ld<U8>(self, 0x28),
                       wd(at(self, 0x14)), 0, ld<U8>(self, 0x29));
    }
    return 1;
}

// 0069D3E0 RET 4: true iff Gob::GetAnimation finds the named animation.
U __fastcall kotor_m15e_anim_base_get_overlay_animation_0069d3e0(void* self, void*, U animation) noexcept(false) {
    if (pv(self, 0xB8) == nullptr) return 0;
    StringSlot name;
    avt(self)->s040(self, nullptr, name.get(), animation);                // 0069D419..28
    name.live = true;                                                    // EH state 0069D441
    void* const body = pv(self, 0xB8);
    const auto* const table = gvt(body);                                 // before CStr, 0069D431
    float time, length;
    char* const text = WindowCStr005e5670(name.get(), nullptr);
    const U result = table->s01c_gob_get_animation(gob(pv(self, 0xB8)), nullptr,
        wd(text), reinterpret_cast<U>(&time), reinterpret_cast<U>(&length));
    return result != 0 ? 1u : 0u;
}

// 0069D8B0 RET 0: restore the kept ref, otherwise the original empty string literal.
U __fastcall kotor_m15e_anim_base_restore_env_map_0069d8b0(void* self, void*) noexcept(false) {
    void* body = pv(self, 0xB8);
    if (body == nullptr) return 0;
    if (ld<U>(self, 0x50) != 0) {
        Name name;
        gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(at(self, 0x40)), nullptr, name.text);
        body = pv(self, 0xB8);
        gvt(body)->s048_gob_set_environment_map(gob(body), nullptr, wd(name.text));
    } else {
        gvt(body)->s048_gob_set_environment_map(gob(body), nullptr, wd(kEmpty_0073d71c));
    }
    return 1;
}

// 0069DC50 RET 4: identity without a body; always return the caller's output pointer.
void* __fastcall kotor_m15e_anim_base_get_orientation_0069dc50(void* self, void*, void* out) noexcept(false) {
    void* const body = pv(self, 0xB8);
    if (body == nullptr) {
        st<U>(out, 12, 0); st<U>(out, 8, 0); st<U>(out, 4, 0); st<U>(out, 0, 0x3F800000u);
    } else {
        gvt(body)->s068_gob_get_orientation(gob(body), nullptr, wd(out));
    }
    return out;
}

// 0069DC90 RET 0: EAX is undefined on the null path and discarded by callers.
void __fastcall kotor_m15e_anim_base_lean_0069dc90(void* self, void*) noexcept(false) {
    void* body = pv(self, 0xB8);
    if (body == nullptr) return;
    Quaternion scratch;
    const Quaternion* argument = reinterpret_cast<const Quaternion*>(
        gvt(body)->s068_gob_get_orientation(gob(body), nullptr, wd(&scratch)));
    float yaw;
    __asm {
        push argument
        call quaternion_yaw_004a9f40
        fstp dword ptr yaw
        add esp, 4
    }
    body = pv(self, 0xB8);                                               // second independent query
    argument = reinterpret_cast<const Quaternion*>(gvt(body)->s068_gob_get_orientation(gob(body), nullptr, wd(&scratch)));
    float pitch;
    __asm {
        push argument
        call quaternion_pitch_004aa020
        fstp dword ptr pitch
        add esp, 4
    }
    float roll;
    std::memcpy(&roll, at(self, 0xD8), sizeof roll);                       // 0069DCD2 raw MOV/PUSH
    Quaternion* const q = yaw_pitch_roll_004acac0(&scratch, yaw, pitch, roll);
    body = pv(self, 0xB8);
    Quaternion out;
    gvt(body)->s060_gob_set_orientation(gob(body), nullptr, wd(&out), ld<U>(q, 0), ld<U>(q, 4), ld<U>(q, 8), ld<U>(q, 12));
}

// 0069DD90 RET 8: lowercase the CResRef name and forward the flag without narrowing.
U __fastcall kotor_m15e_anim_base_enable_bumped_out_shell_0069dd90(void* self, void*, U ref, U flag) noexcept(false) {
    if (pv(self, 0xB8) == nullptr) return 0;
    Name name;
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(ref), nullptr, name.text);
    c2da_current_strlwr_007135df(name.text);
    void* const body = pv(self, 0xB8);
    gvt(body)->s150_gob_enable_render_bumped_out(gob(body), nullptr, wd(name.text), flag);
    return 1;
}

// 006A03C0 RET 8: preserve the real 0069E420 result; EAX is zero on null body.
U __fastcall kotor_m15e_anim_base_set_overlay_animation_006a03c0(void* self, void*, U animation, U flag) noexcept(false) {
    void* const body = pv(self, 0xB8);
    if (body == nullptr) return 0;
    return kotor_m15e_anim_base_set_overlay_animation_0069e420(self, nullptr, body, animation, flag);
}
}
