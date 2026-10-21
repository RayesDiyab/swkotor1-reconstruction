#include "813bcad8da-object_array_remove_native.inc"
extern "C" unsigned __fastcall kotor_fastcall_005eda50(void*,void*,unsigned);
extern "C" void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_module_remove_tag_004c5ef0(void*,void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_ai_remove_004af3d0(void*,void*,void*);
extern "C" void __fastcall recovered_item_destruct_tables(void*,void*);
extern "C" void __fastcall recovered_item_base_destroy_005b4680(void*,void*);
extern "C" void __fastcall recovered_object_destroy_004d0220(void*,void*);
struct NativeItemDestruct final:ItemDestructPorts{NativeMemory&m;explicit NativeItemDestruct(NativeMemory&v):m(v){}
 void tables(Word a)override{recovered_item_destruct_tables(ptr(a),nullptr);}Word client_id(Word a,Word b)override{return kotor_fastcall_005eda50(ptr(a),nullptr,b);}Word client_item(Word a,Word b)override{return word(kotor_m15_client_get_item_by_id_005ed5a0(ptr(a),nullptr,b));}Word client_objects(Word a)override{return m.read(m.read(a+4)+0x14);}Word remove_client_object(Word a,Word b,Word c)override{return recovered_object_array_remove_004d81a0(ptr(a),nullptr,b,static_cast<void**>(ptr(c)));}
 void destroy(Word a,Word b)override{using F=void*(__fastcall*)(void*,void*,unsigned);(void)reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),nullptr,b);}Word module(Word a)override{return word(server_internal_get_module_004b14f0(ptr(m.read(a+4)),nullptr));}void remove_tag(Word a,Word b,Word c)override{(void)recovered_module_remove_tag_004c5ef0(ptr(a),nullptr,ptr(b),c);}void release(Word a)override{::operator delete(ptr(a));}Word area(Word a)override{return word(recovered_object_area_004cb120(ptr(a),nullptr));}void detach(Word a)override{recovered_item_remove_area_005538a0(ptr(a),nullptr);}Word master(Word a)override{return m.read(m.read(a+4)+0x10044);}void remove_ai(Word a,Word b)override{(void)recovered_ai_remove_004af3d0(ptr(a),nullptr,ptr(b));}void localized_destroy(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}void object_destroy(Word a)override{recovered_object_destroy_004d0220(ptr(a),nullptr);}void base_destroy(Word a)override{recovered_item_base_destroy_005b4680(ptr(a),nullptr);}
};
extern "C" void __fastcall recovered_item_destroy_0055ec70(void*self,void*){NativeMemory m;NativeItemDestruct p(m);unsigned out;item_destruct_0055ec70(m,p,word(self),word(&out));}
