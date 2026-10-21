#include "866a226af7-texture.h"
namespace options_video {
namespace {
void DeleteTable(Table* table) {
    using DeletingDestructor=void (__thiscall*)(Table*,U32);
    const auto destructor=reinterpret_cast<DeletingDestructor>(PointerAt(PointerAt(table,0),0));
    destructor(table,1);
}
}
I32 __thiscall Client::SetTexturePack_005f14a0(U8 index,I32 unused) {
    (void)unused;auto& b=texture_bindings;
    String texture,unused_first,unused_second,gui;
    b.default_string_005b3190(&texture,nullptr);b.default_string_005b3190(&unused_first,nullptr);
    b.default_string_005b3190(&unused_second,nullptr);b.default_string_005b3190(&gui,nullptr);
    const auto finish=[&](I32 result) {
        b.destroy_string_005e5c20(&gui,nullptr);b.destroy_string_005e5c20(&unused_second,nullptr);
        b.destroy_string_005e5c20(&unused_first,nullptr);b.destroy_string_005e5c20(&texture,nullptr);return result;
    };
    float ratio=0.0f;I32 memory=0;
    if(index==bytes[0x1a4])return finish(1);
    bytes[0x1a4]=index;
    Table* table=nullptr;bool temporary=false;
    void* const app=*b.app_007a3a28;
    void* const manager=app?PointerAt(app,0xb8):nullptr;
    if(manager) {
        table=static_cast<Table*>(PointerAt(manager,0x128));
    } else {
        temporary=true;
        auto* allocation=static_cast<Table*>(b.allocate_006fa7e6(0x54));
        if(allocation) {
            ResourceRef ref;
            b.resource_ref_00406d80(&ref,nullptr,b.table_resource_0074e860);
            table=b.construct_table_00413cc0(allocation,nullptr,ref,1);
        }
        if(table==nullptr)return finish(0);
        if(b.load_table_004143b0(table,nullptr)==0){DeleteTable(table);return finish(0);}
    }
    if(table==nullptr)return finish(0);
    const I32 rows=IntegerAt(table,0x24);
    const I32 row=static_cast<I32>(index);
    if(rows==0 || row>=rows) {
        b.unload_table_004139e0(table,nullptr);DeleteTable(table);return finish(0);
    }
    String column;
    b.text_string_005e5a90(&column,nullptr,b.texture_column_0074e858);
    (void)b.string_entry_00413270(table,nullptr,row,&column,&texture);
    b.destroy_string_005e5c20(&column,nullptr);
    b.text_string_005e5a90(&column,nullptr,b.gui_column_0074e854);
    (void)b.string_entry_00413270(table,nullptr,row,&column,&gui);
    b.destroy_string_005e5c20(&column,nullptr);
    b.text_string_005e5a90(&column,nullptr,b.ratio_column_0074e848);
    (void)b.float_entry_00413430(table,nullptr,row,&column,&ratio);
    b.destroy_string_005e5c20(&column,nullptr);
    b.text_string_005e5a90(&column,nullptr,b.memory_column_0074e844);
    (void)b.int_entry_00413660(table,nullptr,row,&column,&memory);
    b.destroy_string_005e5c20(&column,nullptr);
    b.reset_textures_00421d50();
    String prefix,combined;
    String* constructed=b.text_string_005e5a90(&prefix,nullptr,b.pack_prefix_0074e834);
    String* sum=b.concatenate_005e5d10(constructed,nullptr,&combined,&texture);
    (void)b.set_pack_0070d800(1,sum);
    b.destroy_string_005e5c20(&combined,nullptr);b.destroy_string_005e5c20(&prefix,nullptr);
    constructed=b.text_string_005e5a90(&prefix,nullptr,b.pack_prefix_0074e834);
    sum=b.concatenate_005e5d10(constructed,nullptr,&combined,&gui);
    (void)b.set_pack_0070d800(2,sum);
    b.destroy_string_005e5c20(&combined,nullptr);b.destroy_string_005e5c20(&prefix,nullptr);
    b.dynamic_ratio_0041e9b0(ratio);
    (void)b.texture_memory_00421d90(memory);
    if(temporary){b.unload_table_004139e0(table,nullptr);DeleteTable(table);}
    bytes[0x1a4]=index;
    return finish(1);
}
}
