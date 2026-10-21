#include "a45128e337-contracts.h"
namespace gui_wrap_draw {
void __fastcall text_draw_00416240(menu_storage::Text* self,void*,U32){
 void* font=PointerAt(self,0x14);
 if(!font)return;
 U32 argument=WordAt(self,0x40);
 if(self->bytes[0x50]&0x40u)argument=*bindings.pulse_0078d3d4;
 using Method=void(__thiscall*)(void*,U32);
 reinterpret_cast<Method>(PointerAt(PointerAt(font,0),0x7c))(font,argument);
}
}
