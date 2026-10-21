extern "C" {unsigned __fastcall recovered_loading_hint_005f4760(void*,void*,const void*,unsigned);void __fastcall kotor_m15_load_screen_set_hint_0067a9c0(void*,void*,unsigned);}
const char* resource_literal(Word a){switch(a){case 0x73d71c:return "";case 0x73d8f8:return "CURRENTGAME:";case 0x7458b8:return "HD0:players";case 0x7458ac:return "LIPS:%s_loc";case 0x745898:return "LIPS:localization";default:required(a);}}
using S=kotor_reconstruction::StartupString;
struct NativeLoading:LoadingPorts {
 void input_005ed310(Word a)override{kotor_fastcall_005ed310(ptr(a),nullptr);}
 Word timer_005e9250(Word a)override{return kotor_input_required::low_resolution_timer_005e9250(ptr(a),nullptr);}
 void literal_005e5a90(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,resource_literal(b));}
 Word choose_005f4760(Word a,Word b,Word c)override{return recovered_loading_hint_005f4760(ptr(a),nullptr,ptr(b),c);}
 void panel_0067a9c0(Word a,Word b)override{kotor_m15_load_screen_set_hint_0067a9c0(ptr(a),nullptr,b);}
 void destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void progress_00417fb0(Word a,Word b)override{kotor_original::kotor_fastcall_00417fb0(ptr(a),nullptr,static_cast<int>(b));}
 void empty_005b3190(Word a)override{std::memset(ptr(a),0,8);}
 void format_005e5680(Word a,Word b,Word c)override{if(b!=0x74ec0c)required(b);kotor_reconstruction::StartupFormat005e5680(static_cast<S*>(ptr(a)),"Load Bar = %d",static_cast<int>(c));}
};
struct NativeResources:ModuleResourcePorts {NativeMemory& m;explicit NativeResources(NativeMemory& v):m(v){}
 void write_byte(Word a,Word v)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}
 void empty_005b3190(Word a)override{m.write(a,0);m.write(a+4,0);}
 Word progress_part_005edff0(Word a,Word b)override{return m.read8(m.read(a+4)+0x3e4+b);}
 void notify_name_0063e7c0(Word a,Word b)override{packed_return_false_0063e7c0(ptr(a),nullptr,ptr(b));}
 void literal_005e5a90(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,resource_literal(b));}
 void add_004087a0(Word a,Word b,Word c)override{(void)static_cast<menu_resources::Manager*>(ptr(a))->encapsulated_004087a0(static_cast<S*>(ptr(b)),c);}
 void destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 void progress_005edd40(Word a,Word b,Word c)override{NativeLoading p;alignas(8) unsigned char scratch[16];loading_progress_005fb3f0(m,p,m.read(a+4),b,c,word(scratch));}
 void pump_00401c10(Word a,Word b,Word c)override{float f;std::memcpy(&f,&a,4);kotor_cdecl_00401c10(f,b,c);}
 Word find_005e5f20(Word a,Word b,Word c)override{return static_cast<Word>(::find_005e5f20(ptr(a),nullptr,static_cast<int>(b),static_cast<int>(c)));}
 Word concat_005e5d10(Word a,Word b,Word c)override{return word(GuiConcat005e5d10(static_cast<S*>(ptr(a)),nullptr,static_cast<S*>(ptr(b)),static_cast<const S*>(ptr(c))));}
 void assign_005e5c50(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 Word length_005e5790(Word a)override{return async_string_length_005e5790(ptr(a),nullptr);}
 Word right_005e6170(Word a,Word b,Word c)override{return word(base_ini_reconstruction::right_005e6170(static_cast<S*>(ptr(a)),nullptr,static_cast<S*>(ptr(b)),static_cast<int>(c)));}
 void prepare_00408800(Word a,Word b)override{(void)static_cast<menu_resources::Manager*>(ptr(a))->directory_00408800(static_cast<S*>(ptr(b)));}
 void resref_00406d60(Word a,Word b)override{(void)async_ref_construct_string_00406d60(static_cast<async_load::Ref*>(ptr(a)),nullptr,static_cast<async_load::String*>(ptr(b)));}
 Word exists_00408bc0(Word a,Word b,Word c,Word d)override{return static_cast<menu_resources::Manager*>(ptr(a))->exists_00408bc0(static_cast<menu_resources::Ref*>(ptr(b)),c,static_cast<unsigned*>(ptr(d)));}
 void finalize_004088d0(Word a,Word b)override{kotor_fastcall_004088d0(ptr(a),nullptr,ptr(b));}
 Word cstr_005e5670(Word a)override{return reinterpret_cast<Word>(WindowCStr005e5670(ptr(a),nullptr));}
 void format_005e5680(Word a,Word b,Word c)override{kotor_reconstruction::StartupFormat005e5680(static_cast<S*>(ptr(a)),resource_literal(b),static_cast<const char*>(ptr(c)));}
 void mount_004064f0(Word a,Word b,Word c,Word d)override{const volatile Word busy=m.read8(a+0x44);while(busy==1){}rim_enter_005e9300(ptr(a+0x40),nullptr);assign_005e5c50(a+0x4c,b);Word thread=m.read(a+0x38);m.write(a+0x54,c);m.write(a+0x58,d);write_byte(a+0x44,1);write_byte(a+0x5c,0);(void)ResumeThread(ptr(thread));rim_leave_005e9310(ptr(a+0x40),nullptr);}
 void sleep_0073d1e4(Word a)override{Sleep(a);}
 void result_005ee0a0(Word a,Word b)override{Word dest=m.read(a+4)+0x498;write_byte(dest,m.read8(dest)|b);}
};
