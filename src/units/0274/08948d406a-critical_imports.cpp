#include "bb96bfd227-critical_section.h"
#include "b0cfb063de-allocator_live.h"
// Zero-address-credit adapters. Compile these once alongside the unchanged
// task27 wrapper and task23 allocator; actual allocator globals remain borrowed.
namespace kotor_startup_dependencies {
void __stdcall initialize(CRITICAL_SECTION* p){::InitializeCriticalSection(p);}
void __stdcall destroy(CRITICAL_SECTION* p){::DeleteCriticalSection(p);}
void __stdcall enter(CRITICAL_SECTION* p){::EnterCriticalSection(p);}
void __stdcall leave(CRITICAL_SECTION* p){::LeaveCriticalSection(p);}
}
