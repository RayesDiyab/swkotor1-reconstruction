// m46_joins.cpp - lane-m46-3dmodel: ABI-verified /alternatename joins (D9/D50 mechanism) from consumer spellings of the
// 3D model/anim/emitter units onto EXISTING single bodies/storage. No code, no data, no address credit.
// Each join was checked: same convention, same stack bytes (callee cleanup), same return width, same object size.
// The integrator may instead carry these rows in its composition join list (then drop this unit).
//
// J1 006857A0 CExoArrayList::DelIndex (anim3d.h:249, emitter.h:174): fastcall(list, edx, I index), RET 4 ->
//    M45 body r4-lowers frame-gui composition.h:23 `void __fastcall frame_RemoveArray_006857a0(void*,void*,int)`.
//    D32/D59 name lane-renderer-bodies (render_pool.cpp kotor_renderer_gl::deleteAt_006857a0, same ABI) as the final
//    owner; when that unit is staged the r4 copy must be dropped and these joins retargeted (single body).
#pragma comment(linker, "/alternatename:@deleteAt_006857a0@12=?deleteAt_006857a0@kotor_renderer_gl@@YIXPAUArrayMutations@kotor_reconstruction@@PAXH@Z")   // [M47 compose] retargeted to the D32/D59 owner (render_pool staged)
// [M47 compose] ?deleteAt_006857a0@kotor_renderer_gl... join removed: the symbol is now defined by lane-renderer-bodies render_pool (Tier B)
// J2 0043F730 MaxTree::~MaxTree (anim3d.h:226 `void __fastcall maxtree_destruct_0043f730(void*, void*)`, RET 0) ->
//    M45 3d-model-model-destroy `void __fastcall aurora_maxtree_destruct_0043f730(MaxTree*, void*)` (model_destroy.cpp:199).
#pragma comment(linker, "/alternatename:@maxtree_destruct_0043f730@8=@aurora_maxtree_destruct_0043f730@8")
// J3 0046BD10 CExoArrayList<T*>::AddUnique (emitter.h:194 fastcall(PointerList*, edx, U value), RET 4) ->
//    M45 texture-epoch-cleanup-slots `void __fastcall texture_join::add_unique_0046bd10(List*, void*, Ref*)`
//    (texture_join.h:13; pointer-width element, identical body for every pointer element type).
#pragma comment(linker, "/alternatename:@exo_array_list_add_unique_0046bd10@12=?add_unique_0046bd10@texture_join@@YIXPAUArrayMutations@kotor_reconstruction@@PAXPAUTexture@font@@@Z")
// J4 007A68A4 renderer vertex pool cell (model_load.hpp:196 `extern aurora_model::PoolObject* volatile
//    kotor_vertex_pool_007a68a4`) -> D39 owner lane-renderer-bodies kotor_renderer_gl_cells::pool_007a68a4 (4-byte pointer
//    cell; already an M44-base unresolved name: this join adds no new name).
#pragma comment(linker, "/alternatename:_kotor_vertex_pool_007a68a4=?pool_007a68a4@kotor_renderer_gl_cells@@3PAVRequestPoolManager@kotor_renderer_gl@@A")
