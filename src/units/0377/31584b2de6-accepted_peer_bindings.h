#pragma once
#include "f3f4667038-renderer_initialize.h"
#include "6ffb6aeefb-peer-track.h"
#include "7912791eae-peer-system.h"
#include "141fd871ec-peer-texture.h"
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
