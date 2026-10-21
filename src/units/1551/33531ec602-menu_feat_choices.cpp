#include "0aec0b9933-menu_row_common.h"
#include "d467fbada0-m15e.h"
using namespace menu_row;
extern "C" void __fastcall kotor_fastcall_00619950(void* self,void*,void* list,void* creature) {
    void* server=server_creature(self);
    if(server==nullptr)return;
    void* area=recovered_object_area_004cb120(server,nullptr);
    if(load<U>(area,0x2B0)!=0)return;
    bool special=false;
    const U id=kotor_m15a_creature_get_equipped_item_id_0060ca80(self,nullptr,0x10);
    if(id!=0x7F000000u){
        void* object=kotor_fastcall_005ed580(client(),nullptr,id);
        if(object!=nullptr){
            const auto* table=load<const KotorM15ObjectMethods_007516c8*>(object);
            if(table->s034(object,nullptr)!=nullptr){
                // The original reloads the vptr and repeats the item cast.
                table=load<const KotorM15ObjectMethods_007516c8*>(object);
                void* item=table->s034(object,nullptr);
                const U base_id=load<U>(item,0xC);
                void* base=kotor_m15b_base_item_array_get_base_item_005b31a0(load<void*>(kotor_cell_rules_007a3a28,0x34),nullptr,static_cast<I>(base_id));
                special=base!=nullptr&&load<std::uint8_t>(base,0x1A)>0;
            }
        }
    }
    const menu_action::CallbackRecord callback{&kotor_fastcall_00617dd0,0,0,0};
    U feat;
    if(special){
        feat=kotor_fastcall_00616140(self,nullptr,0x52,0x12,0x1D);
        kotor_fastcall_00618a30(self,nullptr,feat,list,callback,creature);
        feat=kotor_fastcall_00616140(self,nullptr,0x1A,0x5C,0x1E);
        kotor_fastcall_00618a30(self,nullptr,feat,list,callback,creature);
        feat=kotor_fastcall_00616140(self,nullptr,0x4D,0x14,0x1F);
    }else{
        feat=kotor_fastcall_00616140(self,nullptr,0x51,0x13,8);
        kotor_fastcall_00618a30(self,nullptr,feat,list,callback,creature);
        feat=kotor_fastcall_00616140(self,nullptr,0x35,0x5B,0xB);
        kotor_fastcall_00618a30(self,nullptr,feat,list,callback,creature);
        feat=kotor_fastcall_00616140(self,nullptr,0x53,0x11,0x1C);
    }
    kotor_fastcall_00618a30(self,nullptr,feat,list,callback,creature);
}
