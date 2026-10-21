// lane-m47-core: CResHelper<CResX,T> instances of the CAuroraInterface resource family (see aurora_family.h).
// The template members (getter, ctor, SetResRef, base/derived destructor, scalar deleting destructors, simple
// Load) are shape-identical to the reviewed lane-tpc TXI instance (tpc_txi.cpp: 0070E5E0, 0070FD10, 0070FB90,
// 0070FB30, 0070FDA0, 0070FCF0, 0070FDB0, 0070FAC0), verified per member with tools/norm.py; each entry below
// cites its own address and the instance constants (tag word, CRes allocation size, tables, callees).
// Unwind: getters free the raw 1Ch allocation if the constructor throws (funclet -> 006FA390, as 0073A930);
// constructors run the base destructor if SetResRef throws (state 0, as 0073AA38); SetResRef frees its CRes
// allocation if the CRes constructor throws (state 0, as 0073AA1B).
// Evidence: lane dis/<addr>.asm (dumpbin /disasm swkotor.exe).
#include "76452bc32b-aurora_family_impl.h"

using namespace aurora_family;
using namespace font_callback_read;
using tpc_lower::Word;

using namespace aurora_family_impl;

extern "C" {
// ======================================================================= tables (one slot each; .rdata)
const HelperMethods aurora_layered_helper_methods_0075fb4c{&aurora_layered_base_deleting_destroy_0070dd40};
const HelperMethods aurora_layered_methods_0075fb50{&aurora_layered_deleting_destroy_0070de00};
const HelperMethods aurora_texture_helper_methods_0075fb54{&aurora_texture_base_deleting_destroy_0070ef80};
const HelperMethods aurora_texture_methods_0075fb58{&aurora_texture_deleting_destroy_0070f040};
const HelperMethods aurora_visibility_helper_methods_0075fb5c{&aurora_visibility_base_deleting_destroy_0070f250};
const HelperMethods aurora_visibility_methods_0075fb60{&aurora_visibility_deleting_destroy_0070f310};
const HelperMethods aurora_modelx_helper_methods_0075fb74{&aurora_modelx_base_deleting_destroy_0070ffc0};
const HelperMethods aurora_modelx_methods_0075fb78{&aurora_modelx_deleting_destroy_00710080};
const HelperMethods aurora_model_helper_methods_0075fb7c{&aurora_model_base_deleting_destroy_007102e0};
const HelperMethods aurora_model_methods_0075fb80{&aurora_model_deleting_destroy_007103a0};
const HelperMethods aurora_compressed_helper_methods_0075fb84{&aurora_compressed_base_deleting_destroy_00710690};
const HelperMethods aurora_compressed_methods_0075fb88{&aurora_compressed_deleting_destroy_00710750};
const HelperMethods aurora_texture16_helper_methods_0075fb8c{&aurora_texture16_base_deleting_destroy_00710a70};
const HelperMethods aurora_texture16_methods_0075fb90{&aurora_texture16_deleting_destroy_00710b30};

// ======================================================================= MDL 07D2 (CResMDL 34h via 005CEA50)
__declspec(noinline) void __fastcall aurora_model_base_destroy_00710120(Model* self, void*) noexcept {
    base_destroy(self, &aurora_model_helper_methods_0075fb7c);                   // 00710120..0071017F (table 0075FB7C)
}
__declspec(noinline) void __fastcall aurora_model_destroy_00710390(Model* self, void*) noexcept {
    self->methods = &aurora_model_methods_0075fb80;                              // 00710390
    aurora_model_base_destroy_00710120(self, nullptr);                           // 00710396 JMP
}
__declspec(noinline) void* __fastcall aurora_model_base_deleting_destroy_007102e0(void* self, void*, Word flags) noexcept {
    return deleting<Model>(self, flags, &aurora_model_base_destroy_00710120);    // 007102E0..FB
}
__declspec(noinline) void* __fastcall aurora_model_deleting_destroy_007103a0(void* self, void*, Word flags) noexcept {
    return deleting<Model>(self, flags, &aurora_model_destroy_00710390);         // 007103A0..BB
}
__declspec(noinline) void __fastcall aurora_model_set_ref_00710180(Model* self, void*, menu_resources::Ref* incoming,
                                                                   Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x07d2u, 0x34u, &aurora_mdl_resource_ctor_005cea50); // 00710180 (push 7D2h x2, push 34h)
}
__declspec(noinline) __declspec(safebuffers) Model* __fastcall aurora_model_construct_00710300(
    Model* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_model_helper_methods_0075fb7c, &aurora_model_methods_0075fb80,
                     &aurora_model_set_ref_00710180, &aurora_model_base_destroy_00710120);   // 00710300 (unwind 0073AAB8)
}
__declspec(noinline) Signed __fastcall aurora_model_load_007100d0(Model* self, void*) noexcept(false) {
    return load_tagged(self, 6u, 0x07d2);                                        // 007100D0..FA
}

