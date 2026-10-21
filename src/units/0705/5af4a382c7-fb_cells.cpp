// fb_cells.cpp - lane-3d-fb storage owner for framebuffer/bloom cells that no other lane defines
// (owners_scan.txt: no definition of these addresses in any claude-takeover lane or the strict-link objects).
// Each object is the original cell at the address in its name with the pinned image's cold value (.data bytes
// read by rd.py; BSS = loader zero). No value is preset or forced beyond the original image contents.
#include "056e5913e7-fb_bloom.h"

extern "C" {
volatile float fb_speed_blur_ratio_0078d43c = 0x1.f5c29p-1f;   // 48 E1 7A 3F (0.98f)
volatile kotor_fb::U fb_effect_gate_0078d974 = 1;              // 01 00 00 00
volatile kotor_fb::U fb_effect_gate_0078d978 = 1;              // 01 00 00 00
volatile kotor_fb::U fb_effect_gate_0078d97c = 1;              // 01 00 00 00
volatile kotor_fb::U fb_effect_gate_0078d980 = 1;              // 01 00 00 00
volatile kotor_fb::U fb_effect_gate_0078d984 = 1;              // 01 00 00 00
volatile kotor_fb::U fb_effect_gate_0078d988 = 1;              // 01 00 00 00
volatile float fb_bloom_param_0078daf8 = 0x1.666666p-2f;       // 33 33 B3 3E (0.35f)
volatile float fb_bloom_param_0078db00 = 0x1p+1f;              // 00 00 00 40 (2.0f)
volatile float fb_bloom_param_0078db04 = 0x1.99999ap-2f;       // CD CC CC 3E (0.4f)
volatile kotor_fb::I fb_bloom_count_0078db08 = 5;              // 05 00 00 00
volatile float fb_bloom_param_0078db0c = 0x1p+6f;              // 00 00 80 42 (64.0f)
volatile kotor_fb::U fb_convolution_gate_0078db20 = 1;         // 01 00 00 00
volatile float fb_saturation_param_0078db24 = 0x1p+0f;         // 00 00 80 3F
volatile float fb_saturation_param_0078db28 = 0x1p+0f;         // 00 00 80 3F
volatile float fb_saturation_param_0078db2c = 0x1p+0f;         // 00 00 80 3F
volatile float fb_saturation_param_007a6884 = 0.0f;            // BSS
const float fb_shader_constants_0078e110[18][4] = {
    {0.0f, 0.0f, 0x1p+0f, 0.0f},
    {0.0f, 0x1p+0f, 0.0f, 0.0f},
    {0x1p+0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0x1p+0f, 0.0f},
    {0.0f, 0x1p+0f, 0.0f, 0.0f},
    {0x1p+0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0x1p+0f, 0.0f},
    {0.0f, 0x1p+0f, 0.0f, 0.0f},
    {0x1p+0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0x1p+0f, 0.0f},
    {0.0f, 0x1p+0f, 0.0f, 0.0f},
    {0x1p+0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0x1p+0f, 0.0f},
    {0.0f, 0x1p+0f, 0.0f, 0.0f},
    {0x1p+0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0x1p+0f, 0.0f},
    {0.0f, 0x1p+0f, 0.0f, 0.0f},
    {0x1p+0f, 0.0f, 0.0f, 0.0f}};
const float fb_bloom_kernel_0078e230[28] = {
    0x1.eb851ep-4f, 0x1p-2f, 0x1.28f5c2p-2f, 0x1p-2f, 0x1.eb851ep-4f,
    0x1p-2f, 0x1.f5c29p-2f, 0x1.28f5c2p-1f, 0x1.f5c29p-2f, 0x1p-2f,
    0x1.28f5c2p-2f, 0x1.28f5c2p-1f, 0x1.570a3ep-1f, 0x1.28f5c2p-1f, 0x1.28f5c2p-2f,
    0x1p-2f, 0x1.f5c29p-2f, 0x1.28f5c2p-1f, 0x1.f5c29p-2f, 0x1p-2f,
    0x1.eb851ep-4f, 0x1p-2f, 0x1.28f5c2p-2f, 0x1p-2f, 0x1.eb851ep-4f,
    0.0f, 0.0f, 0.0f};
// pad 0078E294..0078E29F = 000000000000000000000000
const float fb_bloom_kernel_0078e2a0[28] = {
    0x1.eb851ep-4f, 0x1p-2f, 0x1.28f5c2p-2f, 0x1p-2f, 0x1.eb851ep-4f,
    0x1p-2f, 0x1.f5c29p-2f, 0x1.28f5c2p-1f, 0x1.f5c29p-2f, 0x1p-2f,
    0x1.28f5c2p-2f, 0x1.28f5c2p-1f, 0x1.570a3ep-1f, 0x1.28f5c2p-1f, 0x1.28f5c2p-2f,
    0x1p-2f, 0x1.f5c29p-2f, 0x1.28f5c2p-1f, 0x1.f5c29p-2f, 0x1p-2f,
    0x1.eb851ep-4f, 0x1p-2f, 0x1.28f5c2p-2f, 0x1p-2f, 0x1.eb851ep-4f,
    0.0f, 0.0f, 0.0f};
// pad 0078E304..0078E30F = 000000000000000000000000
kotor_fb::I fb_pixel_shaders_cache_0078e5dc = -1;              // FF FF FF FF (glGetIntegerv writes it in place at 0046EDC7)
volatile kotor_fb::U8 fb_speed_blur_007a6880 = 0;              // BSS
volatile kotor_fb::U8 fb_saturation_007a6881 = 0;              // BSS
volatile kotor_fb::U8 fb_scan_noise_007a6882 = 0;              // BSS
volatile kotor_fb::U8 fb_film_noise_flag_007a6883 = 0;         // BSS
volatile kotor_fb::U fb_word_007a68bc = 0;                     // BSS
void* volatile fb_film_noise_texture_007a68d0 = nullptr;       // BSS
volatile kotor_fb::U fb_word_007a6938 = 0;                     // BSS
volatile kotor_fb::U8 fb_ati_frame_rendered_007a698c = 0;      // BSS
volatile kotor_fb::U8 fb_frame_rendered_007a698d = 0;          // BSS
// D69: 00827FB0 storage retired here (lane-r4-lowers render_counts_00827fa8[2] owns it).
}
