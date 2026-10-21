#pragma once
#include "6b711608f5-services.h"
#include <initializer_list>
namespace client_services_start {
using Word=client_services::Word;
enum class Abi:Word { Thiscall,Cdecl,Stdcall };
struct Call {Word site,target,self,count,args[8];Abi abi;};
// Borrowed source-provider routing boundary. No implementation is supplied by
// the production source. Synthetic tests provide an explicit receipt router.
enum class Global:unsigned { Base,Resource,Aurora,Interface,Tlk,TlkClient,Manager,Input,
 AllowWindowed,Width,Height,Count };
struct Globals {void volatile* words[static_cast<unsigned>(Global::Count)];unsigned char volatile* logging_byte_008328e1;unsigned char volatile* depth_0078d1e0;};
extern Globals* globals;
Word read(Global);
void write(Global,Word);
}
extern "C" client_services_start::Word __cdecl unresolved_client_services_provider(const client_services_start::Call*);

// Task6 isolated interface proposal: the 11 slots hold addresses, not
// dereferenced Word objects. Cell32 supplies native DWORD access, including
// pointer-object cells. Same x86 Globals extent; owner review is pending.
static_assert(sizeof(client_services_start::Globals)==52,"11 DWORD addresses and two byte-cell addresses");
