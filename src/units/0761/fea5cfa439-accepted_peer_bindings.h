#pragma once
#include "fce1f86b35-renderer_initialize.h"
#include "39727ccc17-peer-track.h"
#include "8764550a9d-peer-system.h"
#include "875f576c3c-peer-texture.h"
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
