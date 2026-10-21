void* __fastcall gff_destroy_required(void* a,void*,Word b){return gff_resource_deleting_destroy_00413010(a,nullptr,b);}
void* __fastcall ifo_destroy_required(void* a,void*,Word b){return gff_resource_deleting_destroy_00413010(a,nullptr,b);}
Word __fastcall gff_constant(void*,void*){return c2da_resource_constant_word_00406490();}
Word __fastcall gff_zero(void*,void*){return 0;}
const void* const gff_vtable[]={reinterpret_cast<void*>(&gff_destroy_required),reinterpret_cast<void*>(&gff_constant),reinterpret_cast<void*>(&gff_zero),reinterpret_cast<void*>(&gff_resource_freed_00410880),reinterpret_cast<void*>(&gff_resource_serviced_00410740)};
const void* const ifo_vtable[]={reinterpret_cast<void*>(&ifo_destroy_required),reinterpret_cast<void*>(&gff_constant),reinterpret_cast<void*>(&gff_zero),reinterpret_cast<void*>(&gff_resource_freed_00410880),reinterpret_cast<void*>(&gff_resource_serviced_00410740)};
struct NativeModuleGff:ModuleGffPorts {
 void base_00406460(Word a)override{(void)c2da_resource_base_ctor_00406460(ptr(a),nullptr);}
 Word vtable(Word a)override{switch(a){case 0x73e2d0:return reinterpret_cast<Word>(gff_vtable);case 0x745810:return reinterpret_cast<Word>(ifo_vtable);default:required(a);}}
 void write_byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
};
struct NativeModuleReference:ModuleReferencePorts {NativeMemory& m;explicit NativeModuleReference(NativeMemory& v):m(v){}
 Word same_004060b0(Word a,Word b)override{return resource_equal_004060b0(ptr(a),nullptr,ptr(b));}
 Word nonempty_00406060(Word a,Word b)override{return c2da_not_equal_text_00406060(ptr(a),nullptr,resource_literal(b));}
 void release_00409b00(Word a)override{c2da_cancel_00409b00(ptr(a),nullptr);}
 Word remove_00409cf0(Word a,Word b)override{return c2da_release_00409cf0(ptr(a),nullptr,ptr(b));}
 void destroy_resource(Word a,Word b)override{using F=void*(__thiscall*)(void*,Word);reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),b);}
 Word empty_004060d0(Word a,Word b)override{return c2da_equal_text_004060d0(ptr(a),nullptr,resource_literal(b));}
 void assign_literal_00406290(Word a,Word b,Word c)override{(void)unresolved_ref_assign_00406290(ptr(a),nullptr,ptr(b),resource_literal(c));}
 Word lookup_004074d0(Word a,Word b,Word c)override{return word(c2da_lookup_004074d0(ptr(a),nullptr,ptr(b),c));}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word construct_004c30c0(Word a)override{NativeModuleGff p;return module_gff_004c30c0(m,p,a);}
 void insert_00407680(Word a,Word b,Word c,Word d)override{c2da_register_00407680(ptr(a),nullptr,ptr(b),c,ptr(d));}
 void demand_00408620(Word a)override{resource_request_00408620(ptr(a),nullptr);}
 void assign_00406120(Word a,Word b,Word c)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a)),nullptr,static_cast<menu_resources::Ref*>(ptr(b)),static_cast<const menu_resources::Ref*>(ptr(c)));}
};
