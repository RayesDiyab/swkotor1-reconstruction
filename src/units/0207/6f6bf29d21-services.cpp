#include "8cee205705-services.h"
#include "b593572262-m50_vtables.h"   // lane-m50-vtables W-4 scan: typed CClientExoApp table 0074E6D8 (startup46/client_exo_app.cpp)
namespace client_services {
Dependencies dependencies{};
Client* Client::construct_005ee2f0(){
 word(0,kotor_vtable_word(&kotor_vtable_cclientexoapp_0074e6d8)); // 005EE30E mov [esi],0074E6D8h: typed table (lane-m50-vtables)
 void* p=dependencies.allocate_006fa7e6(0x4d4);
 word(4,reinterpret_cast<Word>(p?dependencies.internal_005fbfe0(p,nullptr):nullptr));
 return this;
}
Manager* Manager::construct_00401160(){
 word(4,0);word(8,0);word(0x18,dependencies.tick_0073d0f8());
 void* p=dependencies.allocate_006fa7e6(8);
 word(4,reinterpret_cast<Word>(p?static_cast<Client*>(p)->construct_005ee2f0():nullptr));
 p=dependencies.allocate_006fa7e6(0x3c);
 word(0x14,reinterpret_cast<Word>(p?dependencies.stats_004010c0(p,nullptr):nullptr));
 p=dependencies.allocate_006fa7e6(0x184);
 word(0x0c,reinterpret_cast<Word>(p?dependencies.object_table_004ae5d0(p,nullptr):nullptr));
 p=dependencies.allocate_006fa7e6(0x184);
 word(0x10,reinterpret_cast<Word>(p?dependencies.object_table_004ae5d0(p,nullptr):nullptr));
 word(0,0);word(0,reinterpret_cast<Word>(dependencies.allocate_006fa7e6(0x40000)));
 return this;
}
Word Client::initialize_005ed7b0(){return static_cast<Internal*>(pointer(4))->initialize_005f51c0();}
void Client::input_005eda10(int active,int clear){static_cast<Internal*>(pointer(4))->input_005f6b10(active,clear);}
void Internal::input_005f6b10(int active,int clear){
 void* input=*dependencies.input_007a39e4;
 if(input)dependencies.input_active_005df540(input,nullptr,active);
 if(!active)dependencies.keyboard_off_005f2980(this,nullptr);



 if(clear)dependencies.clear_events_005df4e0(*dependencies.input_007a39e4,nullptr);

 if(active)dependencies.show_mouse_0061fa00(this,nullptr,0);
}
static void* rules_table(){
 void* rules=*dependencies.rules_007a3a28;
 Word p;memcpy(&p,static_cast<unsigned char*>(rules)+0xb8,4);
 Word table;memcpy(&table,reinterpret_cast<unsigned char*>(p)+0x14,4);
 return reinterpret_cast<void*>(table);
}
static int rows(void* table){int n;memcpy(&n,static_cast<unsigned char*>(table)+0x24,4);return n;}
Word Internal::initialize_005f51c0(){
 if(!word(0x278)){
  void* p=dependencies.allocate_006fa7e6(0x6b8);
  word(0x278,reinterpret_cast<Word>(p?dependencies.load_screen_0067a710(p,nullptr,pointer(0x274)):nullptr));
 }
 void* rules=*dependencies.rules_007a3a28;
 if(!rules){
  void* p=dependencies.allocate_006fa7e6(0xdc);
  rules=p?dependencies.rules_00552c50(p,nullptr):nullptr;
  *dependencies.rules_007a3a28=rules;
 }
 if(rules && rows(rules_table())>0){
  unsigned char row=0;
  do {
   String column;dependencies.string_005e5a90(&column,nullptr,"Walk");
   int value; // Original ignores GetINTEntry's result; provider must write this.
   (void)dependencies.get_int_00413660(rules_table(),nullptr,row,&column,&value);
   dependencies.destroy_string_005e5c20(&column,nullptr);



   const Word contribution=static_cast<Word>(value)<<(row&31u);

   word(0xa4,word(0xa4)|contribution);
   ++row; // Original BL wraps at256; no invented row limit.
  }while(static_cast<int>(row)<rows(rules_table()));
 }
 dependencies.gui_initialize_00632720(pointer(0x40),nullptr);
 dependencies.event_descriptions_005eeb10(this,nullptr);
 void* movie=pointer(0x12c);
 if(movie && dependencies.movie_playing_0070c1f0(movie,nullptr)){
  dependencies.unpauseable_0062b410(pointer(0x40),nullptr,1);
  dependencies.clear_events_005df4e0(*dependencies.input_007a39e4,nullptr);
  dependencies.sound_mode_005d5e80(*dependencies.sound_007a39ec,nullptr,*dependencies.sound_mode_0074c5f8);
 }
 word(0x204,1);return 1;
}
}
