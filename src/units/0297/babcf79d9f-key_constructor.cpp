#include "d14298b60a-key_constructor.h"
#include <cstring>
#include <new>

static void Store(void* receiver,unsigned offset,uint32_t word) {
    std::memcpy(static_cast<unsigned char*>(receiver)+offset,&word,4);
}
extern "C" void* __fastcall key_table_construct_0040d030(void* receiver,void*) noexcept(false) {
    auto* name=::new(static_cast<unsigned char*>(receiver)+0x20) menu_resources::String;
    // Modern C++ cleanup uses the actual owned String destructor. Historical
    // frame metadata/exception transport remain outside the normal-path proof.
    try {
#ifdef WRONG_CRC_BEFORE_STRING_ASSIGN
        for(unsigned index=0;index<256;++index)kotor_key_crc_table_007a3d78[index]=0;
#endif
        const char* empty=c2da_empty_0073d71c();
        Store(receiver,0,0);
        Store(receiver,8,0);
        Store(receiver,4,0);
        Store(receiver,0x0c,0);
        Store(receiver,0x10,0);
        Store(receiver,0x14,0);
#ifdef WRONG_SENTINEL_ZERO
        Store(receiver,0x18,0);
#else
        Store(receiver,0x18,0xffffffffu);
#endif
        Store(receiver,0x1c,0);
        (void)GuiAssign005e5140(name,nullptr,empty);
        Store(receiver,0x2c,0);
        Store(receiver,0x30,0);
#ifdef WRONG_ZERO_UNTOUCHED_28
        Store(receiver,0x28,0);
#endif
        for(unsigned index=0;index<256;++index) {
#ifdef WRONG_SKIP_LAST_ENTRY
            if(index==255)break;
#endif
            uint32_t word=index;
            for(unsigned bit=0;bit<8;++bit) {
#ifdef WRONG_POLYNOMIAL
                word=(word>>1)^((word&1)?0xedb88321u:0u);
#else
                word=(word>>1)^((word&1)?0xedb88320u:0u);
#endif
            }
            kotor_key_crc_table_007a3d78[index]=word;
        }
    } catch(...) {
        name->~StartupString();
        throw;
    }
    return receiver;
}
