// Native vtables retain original slot positions; accepted typed owners, no private substitutes.
extern "C" void* __fastcall case40_004c4f30(void*,void*,Word);
extern "C" void* __fastcall case40_004c8940(void*,void*,Word);
extern "C" void* __fastcall case40_004c6bc0(void*,void*,Word);
extern "C" void __fastcall slot_void_return_4_0060e760_a1(void*,void*,Word);
extern "C" void __fastcall slot_void_return_8_005b5e90_a2(void*,void*,Word,Word);
extern "C" unsigned __stdcall c2da_resource_constant_word_0063e7f0();
extern "C" void* __fastcall recovered_module_as_sws_module_004c6bb0(void*,void*);
const void* const module_initial_vtable[]={reinterpret_cast<void*>(&case40_004c4f30)};
const void* const module_primary_vtable[]={reinterpret_cast<void*>(&case40_004c8940)};
#define Z reinterpret_cast<void*>(&c2da_resource_constant_word_0063e7f0)
const void* const module_secondary_vtable[]={reinterpret_cast<void*>(&case40_004c6bc0),reinterpret_cast<void*>(&slot_void_return_4_0060e760_a1),reinterpret_cast<void*>(&slot_void_return_8_005b5e90_a2),Z,Z,Z,Z,Z,reinterpret_cast<void*>(&recovered_module_as_sws_module_004c6bb0),Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z,Z};
#undef Z
static_assert(sizeof(module_secondary_vtable)==28*4,"Original secondary module table has 28 slots");
const char* module_literal(Word a){switch(a){case 0x73d71c:return "";case 0x73d8f8:return "CURRENTGAME:";case 0x745d30:return "MODULE";default:required(a);}}
struct ModuleAllocator:ModuleAllocation {Word allocate(Word n)override{return word(::operator new(n));}};
struct NativeModuleConstructor:ModuleConstructorPorts {NativeMemory& m;explicit NativeModuleConstructor(NativeMemory& a):m(a){}
 Word vtable(Word a)override{switch(a){case 0x7458d4:return reinterpret_cast<Word>(module_initial_vtable);case 0x745960:return reinterpret_cast<Word>(module_primary_vtable);case 0x7458f0:return reinterpret_cast<Word>(module_secondary_vtable);default:required(a);}}
 void write_byte(Word a,Word v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}
 void list_00405ed0(Word a)override{(void)c2da_default_ref_00405ed0(ptr(a),nullptr);}
 Word list_insert_00406290(Word a,Word b,Word c)override{return word(unresolved_ref_assign_00406290(ptr(a),nullptr,ptr(b),module_literal(c)));}
 void object_004d7d60(Word a,Word b,Word c)override{(void)table29_required::gui_game_object_004d7d60(ptr(a),nullptr,b,c);}
 void table_004c4e20(Word a)override{ModuleAllocator p;(void)module_list_004c4e20(m,p,a);}
 void linked_005e9e10(Word a)override{(void)exo_locstring_construct_005e9e10(ptr(a),nullptr);}
 void empty_005b3190(Word a)override{m.write(a,0);m.write(a+4,0);}
 void location_0059afe0(Word a)override{(void)module_flags_0059afe0(m,a);}
 void strings_006fb55f(Word a,Word size,Word count,Word ctor,Word dtor)override{if(size!=8||ctor!=0x5b3190||dtor!=0x5e5c20)required(0x006fb55f);for(Word i=0;i<count;i++)empty_005b3190(a+i*size);}
 Word allocate_006fa7e6(Word a)override{return word(::operator new(a));}
 Word map_00578be0(Word a)override{return module_map_00578be0(m,a);}
 Word literal_005e5a90(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,module_literal(b));return a;}
 Word concat_005e5d10(Word a,Word b,Word c)override{return word(GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b)),static_cast<const kotor_reconstruction::StartupString*>(ptr(c))));}
 Word assign_005e5c50(Word a,Word b)override{return word(async_assign_string_005e5c50(ptr(a),nullptr,ptr(b)));}
 void destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void set_literal_005e5140(Word a,Word b)override{GuiAssign005e5140(ptr(a),nullptr,module_literal(b));}
 void set_name_004c4150(Word a,Word b)override{NativeResources p(m);alignas(8) unsigned char scratch[0x80];module_resources_004c4150(m,p,a,b,word(scratch));}
 void resref_00406d80(Word a,Word b)override{(void)static_cast<menu_resources::Ref*>(ptr(a))->construct_00406d80(module_literal(b));}
 void event_004c4cc0(Word a,Word b,Word c)override{NativeModuleReference p(m);alignas(8) unsigned char scratch[0x40];module_reference_004c4cc0(m,p,a,b,c,word(scratch));}
 Word array_004aed70(Word a)override{return m.read(m.read(a+4)+0x1005c);}
 void register_004d80e0(Word a,Word b,Word c,Word d)override{ModuleAllocator p;(void)object_register_004d80e0(m,p,a,b,c,d);}
 Word timer_004aede0(Word a)override{return word(kotor_fastcall_004aede0(ptr(a),nullptr));}
 void time_004ade40(Word a,Word b,Word c)override{frame_GetWorldTime_004ade40(ptr(a),nullptr,static_cast<unsigned*>(ptr(b)),static_cast<unsigned*>(ptr(c)));}
};
