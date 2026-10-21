#pragma once
#include "ac3feef992-normal.h"
#include "2ef94f949a-normal.h"
namespace task22_strings {
// D1: same x86 entries, slot types re-spelled as __fastcall(receiver, unused EDX, ...).
using Constructor=void*(__fastcall*)(void*,void*);
using Destructor=void(__fastcall*)(void*,void*);
using Assign=void*(__fastcall*)(void*,void*,const char*);
using CStr=const char*(__fastcall*)(void*,void*);
using NotEqual=unsigned(__fastcall*)(void*,void*,const char*);
struct Callbacks {Constructor construct;Destructor destroy;Assign assign;CStr cstr;NotEqual not_equal;};
// x86 code entry labels, not callable C++ cdecl providers. Their explicit
// assembler boundary consumes ECX and 0/1 stack words with RET0/RET4.
extern "C" void K22_CtorEntry(),K22_DtorEntry(),K22_AssignEntry(),K22_CStrEntry(),K22_NotEqualEntry();
Callbacks callbacks() noexcept;
struct Attachment {
 menu_normal_40_49::Providers* gui=nullptr;
 menu_normal_retained_40_49::Providers* retained=nullptr;
 Callbacks previous_gui{},previous_retained{};
 task10_resource_cells::ManagerCell gui_resource_cell=nullptr;
 task10_resource_cells::ManagerCell retained_resource_cell=nullptr;
 bool active=false;
};
enum class Result {PreparedFiveStringSlots,InvalidIdentity,AlreadyAttached,Conflict,AllocatorNotAttached};
// The graph owner retains both tables, canonical String/Ref objects, actual
// resource cells and paired allocator resources until final consumer cleanup.
Result attach(Attachment&,menu_normal_40_49::Providers&,menu_normal_retained_40_49::Providers&) noexcept;
bool detach(Attachment&,bool string_and_reference_consumers_drained) noexcept;
}
