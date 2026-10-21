#pragma once
#include <cstdint>
#include <cstddef>
namespace kotor_menu_aliases {
using Word=std::uint32_t;
// A borrowed cell address, never a copied original value or a storage owner.
// These DWORD accesses also support a canonical typed pointer slot without
// C++ pointer/unsigned type-punning. Native x86 width/alignment is required.
class Cell32 {
    void volatile* address_;
public:
    Cell32()=delete;
    explicit Cell32(void volatile* p) noexcept:address_(p){}
    Cell32(const Cell32&)=default;
    void volatile* address() const noexcept{return address_;}
    Word load() const noexcept {
        void volatile* cell=address_;Word bits;
        __asm {mov eax,cell
            mov eax,dword ptr [eax]
            mov bits,eax}
        return bits;
    }
    void store(Word bits) noexcept {
        void volatile* cell=address_;





        __asm {mov eax,cell
            mov edx,bits
            mov dword ptr [eax],edx}

    }
    operator Word() const noexcept{return load();}
    Cell32& operator=(Word bits) noexcept{store(bits);return *this;}
    // Assignment preserves this binding and copies the source value. It must
    // never rebind a live original-address cell to another view's address.
    Cell32& operator=(const Cell32& other) noexcept{return *this=other.load();}
};
struct Cells8 {
    void volatile* manager_007a39fc;
    void volatile* reinit_007a3a2c;
    void volatile* height_0078d1d8;
    void volatile* render_007a39d8;
    void volatile* top_007a39d4;
    void volatile* instance_007a39c0;
    void volatile* aurora_007a39f8;
    void volatile* input_007a39e4;
};
inline bool valid(const Cells8& c) noexcept {
    void volatile* cells[]={c.manager_007a39fc,c.reinit_007a3a2c,c.height_0078d1d8,c.render_007a39d8,c.top_007a39d4,c.instance_007a39c0,c.aurora_007a39f8,c.input_007a39e4};
    for(unsigned n=0;n<8;++n){if(!cells[n]||(reinterpret_cast<std::uintptr_t>(cells[n])&3))return false;for(unsigned m=0;m<n;++m)if(cells[n]==cells[m])return false;}
    return true;
}
struct WinMain {
    Cell32 manager_7a39fc,reinit_7a3a2c,height_78d1d8,hwnd_7a39d8;
    explicit WinMain(const Cells8& c):manager_7a39fc(c.manager_007a39fc),reinit_7a3a2c(c.reinit_007a3a2c),height_78d1d8(c.height_0078d1d8),hwnd_7a39d8(c.render_007a39d8){}
};
struct Shell {
    Cell32 top_window_7a39d4,render_window_7a39d8,instance_7a39c0;
    explicit Shell(const Cells8& c):top_window_7a39d4(c.top_007a39d4),



        render_window_7a39d8(c.render_007a39d8),

        instance_7a39c0(c.instance_007a39c0){}
};
struct ReInit {
    Cell32 aurora_7a39f8,manager_7a39fc,input_7a39e4,top_7a39d4,render_7a39d8,instance_7a39c0,reinit_7a3a2c;
    explicit ReInit(const Cells8& c):aurora_7a39f8(c.aurora_007a39f8),manager_7a39fc(c.manager_007a39fc),input_7a39e4(c.input_007a39e4),top_7a39d4(c.top_007a39d4),render_7a39d8(c.render_007a39d8),instance_7a39c0(c.instance_007a39c0),reinit_7a3a2c(c.reinit_007a3a2c){}
};
struct Message {
    Cell32 render_7a39d8,input_7a39e4,manager_7a39fc,aurora_7a39f8,reinit_7a3a2c,height_78d1d8;
    explicit Message(const Cells8& c):render_7a39d8(c.render_007a39d8),input_7a39e4(c.input_007a39e4),manager_7a39fc(c.manager_007a39fc),aurora_7a39f8(c.aurora_007a39f8),reinit_7a3a2c(c.reinit_007a3a2c),height_78d1d8(c.height_0078d1d8){}
};
static_assert(sizeof(void*)==4&&sizeof(Cell32)==4&&sizeof(Cells8)==32,"native x86 borrowed cells");
}
// Main6 defines this descriptor once, over its canonical live cells. This worker
// supplies no definition, initialization values, objects, heap or Globals copy.
extern kotor_menu_aliases::Cells8 const* const kotor_menu_window_shared_cells;
