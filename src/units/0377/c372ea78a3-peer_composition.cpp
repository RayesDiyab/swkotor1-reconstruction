#include "414df62b85-renderer_adoption.h"
#pragma comment(linker,"/alternatename:?VoidReturn_004015a0@kotor_renderer_init@@YAXI@Z=_renderer_void_004015a0")
#pragma comment(linker,"/alternatename:?TrackMatrices_004a2400@kotor_renderer_init@@YAXXZ=_renderer_track_004a2400")
#pragma comment(linker,"/alternatename:?InitRenderSystem_00425c30@kotor_renderer_init@@YAXXZ=_renderer_system_00425c30")
#pragma comment(linker,"/alternatename:?TextureRefreshAll_00422360@kotor_renderer_init@@YAXXZ=_renderer_texture_00422360")
namespace renderer_track {Views& bindings() noexcept{return *kotor_renderer_init::accepted_renderer_peer_views.track;}}
namespace renderer_system {Views& bindings() noexcept{return *kotor_renderer_init::accepted_renderer_peer_views.system;}}
namespace renderer_texture {Views& bindings() noexcept{return *kotor_renderer_init::accepted_renderer_peer_views.texture;}}
