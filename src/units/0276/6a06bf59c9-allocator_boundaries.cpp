// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit modern-allocator-boundary
// (task-31 modern-critical-boundary/allocator_boundaries.cpp). Only change: one kotor_host_diag call (D94
// proposal) before the unchanged ExitProcess(86). This site fires if allocate_internal/free_internal run
// before GameHeap::publish (e.g. from a CRT static initializer, W-0) or after the heap is gone.
#include "1be714b0f9-critical_section.h"
#include "4e2136d0b8-functional_runtime.h"
#include "05a3c5e4fb-allocator_live.h"
// Move-only proposal: Task31 retains sole body ownership; apply deletion hunks first.
// No console/FileThread/canonical heap storage is defined in this unit.
namespace kotor_production {
bool AllocatorBindingsReady() noexcept {
 return kotor_functional::source_heap_identity();
}
void RequireAllocatorBindings(){
 if(!AllocatorBindingsReady()){
  ;
  ::ExitProcess(86);
 }
}
}
namespace kotor_startup_dependencies {
void* __cdecl allocate_internal(std::uint32_t n){kotor_production::RequireAllocatorBindings();return live_operator_new(n);}
void __cdecl free_internal(void* p){kotor_production::RequireAllocatorBindings();live_free(p);}
}
