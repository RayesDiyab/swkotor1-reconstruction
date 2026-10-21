// lane-audit-r6allow unit xc-0073b160-file-thread (authored for allowlist r6; __xc scan item (d)).
// The original CRT runs this initializer from its __xc table; here the compiler-emitted .CRT$XCU record of the
// named-namespace root below is the modern _initterm entry for the same role (one record in this object).
// Original (dumpbin, swkotor.exe sha256 9c10e045...): __xc slot 0078D0DC -> 0073B160, walked by _cinit 006FB11A..006FB138.
//   initializer: 0073B160 mov ecx,8328B0h / 0073B165 call 005EBED0 / 0073B16A push 73BF00h / 0073B16F call 006FB332 / 0073B174 pop ecx / 0073B175 ret
//   005EBED0 CExoFileThread::CExoFileThread: CS ctor 005E9290 on +8; +0C/+10/+14/+18/+1C/+20/+24 = 0;
//            CreateThread(0, 10000h, 005EBE10, &local, CREATE_SUSPENDED(4), this+4) [IAT 0073D1F0] -> [this];
//            SetThreadPriority([this], -2) [IAT 0073D1F4]  (005EBED0..005EBF22)
//   atexit disposer: 0073BF00 mov ecx,8328B0h / 0073BF05 jmp 005EB570 (CExoFileThread::~CExoFileThread:
//            +10 = 1; ResumeThread; if handle != -1 {WaitForSingleObject(h,-1); CloseHandle(h)}; CS dtor 005E9320)
// Bodies 005EBED0 / 005EBE10 / 005EB570 are the reviewed task-30 donor (source/file_thread.cpp, sha256 15daa290...,
// copied verbatim as unit file-thread-bodies; no new address credit here). Storage 008328B0 (40 bytes) has ONE owner:
// main-filethread-singleton kotor_file::global_file_thread (its comment "Construct remains explicit": this is that call).
// Consumers on the async read path: CExoFileInternal::ReadAsync 005EBBF0 (r31 async_producer.cpp) and the completion
// poll (r31 completion.cpp), reached from CExoResMan::Update 00408D40 -> Service* (async=1) -> ReadResourceAsync.
#include "9f442eedcf-file_thread.h"     // task-30 kotor_file::FileThread (header root task-30\source via stage include roots)
#include <cstdlib>
// 0073BF00: mov ecx,8328B0h / jmp 005EB570.
extern "C" __declspec(noinline) void __cdecl kotor_xc_dispose_0073bf00() {
    kotor_file::global_file_thread.Destroy();                          // 0073BF00..0073BF05 -> 005EB570
}
extern "C" __declspec(noinline) int __cdecl kotor_xc_init_0073b160() {
    (void)kotor_file::global_file_thread.Construct();                 // 0073B160 mov ecx,8328B0h / 0073B165 call 005EBED0 (EAX discarded)
    return std::atexit(&kotor_xc_dispose_0073bf00);                    // 0073B16A push 73BF00h / call 006FB332 / pop ecx / ret
}
namespace kotor_xc_0078d0dc {
extern const int root_0073b160;
const int root_0073b160 = kotor_xc_init_0073b160();   // .CRT$XCU record
}
