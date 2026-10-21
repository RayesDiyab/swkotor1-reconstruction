struct NativeDoorMeshCallbacks final:DoorMeshCallbackPorts {
 NativeMemory&m;explicit NativeDoorMeshCallbacks(NativeMemory&v):m(v){}
 void empty(Word a)override{m.write(a,0);m.write(a+4,0);}
 void ref_string(Word a,Word b)override{c2da_ref_to_string_00405f70(ptr(a),nullptr,ptr(b));}
 Word text(Word a)override{return reinterpret_cast<Word>(WindowCStr005e5670(ptr(a),nullptr));}
 void format(Word a,Word b,Word c)override{kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),"%s%d",static_cast<const char*>(ptr(b)),static_cast<int>(c));}
 void assign_string(Word a,Word b,Word c)override{async_ref_assign_string_004061f0(static_cast<async_load::Ref*>(ptr(a)),nullptr,static_cast<async_load::Ref*>(ptr(b)),static_cast<async_load::String*>(ptr(c)));}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 bool ordered_zero(Word value)override{const Word zero=0;unsigned short status;
 __asm{fld dword ptr value
 fcomp dword ptr zero
 fnstsw status}
 return (status&0x4400u)==0x4000u;
 }
};
void __fastcall door_mesh_reference_callback(void*self,void*,void*out){alignas(4) unsigned char scratch[0x20];NativeMemory m;NativeDoorMeshCallbacks p(m);door_mesh_reference_005ce8b0(m,p,reinterpret_cast<Word>(self),reinterpret_cast<Word>(out),reinterpret_cast<Word>(scratch));}
Word __fastcall door_mesh_binary_callback(void*self,void*,void*resource){alignas(4) unsigned char scratch[0x20];NativeMemory m;NativeDoorMeshCallbacks p(m);return door_mesh_binary_005ce770(m,p,reinterpret_cast<Word>(self),reinterpret_cast<Word>(resource),reinterpret_cast<Word>(scratch));}
