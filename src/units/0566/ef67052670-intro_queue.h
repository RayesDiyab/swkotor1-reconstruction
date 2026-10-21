#pragma once
// lane-config: intro movie queue (swkotor.exe GOG K1, base 0x400000)
//   005ED300 CClientExoApp::BeginIntro (forwarder)  00602DC0 Internal::BeginIntro
//   00602D40 PlayMovieQueue   00602AF0 PlayMoviesInExoArrayList
//   005FBF50 AddMovieToMovieQueue
// D1 form: original __thiscall entries are __fastcall(self, unused EDX, ...).
// Disable Movies ([+1F8]!=0) keeps the queueing and returns at 00602AF3.
#include "67a3cbeb44-options.h"               // options_bindings: shared String ops (D1 epoch)
#include "369787b919-window_shared_cells.h"   // input_007a39e4
#include <cstdint>
namespace client_intro {
using options_video::U8;using options_video::U32;using options_video::I32;
using options_video::String;using options_video::Client;
struct IntroBindings {
    const char* probe_movie_0074ecd8;   // ".\\movies\\55.bik"
    const char* leclogo_0074ecf8;       // "leclogo"
    const char* biologo_0074ecf0;       // "biologo"
    const char* legal_0074ece8;         // "legal"
    I32 (__fastcall* string_list_count_004b2640)(void*,void*,String);           // by-value String, callee destroys, RET 8
    I32 (__fastcall* add_movie_to_list_005fbbf0)(Client*,void*,String*,void*);  // RET 8
    void (__fastcall* int_list_allocate_005e03c0)(void*,void*,U32);             // RET 4
    void (__fastcall* hide_mouse_0061f9c0)(void*,void*,std::uint32_t);          // RET 4
    void (__fastcall* show_mouse_0061fa00)(void*,void*,int);                    // RET 4
    void (__fastcall* unpauseable_0062b410)(void*,void*,std::uint32_t);         // CGuiInGame, RET 4
    std::uint32_t (__fastcall* input_get_active_005df510)(void*,void*);
    void (__fastcall* input_set_active_005df540)(void*,void*,int);              // RET 4
    void (__fastcall* input_clear_events_005df4e0)(void*,void*);
    void (__fastcall* sound_set_mode_005d5e80)(void*,void*,std::uint32_t);      // RET 4
    void (__cdecl* change_resolution_for_movie_00403cf0)();
    void (__cdecl* restore_resolution_from_movie_00403de0)();
    void (__fastcall* movie_show_fade_0070c210)(void*,void*);
    void (__fastcall* movie_play_async_0070c200)(void*,void*,void*,U32,void*); // RET 0Ch
    void (__fastcall* movie_hide_fade_0070c220)(void*,void*);
    I32 (__fastcall* wait_for_movie_005f6f20)(Client*,void*,I32);               // RET 4
    void (__fastcall* keyboard_modal_off_005f2980)(void*,void*);
};
extern IntroBindings& intro_bindings;
I32 __fastcall CountStrings_004b2640(void* list,void* unused_edx,String value);                     // RET 8, destroys value
void __fastcall BeginIntro_005ed300(void* app,void* unused_edx);                                   // mov ecx,[ecx+4]; jmp 00602DC0
void __fastcall BeginIntro_00602dc0(Client* self,void* unused_edx);                                // RET
void __fastcall PlayMovieQueue_00602d40(Client* self,void* unused_edx,I32 argument);               // RET 4
I32 __fastcall PlayMoviesInExoArrayList_00602af0(Client* self,void* unused_edx,void* list,I32* out,I32 wait,U32 forwarded,void* flags); // RET 14h
I32 __fastcall AddMovieToMovieQueue_005fbf50(Client* self,void* unused_edx,String* name,I32 flag); // RET 8
}
