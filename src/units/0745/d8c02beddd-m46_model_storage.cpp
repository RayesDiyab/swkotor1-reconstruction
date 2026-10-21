// m46_model_storage.cpp - lane-m46-3dmodel: storage for the five task-7 historical extents the M45 lane-3d-model units
// reference through the staged donor header 3d/model/task7/storage.h (model_cache / model_destroy / model_lifetime /
// model_instance). Verbatim slice of the donor task-7 cpp-range-storage/storage.cpp (sha256 e976a12e...; header
// 4e43673a... == staged 3d/model/task7/storage.h): ONLY these five objects. The donor's other extents already have
// single owners in M45 (007A477C texture_cells, 007B8FD8 aurres, 007A8A58 3d-model _aurora_gobs_007a8a58) or other
// lane owners (007B90FC lane-r4-lowers D50, 007A693C lane-world-los D55), so the donor file cannot be staged whole.
// The donor's _borrowed_pointer_* /alternatename lines are not carried (no M45 object references those names).
// Original: BSS, zero; each extent has an original static initializer registering an atexit free of its data word:
//   007A8A88 xc 0073AE50 -> atexit 0073B870;  007A8A7C 0073AE60 -> 0073B890;  007A8A70 0073AE70 -> 0073B8B0;
//   007A8A64 0073AE80 -> 0073B8D0;            007B9398 0073AEE0 -> 0073B990   (mov eax,[cell]; test; push; call 006FA390)
// That shutdown-only cleanup is NOT reproduced (no allowlisted initializer; same recorded-deviation class as D60).
// Constant-initialised (.bss): no dynamic initializer / .CRT$XCU.
// Uses (consumer side): 007A8A64 model cache list (FindModel/BinarySearchModel/InsertModel); 007A8A70/7C/88 lane-3d-model
// lifetime lists; 007B9398 scene list (data 7B9398, count 7B939C, capacity 7B93A0 - also kotor_scene3d::Cells
// scene_list_007b9398, which the 3D-core Cells owner must point at THIS object: typed CleanupStorage12 vs PointerList).
#include "2a4dd639d6-storage.h"

extern "C" {
alignas(4) CleanupStorage12 historical_storage_007a8a88 = {};
alignas(4) CleanupStorage12 historical_storage_007a8a7c = {};
alignas(4) CleanupStorage12 historical_storage_007a8a70 = {};
alignas(4) CleanupStorage12 historical_storage_007a8a64 = {};
alignas(4) CleanupStorage12 historical_storage_007b9398 = {};
}
