// fb_executed.cpp - lane-3d-fb: framebuffer_bloom functions that execute on (or are reachable from) the first
// main-menu frame. Evidence: swkotor.exe disassembly ranges cited per statement (lane-local d/<addr>.txt).
//   00426650 GLRender::SetFilmNoiseTexture   - every RenderSinglePass (00451935), unconditional
//   00450BB0 Scene::RenderMotionBlur          - every RenderSinglePass via Scene vtbl+CC (0045191E), unconditional
//   0046EDA0 AurPixelShadersAvailable         - PartTriMesh::Draw render paths (00474F29/004754FE/0047675C/0047715B)
#include "cd769faf0f-fb_bloom.h"

namespace {
using namespace kotor_fb;
using kotor_scene3d::cells;

// Receiver's typed Scene vtable (D41/D42: slots are read only through the published typed table; the object's
// first DWORD is a pointer to that table - object-pointer conversion only, no function-pointer cast).
inline const kotor_scene3d::SceneVTable* scene_table(void* scene) {
    return *static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene);
}
} // namespace

extern "C" {

// 00426650 GLRender::SetFilmNoiseTexture (cdecl, 1 arg, plain RET)
void __cdecl gl_set_film_noise_texture_00426650(void* texture) {
    fb_film_noise_texture_007a68d0 = texture;                         // 00426650 mov eax,[esp+4]; 00426654 mov [007A68D0],eax
}                                                                     // 00426659 ret

// 00450BB0 Scene::RenderMotionBlur (thiscall, ECX = scene, no stack args, RET 0; Scene vtbl+CC)
// The original never sets EAX for the caller; 0045191E..00451924 ignores it. lane-m48-a adaptation: spelled with the
// canonical Scene vtable slot type (lane-3d-render vtables_gen.h +CC returns U; the fb void-slot proposal is not adopted).
U __fastcall scene_render_motion_blur_00450bb0(void* scene, void*) {
    using namespace kotor_renderer_init;
    // esi = [0073D260] glEnableClientState (00450BB7), called four times
    glEnableClientState_0073d260(kGlTexCoordArray_0073f398);          // 00450BB0 / 00450BBE / 00450BC1
    glEnableClientState_0073d260(kGlVertexArray_0073f3a4);            // 00450BC3 / 00450BC9 / 00450BCA
    glEnableClientState_0073d260(kGlNormalArray_0073f39c);            // 00450BCC / 00450BD2 / 00450BD3
    glEnableClientState_0073d260(kGlColorArray_0073f3a0);             // 00450BD5 / 00450BDA / 00450BDB
    // esi = [0073D390] glEnable (00450BE3)
    glEnable_0073d390(*cells.gl_blend_0078d448);                      // 00450BDD / 00450BE9 / 00450BEA
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                 // 00450BEC / 00450BF2 / 00450BF3
    // ebx = [0073D28C] glDepthMask (00450BF5)
    glDepthMask_0073d28c(0);                                          // 00450BFB push 0 / 00450BFD call ebx
    glBlendFunc_0073d38c(kBlendSrcAlpha_0073f270, kBlendOneMinusSrcAlpha_0073f274); // 00450BFF..00450C0C (dst pushed first)
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                   // 00450C12 / 00450C18 / 00450C19
    scene_table(scene)->s0dc_scene_do_blur_buckets(scene, nullptr);   // 00450C1B mov eax,[edi]; 00450C1F call [eax+DC] (DoBlurBuckets)
    glDepthMask_0073d28c(1);                                          // 00450C25 push 1 / 00450C27 call ebx
    return 0;  // D2 label (D69 precedent): EAX at 00450C2C ret is glDepthMask's unspecified scratch; caller 0045191E discards it
}                                                                     // 00450C2C ret

// 0046EDA0 AurPixelShadersAvailable (cdecl, no args, EAX result; cache cell 0078E5DC, cold -1)
U __cdecl aur_pixel_shaders_available_0046eda0() {
    const I cached = fb_pixel_shaders_cache_0078e5dc;                 // 0046EDA0
    if (cached != -1) return static_cast<U>(cached);                  // 0046EDA5 cmp eax,-1; 0046EDA8 jne 0046EDF5 (EAX = cache)
    if (aur_ati_fragment_shaders_bump_map_available_0045fb90()) {     // 0046EDAA call 0045FB90; 0046EDAF test al,al; je 0046EDE1
        if (aur_ati_fragment_shader_available_0045f560() == 0) {      // 0046EDB3 call 0045F560; 0046EDB8 test eax,eax; jne 0046EDD6
            // glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &cache): the original writes straight into the cache cell.
            kotor_renderer_init::glGetIntegerv_0073d244(kGlMaxGeneralCombinersNV_0073f26c,
                &fb_pixel_shaders_cache_0078e5dc);                    // 0046EDBC..0046EDC7 push 78E5DCh; push [0073F26C]; call [0073D244]
            if (fb_pixel_shaders_cache_0078e5dc < 8) goto unavailable; // 0046EDCD cmp dword [0078E5DC],8; 0046EDD4 jl 0046EDE1
        }
        fb_pixel_shaders_cache_0078e5dc = 1;                          // 0046EDD6 mov eax,1; 0046EDDB mov [0078E5DC],eax
        return 1;                                                     // 0046EDE0 ret
    }
unavailable:
    fb_pixel_shaders_cache_0078e5dc = 0;                              // 0046EDE1 mov [0078E5DC],0
    {
        const I ati = aur_ati_fragment_shader_available_0045f560();   // 0046EDEB call 0045F560
        fb_pixel_shaders_cache_0078e5dc = ati;                        // 0046EDF0 mov [0078E5DC],eax
        return static_cast<U>(ati);                                   // 0046EDF5 ret (EAX)
    }
}

} // extern "C"
