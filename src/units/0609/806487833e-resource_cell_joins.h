#pragma once
#include "17765446a4-string_joins.h"
namespace task10_resource_cells {
struct Attachment {
 task22_strings::Attachment* strings=nullptr;
 ManagerCell previous_gui=nullptr,previous_retained=nullptr;
 bool active=false;
};
enum class Result {PreparedManagerCells,InvalidOwners,Conflict,AlreadyAttached};
// Attach after the existing five String slots. Publication and Manager/worker
// lifetime remain with the graph owner; this join changes two addresses only.
Result attach(Attachment&,task22_strings::Attachment&) noexcept;
// Consumer/worker drain precedes this restore, then existing String detach.
bool detach(Attachment&,bool manager_consumers_drained) noexcept;
}
