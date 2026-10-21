#include "77c58f98ec-native_string_deps.h"
#include <cstring>
extern "C" void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref* self,void*,menu_resources::String* output) {
    // Original snapshots all sixteen bytes before invoking assignment. Bytes
    // after an embedded NUL still belong to the snapshot; assignment scans it.
    alignas(4) char snapshot[20];
    std::memcpy(snapshot,self->bytes,16);



    snapshot[16]=0;
    (void)c2da_assign_text_005e5140(output,nullptr,snapshot);
}
