#pragma once
#include "fce1f86b35-renderer_initialize.h"
#include "fea5cfa439-accepted_peer_bindings.h"
#include "9d6729921f-live_extensions.h"
#include "4691f07671-factory_contracts.h"
#include <cstddef>
namespace renderer_adoption {
using namespace kotor_renderer_init;
using Delete=U32(__fastcall*)(Renderer*,void*,U32);
using Fullscreen=U32(__fastcall*)(Renderer*,void*);
using Initialize=I32(__fastcall*)(Renderer*,void*,void*,void*,U32,U32,U32,U32,I32,I32,I32);
using ValidMode=U32(__stdcall*)(U32*,U32*,U8*,U32*,U32);
using Editor=I32(__fastcall*)(Renderer*,void*,U32,U32,U32,U32,U32,U32,U32);
using Reshape=void(__fastcall*)(Renderer*,void*,U32,U32);
using Restore=U32(__fastcall*)(Renderer*,void*);
using Shutdown=void(__fastcall*)(Renderer*,void*,U32,U32);
using Gamma=U32(__fastcall*)(Renderer*,void*,U32,U32);
using Context=void*(__fastcall*)(Renderer*,void*);
using Current=I32(__stdcall*)(void*,void*);
struct VTable {Delete destroy;Fullscreen fullscreen;Initialize initialize;ValidMode mode;Editor editor;Reshape reshape;Restore desktop,display;Shutdown shutdown;Gamma gamma;Context context;Current current;};
static_assert(sizeof(VTable)==48&&offsetof(VTable,gamma)==0x24&&offsetof(VTable,current)==0x2c,"twelve native x86 function pointers");
struct ProgramRecordInput {char* nonzero;char* zero;char* const* dynamic_nonzero;U32* const* parameters;void** output;};
struct Inputs {
    const VTable* table;
    void* volatile* singleton;U32 volatile* counter;U16* state;
    SharedBindings scalar_borrows;
    ProgramRecordInput programs[10];
    renderer_track::Views* track;renderer_system::Views* system;renderer_texture::Views* texture;
    aurora_context_adoption::ChooseArb volatile* choose;aurora_context_adoption::SwapInterval volatile* swap;
    aurora_context_adoption::CompressedTexImage2D volatile* compressed; // D11: original cell 007BB774 (glCompressedTexImage2DARB)
};
// Descriptor storage owns no original scalar, handle, shader, parameter or output.
// Main6 retains this object at one address, fills it before first admission,
// and retains every referenced input. No constructor or automatic publication.
struct Publication {kotor_aurora_factory::Views factory;SharedBindings shared;kotor_renderer_init::AcceptedRendererPeerViews peers;aurora_context_adoption::LiveExtensions extensions;};
extern Publication proposed_publication;
bool prepare(Publication&,const Inputs&) noexcept;
}
