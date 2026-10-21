#pragma once
#include "be619a120a-renderer_initialize.h"
#include "44b0a54d26-peer-track.h"
#include "20d87046de-peer-system.h"
#include "baf1ebaa25-peer-texture.h"
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
