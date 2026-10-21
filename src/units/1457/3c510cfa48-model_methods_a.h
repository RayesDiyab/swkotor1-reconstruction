#pragma once
#include <cstdint>

extern "C" {
std::uint32_t __fastcall model_set_animation_on_subobject_0069d520(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept(false);
std::uint32_t __fastcall model_set_animation_visemes_0069d540(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept(false);
std::uint32_t __fastcall model_set_animation_on_subobject_internal_0069d5a0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t) noexcept(false);
void __fastcall model_set_always_render_0069d6e0(void*, void*, std::uint32_t) noexcept(false);
void __fastcall model_set_in_cutscene_0069d710(void*, void*, std::uint32_t) noexcept(false);
void __fastcall model_set_lean_0069dd30(void*, void*, std::uint32_t) noexcept(false);
std::uint32_t __fastcall model_disable_bumped_out_shell_0069ddf0(void*, void*) noexcept(false);
std::uint32_t __fastcall model_set_motion_blur_0069de10(void*, void*, std::uint32_t) noexcept(false);
std::uint32_t __fastcall model_set_max_motion_blur_length_0069de40(void*, void*, std::uint32_t) noexcept(false);
// The original writable .data float, initially 5.0. No immediate .text writers found.
extern float model_lean_scale_007a2680;
}
