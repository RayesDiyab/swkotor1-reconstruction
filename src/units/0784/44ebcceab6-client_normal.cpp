#include "fce478927f-client_normal.h"
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit service-client30-6 (base sha256
// 03027c92247de67f73805261d03ed863c4f566cd7cb4029c5d5ecea3a4a11207).
// D76 FIX (FIRST FRAME): kotor_client_normal::providers is never filled (m47-compose D76-CHECK.json 11/11 null slots) and
// all four entries run in StartServices: CreateWorldJournal 005EE990 (005F8750), LoadClientOptions 005F4710 (005F903D),
// CreateQuestJournal 005EE920 (005F937C), InitializeMoviePlayer 005F2BF0 (005F9488). Each provider is now called directly:
//   006FA7E6 live_operator_new
//   00676DA0 CSWCWorldJournal::CSWCWorldJournal, 006770C0 CSWCJournal::CSWCJournal - authored below (no other owner;
//            lane dis/00676da0.asm, dis/006770c0.asm; their EH maps 0072DE48/0072DEA3 cover member arrays that these
//            store-only constructors never throw from)
//   0070C170 CExoMoviePlayer::CExoMoviePlayer -> kotor_fastcall_0070c170 (lane-m47-core rev2 movie_player.cpp)
//   0063E7C0 (XOR EAX,EAX; RET 4) -> packed_return_false_0063e7c0 (packed-file-family)
//   0061DBE0 client_options::LoadOptions_0061dbe0 (lane-config), 0061DA60 menu_providers_40_49::Options::defaults_0061da60
//   005ED700 kotor_menu20::ClientApp::get_options_005ed700, 0061D590 kotor_fastcall_0061d590 (aurora-window, ST0 result)
//   [007A39F8] table slot +24 = renderer_adoption::VTable::gamma (same typed view as window_activation.cpp 00401E55..7F)
// The never-filled `providers` table is no longer defined (client_normal.h only declares it; no reader remains); the
// readiness predicates keep their names and report the direct binding (true).
#include "899daa3c24-renderer_adoption.h"
#include "5c6f651f13-window_shared_cells.h"
#include <cstdint>
#include <cstring>
#include <initializer_list>
namespace kotor_menu20 { struct ClientApp { __declspec(noinline) void* __thiscall get_options_005ed700(); }; }
namespace menu_providers_40_49 { struct Options { __declspec(noinline) void __thiscall defaults_0061da60(); }; }
namespace client_options { std::int32_t __fastcall LoadOptions_0061dbe0(void* self, void* unused_edx); }
namespace kotor_packed_file { using Word = std::uint32_t; }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
double __fastcall kotor_fastcall_0061d590(const void* options, void*);
kotor_packed_file::Word __fastcall packed_return_false_0063e7c0(void*, void*, kotor_packed_file::Word);
void* __fastcall kotor_fastcall_0070c170(void* self, void*) noexcept(false);
}
namespace kotor_client_normal {
namespace {
void* ReadPointer(const void* base,std::size_t offset){void* value;std::memcpy(&value,static_cast<const unsigned char*>(base)+offset,4);return value;}
void WritePointer(void* base,std::size_t offset,void* value){std::memcpy(static_cast<unsigned char*>(base)+offset,&value,4);}
void store(void* base,std::size_t offset,Word value){std::memcpy(static_cast<unsigned char*>(base)+offset,&value,4);}
}
}
extern "C" {
// 00676DA0 CSWCWorldJournal::CSWCWorldJournal (EAX = this).
__declspec(noinline) void* __fastcall kotor_fastcall_00676da0(void* self, void*) {
    using kotor_client_normal::store;
    for (unsigned off : {4u, 8u, 0u, 0x10u, 0x14u, 0x0Cu}) store(self, off, 0);    // 00676DB9..C7
    static_cast<unsigned char*>(self)[0x18] = 0;                                    // 00676DCA
    kotor_client_normal::Word flags; std::memcpy(&flags, static_cast<unsigned char*>(self) + 0x1C, 4);
    store(self, 0x1C, flags & 0xFFFFFFFEu);                                         // 00676DCD
    return self;
}
// 006770C0 CSWCJournal::CSWCJournal (EAX = this).
__declspec(noinline) void* __fastcall kotor_fastcall_006770c0(void* self, void*) {
    using kotor_client_normal::store;
    for (unsigned off : {4u, 8u, 0u, 0x10u, 0x14u, 0x0Cu, 0x18u, 0x1Cu, 0x24u, 0x28u, 0x20u, 0x30u, 0x34u, 0x2Cu,
                         0x38u, 0x3Cu, 0x48u, 0x4Cu, 0x44u}) store(self, off, 0);   // 006770D9..0E
    kotor_client_normal::Word flags; std::memcpy(&flags, static_cast<unsigned char*>(self) + 0x40, 4);
    store(self, 0x40, flags | 1u);                                                  // 00677111
    return self;
}
}
namespace kotor_client_normal {
bool WorldReady() noexcept{return true;}
bool QuestReady() noexcept{return true;}
bool MovieReady() noexcept{return true;}
bool OptionsReady() noexcept{return true;}
void ClientPrefix::CreateWorldJournal(){
    if(ReadPointer(this,0x3c)==nullptr){                                            // 005EE9A9..AE
        void* const allocation=live_operator_new(0x20);                             // 005EE9B0..B2 (EH state 0)
        void* result=nullptr;
        if(allocation){
            try{result=kotor_fastcall_00676da0(allocation,nullptr);}                // 005EE9CA..CC
            catch(...){live_free(allocation);throw;}                                // 00726560
        }
        WritePointer(this,0x3c,result);                                             // 005EE9D1 / 005EE9E6
    }
}
void ClientPrefix::CreateQuestJournal(){
    if(ReadPointer(this,0x38)==nullptr){                                            // 005EE939..3E
        void* const allocation=live_operator_new(0x50);                             // 005EE940..42
        void* result=nullptr;
        if(allocation){
            try{result=kotor_fastcall_006770c0(allocation,nullptr);}                // 005EE95A..5C
            catch(...){live_free(allocation);throw;}
        }
        WritePointer(this,0x38,result);                                             // 005EE961 / 005EE976
    }
}
void ClientPrefix::InitializeMoviePlayer(){
    WritePointer(this,0x12c,nullptr);                                               // 005F2C0B
    void* const allocation=live_operator_new(4);                                    // 005F2C09..15
    void* result=nullptr;
    if(allocation){
        try{result=kotor_fastcall_0070c170(allocation,nullptr);}                    // 005F2C2D..2F
        catch(...){live_free(allocation);throw;}
    }
    WritePointer(this,0x12c,result);                                                // 005F2C44
    (void)packed_return_false_0063e7c0(result,nullptr,0); // Deliberately unconditional (005F2C38..4A).
}
int ClientPrefix::LoadClientOptions(){
    if(client_options::LoadOptions_0061dbe0(ReadPointer(this,4),nullptr)==0)        // 005F4713..1D
        static_cast<menu_providers_40_49::Options*>(ReadPointer(this,4))->defaults_0061da60(); // 005F471F..22
    void* const client=ReadPointer(kotor_menu_window_cells::manager_007a39fc,4);   // 005F4727/005F4732
    auto* const renderer=static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8); // 005F472C
    // The original caches the table base, not its method word (005F4735 mov esi,[edx]); the receiver is re-read
    // from the live renderer global after the getter (005F4746).
    const renderer_adoption::VTable* const table=reinterpret_cast<const renderer_adoption::VTable*>(
        static_cast<std::uintptr_t>(renderer->vtable_word));
    // 005F4737 push 0 is not consumed by 005ED700 (RET 0); it is the second SetGamma word.
    void* const options=static_cast<kotor_menu20::ClientApp*>(client)->get_options_005ed700();   // 005F4739
    const float gamma=static_cast<float>(kotor_fastcall_0061d590(options,nullptr)); // 005F4740; 005F474C fstp dword
    Word gamma_bits;std::memcpy(&gamma_bits,&gamma,4);
    auto* const receiver=static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8); // 005F4746
    (void)table->gamma(receiver,nullptr,gamma_bits,0);                              // 005F474F call [esi+24h]
    return 1;                                                                       // 005F4752
}
}
