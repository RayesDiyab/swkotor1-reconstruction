// lane-upload: renderer capability leaves used by the texture upload chain.
// AurATIValidate 0045F640, AurCubeMapAvailable 0045F8B0, AurSGIMipmapAvailable 0045F910, AurBumpMapAvailable 0045FB10.
// Masks/capability word/control/vertex cache/programs are borrowed from their single storage owner
// (lane-renderer-bodies: kotor_renderer_gl_procs masks, kotor_renderer_gl_cells words). The three caches
// 0078E50C/0078E530/0078E538 are read/written only by these leaves (full-image xref scan), so their
// storage is defined here with the original .data cold value FFFFFFFF.
#include <cstdint>
#include "562f5b4ae6-renderer_gl_procs.h"
#include "f90e611d0e-renderer_gl_cells.h"
#include "b195664429-upload.h"

namespace kotor_renderer_capability_caches {
std::uint32_t ati_validate_0078e50c=0xffffffffu;   // .data FF FF FF FF
std::uint32_t cube_map_0078e530=0xffffffffu;       // .data FF FF FF FF
std::uint32_t sgi_mipmap_0078e538=0xffffffffu;     // .data FF FF FF FF
}

namespace {
namespace procs=kotor_renderer_gl_procs;
namespace cells=kotor_renderer_gl_cells;
namespace caches=kotor_renderer_capability_caches;
using U32=std::uint32_t;
// (caps & required) - required; neg; sbb; inc  ==  ((caps & required) == required) ? 1 : 0
inline U32 all_present(U32 caps,U32 required){return ((caps&required)==required)?1u:0u;}
}

namespace kotor_renderer_init {
// 0045F640
bool __cdecl ATIValidate_0045f640(){
 U32 value=caches::ati_validate_0078e50c;                            // 0045F640
 if(value==0xffffffffu){
  const U32 d0=procs::mask_0078e4d0;                                 // 0045F64B
  const U32 present=all_present(cells::capabilities_007bb85c,d0);    // 0045F651..0045F664
  const U32 control=cells::control_0078e4fc;                         // 0045F65A
  value=(control&present)!=0?1u:0u;                                  // and; neg; sbb; neg
  caches::ati_validate_0078e50c=value;                               // 0045F66D
 }
 return value==1u;                                                   // cmp ecx,1; sete al
}
// 0045F8B0
U32 __cdecl CubeMapAvailable_0045f8b0(){
 U32 value=caches::cube_map_0078e530;
 if(value==0xffffffffu){
  const U32 required=procs::mask_0078e484|procs::mask_0078e490;      // 0045F8BA..0045F8C5
  value=all_present(cells::capabilities_007bb85c,required);
  caches::cube_map_0078e530=value;                                   // 0045F8D5
 }
 return value;
}
// 0045F910
U32 __cdecl SGIMipmapAvailable_0045f910(){
 U32 value=caches::sgi_mipmap_0078e538;
 if(value==0xffffffffu){
  const U32 caps=cells::capabilities_007bb85c;                       // 0045F91A
  const U32 required=procs::mask_0078e4cc;                           // 0045F925 (loaded before the branch)
  if((procs::mask_0078e4d0&caps)!=0){caches::sgi_mipmap_0078e538=0;return 0;} // 0045F91F..0045F934
  value=all_present(caps,required);
  caches::sgi_mipmap_0078e538=value;                                 // 0045F93E
 }
 return value;
}
// 0045FB10
U32 __cdecl BumpMapAvailable_0045fb10(){
 const U32 d4=procs::mask_0078e4d4;                                  // esi
 const U32 required=d4|procs::mask_0078e490|procs::mask_0078e48c|procs::mask_0078e484|procs::mask_0078e480;
 const U32 caps=cells::capabilities_007bb85c;                        // edx, reused below
 if(all_present(caps,required)!=0)return 1;                          // 0045FB48 jne -> eax=1
 U32 vertex=cells::cache_vertex_0078e51c;                            // 0045FB4A
 if(vertex==0xffffffffu){vertex=all_present(caps,d4);cells::cache_vertex_0078e51c=vertex;} // 0045FB54..0045FB5F
 if((static_cast<U32>(cells::programs_0078e5ec)&vertex)==0)return 0; // test [0078E5EC],eax
 const U32 ati=all_present(caps,procs::mask_0078e4d0);               // 0045FB6C..0045FB7E
 return (ati&cells::control_0078e4fc)!=0?1u:0u;                      // 0045FB7F..0045FB8A
}
}
