// texture_vtable.cpp - lane-texture-epoch PROPOSAL (D1/D21): typed constant vtables, slot order = pinned PE words
// 0073F160 (CAurTextureBasic) and 0073F0A8 (CAurTexture), dumped by tools/vtable.py. Constant initialization only.
#include "8945d3b71e-texture_vtable.hpp"
namespace texture_vtable {
const BasicVTable basic_vtable_0073f160={{
 &texture_owner::scalar_delete_004233c0,                    // +00 ~CAurTextureBasic (scalar deleting)
 &texture_owner::basic_query_00641db0,                      // +04 self_return
 {
  &texture_join::referenced_004205c0,                       // +08
  &texture_is_cube_map_0041ebf0,                            // +0C
  &texture_is_invalid_004205d0,                             // +10
  &texture_is_diffuse_bump_map_0041ee70,                    // +14
  &texture_is_specular_bump_map_0041eeb0,                   // +18
  &texture_is_bump_map_0041eef0,                            // +1C
  &texture_is_normal_map_0041ef50,                          // +20
  &texture_is_using_global_alpha_0041ef80,                  // +24
  &texture_is_fully_loaded_0041ec10,                        // +28
  &texture_is_environment_mapped_0041ed40,                  // +2C
  &texture_is_processed_0041ec30,                           // +30
  &texture_is_txi_loaded_0041ec50,                          // +34
  &texture_font_info_0041efa0,                              // +38
  &texture_get_image_0041efc0,                              // +3C
  &texture_get_name_0041efe0,                               // +40
  &texture_get_file_range_0041f000,                         // +44
  &texture_get_width_0041f020,                              // +48
  &texture_get_height_0041f040,                             // +4C
  &texture_get_bpp_0041f060,                                // +50
  &texture_get_num_mip_maps_0041f080,                       // +54
  &texture_get_num_x_0041f0a0,                              // +58
  &texture_get_num_y_0041f0c0,                              // +5C
  &texture_get_clamp_0041f140,                              // +60
  &texture_get_compressed_size_0041f1a0,                    // +64
  &texture_get_env_map_alpha_0041f180,                      // +68
  &texture_get_mip_map_0041f0e0,                            // +6C
  &texture_get_filter_0041f100,                             // +70
  &texture_get_map_texels_to_pixels_0041f120,               // +74
  &texture_get_bump_map_scaling_0041f160,                   // +78
  &texture_get_bump_intensity_0041edd0,                     // +7C
  &texture_get_specular_bump_intensity_0041edf0,            // +80
  &texture_get_diffuse_bump_intensity_0041ee30,             // +84
  &texture_get_specular_color_0041ed60,                     // +88
  &texture_get_compression_format_0041f1c0,                 // +8C
  &texture_get_txi_size_0041ed20,                           // +90
  &texture_get_txi_data_0041ec90,                           // +94
  &texture_get_texture_format_0041f1f0,                     // +98
  &texture_set_texture_format_0041f260,                     // +9C
  &texture_set_txi_data_0041ecb0,                           // +A0
  &texture_set_txi_loaded_0041ec70,                         // +A4
  &texture_increment_tid_index_0041f3b0,                    // +A8
  &texture_gl_image_00421430,                               // +AC
 }},
 &texture_resource::free_image_0041f570,                    // +B0 CAurTextureBasic::FreeImage
 &texture_bind_004214b0};                                   // +B4 CAurTexture::Bind (lane-upload)
const ReferenceVTable reference_vtable_0073f0a8={{
 &texture_reference_scalar_delete_00423130,                 // +00 ~CAurTexture
 &texture_reference_query_0063e7f0,                         // +04 0063E7F0 return_zero as the slot's own type (D24/E-D2)
 {
  &texture_join::referenced_004205c0,                       // +08
  &texture_is_cube_map_0041ebf0,                            // +0C
  &texture_is_invalid_004205d0,                             // +10
  &texture_is_diffuse_bump_map_0041ee70,                    // +14
  &texture_is_specular_bump_map_0041eeb0,                   // +18
  &texture_is_bump_map_0041eef0,                            // +1C
  &texture_is_normal_map_0041ef50,                          // +20
  &texture_is_using_global_alpha_0041ef80,                  // +24
  &texture_is_fully_loaded_0041ec10,                        // +28
  &texture_is_environment_mapped_0041ed40,                  // +2C
  &texture_is_processed_0041ec30,                           // +30
  &texture_is_txi_loaded_0041ec50,                          // +34
  &texture_font_info_0041efa0,                              // +38
  &texture_get_image_0041efc0,                              // +3C
  &texture_get_name_0041efe0,                               // +40
  &texture_get_file_range_0041f000,                         // +44
  &texture_get_width_0041f020,                              // +48
  &texture_get_height_0041f040,                             // +4C
  &texture_get_bpp_0041f060,                                // +50
  &texture_get_num_mip_maps_0041f080,                       // +54
  &texture_get_num_x_0041f0a0,                              // +58
  &texture_get_num_y_0041f0c0,                              // +5C
  &texture_get_clamp_0041f140,                              // +60
  &texture_get_compressed_size_0041f1a0,                    // +64
  &texture_get_env_map_alpha_0041f180,                      // +68
  &texture_get_mip_map_0041f0e0,                            // +6C
  &texture_get_filter_0041f100,                             // +70
  &texture_get_map_texels_to_pixels_0041f120,               // +74
  &texture_get_bump_map_scaling_0041f160,                   // +78
  &texture_get_bump_intensity_0041edd0,                     // +7C
  &texture_get_specular_bump_intensity_0041edf0,            // +80
  &texture_get_diffuse_bump_intensity_0041ee30,             // +84
  &texture_get_specular_color_0041ed60,                     // +88
  &texture_get_compression_format_0041f1c0,                 // +8C
  &texture_get_txi_size_0041ed20,                           // +90
  &texture_get_txi_data_0041ec90,                           // +94
  &texture_get_texture_format_0041f1f0,                     // +98
  &texture_set_texture_format_0041f260,                     // +9C
  &texture_set_txi_data_0041ecb0,                           // +A0
  &texture_set_txi_loaded_0041ec70,                         // +A4
  &texture_increment_tid_index_0041f3b0,                    // +A8
  &texture_gl_image_00421430,                               // +AC
 }},
 &texture_free_image_0041f310,                              // +B0 CAurTexture::FreeImage
 &texture_bind_004214b0};                                   // +B4 CAurTexture::Bind (lane-upload)
}
