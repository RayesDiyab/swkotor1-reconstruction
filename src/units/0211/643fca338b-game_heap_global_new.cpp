// game_heap_global_new.cpp - candidate 1118 single-heap composition object (authored, not a reconstructed original body).
//
// Original swkotor.exe has ONE C++ allocation heap: operator new is 006FA7E6 (_nh_malloc(size,1) on the game CRT heap
// _crtheap 00834338) and free/operator delete is 006FA390 on the same heap.  In this reconstruction those bodies are the
// task-23 live allocator in 0004-live-policy.obj (_live_operator_new / _live_free), published at .CRT$XIB by
// 0006/0007 before every C (.CRT$XIC) and C++ (.CRT$XCU) initializer.  Objects 0841-1200 called the modern libcmt
// ::operator new/delete (process heap) and objects freed those blocks through _live_free, or vice versa (cross-heap free,
// dynamically confirmed: RtlFreeHeap(live heap, process-heap block)).
//
// This object supplies the replaceable global allocation functions for the whole image so that every ::operator new /
// delete (including those inside libcmt/libcpmt C++ code) uses the single game heap, as in the original.  Allocation
// failure behaviour follows the original 006FA7E6 (new-handler loop, then null) rather than throwing std::bad_alloc.
// Declarations identical to allocator_live.h (task-23/sbh-slice2/live); windows.h is not pulled into this TU.
#include <cstddef>
#include <cstdint>
#include <new>

extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
}

void* __cdecl operator new(std::size_t size) { return live_operator_new(static_cast<std::uint32_t>(size)); }
void* __cdecl operator new[](std::size_t size) { return live_operator_new(static_cast<std::uint32_t>(size)); }
void* __cdecl operator new(std::size_t size, const std::nothrow_t&) noexcept { return live_operator_new(static_cast<std::uint32_t>(size)); }
void* __cdecl operator new[](std::size_t size, const std::nothrow_t&) noexcept { return live_operator_new(static_cast<std::uint32_t>(size)); }

void __cdecl operator delete(void* block) noexcept { live_free(block); }
void __cdecl operator delete[](void* block) noexcept { live_free(block); }
void __cdecl operator delete(void* block, std::size_t) noexcept { live_free(block); }
void __cdecl operator delete[](void* block, std::size_t) noexcept { live_free(block); }
void __cdecl operator delete(void* block, const std::nothrow_t&) noexcept { live_free(block); }
void __cdecl operator delete[](void* block, const std::nothrow_t&) noexcept { live_free(block); }
