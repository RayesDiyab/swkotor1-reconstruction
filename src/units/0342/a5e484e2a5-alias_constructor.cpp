#include "28b9fbfc84-alias_constructor.h"
#include "9e11ed2f44-allocator_live.h"
#include <new>
namespace alias_path {
AliasListView* __fastcall ConstructAlias005e6810(AliasListView* storage,void*) {
    auto* self=::new(static_cast<void*>(storage)) AliasListView;
    void* memory=live_operator_new(12);
    InternalView* created=memory
        ? ConstructAliasInternal005eb5b0(static_cast<InternalView*>(memory),nullptr)
        : nullptr;
    self->internal=created;
    return self;
}
}
