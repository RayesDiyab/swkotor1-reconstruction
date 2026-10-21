#include "0aec0b9933-menu_row_common.h"
using namespace menu_row;
extern "C" void __fastcall kotor_fastcall_00618a30(void* self,void*,U feat,void* list,menu_action::CallbackRecord callback,void* creature) {
    if(feat==0xFFFFFFFFu)return;
    void* data=kotor_rules::kotor_fastcall_00550c00(kotor_cell_rules_007a3a28,nullptr,static_cast<std::uint16_t>(feat));
    const U index=load<U>(list,4);
    kotor_fastcall_004fddd0(list,nullptr,static_cast<I>(index+1));
    StringStorage label_storage;
    kotor_fastcall_005edeb0(client(),nullptr,label_storage.get(),load<U>(data,8));
    StringCleanup label_cleanup{label_storage.get()};
    String* label=label_storage.get();
    if(creature==self){
        String closing(")");
        String opening(" (");
        StringStorage detail_storage;
        String* detail=kotor_fastcall_005edeb0(client(),nullptr,detail_storage.get(),0x9475);
        StringCleanup detail_cleanup{detail_storage.get()};
        StringStorage first_storage;
        String* first=GuiConcat005e5d10(label,nullptr,first_storage.get(),&opening);
        StringCleanup first_cleanup{first_storage.get()};
        StringStorage second_storage;
        String* second=GuiConcat005e5d10(first,nullptr,second_storage.get(),detail);
        StringCleanup second_cleanup{second_storage.get()};
        StringStorage third_storage;
        String* third=GuiConcat005e5d10(second,nullptr,third_storage.get(),&closing);
        StringCleanup third_cleanup{third_storage.get()};
        label->Assign005e5c50(third);
    }
    const U offset=index*0x38;
    assign_row_label(list,offset,label);
    set_callback(list,offset,callback);
    store(row(list,offset),0x1C,load<U>(creature,4));
    store(row(list,offset),8,feat|0x10000000u);
    menu_resources::Ref discarded;
    resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(at(row(list,offset),0x20)),nullptr,&discarded,static_cast<const menu_resources::Ref*>(at(data,0x18)));
}
