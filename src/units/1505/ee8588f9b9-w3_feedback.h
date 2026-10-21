#pragma once
#include <cstdint>
namespace frame_log { struct String; }
extern "C" {
void __fastcall feedback_add_005fcd10(void*,void*,std::uint32_t,void*,void*);
void __fastcall frame_LogStringCtor_005e5a90(frame_log::String*,void*,const char*) noexcept(false);
void __fastcall frame_LogStringDtor_005e5c20(frame_log::String*,void*);
void __fastcall recovered_client_add_feedback_message_005ed360(void*,void*,std::uint32_t,void*,void*);
}

