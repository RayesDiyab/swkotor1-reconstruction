#include "ef7355cd52-ini_section.h"
namespace ini_section {
using namespace base_ini_reconstruction;
Section* __fastcall ConstructSection005ea600(Section* self,void*) {
    // Default placement starts the trivial enclosing and wrapper lifetimes;
    // it does not value-initialize their storage. String has its own lifetime.
    ::new(self) Section;
    ::new(static_cast<void*>(self->name_storage)) String;
    self->lines.construct_005ea4d0(); // unchanged genuine nested constructor
    return self; // nested allocation null remains a normal Section return
}
}
