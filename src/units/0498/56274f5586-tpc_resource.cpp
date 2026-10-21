#include "a6c0baad11-tpc.hpp"
// Small CResTPC (0x48-byte resource) lowers. Same pattern as the M40 CRes2DA
// successor sources (resource-successor/callees, table). Service 00712FF0 is
// NOT authored here; its table cell references the named missing provider.
using namespace tpc_lower;

extern "C" const ResourceMethods aurora_tpc_resource_methods_0075fc5c{
    &aurora_tpc_resource_deleting_destroy_00713170,
    &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0,
    &aurora_tpc_resource_freed_00712fd0,
    &aurora_tpc_resource_serviced_00712ff0
};

// 00712EA0 (complete, RET0): base CRes ctor 00406460, zero +28..+44,
// table 0075FC5C, +24 = 1, return this.
extern "C" __declspec(noinline) void* __fastcall aurora_tpc_resource_ctor_00712ea0(void* self,void*) {
    (void)c2da_resource_base_ctor_00406460(self,nullptr);                    // 2EA3
    for(unsigned offset=0x28u;offset<=0x44u;offset+=4u)store(self,offset,Word{0}); // 2EAA-2EBF
    store<const void*>(self,0,&aurora_tpc_resource_methods_0075fc5c);       // 2EC2
    store(self,0x24,Word{1});                                                // 2EC8
    return self;
}

// 00712EE0 (complete): table store then tail JMP base destructor 00409D40.
extern "C" __declspec(noinline) void __fastcall aurora_tpc_resource_destroy_00712ee0(void* self,void*) noexcept {
    store<const void*>(self,0,&aurora_tpc_resource_methods_0075fc5c);
    c2da_resource_base_destroy_00409d40(self,nullptr);
}

// 00713170 (complete): slot0 of 0075FC5C.
extern "C" __declspec(noinline) void* __fastcall aurora_tpc_resource_deleting_destroy_00713170(
    void* self,void*,Word flags) noexcept {
    aurora_tpc_resource_destroy_00712ee0(self,nullptr);                      // 3173
    if((flags&1u)!=0u)live_free(self);                                       // 3178-3185 006FA390
    return self;
}

// 00712FD0 (complete): OnResourceFreed - zero +28..+44, return 1.
extern "C" __declspec(noinline) Word __fastcall aurora_tpc_resource_freed_00712fd0(void* self,void*) {
    for(unsigned offset=0x28u;offset<=0x44u;offset+=4u)store(self,offset,Word{0});
    return 1u;
}

// 00712EF0 (complete, ECX resource, eight stack outputs, RET 20h). The header
// cell +40 is re-read before each field, as in the original. Alpha is copied
// as raw DWORD bits (2F11-2F1B), never through an x87 load.
extern "C" __declspec(noinline) font_callback_read::Signed __fastcall processed_attrib_lower_00712ef0(
    void* self,void*,Signed* width,Signed* height,char* encoding,Signed* compressed_bytes,
    Signed* image_bytes,Signed* txi_bytes,float* alpha,char* mip_count) noexcept(false) {
    if(!load<void*>(self,0x40))return 0;                                     // 2EF0-2EF7 (EAX = 0)
    store<Signed>(width,0,static_cast<Signed>(load<std::uint16_t>(load<void*>(self,0x40),8)));   // 2EFA-2F02
    store<Signed>(height,0,static_cast<Signed>(load<std::uint16_t>(load<void*>(self,0x40),0x0a))); // 2F04-2F0F
    store<Word>(alpha,0,load<Word>(load<void*>(self,0x40),4));               // 2F11-2F1B
    store<char>(mip_count,0,load<char>(load<void*>(self,0x40),0x0d));        // 2F1D-2F27
    store<Word>(compressed_bytes,0,load<Word>(self,0x34));                  // 2F29-2F30
    store<Word>(image_bytes,0,load<Word>(self,0x30));                       // 2F32-2F39
    store<Word>(txi_bytes,0,load<Word>(self,0x38));                         // 2F3B-2F42
    const unsigned char flags=load<unsigned char>(load<void*>(self,0x40),0x0c); // 2F44-2F47
    if(flags&1u){*encoding=1;return 1;}                                      // 2F4A-2F5A
    if(flags&2u){*encoding=3;return 1;}                                      // 2F5D-2F6D
    if(flags&4u)*encoding=4;                                                 // 2F70-2F78
    return 1;                                                                // 2F7B
}
