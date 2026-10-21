#pragma once
#include "a39d5fa52e-renderer_initialize.h"
#include "e2ee0f67fb-peer-track.h"
#include "5ad123f579-peer-system.h"
#include "9a531906fe-peer-texture.h"
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
