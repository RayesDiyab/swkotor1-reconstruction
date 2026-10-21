#include "ef67052670-intro_queue.h"
#include "c534281817-startup_string.h"
#include <cstring>
#include <new>
// Owner declarations repeated exactly (lane-prepare game_prepare_producers.h:29-30, D36/F8).
extern "C" void* kotor_cell_sound_007a39ec;                         // lane-sound owner
extern "C" const std::uint32_t kotor_rdata_sound_mode_0074c5f8;     // lane-sound owner (.rdata 3)
// CExoString::operator== 005E5200 (M41 add-key-equal-transport.obj, exact declaration).
namespace add_key_lower { int __fastcall Equal005e5200(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*); }
namespace client_intro {
namespace {
using options_video::PointerAt; // contracts.h memcpy word read
U32 WordAt(const void* p,std::size_t o){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
void StoreWordAt(void* p,std::size_t o,U32 v){std::memcpy(static_cast<U8*>(p)+o,&v,4);}
U8* Field(Client* c,std::size_t o){return c->bytes+o;}
}

// 005ED300 CClientExoApp::BeginIntro: ECX=CClientExoApp, internal at +4.
void __fastcall BeginIntro_005ed300(void* app,void* unused_edx) {
    (void)unused_edx;
    BeginIntro_00602dc0(static_cast<Client*>(PointerAt(app,4)),nullptr); // 005ED300/005ED303
}

// 00602DC0 CClientExoAppInternal::BeginIntro.
void __fastcall BeginIntro_00602dc0(Client* self,void* unused_edx) {
    (void)unused_edx;auto& o=options_video::options_bindings;auto& b=intro_bindings;
    if(WordAt(self,0x1fc)!=0)return;                                    // 00602DDB..00602DE3
    String name;
    o.text_string_005e5a90(&name,nullptr,b.leclogo_0074ecf8);           // 00602DF2
    (void)AddMovieToMovieQueue_005fbf50(self,nullptr,&name,1);          // 00602E08
    o.destroy_string_005e5c20(&name,nullptr);                           // 00602E19
    o.text_string_005e5a90(&name,nullptr,b.biologo_0074ecf0);           // 00602E27
    (void)AddMovieToMovieQueue_005fbf50(self,nullptr,&name,1);          // 00602E3D
    o.destroy_string_005e5c20(&name,nullptr);                           // 00602E4E
    o.text_string_005e5a90(&name,nullptr,b.legal_0074ece8);             // 00602E5C
    (void)AddMovieToMovieQueue_005fbf50(self,nullptr,&name,1);          // 00602E72
    o.destroy_string_005e5c20(&name,nullptr);                           // 00602E83
    PlayMovieQueue_00602d40(self,nullptr,1);                            // 00602E88..00602E8C
}

// 00602D40 CClientExoAppInternal::PlayMovieQueue(int), RET 4.
void __fastcall PlayMovieQueue_00602d40(Client* self,void* unused_edx,I32 argument) {
    (void)unused_edx;auto& o=options_video::options_bindings;auto& b=intro_bindings;
    void* const list=Field(self,0x2e4);                                 // 00602D58
    // 00602D43..00602D53: the probe String is built in the outgoing by-value
    // argument area; count 004B2640 consumes and destroys it (RET 8).
    String probe;o.text_string_005e5a90(&probe,nullptr,b.probe_movie_0074ecd8);
    if(b.string_list_count_004b2640(list,nullptr,probe)!=0) {           // 00602D60..00602D67
        StoreWordAt(self,0x4bc,1);                                      // 00602D70
        b.hide_mouse_0061f9c0(self,nullptr,0x80);                       // 00602D69/00602D7A
    }
    // 00602D7F reads the caller's argument word; 00602D8D passes that same
    // argument slot as the result out-pointer.
    I32 argument_slot=argument;
    (void)PlayMoviesInExoArrayList_00602af0(self,nullptr,list,&argument_slot,0,static_cast<U32>(argument),Field(self,0x2f0)); // 00602D95
    StoreWordAt(self,0x2e8,0);                                          // 00602D9A
    StoreWordAt(self,0x2f4,0);                                          // 00602DA5
}

// 00602AF0 CClientExoAppInternal::PlayMoviesInExoArrayList, RET 14h.
I32 __fastcall PlayMoviesInExoArrayList_00602af0(Client* self,void* unused_edx,void* list,I32* out,I32 wait,U32 forwarded,void* flags) {
    (void)unused_edx;auto& b=intro_bindings;
    if(WordAt(self,0x1f8)!=0)return 0;                                  // 00602AF3..00602B00 Disable Movies
    b.unpauseable_0062b410(PointerAt(self,0x40),nullptr,1);             // 00602B03..00602B09 (unchecked)
    *out=0;                                                             // 00602B12
    const I32 active=b.input_get_active_005df510(kotor_menu_window_cells::input_007a39e4,nullptr); // 00602B19..00602B1F
    if(active==0) {                                                     // 00602B2A
        void* const input=kotor_menu_window_cells::input_007a39e4;      // 00602B2C
        if(input!=nullptr)b.input_set_active_005df540(input,nullptr,1); // 00602B36/00602B38
        b.show_mouse_0061fa00(self,nullptr,0);                          // 00602B3D..00602B41
    }
    b.input_clear_events_005df4e0(kotor_menu_window_cells::input_007a39e4,nullptr); // 00602B46..00602B4E
    b.sound_set_mode_005d5e80(kotor_cell_sound_007a39ec,nullptr,kotor_rdata_sound_mode_0074c5f8); // 00602B53..00602B5F
    b.hide_mouse_0061f9c0(self,nullptr,8);                              // 00602B64..00602B68
    b.change_resolution_for_movie_00403cf0();                           // 00602B6D
    b.movie_show_fade_0070c210(PointerAt(self,0x12c),nullptr);          // 00602B72/00602B78
    b.movie_play_async_0070c200(PointerAt(self,0x12c),nullptr,list,forwarded,flags); // 00602B7D..00602B92
    Sleep(2);                                                           // 00602B97/00602B99 kernel32 import 0073D1E4
    *out=b.wait_for_movie_005f6f20(self,nullptr,wait);                  // 00602B9F..00602BAB
    b.restore_resolution_from_movie_00403de0();                         // 00602BAE
    b.show_mouse_0061fa00(self,nullptr,8);                              // 00602BB3..00602BB7
    b.movie_hide_fade_0070c220(PointerAt(self,0x12c),nullptr);          // 00602BBC/00602BC2
    StoreWordAt(list,4,0);                                              // 00602BCB
    if(flags!=nullptr)StoreWordAt(flags,4,0);                           // 00602BC9..00602BD0
    if(active==0) {                                                     // 00602BD3..00602BD9
        void* const input=kotor_menu_window_cells::input_007a39e4;      // 00602BDB
        if(input!=nullptr)b.input_set_active_005df540(input,nullptr,0); // 00602BE5/00602BE6
        b.keyboard_modal_off_005f2980(self,nullptr);                    // 00602BEB/00602BED
    }
    b.unpauseable_0062b410(PointerAt(self,0x40),nullptr,0);             // 00602BF2..00602BF6
    return 1;                                                           // 00602BFC
}

// 005FBF50 CClientExoAppInternal::AddMovieToMovieQueue(CExoString&,int), RET 8.
I32 __fastcall AddMovieToMovieQueue_005fbf50(Client* self,void* unused_edx,String* name,I32 flag) {
    (void)unused_edx;auto& b=intro_bindings;
    void* const flags=Field(self,0x2f0);                                // 005FBF62 CExoArrayList<int>
    const U32 capacity=WordAt(self,0x2f8),count=WordAt(self,0x2f4);     // 005FBF54/005FBF5A
    if(count==capacity)b.int_list_allocate_005e03c0(flags,nullptr,capacity!=0?capacity+capacity:0x10); // 005FBF60..005FBF7A
    U32* const data=static_cast<U32*>(PointerAt(flags,0));              // 005FBF82
    const U32 index=WordAt(flags,4);                                    // 005FBF7F
    std::memcpy(data+index,&flag,4);                                    // 005FBF88
    StoreWordAt(flags,4,WordAt(flags,4)+1);                             // 005FBF8B/005FBF99/005FBF9D
    return b.add_movie_to_list_005fbbf0(self,nullptr,name,Field(self,0x2e4)); // 005FBF92..005FBFA0, EAX returned
}

// 004B2640 CExoArrayList<CExoString>::count(CExoString): ECX=list {data,count},
// by-value String argument destroyed by the callee, RET 8, EAX=matches.
I32 __fastcall CountStrings_004b2640(void* list,void* unused_edx,String value) {
    (void)unused_edx;
    using Canonical=kotor_reconstruction::StartupString;
    static_assert(sizeof(Canonical)==sizeof(String),"8-byte CExoString view");
    Canonical* const probe=std::launder(static_cast<Canonical*>(static_cast<void*>(&value)));
    I32 matches=0;                                                      // 004B265D ebx
    for(I32 i=0;i<static_cast<I32>(WordAt(list,4));++i) {               // 004B265A..004B268A (count reloaded each pass)
        U8* const data=static_cast<U8*>(PointerAt(list,0));             // 004B2670 reload
        Canonical* const element=std::launder(static_cast<Canonical*>(static_cast<void*>(data+static_cast<std::size_t>(i)*8)));
        if(add_key_lower::Equal005e5200(element,nullptr,probe)!=0)++matches; // 004B2672..004B2683
    }
    options_video::options_bindings.destroy_string_005e5c20(&value,nullptr); // 004B268C..004B2698
    return matches;                                                     // 004B26A3
}
}
