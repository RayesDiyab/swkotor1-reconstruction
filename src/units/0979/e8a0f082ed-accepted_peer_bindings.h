#pragma once
#include "4b2b3e80d1-renderer_initialize.h"
#include "ed365da85d-peer-track.h"
#include "07bd4cb701-peer-system.h"
#include "6818f5d841-peer-texture.h"
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
