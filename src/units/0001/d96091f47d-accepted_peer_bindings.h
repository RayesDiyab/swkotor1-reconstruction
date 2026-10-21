#pragma once
#include "a9383cea1d-renderer_initialize.h"
#include "2283c3a06f-peer-track.h"
#include "053c8e0236-peer-system.h"
#include "ba01a61bb3-peer-texture.h"
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
