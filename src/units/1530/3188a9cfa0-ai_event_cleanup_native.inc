// 0886-gates (Claude): private copy; CVirtualMachine::DeleteScriptSituation 005D1110 RET 4 (W6 1194s) bound.
// eventcleanup (Claude): destroy() now binds every destructor 004B0AB0 dispatches to (jump table 004B0B34,
// index bytes 004B0B4C): kind 1 -> 005D1110 (no free); 8,0x13 -> 004AF150; 0x0A,0x18,0x1A -> 004D7590;
// 0x15 -> 004D4480 (004B0AD1); 0x16 -> 004D6A40 (004B0AE5); every other kind 2..0x19 -> 004B0B2D (no-op).
// Each destructor case is followed by free 006FA390 in recovered_ai_event_cleanup::cleanup.
extern "C" void __fastcall recovered_vm_delete_script_situation_005d1110(void*,void*,void*);
extern "C" void __fastcall recovered_attack_data_destruct_004d4480(void*,void*);   // creature_destructor_r2.obj
extern "C" void __fastcall recovered_message_data_destroy_004d6a40(void*,void*);   // w1_native_b7_0056cbf0.obj
// eventcleanup: kotor_reconstruction::StartupString is trivially destructible in this TU (its only member is
// BorrowedEquals005e5290), so ~StartupString() compiled to nothing and leaked every CExoString buffer. Both
// original sites name CExoString::~CExoString 005E5C20 (004D7611/004D7657 array element, 004AF153 jmp), bound here.
void __fastcall ai_event_string_destroy(kotor_reconstruction::StartupString*s,void*){frame_LogStringDtor_005e5c20(s,nullptr);}
struct NativeAIEventCleanup:recovered_ai_event_cleanup::Ports{
 NativeMemory m;
 unsigned read(unsigned a)override{return m.read(a);} // [007A3A00] CVirtualMachine* through NativeMemory
 void write(unsigned a,unsigned b)override{m.write(a,b);}
 void destroy(unsigned a,unsigned b)override{
  switch(a){
  case 0x4d7590:recovered_ai_event_cleanup::event_destroy(b,*this);return;                       // 004B0B1F CSWSEventData dtor
  case 0x4af150:frame_LogStringDtor_005e5c20(ptr(b+0x1c),nullptr);return;                       // 004B0AFB: ecx+0x1c; jmp 005E5C20
  case 0x4d4480:recovered_attack_data_destruct_004d4480(ptr(b),nullptr);return;                 // 004B0AD3 CSWSCombatAttackData dtor
  case 0x4d6a40:recovered_message_data_destroy_004d6a40(ptr(b),nullptr);return;                 // 004B0AE7 CSWCMessageData dtor
  }
  required(a); // unreachable: cleanup() passes only the four addresses above
 }
 void release(unsigned a)override{::live_free(ptr(a));}
 void script_release(unsigned a,unsigned b)override{recovered_vm_delete_script_situation_005d1110(ptr(a),nullptr,ptr(b));}
 void strings(unsigned a,unsigned n)override{c2da_destroy_array_006fa852(ptr(a),8,n,&ai_event_string_destroy);}
};
extern "C" void __fastcall recovered_ai_event_cleanup_004b0ab0(void*,void*,unsigned kind,void*payload){NativeAIEventCleanup p;recovered_ai_event_cleanup::cleanup(kind,word(payload),p);}
extern "C" void __fastcall recovered_event_destroy_004d7590(void*self,void*){NativeAIEventCleanup p;recovered_ai_event_cleanup::event_destroy(word(self),p);}
