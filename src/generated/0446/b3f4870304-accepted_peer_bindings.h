#pragma once
#include "5e26ea8357-renderer_initialize.h"
#include "a68164df42-peer-track.h"
#include "502f4dd31a-peer-system.h"
#include "b8f3484ae5-peer-texture.h"
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
