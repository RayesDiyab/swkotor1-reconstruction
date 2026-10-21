// 0886-gates (Claude): private copy; module load / finish ports bound to real bodies (RET checked).
extern "C" {
void* __fastcall recovered_server_creature_construct_004f7a10(void*,void*,unsigned,unsigned);   // 004F7A10 CSWSCreature::CSWSCreature RET 8 (0963)
void __fastcall w1x_object_set_orientation_004cd170(void*,void*,void*);                         // 004CD170 CSWSObject::SetOrientation RET 4 (this unit)
void __fastcall kotor_fastcall_004c83b0(void*,void*,unsigned);                                  // 004C83B0 CSWSModule::AddObjectToLimbo RET 4 (claude_0886_bodies_server)
void __fastcall case40_004c44d0(void*,void*,kotor_reconstruction::StartupString*);              // 004C44D0 CSWSModule::RemoveModuleResources RET 4 (this unit)
}
const char* load_literal(Word a){switch(a){
case 0x73d71c:return "";
case 0x73d8f8:return "CURRENTGAME:";
case 0x744c24:return "ObjectId";
case 0x745320:return "Mod_PlayerList";
case 0x745840:return "RepList";
case 0x745848:return "FactionList";
case 0x745854:return "FAC ";
case 0x745874:return "Mod_IsPrimaryPlr";
case 0x745888:return "Mod_CommntyName";
case 0x7458dc:return "Creature List";
case 0x745964:return "Mod_TokensValue";
case 0x745974:return "Mod_TokensNumber";
case 0x745988:return "Mod_Tokens";
case 0x745994:return "Area_Name";
case 0x7459a0:return "Mod_Area_list";
case 0x7459b0:return "Mod_OnPlrLvlUp";
case 0x7459c0:return "Mod_OnPlrRest";
case 0x7459d0:return "Mod_OnSpawnBtnDn";
case 0x7459e4:return "Mod_OnPlrDying";
case 0x7459f4:return "Mod_OnPlrDeath";
case 0x745a04:return "Mod_OnModStart";
case 0x745a14:return "Mod_OnModLoad";
case 0x745a24:return "Mod_OnUnAqreItem";
case 0x745a38:return "Mod_OnAcquirItem";
case 0x745a4c:return "Mod_OnActvtItem";
case 0x745a5c:return "Mod_OnClientLeav";
case 0x745a70:return "Mod_OnClientEntr";
case 0x745a84:return "Mod_OnUsrDefined";
case 0x745a98:return "Mod_OnHeartbeat";
case 0x745aa8:return "CutScene_ID";
case 0x745ab4:return "CutScene_Name";
case 0x745ac4:return "Mod_CutSceneList";
case 0x745ad8:return "Expansion_ID";
case 0x745ae8:return "Expansion_Name";
case 0x745af8:return "Mod_Expan_List";
case 0x745b08:return "Mod_XPScale";
case 0x745b14:return "Mod_PauseTime";
case 0x745b24:return "Mod_PauseDay";
case 0x745b34:return "Mod_StartMiliSec";
case 0x745b48:return "Mod_StartSecond";
case 0x745b58:return "Mod_StartMinute";
case 0x745b68:return "Mod_Transition";
case 0x745b78:return "Mod_StartHour";
case 0x745b88:return "Mod_StartDay";
case 0x745b98:return "Mod_StartMonth";
case 0x745ba8:return "Mod_StartYear";
case 0x745bb8:return "Mod_DuskHour";
case 0x745bc8:return "Mod_DawnHour";
case 0x745bd8:return "Mod_MinPerHour";
case 0x745be8:return "Mod_Entry_Dir_Y";
case 0x745bf8:return "Mod_Entry_Dir_X";
case 0x745c08:return "Mod_Entry_Z";
case 0x745c14:return "Mod_Entry_Y";
case 0x745c20:return "Mod_Entry_X";
case 0x745c2c:return "Mod_Entry_Area";
case 0x745c3c:return "Mod_Tag";
case 0x745c44:return "Mod_NextObjId1";
case 0x745c54:return "Mod_NextObjId0";
case 0x745c64:return "Mod_NextCharId1";
case 0x745c74:return "Mod_NextCharId0";
case 0x745c84:return "Mod_Effect_NxtId";
case 0x745ca0:return "Mod_NWMResName";
case 0x745cb0:return "Mod_IsNWMFile";
case 0x745cc0:return "Mod_IsSaveGame";
case 0x745cd0:return "Mod_Description";
case 0x745ce0:return "Mod_Name";
case 0x745cec:return "Mod_Version";
case 0x745cf8:return "Mod_Creator_ID";
case 0x745d08:return "Mod_ID";
case 0x745d10:return "Mod_LastName";
case 0x745d20:return "Mod_FirstName";
case 0x745d38:return "XOrientation";
case 0x745d48:return "YOrientation";
case 0x745d58:return "ZOrientation";
case 0x745d68:return "ZPosition";
case 0x745d74:return "YPosition";
case 0x745d80:return "XPosition";
case 0x745d8c:return "REPUTE";
case 0x745d94:return "Mod_OnEquipItem";
case 0x745da4:return "Mod_StartMovie";
case 0x74699c:return "Y";
case 0x7476b0:return "Destination";
case 0x7476bc:return "Path_Conections";
case 0x7476cc:return "First_Conection";
case 0x7476dc:return "Conections";
case 0x7476e8:return "X";
case 0x7476ec:return "Path_Points";
case 0x7476f8:return "PTH ";
case 0x747b50:return "WeatherStarted";
case 0x747b60:return "CurrentWeather";
case 0x747b70:return "GIT ";
case 0x747c18:return "UseTemplates";
case 0x7485c8:return "insane";
case 0x7485d0:return "neutral";
case 0x7485d8:return "friendly_2";
case 0x7485e4:return "hostile_2";
case 0x7485f0:return "friendly_1";
case 0x7485fc:return "hostile_1";
case 0x748608:return "player";
case 0x748610:return "LABEL";
case 0x748618:return "FactionGlobal";
case 0x748628:return "FactionParentID";
case 0x748638:return "FactionName";
case 0x748644:return "FactionRep";
case 0x748650:return "FactionID2";
case 0x74865c:return "FactionID1";
default:required(a);}}
struct NativeModuleHelpers:ModuleHelperPorts {NativeMemory& m;explicit NativeModuleHelpers(NativeMemory& v):m(v){}
 void assign_005e5c50(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 Word equals_005e5290(Word a,Word b)override{const char* label=nullptr;switch(b){case 0x748778:label="ebo_m12aa";break;case 0x74876c:label="tar_m02af";break;case 0x748760:label="ebo_m40ad";break;default:required(b);}return static_cast<kotor_reconstruction::StartupString*>(ptr(a))->BorrowedEquals005e5290(label);}
 void write_byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 Word lower_005e6080(Word a,Word b)override{return word(directory_list::LowerCase005e6080(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b))));}
 void destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word lookup_004076e0(Word a,Word b)override{return word(c2da_manager_004076e0(ptr(a),nullptr,ptr(b)));}
 Word resource_available_00406720(Word,Word b)override{return resource_type_00406720(b);}
};
struct NativeGffExtra:GffExtraPorts {NativeMemory& m;explicit NativeGffExtra(NativeMemory& v):m(v){}
 Word label_00411630(Word a,Word b,Word c)override{return gff_label_00411630(ptr(a),nullptr,ptr(b),static_cast<const char*>(ptr(c)));}
 Word field_00410990(Word a,Word b,Word c)override{return word(static_cast<kotor_q05::CResGFF*>(ptr(a))->GetField(static_cast<kotor_q05::StructHandle*>(ptr(b)),c));}
 Word data_00410a20(Word a,Word b,Word c)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetDataField(static_cast<kotor_q05::FieldRecord*>(ptr(b)),static_cast<unsigned*>(ptr(c)));}
 void write_byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
};
struct NativeLocalized:LocalizedPorts {NativeMemory& m;explicit NativeLocalized(NativeMemory& v):m(v){}
 Word allocate(Word a)override{return word(::operator new(a));}
 void empty_005b3190(Word a)override{m.write(a,0);m.write(a+4,0);}
 void string_assign_005e5c50(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 void clear_005ecb10(Word a)override{exo_locinternal_clear_005ecb10(ptr(a),nullptr);}
 Word internal_construct_005ecaf0(Word a)override{return word(exo_locinternal_construct_005ecaf0(ptr(a),nullptr));}
 void append_005e9dc0(Word a,Word b)override{(void)resource_append_005e9dc0(ptr(a),nullptr,ptr(b));}
 Word next_005e9b90(Word a,Word b)override{return unresolved_list_next_005e9b90(ptr(a),nullptr,static_cast<void**>(ptr(b)));}
 void loc_construct_005e9e10(Word a)override{(void)exo_locstring_construct_005e9e10(ptr(a),nullptr);}
 void loc_destroy_005ea000(Word a)override{exo_locstring_destroy_005ea000(ptr(a),nullptr);}
 Word label_00411630(Word a,Word b,Word c)override{return gff_label_00411630(ptr(a),nullptr,ptr(b),static_cast<const char*>(ptr(c)));}
 Word field_00410990(Word a,Word b,Word c)override{return word(static_cast<kotor_q05::CResGFF*>(ptr(a))->GetField(static_cast<kotor_q05::StructHandle*>(ptr(b)),c));}
 Word data_00410a20(Word a,Word b,Word c)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetDataField(static_cast<kotor_q05::FieldRecord*>(ptr(b)),static_cast<unsigned*>(ptr(c)));}
 Word string_bytes_005e5b70(Word a,Word b,Word c)override{return word(resman_string_counted_005e5b70(ptr(a),nullptr,static_cast<const char*>(ptr(b)),c));}
 void loc_add_005ea340(Word a,Word b,Word c,Word d)override{(void)exo_locstring_add_005ea340(ptr(a),nullptr,b,ptr(c),static_cast<int>(d));}
 void string_destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
};
struct NativeModuleDate:ModuleDatePorts {void write_byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}void update_004adbd0(Word a)override{frame_R4_004adbd0(ptr(a),nullptr);}};
struct NativeMount:ModuleMountPorts {
 void literal_005e5a90(Word a,Word b)override{if(b!=0x74514c)required(b);frame_LogStringCtor_005e5a90(ptr(a),nullptr,"GAMEINPROGRESS:");}
 void directory_00408800(Word a,Word b)override{(void)static_cast<menu_resources::Manager*>(ptr(a))->directory_00408800(static_cast<kotor_reconstruction::StartupString*>(ptr(b)));}
 void release_004088d0(Word a,Word b)override{kotor_fastcall_004088d0(ptr(a),nullptr,ptr(b));}
 void destroy_005e5c20(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word lookup_004071a0(Word a,Word b,Word c,Word d,Word e,Word f,Word g)override{return unresolved_key_entry_from_table_004071a0(ptr(a),nullptr,ptr(b),ptr(c),d,ptr(e),ptr(f),g);}
};
struct NativeFinish:ModuleFinishPorts {NativeMemory&m;explicit NativeFinish(NativeMemory&v):m(v){}
 Word list_004118c0(Word a,Word b,Word c,Word d)override{return gff_list_004118c0(ptr(a),nullptr,ptr(b),ptr(c),load_literal(d));}
 Word count_00411940(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 Word element_00411990(Word a,Word b,Word c,Word d)override{return gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word type_004111c0(Word a,Word b)override{return static_cast<kotor_q05::CResGFF*>(ptr(a))->GetElementType(static_cast<kotor_q05::StructHandle*>(ptr(b)));}
 Word word_00411c20(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 Word allocate(Word a)override{return word(::operator new(a));}
 Word creature_004f7a10(Word a,Word b,Word c)override{return word(recovered_server_creature_construct_004f7a10(ptr(a),nullptr,b,c));}
 Word load_00500350(Word a,Word b,Word c,Word d)override{return kotor_fastcall_00500350(ptr(a),nullptr,ptr(b),ptr(c),d);/* closeB */}
 void destroy(Word a,Word b)override{using F=void*(__thiscall*)(void*,Word);reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),b);}
 Word float_bits_00411d00(Word a,Word b,Word c,Word d,Word e)override{float fallback;std::memcpy(&fallback,&e,4);float value=gui_load_gff_float_00411d00(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),fallback);Word bits;std::memcpy(&bits,&value,4);return bits;}
 void position_004cd170(Word a,Word b)override{w1x_object_set_orientation_004cd170(ptr(a),nullptr,ptr(b));}
 void attach_004c83b0(Word a,Word b)override{kotor_fastcall_004c83b0(ptr(a),nullptr,b);}
};
struct NativeFaction:FactionPorts {NativeMemory&m;explicit NativeFaction(NativeMemory&v):m(v){}
 void empty(Word a)override{m.write(a,0);m.write(a+4,0);}
 void literal(Word a,Word b)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,load_literal(b));}
 void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
 Word count(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 void element(Word a,Word b,Word c,Word d)override{(void)gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word string(Word a,Word b,Word c,Word d,Word e,Word f)override{return word(gui_load_gff_string_00411ec0(ptr(a),nullptr,ptr(b),ptr(c),load_literal(d),static_cast<unsigned*>(ptr(e)),ptr(f)));}
 Word dword(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),e);}
 Word short_value(Word a,Word b,Word c,Word d,Word e)override{NativeGffExtra p(m);return gff_short_00411b40(m,p,a,b,reinterpret_cast<Word>(load_literal(c)),d,e);}
 Word allocate(Word a)override{return word(::operator new(a));}
 void grow_005e03c0(Word a,Word b)override{menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,b);}
 Word compare_005e58c0(Word a,Word b)override{return ini_comparison::CompareNC005e58c0(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,load_literal(b));}
 void table_string_00413270(Word a,Word b,Word c,Word d)override{(void)c2da_current_get_string_00413270(ptr(a),nullptr,static_cast<int>(b),ptr(c),ptr(d));}
};
struct NativeReputation:ReputationPorts {
 Word allocate(Word a)override{return word(::operator new(a));}
 void free(Word a)override{::operator delete(ptr(a));}
 Word table_int_00413510(Word a,Word b,Word c,Word d)override{return c2da_current_get_int_entry_00413510(ptr(a),nullptr,static_cast<int>(b),static_cast<int>(c),static_cast<int*>(ptr(d)));}
 Word count_00411940(Word a,Word b)override{return gff_list_count_00411940(ptr(a),nullptr,ptr(b));}
 void element_00411990(Word a,Word b,Word c,Word d)override{(void)gff_list_element_00411990(ptr(a),nullptr,ptr(b),ptr(c),d);}
 Word dword_00411c20(Word a,Word b,Word c,Word d,Word e)override{return gui_load_gff_dword_00411c20(ptr(a),nullptr,ptr(b),load_literal(c),static_cast<unsigned*>(ptr(d)),e);}
};
struct NativeModuleMessage:ModuleMessagePorts {NativeMemory&m;explicit NativeModuleMessage(NativeMemory&v):m(v){}
 Word allocate(Word a)override{return word(::operator new(a));}
 void free(Word a)override{::operator delete(ptr(a));}
 void write_byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}
 Word network_getter(Word a)override{using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(m.read(m.read(a)+0x10))(ptr(a)));}
 Word send_005d4cb0(Word a,Word b,Word c,Word d,Word e)override{return static_cast<Word>(net_layer_send_message_to_player_005d4cb0(ptr(a),nullptr,b,static_cast<const char*>(ptr(c)),d,e));}
};
struct NativeModuleLoad:ModuleLoadPorts {NativeMemory& m;explicit NativeModuleLoad(NativeMemory& v):m(v){}
 void empty_005b3190(Word a0)override{m.write(a0,0);m.write(a0+4,0);}
 void manager_begin_004070a0(Word a0)override{c2da_manager_004070a0(ptr(a0),nullptr);}
 Word objects_004aed70(Word a0)override{return m.read(m.read(a0+4)+0x1005c);}
 Word literal_005e5a90(Word a0,Word a1)override{frame_LogStringCtor_005e5a90(ptr(a0),nullptr,load_literal(a1));return a0;}
 Word concat_005e5d10(Word a0,Word a1,Word a2)override{return word(GuiConcat005e5d10(static_cast<S*>(ptr(a0)),nullptr,static_cast<S*>(ptr(a1)),static_cast<const S*>(ptr(a2))));}
 void assign_005e5c50(Word a0,Word a1)override{(void)async_assign_string_005e5c50(ptr(a0),nullptr,ptr(a1));}
 void destroy_005e5c20(Word a0)override{frame_LogStringDtor_005e5c20(ptr(a0),nullptr);}
 Word compare_005e5310(Word a0,Word a1)override{return !async_string_equal_005e5200(ptr(a0),nullptr,ptr(a1));}
 Word length_005e5790(Word a0)override{return async_string_length_005e5790(ptr(a0),nullptr);}
 void release_path_004c44d0(Word a,Word b)override{case40_004c44d0(ptr(a),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b)));}
 void setup_004c4150(Word a0,Word a1)override{NativeResources p(m);alignas(8) unsigned char scratch[0x80];module_resources_004c4150(m,p,a0,a1,word(scratch));}
 Word resource_ready_00409b10(Word a0)override{return c2da_current_resource_demand_00409b10(ptr(a0),nullptr);}
 void root_00411240(Word a0,Word a1)override{(void)static_cast<kotor_q05::CResGFF*>(ptr(a0))->GetTopLevelStruct(static_cast<kotor_q05::StructHandle*>(ptr(a1)));}
 Word byte_00411a60(Word a0,Word a1,Word a2,Word a3,Word a4)override{return gui_load_gff_byte_00411a60(ptr(a0),nullptr,ptr(a1),load_literal(a2),static_cast<unsigned*>(ptr(a3)),static_cast<unsigned char>(a4));}
 Word string_00411ec0(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5)override{return word(gui_load_gff_string_00411ec0(ptr(a0),nullptr,ptr(a1),ptr(a2),load_literal(a3),static_cast<unsigned*>(ptr(a4)),ptr(a5)));}
 void assign_string_005e5c50(Word a0,Word a1)override{(void)async_assign_string_005e5c50(ptr(a0),nullptr,ptr(a1));}
 void destroy_string_005e5c20(Word a0)override{frame_LogStringDtor_005e5c20(ptr(a0),nullptr);}
 Word resource_path_00407780(Word a0,Word a1,Word a2)override{NativeModuleHelpers p(m);return kotor::server_mainloop::resource_path_00407780(m,p,a0,a1,a2);}
 void data_00412380(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5,Word a6)override{NativeGffExtra p(m);Word size;(void)gff_data_00412380(m,p,a0,a1,a2,a3,reinterpret_cast<Word>(load_literal(a4)),a5,a6,word(&size));}
 Word signed_00411c90(Word a0,Word a1,Word a2,Word a3,Word a4)override{return static_cast<Word>(gui_load_gff_int_00411c90(ptr(a0),nullptr,ptr(a1),load_literal(a2),static_cast<unsigned*>(ptr(a3)),static_cast<int>(a4)));}
 Word word_00411c20(Word a0,Word a1,Word a2,Word a3,Word a4)override{return gui_load_gff_dword_00411c20(ptr(a0),nullptr,ptr(a1),load_literal(a2),static_cast<unsigned*>(ptr(a3)),a4);}
 Word module_id_004aec90(Word a0)override{return m.read(m.read(a0+4)+0x10004);}
 void update_id_0054eaa0(Word a0,Word a1)override{NativeModuleHelpers p(m);module_identifier_0054eaa0(m,p,a0,a1);}
 Word loc_empty_005e9e10(Word a0)override{return word(exo_locstring_construct_005e9e10(ptr(a0),nullptr));}
 Word loc_read_00411fd0(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5)override{NativeLocalized p(m);unsigned scratch[6];return gff_localized_00411fd0(m,p,a0,a1,a2,reinterpret_cast<Word>(load_literal(a3)),a4,a5,word(scratch));}
 void loc_assign_005e9e70(Word a0,Word a1)override{NativeLocalized p(m);unsigned scratch; (void)localized_assign_005e9e70(m,p,a0,a1,word(&scratch));}
 void loc_destroy_005ea000(Word a0)override{exo_locstring_destroy_005ea000(ptr(a0),nullptr);}
 void ref_literal_00406d80(Word a0,Word a1)override{(void)static_cast<menu_resources::Ref*>(ptr(a0))->construct_00406d80(load_literal(a1));}
 Word ref_read_00411e10(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5)override{return word(gui_load_gff_resref_00411e10(ptr(a0),nullptr,ptr(a1),ptr(a2),load_literal(a3),static_cast<unsigned*>(ptr(a4)),ptr(a5)));}
 void ref_assign_00406120(Word a0,Word a1,Word a2)override{(void)resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(ptr(a0)),nullptr,static_cast<menu_resources::Ref*>(ptr(a1)),static_cast<const menu_resources::Ref*>(ptr(a2)));}
 void set_tag_004c3060(Word a0,Word a1)override{NativeModuleHelpers p(m);unsigned scratch[2];module_tag_004c3060(p,a0,a1,word(scratch));}
 Word float_bits_00411d00(Word a0,Word a1,Word a2,Word a3,Word a4)override{float fallback;std::memcpy(&fallback,&a4,4);float result=gui_load_gff_float_00411d00(ptr(a0),nullptr,ptr(a1),load_literal(a2),static_cast<unsigned*>(ptr(a3)),fallback);Word bits;std::memcpy(&bits,&result,4);return bits;}
 void write_byte(Word a0,Word a1)override{*static_cast<unsigned char*>(ptr(a0))=static_cast<unsigned char>(a1);}
 Word short_00411b40(Word a0,Word a1,Word a2,Word a3,Word a4)override{NativeGffExtra p(m);return gff_short_00411b40(m,p,a0,a1,reinterpret_cast<Word>(load_literal(a2)),a3,a4);}
 void date_004aebd0(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5,Word a6,Word a7)override{NativeModuleDate p;server_date_004b2320(m,p,m.read(a0+4),a1,a2,a3,a4,a5,a6,a7);}
 Word tick_004af0d0(Word a0)override{return m.read(m.read(a0+4)+0x100ac);}
 Word day_004af0b0(Word a0)override{return m.read(m.read(a0+4)+0x100b0);}
 Word timer_004aede0(Word a0)override{return m.read(m.read(a0+4)+0x10048);}
 Word timer_004aedf0(Word a0)override{return m.read(m.read(a0+4)+0x1004c);}
 Word timer_004aee00(Word a0)override{return m.read(m.read(a0+4)+0x10050);}
 void timer_scale_004adba0(Word a0,Word a1)override{NativeModuleHelpers p(m);kotor::server_mainloop::timer_scale_004adba0(m,p,a0,a1);}
 void timer_date_004ae3e0(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5,Word a6,Word a7,Word a8)override{NativeModuleDate p;kotor::server_mainloop::timer_date_004ae3e0(m,p,a0,a1,a2,a3,a4,a5,a6,a7,a8);}
 void timer_copy_004adef0(Word a0,Word a1)override{kotor_fastcall_004adef0(ptr(a0),nullptr,ptr(a1));}
 void timer_fields_004ade40(Word a0,Word a1,Word a2)override{frame_GetWorldTime_004ade40(ptr(a0),nullptr,static_cast<unsigned*>(ptr(a1)),static_cast<unsigned*>(ptr(a2)));}
 Word list_004118c0(Word a0,Word a1,Word a2,Word a3)override{return gff_list_004118c0(ptr(a0),nullptr,ptr(a1),ptr(a2),load_literal(a3));}
 Word count_00411940(Word a0,Word a1)override{return gff_list_count_00411940(ptr(a0),nullptr,ptr(a1));}
 void element_00411990(Word a0,Word a1,Word a2,Word a3)override{(void)gff_list_element_00411990(ptr(a0),nullptr,ptr(a1),ptr(a2),a3);}
 Word allocate(Word a0)override{return word(::operator new(a0));}
 void ref_empty_00405ed0(Word a0)override{(void)c2da_default_ref_00405ed0(ptr(a0),nullptr);}
 void append_005e9dc0(Word a0,Word a1)override{(void)resource_append_005e9dc0(ptr(a0),nullptr,ptr(a1));}
 void resource_release_00409b80(Word a0)override{c2da_release_demand_00409b80(ptr(a0),nullptr);}
 void ref_string_00405f70(Word a0,Word a1)override{c2da_ref_to_string_00405f70(ptr(a0),nullptr,ptr(a1));}
 Wide wide_00411d70(Word a0,Word a1,Word a2,Word a3,Wide a4)override{NativeGffExtra p(m);Word size;return gff_wide_00411d70(m,p,a0,a1,reinterpret_cast<Word>(load_literal(a2)),a3,a4,word(&size));}
 void talk_0041db50(Word a0,Word a1,Word a2)override{kotor_fastcall_0041db50(ptr(a0),nullptr,a1,ptr(a2));}
 void variables_0059aa80(Word a0,Word a1,Word a2)override{native_local_variables(a0,a1,a2);}
 void effects_0059b0f0(Word a0,Word a1,Word a2)override{native_builtin_variables(a0,a1,a2);}
 Word messages_004aed80(Word a0)override{return m.read(m.read(a0+4)+0x10044);}
 void messages_load_004b0a00(Word a0,Word a1,Word a2)override{(void)kotor_fastcall_004b0a00(ptr(a0),nullptr,ptr(a1),ptr(a2));/* loadgame */}
 Word party_004aee70(Word a0)override{return word(lane_config_required::GetPartyTable_004aee70(ptr(a0),nullptr));}
 void party_begin_005638d0(Word a0)override{NativeMount p;unsigned scratch[2];party_mount_005638d0(m,p,a0,word(scratch));}
 void party_end_00563950(Word a0)override{NativeMount p;unsigned scratch[2];party_unmount_00563950(m,p,a0,word(scratch));}
 Word resource_exists_00408c00(Word a0,Word a1,Word a2,Word a3)override{NativeMount p;unsigned scratch[2];return resource_scope_00408c00(m,p,a0,a1,a2,a3,word(scratch));}
 Word gff_construct_00410630(Word a0,Word a1,Word a2,Word a3)override{return word(gui_load_gff_construct_00410630(static_cast<kotor_q05::CResGFF*>(ptr(a0)),nullptr,a1,load_literal(a2),static_cast<const menu_resources::Ref*>(ptr(a3))));}
 void resource_destroy(Word a0,Word a1)override{using F=void*(__thiscall*)(void*,Word);reinterpret_cast<F>(m.read(m.read(a0)))(ptr(a0),a1);}
 Word faction_load_0052b5c0(Word a0,Word a1,Word a2)override{NativeFaction p(m);unsigned scratch[48];return kotor::server_mainloop::faction_load_0052b5c0(m,p,a0,a1,a2,word(scratch));}
 void reputation_load_0052bbe0(Word a0,Word a1,Word a2)override{NativeReputation p;unsigned scratch[28];kotor::server_mainloop::reputation_load_0052bbe0(m,p,a0,a1,a2,word(scratch));}
 Word faction_check_0052b490(Word a0,Word a1)override{NativeFaction p(m);unsigned scratch[20];return faction_defaults_0052b490(m,p,a0,a1,word(scratch));}
 void faction_a_0052b9e0(Word a0)override{NativeReputation p;unsigned scratch[8];(void)reputation_defaults_0052b9e0(m,p,a0,word(scratch));}
 void faction_b_0052bce0(Word self)override{/* closeB: 0052BCE0 CFactionManager::CreateDefaultFactions */NativeFaction p(m);unsigned scratch[8];static const Word slots[7]={0x18,0x10,0x14,0xc,8,0,4};for(Word o:slots){const Word a=word(::operator new(0x24));m.write(self+o,a?faction_construct_005bf860(m,p,a,word(scratch)):0);}const Word n=m.read(self+0x1c);const Word total=(n+1)*n;const Word list=word(::operator new(0xc));if(list){m.write(list+4,0);m.write(list+8,0);m.write(list,0);}m.write(self+0x20,list);/* old +20 list leaks (original) */auto add=[&](Word v){const Word cap=m.read(list+8);if(m.read(list+4)==cap)p.grow_005e03c0(list,cap?cap*2:16);m.write(m.read(list)+m.read(list+4)*4,v);m.write(list+4,m.read(list+4)+1);};add(0);if(static_cast<std::int32_t>(total)>1)for(Word k=total-1;k;--k)add(0x64);const Word size=m.read(list+4);if(size==0){if(m.read(list)){::operator delete(ptr(m.read(list)));m.write(list,0);m.write(list+8,0);}}else{const Word old=m.read(list);m.write(list+8,size);m.write(list,word(::operator new(size*4)));for(Word k=0;static_cast<std::int32_t>(k)<static_cast<std::int32_t>(m.read(list+4));++k)m.write(m.read(list)+k*4,m.read(old+k*4));if(old)::operator delete(ptr(old));}m.write(list+4,size);}
 void finish_004c8c70(Word a0,Word a1,Word a2,Word a3)override{NativeFinish p(m);unsigned scratch[20];(void)module_finish_004c8c70(m,p,a0,a1,a2,a3,word(scratch));}
 Word message_service_004aedb0(Word a0)override{return m.read(m.read(a0+4)+0x10010);}
 void message_date_0056c910(Word a0,Word a1,Word a2)override{NativeModuleMessage p(m);unsigned scratch[2];kotor::server_mainloop::message_date_0056c910(m,p,a0,a1,a2,word(scratch));}
};
