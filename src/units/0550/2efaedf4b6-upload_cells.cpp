// lane-upload: single storage owner for the original globals of the upload/bind chain that no other
// lane or M41 object owns, plus the one `texture_upload::cells` view. Cold values are the original
// PE image bytes (file-backed .data/.rdata; BSS = 0). Writer census: full-image dumpbin xref scan.
// Borrowed (not defined here): 0078D458 lane-gui-draw kotor_renderer_cells::enable_0078d458;
// 007A477C lane-texture-epoch texture_cells::null_texture_007a477c (the object; constant-initialized view).
#include <cstdint>
#include "b195664429-upload.h"

namespace kotor_renderer_cells {extern std::uint32_t enable_0078d458;}     // lane-gui-draw gl_enum_cells.h
namespace texture_cells {extern font::Safe null_texture_007a477c;}            // lane-texture-epoch owner object (texture_cells.hpp:102, texture_cells.cpp D19)

namespace texture_upload_cells {
using U=std::uint32_t;using I=std::int32_t;
U gl_texture_cube_map_0078d478=0x00008513u;    // .data 13 85 00 00; read 00420505.., no writer
U enable_texture_controllers_0078d430=1u;      // .data 01 00 00 00; read 00421533, no writer
I anisotropy_007a685c=0;                       // BSS; writer 0044F2DE (outside this packet)
U texture_binds_count_007a4710=0u;             // BSS; writers 00420572/004205AA/0046EE02
I bind_gate_007a470c=0;                        // BSS; read 0042052B only, no writer
U bind_zero_007a4714=0u;                       // BSS; read 004214B0 only, no writer
U current_tid_007a47b8=0u;                     // BSS; writer 004214FF
// .rdata 0073F36C..0073F3C8: GL internal formats indexed by GetCompressionFormat (24 entries; the
// following dwords are unrelated zero/double data). Read sites 00424AE3/00424B73/00424F24/00424FB3/004242DC/00424365.
const U gl_format_table_0073f36c[24]={
 0x00001908u,0x00001907u,0x0000190Au,0x00001909u,0x000080E1u,0x000084EAu,0x000084EBu,0x000083F0u,
 0x000083F3u,0x000086A1u,0x000086A0u,0x00008078u,0x00008075u,0x00008076u,0x00008074u,0x00008651u,
 0x00008653u,0x00008654u,0x00008655u,0x0000865Cu,0x0000865Du,0x0000865Eu,0x0000865Fu,0x00008533u};
U cube_face_rotation_0078db38[6]={3u,1u,0u,2u,2u,0u}; // .data; read 0042409E only, no writer
U cube_face_target_0078db50[6]={1u,0u,2u,3u,4u,5u};   // .data; read 00423FCB only, no writer
}

namespace {
namespace c=texture_upload_cells;
const texture_upload::Cells storage{
 &c::gl_texture_cube_map_0078d478,
 &kotor_renderer_cells::enable_0078d458,
 &c::anisotropy_007a685c,
 &c::texture_binds_count_007a4710,
 &c::bind_gate_007a470c,
 &c::bind_zero_007a4714,
 &texture_cells::null_texture_007a477c,
 &c::current_tid_007a47b8,
 &c::enable_texture_controllers_0078d430,
 c::gl_format_table_0073f36c,
 c::cube_face_target_0078db50,
 c::cube_face_rotation_0078db38};
}
namespace texture_upload {const Cells& cells=storage;}
