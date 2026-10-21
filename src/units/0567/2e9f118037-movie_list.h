#pragma once
// lane-config (D46): AddMovieToExoArrayList 005FBBF0 and its file/list callees
// (swkotor.exe GOG K1, base 0x400000), D1 form __fastcall(self, unused EDX, ...).
//   005FBBF0 CClientExoAppInternal::AddMovieToExoArrayList   005F7C10 CExoArrayList<CExoString>::AddUnique
//   005E5390 CExoString::operator!=(char const*)              005E5EB0 CExoString::Find(CExoString const&,int)
//   004138B0 C2DA::GetRowIndex                                0061D230 CClientOptions::SetMovieShown
//   0061B780 CClientOptions::SaveOptions  005F46F0 CClientExoAppInternal::SaveOptions  005EDF70 CClientExoApp::SaveOptions
#include "67a3cbeb44-options.h"   // options_video::String/Client views, options_bindings (D1)
namespace client_movies {
using options_video::U8;using options_video::U32;using options_video::I32;
using options_video::String;using options_video::Client;
I32 __fastcall AddMovieToExoArrayList_005fbbf0(Client* self,void* unused_edx,String* name,void* list); // RET 8
void __fastcall AddUnique_005f7c10(void* list,void* unused_edx,String value);                       // RET 8, destroys value
I32 __fastcall StringNotEqual_005e5390(String* self,void* unused_edx,const char* text);             // RET 4
I32 __fastcall StringFind_005e5eb0(String* self,void* unused_edx,String* needle,I32 start);         // RET 8
I32 __fastcall C2daGetRowIndex_004138b0(void* table,void* unused_edx,String* label);                // RET 4
void __fastcall SetMovieShown_0061d230(void* options,void* unused_edx,String* name,I32 shown);       // RET 8
I32 __fastcall SaveOptions_0061b780(void* options,void* unused_edx);                                // RET, EAX=1
I32 __fastcall SaveOptionsInternal_005f46f0(void* internal,void* unused_edx);                       // ECX unused, EAX from 0061B780
I32 __fastcall SaveOptionsApp_005edf70(void* app,void* unused_edx);                                 // mov ecx,[ecx+4]; jmp 005F46F0
}