// ======================================================================= MDX 0BC0 (CResMDX 34h via 00710E10)
__declspec(noinline) void __fastcall aurora_modelx_base_destroy_0070fe00(ModelX* self, void*) noexcept {
    base_destroy(self, &aurora_modelx_helper_methods_0075fb74);                  // 0070FE00 (table 0075FB74)
}
__declspec(noinline) void __fastcall aurora_modelx_destroy_00710070(ModelX* self, void*) noexcept {
    self->methods = &aurora_modelx_methods_0075fb78;                             // 00710070
    aurora_modelx_base_destroy_0070fe00(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_modelx_base_deleting_destroy_0070ffc0(void* self, void*, Word flags) noexcept {
    return deleting<ModelX>(self, flags, &aurora_modelx_base_destroy_0070fe00);
}
__declspec(noinline) void* __fastcall aurora_modelx_deleting_destroy_00710080(void* self, void*, Word flags) noexcept {
    return deleting<ModelX>(self, flags, &aurora_modelx_destroy_00710070);
}
__declspec(noinline) void __fastcall aurora_modelx_set_ref_0070fe60(ModelX* self, void*, menu_resources::Ref* incoming,
                                                                    Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x0bc0u, 0x34u, &aurora_mdx_resource_ctor_00710e10); // 0070FE60 (0BC0h, 34h)
}
__declspec(noinline) __declspec(safebuffers) ModelX* __fastcall aurora_modelx_construct_0070ffe0(
    ModelX* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_modelx_helper_methods_0075fb74, &aurora_modelx_methods_0075fb78,
                     &aurora_modelx_set_ref_0070fe60, &aurora_modelx_base_destroy_0070fe00);   // 0070FFE0 (0073AA78)
}
__declspec(noinline) Signed __fastcall aurora_modelx_load_0070fdd0(ModelX* self, void*) noexcept(false) {
    return load_tagged(self, 6u, 0x0bc0);                                        // 0070FDD0
}
__declspec(noinline) ModelX* __cdecl aurora_modelx_target_0070e7c0(String* name) noexcept(false) {
    return getter<ModelX>(name, &aurora_modelx_construct_0070ffe0, &aurora_helper_info_0070faf0,
                          &aurora_modelx_load_0070fdd0, 0x0bc0);                 // 0070E7C0 (info 0070FAF0, tag 0BC0h)
}

// ======================================================================= VIS 0BB9 (CResVIS 34h via 00710D50)
__declspec(noinline) void __fastcall aurora_visibility_base_destroy_0070f090(Visibility* self, void*) noexcept {
    base_destroy(self, &aurora_visibility_helper_methods_0075fb5c);              // 0070F090 (table 0075FB5C)
}
__declspec(noinline) void __fastcall aurora_visibility_destroy_0070f300(Visibility* self, void*) noexcept {
    self->methods = &aurora_visibility_methods_0075fb60;                         // 0070F300
    aurora_visibility_base_destroy_0070f090(self, nullptr);
}
__declspec(noinline) void* __fastcall aurora_visibility_base_deleting_destroy_0070f250(void* self, void*, Word flags) noexcept {
    return deleting<Visibility>(self, flags, &aurora_visibility_base_destroy_0070f090);
}
__declspec(noinline) void* __fastcall aurora_visibility_deleting_destroy_0070f310(void* self, void*, Word flags) noexcept {
    return deleting<Visibility>(self, flags, &aurora_visibility_destroy_0070f300);
}
__declspec(noinline) void __fastcall aurora_visibility_set_ref_0070f0f0(Visibility* self, void*, menu_resources::Ref* incoming,
                                                                        Signed flag) noexcept(false) {
    set_ref(self, incoming, flag, 0x0bb9u, 0x34u, &aurora_vis_resource_ctor_00710d50); // 0070F0F0 (0BB9h, 34h)
}
__declspec(noinline) __declspec(safebuffers) Visibility* __fastcall aurora_visibility_construct_0070f270(
    Visibility* self, void*, menu_resources::Ref incoming, Signed mode) noexcept(false) {
    return construct(self, &incoming, mode, &aurora_visibility_helper_methods_0075fb5c, &aurora_visibility_methods_0075fb60,
                     &aurora_visibility_set_ref_0070f0f0, &aurora_visibility_base_destroy_0070f090); // 0070F270 (0073A9B8)
}
__declspec(noinline) Signed __fastcall aurora_visibility_load_0070f060(Visibility* self, void*) noexcept(false) {
    return load_tagged(self, 6u, 0x0bb9);                                        // 0070F060
}
__declspec(noinline) Visibility* __cdecl aurora_visibility_target_0070e6d0(String* name) noexcept(false) {
    return getter<Visibility>(name, &aurora_visibility_construct_0070f270, &aurora_helper_info_0070faf0,
                              &aurora_visibility_load_0070f060, 0x0bb9);         // 0070E6D0 (info 0070FAF0, tag 0BB9h)
}
}
