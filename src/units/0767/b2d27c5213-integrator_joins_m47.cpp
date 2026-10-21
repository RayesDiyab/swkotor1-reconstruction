// M47 composition-time integrator joins (claude-takeover m47-compose). Linker directives only; defines nothing.
// Requested names verified undefined in the M47 selection; targets are the single staged bodies (D9 mechanism).
// D76: 00423A60 AurTextureGetReference 3D spelling -> single body _reference_00423a60 (core47-texture-reference); cdecl, 2 dwords, EAX pointer.
#pragma comment(linker, "/alternatename:_aur_texture_get_reference_00423a60=_reference_00423a60")
// D32/D59: 006857A0 single body = lane-renderer-bodies render_pool deleteAt (r4-lowers body dropped in the composer copy); fastcall @12 both.
#pragma comment(linker, "/alternatename:@frame_RemoveArray_006857a0@12=?deleteAt_006857a0@kotor_renderer_gl@@YIXPAUArrayMutations@kotor_reconstruction@@PAXH@Z")
// 0045F710 / 004AAAB0: Tier B nv_combiner_lowers copies dropped in favour of the existing M45 bodies; cdecl both.
#pragma comment(linker, "/alternatename:?MultitextureAvailable_0045f710@kotor_renderer_gl@@YAIXZ=_aur_multi_texture_available_0045f710")
#pragma comment(linker, "/alternatename:?Homogenize_004aaab0@kotor_renderer_gl@@YAPAMPBM0@Z=_homogenize_004aaab0")
// ---- single-body joins (address-level duplicates removed from composer copies; D68 earliest-staged owner)
// 0063E7A0: no stack arguments, callee clean 0, EAX=1 in both spellings (fastcall(ECX,EDX) slot vs stdcall()); trivial return-true, machine-identical (D68)
#pragma comment(linker, "/alternatename:@input_desc_return_true_0063e7a0@8=_cres_base_constant_word_0063e7a0@0")
// 0064B510: M51 retired the M47 row "/alternatename:@kotor_fastcall_0064b510@8=?Construct@Message@services_tail5@@QAEPAU12@XZ" -
// it formed a cycle with the M47 service-tail-8 forwarder (M50 hang); service-tail-8 now owns the genuine body and nothing references the fastcall name.
// ---- sound chain joins (lane-sound r5/rev3 consumer spellings -> existing single bodies; sound copies dropped in composer copies)
// 00409B10 CRes::Demand single body lane-resman (D18); fastcall(ECX,EDX) @8, both return EAX
#pragma comment(linker, "/alternatename:?res_demand_00409b10@kotor_resource_required@@YIIPAX0@Z=@c2da_current_resource_demand_00409b10@8")
// 00409B80 CRes::Release single body lane-resman (D18); fastcall @8. CAUTION: the D18 body is declared void (drops the 00408C90 EAX) while the original returns it and sound callers read it (SND47-2)
#pragma comment(linker, "/alternatename:?res_release_00409b80@kotor_resource_required@@YIIPAX0@Z=@c2da_release_demand_00409b80@8")
// 004AA980 cdecl U(const float*,const float*) == cdecl U(const void*,const void*); existing M44 body (x87 asm)
#pragma comment(linker, "/alternatename:?Equal004aa980@kotor_sound_math@@YAIPBM0@Z=_aurora_vector_equal_004aa980")
// 0056B6F0 fastcall(ECX,EDX) @8 void both; existing M45 body
#pragma comment(linker, "/alternatename:?ArrayListDestroy0056b6f0@kotor_sound_internal@@YIXPAX0@Z=@exo_array_list_destruct_0056b6f0@8")
// 005E58A0 fastcall(ECX,EDX) @8, both return unsigned EAX; earliest-staged owner lane-r4-lowers (D68)
#pragma comment(linker, "/alternatename:?StringIsEmpty005e58a0@kotor_sound_path@@YIIPAUStartupString@kotor_reconstruction@@PAX@Z=@frame_LogIsEmpty_005e58a0@8")
