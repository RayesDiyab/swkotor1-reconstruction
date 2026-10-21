extern "C" void __fastcall c2da_resource_base_destroy_00409d40(void*,void*);
extern "C" Word __fastcall recovered_layout_parse_005de900(void*,void*,void*);
void* __fastcall layout_delete_base(void*,void*,Word);
void* __fastcall layout_delete_derived(void*,void*,Word);
void* __fastcall layout_resource_delete(void*,void*,Word);
Word __fastcall layout_freed(void*a,void*){NativeMemory m;return layout_resource_freed_00710e60(m,word(a));}
Word __fastcall layout_serviced(void*a,void*){NativeMemory m;return layout_resource_serviced_005d4c60(m,word(a));}
const void* const layout_base_vtable[]={reinterpret_cast<void*>(&layout_delete_base)};
const void* const layout_derived_vtable[]={reinterpret_cast<void*>(&layout_delete_derived)};
const void* const layout_resource_vtable[]={reinterpret_cast<void*>(&layout_resource_delete),reinterpret_cast<void*>(&gff_constant),reinterpret_cast<void*>(&gff_zero),reinterpret_cast<void*>(&layout_freed),reinterpret_cast<void*>(&layout_serviced)};
struct NativeLayoutResource:LayoutResourcePorts{void base(Word a)override{(void)c2da_resource_base_ctor_00406460(ptr(a),nullptr);}Word vtable()override{return reinterpret_cast<Word>(layout_resource_vtable);}};
struct NativeLayoutReference:LayoutReferencePorts {NativeMemory&m;NativeModuleReference p;explicit NativeLayoutReference(NativeMemory&v):m(v),p(v){}
 Word same_004060b0(Word a,Word b)override{return p.same_004060b0(a,b);}Word nonempty_00406060(Word a,Word b)override{return p.nonempty_00406060(a,b);}void release_00409b00(Word a)override{p.release_00409b00(a);}Word remove_00409cf0(Word a,Word b)override{return p.remove_00409cf0(a,b);}void destroy_resource(Word a,Word b)override{p.destroy_resource(a,b);}Word empty_004060d0(Word a,Word b)override{return p.empty_004060d0(a,b);}void assign_literal_00406290(Word a,Word b,Word c)override{p.assign_literal_00406290(a,b,c);}Word lookup_004074d0(Word a,Word b,Word c)override{return p.lookup_004074d0(a,b,c);}Word allocate(Word a)override{return p.allocate(a);}Word construct_005df3a0(Word a)override{NativeLayoutResource q;return layout_resource_005df3a0(m,q,a);}void insert_00407680(Word a,Word b,Word c,Word d)override{p.insert_00407680(a,b,c,d);}void demand_00408620(Word a)override{p.demand_00408620(a);}void assign_00406120(Word a,Word b,Word c)override{p.assign_00406120(a,b,c);}
};
struct NativeLayoutLifecycle:LayoutLifecyclePorts{
 Word vtable(Word a)override{if(a==0x74d350)return reinterpret_cast<Word>(layout_base_vtable);if(a==0x74d354)return reinterpret_cast<Word>(layout_derived_vtable);required(a);}
 void ref_default(Word a)override{(void)c2da_default_ref_00405ed0(ptr(a),nullptr);}void assign_literal(Word a,Word b,Word c)override{(void)unresolved_ref_assign_00406290(ptr(a),nullptr,ptr(b),resource_literal(c));}void release(Word a)override{c2da_release_demand_00409b80(ptr(a),nullptr);}void free(Word a)override{::operator delete(ptr(a));}void string_destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}void empty(Word a)override{NativeMemory m;m.write(a,0);m.write(a+4,0);}void ref_string(Word a,Word b)override{c2da_ref_to_string_00405f70(ptr(a),nullptr,ptr(b));}Word parse(Word a,Word b)override{return recovered_layout_parse_005de900(ptr(a),nullptr,ptr(b));}
};
void* __fastcall layout_delete_base(void*a,void*,Word flags){NativeMemory m;NativeLayoutReference r(m);NativeLayoutLifecycle p;layout_base_destroy_005de590(m,r,p,word(a));if(flags&1)::operator delete(a);return a;}
void* __fastcall layout_delete_derived(void*a,void*,Word flags){NativeMemory m;NativeLayoutReference r(m);NativeLayoutLifecycle p;layout_destroy_005de7e0(m,r,p,word(a));if(flags&1)::operator delete(a);return a;}
void* __fastcall layout_resource_delete(void*a,void*,Word flags){NativeMemory m;m.write(word(a),reinterpret_cast<Word>(layout_resource_vtable));c2da_resource_base_destroy_00409d40(a,nullptr);if(flags&1)::operator delete(a);return a;}
extern "C" void __fastcall recovered_layout_reference_005de5f0(void*a,void*,void*b,Word demand){NativeMemory m;NativeLayoutReference p(m);alignas(8) unsigned char scratch[0x40];layout_reference_005de5f0(m,p,word(a),word(b),demand,word(scratch));}
