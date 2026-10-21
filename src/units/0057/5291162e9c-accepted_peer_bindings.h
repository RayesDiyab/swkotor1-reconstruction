#pragma once
#include "85474cc56b-renderer_initialize.h"
#include "2cf630ca8e-peer-track.h"
#include "83f7036838-peer-system.h"
#include "888396bfd7-peer-texture.h"
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
