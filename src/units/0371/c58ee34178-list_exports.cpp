#include "20bf4c9f83-next_step.h"
#include "23c6e040ff-key_entry_from_table.h"
#include <cstring>
namespace {
void __cdecl PublishOpaque(void* slot,base_ini_reconstruction::Node* next) {
    // slot originated as void**. No Node** lvalue aliases a void* object.
    *static_cast<void**>(slot)=next;
}





}
extern "C" __declspec(noinline) uint32_t __fastcall unresolved_list_at_pos_005e9b60(
    void*,void*,void* position) {
    if(!position) {
        using namespace base_ini_reconstruction;



        const int line=236;

        warning_005e9400(static_cast<kotor_menu20::Debug*>(current_root_base()->debug),
                        nullptr,line,diagnostic_source_0074e208,diagnostic_at_pos_0074e264);
        return 0;
    }
    uint32_t payload;
    std::memcpy(&payload,static_cast<const unsigned char*>(position)+8,4);



    return payload;

}
extern "C" __declspec(noinline) uint32_t __fastcall unresolved_list_next_005e9b90(
    void*,void*,void** position) {
    // The opaque cursor is an actual void* object. Its nonnull value must refer
    // to a live canonical Node (as constructed by the selected AddTail owner).
    auto* current=static_cast<base_ini_reconstruction::Node*>(*position);





    void* result=list_accessor_adoption::NextStep005e9b90(current,position,&PublishOpaque);

    return reinterpret_cast<uint32_t>(result);
}
