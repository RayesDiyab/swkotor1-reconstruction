// 0886-gates (Claude): private copy; CVirtualMachine::RunScriptSituation 005D0FD0 RET 0Ch (W6 1194s) bound.
extern "C" int __fastcall recovered_vm_run_script_situation_005d0fd0(void*,void*,void*,unsigned,unsigned);
struct NativeModuleEvent:recovered_module_event::Ports,recovered_event_values::Ports{
 NativeMemory m;
 unsigned read(unsigned a)override{if(a==0x7a3a00)return frame_control::message_manager_007a3a00;return m.read(a);}
 unsigned read16(unsigned a)override{return m.read16(a);}void write(unsigned a,unsigned b)override{m.write(a,b);}void byte(unsigned a,unsigned b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 unsigned spill(unsigned bits)override{unsigned out;__asm{fld dword ptr bits}__asm{fstp dword ptr out}return out;}
 unsigned integer(unsigned a,unsigned i)override{return recovered_event_values::integer(a,i,*this);}unsigned object(unsigned a,unsigned i)override{return recovered_event_values::object(a,i,*this);}unsigned real_bits(unsigned a,unsigned i)override{return recovered_event_values::real(a,i,*this);}
 void saved_script(unsigned a,unsigned b,unsigned c,unsigned d)override{(void)recovered_vm_run_script_situation_005d0fd0(ptr(a),nullptr,ptr(b),c,d);}
 void script(unsigned vm,unsigned name,unsigned id,unsigned flag)override{frame_R4_005d0fc0(ptr(vm),nullptr,static_cast<unsigned*>(ptr(name)),id,flag);}
 unsigned objects(unsigned a)override{return read(read(a+4)+0x1005c);}
 void lookup(unsigned a,unsigned id,unsigned out)override{(void)kotor_fastcall_004d8230(ptr(a),nullptr,id,static_cast<void**>(ptr(out)));}
 unsigned creature(unsigned a)override{using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(read(read(a)+0x30))(ptr(a)));}
 void remove(unsigned a,unsigned id)override{recovered_object_array_erase_004d8290(ptr(a),nullptr,id);}
 void destroy(unsigned a)override{recovered_event_destroy_004d7590(ptr(a),nullptr);}void release(unsigned a)override{::live_free(ptr(a));}
};
extern "C" void __fastcall recovered_module_event_004c5120(void*self,void*,unsigned kind,unsigned source,void*data,unsigned day,unsigned tick){NativeModuleEvent p;alignas(4)unsigned char scratch[0x1c];recovered_module_event::dispatch(word(self),kind,source,word(data),day,tick,word(scratch),p);}
