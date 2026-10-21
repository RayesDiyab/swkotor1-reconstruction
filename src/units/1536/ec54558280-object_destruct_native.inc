namespace {
extern "C" void __fastcall recovered_effect_destroy_00503590(void*,void*);
void __fastcall local_record_destroy(kotor_reconstruction::StartupString* self,void*){frame_LogStringDtor_005e5c20(self,nullptr);}
struct CleanupArrays final:recovered_object_cleanup_arrays::Ports{
 M m;U read(U a)override{return m.read(a);}void write(U a,U b)override{m.write(a,b);}void string_destroy(U a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}void release(U a)override{::live_free(ptr(a));}void records(U a,U n)override{c2da_destroy_array_006fa852(ptr(a),16,n,&local_record_destroy);}
};
struct ObjectDestruct final:recovered_object_destruct::Ports{
 M m;U read(U a)override{return m.read(a);}void write(U a,U b)override{m.write(a,b);}U table(U a)override{if(a==0x745f18)return word(object_table);if(a==0x7457a0)return word(&kotor_vtable_cgameobject_007457a0);required(a);}
 void virtual_destroy(U a,U flags)override{using F=void*(__thiscall*)(void*,U);(void)reinterpret_cast<F>(read(read(a)))(ptr(a),flags);}
 void destroy(U fn,U a)override{CleanupArrays p;switch(fn){case 0x4cfa50:recovered_object_cleanup_arrays::strings(a,p);return;case 0x59a980:recovered_object_cleanup_arrays::locals(a,p);return;case 0x5e5c20:frame_LogStringDtor_005e5c20(ptr(a),nullptr);return;case 0x5ea000:exo_locstring_destroy_005ea000(ptr(a),nullptr);return;case 0x5e9ab0:base_ini_reconstruction::destroy_list_005e9ab0(static_cast<base_ini_reconstruction::List*>(ptr(a)),nullptr);return;case 0x503590:recovered_effect_destroy_00503590(ptr(a),nullptr);return;case 0x59d9e0:body_0059d9e0(ptr(a),nullptr);return;case 0x4cc4d0:recovered_action_node_destroy_004cc4d0(ptr(a),nullptr);return;case 0x63d530:body_0063d530(ptr(a),nullptr);return;default:required(fn);}}
 void release(U a)override{::live_free(ptr(a));}U app()override{return word(kotor_menu_window_cells::manager_007a39fc);}U objects(U a)override{return read(read(a+4)+0x1005c);}void remove(U a,U id)override{U scratch=id;(void)recovered_object_array_remove_004d81a0(ptr(a),nullptr,id,reinterpret_cast<void**>(&scratch));}U pop(U a)override{return word(base_ini_reconstruction::remove_head_005e9c80(static_cast<base_ini_reconstruction::List*>(ptr(a)),nullptr));}
};
}
extern "C" void __fastcall recovered_object_destroy_004d0220(void*self,void*){ObjectDestruct p;recovered_object_destruct::destroy(word(self),p);}
extern "C" void __fastcall recovered_item_destruct_tables(void*self,void*){M m;m.write(word(self),word(item_table_00748da0));m.write(word(self)+0x10,word(item_table_00748cc0));}
extern "C" void __fastcall recovered_item_base_destroy_005b4680(void*self,void*){M m;m.write(word(self),word(item_table_0074b2a0));}
