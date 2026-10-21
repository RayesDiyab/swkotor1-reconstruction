#include "25ddf92eb3-gui_size.h"
// lane-m47-core ADAPTED COPY of the M46 stage unit service-gui_0040BE70-25 (base sha256
// 08b0318a91a4acc8b38bfc74f160a78edbd548f51215c43459716f6058a37bc7).
// FIRST-FRAME FIX: gui_size::bindings is never populated in the M46 stage (tools/bindings_scan.py), but StartServices
// 005F93FA calls SetSize(0078D1D4, 0078D1D8) on the manager built at 005F93D7 with a 0x0 size, so the changed-size path
// below always runs: 0040BE95 call 0040B420 (UpdateAllFonts) through a null pointer. The two callees are now the genuine
// providers, called directly with their published D1 types (no binding table):
//   0040BE95 call 0040B420 -> menu_tables_required::UpdateAllFonts_0040b420 (lane-m47-core gui_manager_leaves.cpp)
//   0040BEB0 call 0040ACC0 -> kotor_fastcall_0040acc0 (gui-load mainmenu.cpp, DetermineNeedToDraw)
// The Bindings object stays defined (declared by gui_size.h) but nothing reads it any more.
namespace menu_tables_required { void __fastcall UpdateAllFonts_0040b420(void* gui, void* unused_edx); }
extern "C" void __fastcall kotor_fastcall_0040acc0(void* manager, void*) noexcept(false);
namespace gui_size {
Bindings bindings{};
void __thiscall Gui::set_size_0040be70(std::int32_t width,std::int32_t height) {
    std::int16_t old_width,old_height;
    std::memcpy(&old_width,bytes+0x6c,2);std::memcpy(&old_height,bytes+0x6e,2);



    const bool same=static_cast<std::int32_t>(old_width)==width&&static_cast<std::int32_t>(old_height)==height;

    if(same)return;
    const std::uint16_t new_height=static_cast<std::uint16_t>(height),new_width=static_cast<std::uint16_t>(width);
    std::memcpy(bytes+0x6e,&new_height,2);std::memcpy(bytes+0x6c,&new_width,2);



    menu_tables_required::UpdateAllFonts_0040b420(this,nullptr);                 // 0040BE8F..95
    Word popup;std::memcpy(&popup,bytes+0x7c,4);
    if(popup){
        void* const object=reinterpret_cast<void*>(popup);ScalarDelete* table;
        std::memcpy(&table,object,4);
        table[0](object,1);

        const Word zero=0;std::memcpy(bytes+0x7c,&zero,4);

        kotor_fastcall_0040acc0(this,nullptr);                                    // 0040BEA7..B0
    }
}
}
