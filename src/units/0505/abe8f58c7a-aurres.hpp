// aurres.hpp - lane-aurres: Aurora resource text/data stream layer (swkotor.exe 0044BD10..0044CB03).
// Evidence: lane-aurres/disasm/range.txt (dumpbin /disasm 0x44BB90..0x44CB80 of the GOG swkotor.exe).
// Observed facts are cited by instruction address in aurres.cpp; anything marked INFERRED is a naming choice.
#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include "83031bbd33-growth.h"            // M40 list-growth: menu_list_growth::allocate_005e03c0 + task-14 ArrayMutations (00445080 = RemoveReverse32)

namespace aurres {
using U=std::uint32_t;
using I=std::int32_t;

// 28-byte resource descriptor (0044C6A8/0044C7DB/0044C871 allocate 1Ch through 006FA7E6).
struct Descriptor {
    std::FILE* volatile file;        // +00 FILE* (file descriptor) or 0 (memory descriptor)     0044C80B / 0044C6B6
    volatile I line;                 // +04 line counter: memory -1 (0044C6B8), file 0 (0044C80D), ++ per file line (0044BE32)
    char* volatile scratch;          // +08 lazily allocated scratch (0044BE91), freed by 0044C964
    unsigned char* volatile current; // +0C memory cursor (0044C6DB)
    volatile I remaining;            // +10 memory bytes remaining, signed (0044C6E6/0044C6F0)
    volatile I scratch_size;         // +14 2000h memory (0044C6BF) / 7A120h file (0044C7F2, 0044C88A)
    void* volatile base;             // +18 memory base = release token for 007B8FC8 (0044C6D8, 0044C918)
};
static_assert(sizeof(void*)==4,"x86 only");
static_assert(sizeof(Descriptor)==0x1c&&offsetof(Descriptor,line)==4&&offsetof(Descriptor,scratch)==8&&
              offsetof(Descriptor,current)==0xc&&offsetof(Descriptor,remaining)==0x10&&
              offsetof(Descriptor,scratch_size)==0x14&&offsetof(Descriptor,base)==0x18,"0044C6B6..0044C6CF layout");

// 007B8FC4: memory provider, cdecl(name, ext, int* size) -> data or 0 (call 0044C68F, caller pops 0Ch).
// Original writer 0070D9C7 stores 0070C920. 007B8FC8: release, cdecl(base, size) (call 0044C91D, caller pops 8);
// writer 0070D9D1 stores 0070CBD0. Return value of the release callback is not consumed.
using GetCallback=void* (__cdecl*)(const char*,const char*,I*);
using ReleaseCallback=void (__cdecl*)(void*,I);
}

extern "C" {
// ---- cells (original .bss addresses; zero-initialized like the original image) ----
extern aurres::GetCallback volatile aurres_get_callback_007b8fc4;
extern aurres::ReleaseCallback volatile aurres_release_callback_007b8fc8;
extern kotor_reconstruction::ArrayMutations aurres_stack_007b8fd8;   // {Descriptor** data, size, capacity}
extern char aurres_dofile_message_007b8fe8[0x100];                   // next referenced cell 007B90E8

// ---- providers ----
void* __cdecl aurres_data_bytes_0044bd10(aurres::U count,void* descriptor);          // AurResGetDataBytes
void __cdecl aurres_free_data_bytes_0044bd50(void* descriptor,void* bytes);           // AurResFreeDataBytes
void __cdecl aurres_remove_0044bdc0(void* descriptor);                                // AurResRemove
char* __cdecl resource_next_0044bfa0();                                               // AurResGetNextLine (lane-font name)
const char* __cdecl aurres_next_line_const_0044bfa0();                                // typed view for texture::Providers::next_line
void* __cdecl resource_read_0044bfd0(aurres::U count);                                // AurResGetData (lane-font name)
void* __cdecl aurres_get_0044c740(const char* name,const char* ext,aurres::I volatile* size,aurres::U memory_only);
void* __cdecl aurres_get_file_0044c870(std::FILE* file);                              // AurResGet(FILE*)
aurres::U __cdecl aurres_release_0044c8f0(void* descriptor,aurres::I size);           // AurResFree
char* __cdecl aurres_dofile_0044c980(const char* file_name);                          // dofile

// ---- imports ----
void* __cdecl live_operator_new(std::uint32_t);   // 006FA7E6
void __cdecl live_free(void*);                    // 006FA390
void __cdecl aurres_func_interp_0044c1f0(char* line); // FuncInterp (unbound; return value not consumed by 0044CAF7)
}
