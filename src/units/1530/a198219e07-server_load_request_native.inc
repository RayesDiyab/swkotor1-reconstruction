// 0886-gates (Claude): private copy; SendServerToPlayerSaveLoad_Status 0056CB80 RET 0Ch (claude_0886_bodies_server).
extern "C" unsigned __fastcall kotor_fastcall_0056cb80(void*,void*,void*,unsigned,unsigned);
extern "C" {
unsigned __fastcall recovered_save_allowed_004b20e0(void*,void*,const void*);
void __fastcall recovered_estimate_004b5f90(void*,void*,const void*,unsigned);
unsigned __fastcall gui_load_resman_create_directory_004068c0(void*,void*,void*);
void __fastcall gui_load_resman_clean_directory_00409460(void*,void*,void*,unsigned,unsigned);
unsigned __fastcall gui_load_resman_free_disk_space_004067b0(void*,void*,void*,void*);
void* __fastcall kotor_fastcall_005e6890(void*,void*,void*,unsigned);
void* __fastcall frame_LogFileCtor_005e68d0(void*,void*,void*,unsigned,void*);
void __fastcall frame_LogFileDtor_005e69e0(void*,void*);
unsigned __fastcall frame_LogFileSize_005e6950(void*,void*);
}
struct NativeLoadRequest:LoadPorts{NativeMemory&m;NativeServerInfo text;explicit NativeLoadRequest(NativeMemory&v):m(v),text(v){}
using S=kotor_reconstruction::StartupString;
static S*str(Word a){return static_cast<S*>(ptr(a));}
Word module_004b14f0(Word a)override{return word(server_internal_get_module_004b14f0(ptr(a),nullptr));}
void unload_004b9240(Word a)override{(void)closeB_server_unload_module_004b9240(ptr(a),nullptr);/* closeB: claude_closeB_unload.cpp */}
void empty_005b3190(Word a)override{text.empty(a);}
const char*literal_text(Word a){switch(a){case 0x73d8f8:return "CURRENTGAME:";case 0x73d8f4:return "\\";case 0x74514c:return "GAMEINPROGRESS:";case 0x7454a0:return "NWMFILES:";case 0x73d90c:return "MODULES:";case 0x7453c0:return "LIVE%d";case 0x745494:return ":MODULES";case 0x73d5b0:return "rb";default:required(a);}}
void literal_005e5a90(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,literal_text(b));}
void set_literal_005e5140(Word a,Word b)override{c2da_assign_text_005e5140(ptr(a),nullptr,literal_text(b));}
void assign_005e5c50(Word a,Word b)override{text.assign(a,b);}void destroy_005e5c20(Word a)override{text.destroy(a);}
Word concat_005e5d10(Word a,Word b,Word c)override{return text.concat(a,b,c);}
Word right_005e6170(Word a,Word b,Word c)override{return word(base_ini_reconstruction::right_005e6170(str(a),nullptr,str(b),static_cast<int>(c)));}
Word find_005e5eb0(Word a,Word b,Word c)override{return static_cast<Word>(client_movies::StringFind_005e5eb0(static_cast<options_video::String*>(ptr(a)),nullptr,static_cast<options_video::String*>(ptr(b)),static_cast<int>(c)));}
Word length_005e5790(Word a)override{return async_string_length_005e5790(ptr(a),nullptr);}Word buffer_005e5670(Word a)override{return text.cstr(a);}
void format_005e5680(Word a,Word b,Word c)override{kotor_reconstruction::StartupFormat005e5680(str(a),literal_text(b),c);}
void timestamp_005e9410(Word a,Word b)override{recovered_debug_timestamp_005e9410(ptr(a),nullptr,ptr(b));}
Word directory_exists_004068c0(Word a,Word b)override{return gui_load_resman_create_directory_004068c0(ptr(a),nullptr,ptr(b));}
void create_directory_00409460(Word a,Word b,Word c,Word d)override{gui_load_resman_clean_directory_00409460(ptr(a),nullptr,ptr(b),c,d);}
Word include_save_004b20e0(Word a,Word b)override{return recovered_save_allowed_004b20e0(ptr(a),nullptr,ptr(b));}
Word party_004aee70(Word a)override{return word(lane_config_required::GetPartyTable_004aee70(ptr(a),nullptr));}
void add_game_005638d0(Word a)override{NativeMount p;unsigned scratch[2];party_mount_005638d0(m,p,a,word(scratch));}void remove_game_00563950(Word a)override{NativeMount p;unsigned scratch[2];party_unmount_00563950(m,p,a,word(scratch));}
void resref_00406d60(Word a,Word b)override{async_ref_construct_string_00406d60(ptr(a),nullptr,ptr(b));}
Word resource_exists_00408bc0(Word a,Word b,Word c,Word d)override{return static_cast<menu_resources::Manager*>(ptr(a))->exists_00408bc0(static_cast<menu_resources::Ref*>(ptr(b)),c,static_cast<unsigned*>(ptr(d)));}
Word resolve_005e68a0(Word a,Word b,Word c,Word d)override{return word(path_resolution::resolve_005e68a0(static_cast<alias_path::AliasListView*>(ptr(a)),nullptr,str(b),str(c),d));}
void estimate_004b5f90(Word a,Word b,Word c)override{recovered_estimate_004b5f90(ptr(a),nullptr,ptr(b),c);}
void add_directory_00408800(Word a,Word b)override{static_cast<menu_resources::Manager*>(ptr(a))->directory_00408800(str(b));}
void remove_directory_004088d0(Word a,Word b)override{kotor_fastcall_004088d0(ptr(a),nullptr,ptr(b));}
Word alias_005e6890(Word a,Word b,Word c)override{return word(kotor_fastcall_005e6890(ptr(a),nullptr,ptr(b),c));}
Word free_space_004067b0(Word a,Word b,Word c)override{return gui_load_resman_free_disk_space_004067b0(ptr(a),nullptr,ptr(b),ptr(c));}
Word allocate_006fa7e6(Word a)override{return word(::operator new(a));}void free_006fa390(Word a)override{::operator delete(ptr(a));}
Word file_005e68d0(Word a,Word b,Word c,Word d)override{return word(frame_LogFileCtor_005e68d0(ptr(a),nullptr,ptr(b),c,ptr(d)));}
Word file_size_005e6950(Word a)override{return frame_LogFileSize_005e6950(ptr(a),nullptr);}void file_destroy_005e69e0(Word a)override{frame_LogFileDtor_005e69e0(ptr(a),nullptr);}
void load_reply_0056cb80(Word a,Word b,Word c,Word d)override{(void)kotor_fastcall_0056cb80(ptr(a),nullptr,ptr(b),c,d);}Word messages_004aedb0(Word a)override{return m.read(m.read(a+4)+0x10010);}
void stall_0056c8b0(Word a,Word b)override{NativeModuleMessage p(m);unsigned scratch[2];message_stall_0056c8b0(m,p,a,b,word(scratch));}
};
