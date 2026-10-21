#include "4d1de05f14-alias_constructor.h"
#include "d735aa7dd1-string_priority.h"
#include "77e8b6221f-menu_targets.h"
#include <new>
namespace alias_path {
InternalView* __fastcall ConstructAliasInternal005eb5b0(InternalView* storage,void*) {
    // Member default construction invokes the existing actual String once.
    auto* self=::new(static_cast<void*>(storage)) InternalView;
    self->head=nullptr;
    GuiAssign005e5140(&self->fallback,nullptr,kotor_menu20::bindings.initial_text_0073d71c);
    return self;
}
}
