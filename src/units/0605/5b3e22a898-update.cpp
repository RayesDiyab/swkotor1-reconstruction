#include "6a3a57a963-composition.h"
#include <cstring>
namespace {
using U32=menu_continuation::U32;using I32=menu_continuation::I32;
U32 update_load(U32 p,U32 off){U32 v;std::memcpy(&v,reinterpret_cast<const void*>(p+off),4);return v;}
void update_store(U32 p,U32 off,U32 v){std::memcpy(reinterpret_cast<void*>(p+off),&v,4);}
I32 update_signed(U32 bits){I32 v;std::memcpy(&v,&bits,4);return v;}
void update_virtual(U32 panel,float delta){
 const U32 table=update_load(panel,0);
 reinterpret_cast<void(__thiscall*)(void*,float)>(update_load(table,0x38))(reinterpret_cast<void*>(panel),delta);
}
void update_delete(U32 panel,U32 table){
 reinterpret_cast<void*(__thiscall*)(void*,U32)>(update_load(table,0))(reinterpret_cast<void*>(panel),1u);
}
}
void __thiscall menu_continuation::GuiManager::Update_0040ce70(float delta){
 const U32 self=reinterpret_cast<U32>(this);
 const I32 first_count=update_signed(update_load(self,0x8c));
 for(I32 i=0;i<first_count;++i){
  const U32 panel=update_load(update_load(self,0x88),static_cast<U32>(i)*4u);
  if(!panel){frame_RemoveArray_006857a0(reinterpret_cast<void*>(self+0x88),nullptr,i);--i;continue;}
  if((update_load(panel,0x44)&4u)!=0 && frame_FindPopup_0040ab70(this,nullptr,reinterpret_cast<void*>(panel))==0xffffffffu)update_virtual(panel,delta);



 }
 const I32 popup_count=update_signed(update_load(self,0x98));
 for(I32 i=0;i<popup_count;++i){
  const U32 panel=update_load(update_load(self,0x94),static_cast<U32>(i)*4u);
  if(!panel){frame_RemoveArray_006857a0(reinterpret_cast<void*>(self+0x94),nullptr,i);--i;continue;}
  if((update_load(panel,0x44)&4u)!=0){



   update_virtual(panel,delta);
  }
 }
 // The original decrements a DWORD before testing its sign. INT_MIN wraps.
 for(I32 i=update_signed(update_load(self,0x8c)-1u);i>=0;--i){
  const U32 panel=update_load(update_load(self,0x88),static_cast<U32>(i)*4u),flags=update_load(panel,0x44);

  if((flags&0x100u)!=0)continue;

  const U32 action=(flags>>9)&3u;
  if(action==2u){



   frame_RemovePanel_0040c830(this,nullptr,reinterpret_cast<void*>(panel));

   const U32 table=update_load(panel,0);

   update_delete(panel,table);
  }else if(action!=0u){



   update_store(panel,0x44,flags&0xfffff8ffu);

   frame_RemovePanel_0040c830(this,nullptr,reinterpret_cast<void*>(panel));
  }
 }
}
