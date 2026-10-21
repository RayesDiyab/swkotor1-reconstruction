#pragma once
#include "150dfc8eb9-renderer_initialize.h"
#include "e8f71ccd4c-peer-track.h"
#include "96c424ac30-peer-system.h"
#include "e418a9ce27-peer-texture.h"
namespace kotor_renderer_init {
struct AcceptedRendererPeerViews {
    renderer_track::Views* track;
    renderer_system::Views* system;
    renderer_texture::Views* texture;
};
// Task6 supplies one stable canonical view owner. No defaults/cells here.
extern AcceptedRendererPeerViews& accepted_renderer_peer_views;
}
extern "C" void __cdecl renderer_void_004015a0();
