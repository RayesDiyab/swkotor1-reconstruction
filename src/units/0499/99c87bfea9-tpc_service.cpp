#include "a6c0baad11-tpc.hpp"
// CResTPC::OnResourceServiced 00712FF0, CResHelper<CResTPC> metadata 0070FA20
// and encoded-size 0070F690. Authored from original listings (lane evidence
// 00712fd0-007131a0.txt; task-4 processed-acquisition-r1 0070fa20.txt /
// 0070f690.txt). Products use Word (IMUL low 32 bits); SAR/IDIV use Signed.
using namespace tpc_lower;
using font_callback_read::ProcessedTexture;

// 00712FF0 (complete, ECX CResTPC, RET0, EAX 0/1). Fields: +10 raw payload,
// +18 payload size, +28 compressed flag, +2C serviced, +30 image bytes,
// +34 compressed bytes, +38 TXI bytes, +3C data (payload+0x80), +40 header,
// +44 TXI pointer.
extern "C" __declspec(noinline) Word __fastcall aurora_tpc_resource_serviced_00712ff0(void* self,void*) {
    Word faces=1;                                              // 713001 [E-10h]
    unsigned char* const raw=load<unsigned char*>(self,0x10);  // 712FF5
    if(!raw)return 0;                                          // 713005 -> 71301A
    if(load<Word>(self,0x2c)!=0)return 1;                      // 713007 -> 713168 (EAX still 1)
    if(load<void*>(self,0x40)!=nullptr||load<void*>(self,0x3c)!=nullptr)return 0; // 713010-713018
    unsigned char* const data=raw+0x80;                        // 713022
    store<void*>(self,0x40,raw);                               // 713028
    store<void*>(self,0x3c,data);                              // 71302F
    const unsigned char flags=raw[0x0c];                       // 713032
    // Quirk 713056: with no format bit the multiplier is the data pointer
    // value itself (stack slot [E-04h] holding payload+0x80), not a constant.
    Word bpp;
    if(flags&1u)bpp=1;                                         // 71303C
    else if(flags&2u)bpp=3;                                    // 713045
    else if(flags&4u)bpp=4;                                    // 71304F
    else bpp=static_cast<Word>(reinterpret_cast<std::uintptr_t>(data)); // 713056
    Word consumed;
    if(load<Word>(raw,0)==0) {                                 // 71305A uncompressed
        Signed w=static_cast<Signed>(load<std::uint16_t>(raw,8));    // 71305E
        Signed h=static_cast<Signed>(load<std::uint16_t>(raw,0x0a)); // 713062
        store(self,0x30,Word{0});                              // 713066
        store(self,0x34,Word{0});                              // 713069
        if(raw[0x0d]!=0) {                                     // 71306C JBE (unsigned byte)
            Signed i=0;
            do {
                store(self,0x30,load<Word>(self,0x30)+static_cast<Word>(h)*static_cast<Word>(w)*bpp); // 713071-71307E
                const Signed count=load<unsigned char>(load<void*>(self,0x40),0x0d); // 713081-713084
                w>>=1;h>>=1;                                   // 713088/71308A SAR
                ++i;
                if(!(i<count))break;                           // 71308C-71308F JL
            } while(true);
        }
        consumed=load<Word>(self,0x30);                        // 713091
        store<void*>(self,0x44,data+consumed);                 // 713094-71309A
    } else {                                                   // 7130A7 compressed
        store(self,0x28,Word{1});                              // 7130AA
        store(self,0x30,Word{0});                              // 7130AD
        store(self,0x34,load<Word>(raw,0));                    // 7130B0-7130B2
        // Decision D2: [E-0Ch] block is written only for bpp 3 (8) / 4 (16);
        // otherwise the original reads an uninitialized slot. 0 is used here.
        Word block=0;
        if(bpp==3)block=8;                                     // 7130B7
        else if(bpp==4)block=16;                               // 7130C6
        Signed h=static_cast<Signed>(load<std::uint16_t>(raw,0x0a)); // 7130CE
        Signed w=static_cast<Signed>(load<std::uint16_t>(raw,8));    // 7130D2
        const Signed ratio=h/w;                                // 7130D6-7130D9 IDIV
        if(ratio==6) {faces=6;h=h/6;}                          // 7130DB-7130F2
        consumed=load<Word>(raw,0)*faces;                      // 7130F8-7130FA
        const Signed mips=static_cast<Signed>(raw[0x0d]);      // 7130F4 MOVZX
        if(mips-1>0) {                                         // 7130FF-713102
            Word remaining=static_cast<Word>(load<unsigned char>(load<void*>(self,0x40),0x0d))-1u; // 713104-71310C
            do {
                h>>=1;                                         // 713110 SAR
                const Word a=static_cast<Word>(blocks4(h));    // 713112-713128
                w>>=1;                                         // 71311D SAR
                const Word b=static_cast<Word>(blocks4(w));
                consumed+=a*b*block*faces;                     // 71312E-71313F
            } while(--remaining!=0);                           // 71313B-713146
        }
        store<void*>(self,0x44,data+consumed);                 // 713148-713151
    }
    store(self,0x38,load<Word>(self,0x18)-consumed-0x80u);     // 71309D-7130A0 / 71314C-713154, 713156-71315B
    store(self,0x2c,Word{1});                                  // 713164
    return 1;
}

// 0070F690 (complete, ECX helper, RET0): encoded data size of the selected
// payload (compressed with mip/cube sum, else image bytes).
extern "C" __declspec(noinline) Word __fastcall aurora_processed_size_0070f690(void* raw_self,void*) {
    ProcessedTexture* const self=static_cast<ProcessedTexture*>(raw_self);
    Signed faces=1;                                            // F69C [E-18h]
    // Decision D2: when the resource is absent or not demanded (F6A4/F6AE)
    // 00712EF0 is skipped and the original reads these slots uninitialized
    // ([E-1Ch] compressed at F6E0, [E-0Ch] image at F77F); value-initialized.
    Signed width=0,height=0,compressed=0,image=0,txi=0;float alpha=0.0f;char encoding=0,mips=0;
    if(void* resource=self->resource) {                        // F696-F6A4
        if(aurora_get_demands_004064b0(resource,nullptr)>=1)   // F6A6-F6AE
            (void)processed_attrib_lower_00712ef0(self->resource,nullptr,&width,&height,&encoding,
                &compressed,&image,&txi,&alpha,&mips);         // F6B0-F6DB
    }
    Word total=static_cast<Word>(compressed);                  // F6E0 EDI
    if(total==0)return static_cast<Word>(image);               // F6E4 -> F77F
    // Quirk: the block multiplier shares the compressed-bytes slot [E-1Ch];
    // it is overwritten only for encoding 3 (8) / 4 (16) (F6F4/F702), so other
    // encodings multiply by the compressed byte count itself.
    Word block=static_cast<Word>(compressed);
    if(encoding==3)block=8;else if(encoding==4)block=16;      // F6EC-F702
    Signed h=height,w=width;                                   // F70A/F70E
    const Signed ratio=h/w;                                    // F712-F715 IDIV
    if(ratio==6) {faces=ratio;h=h/6;}                          // F717-F72C
    for(Signed n=static_cast<Signed>(static_cast<signed char>(mips))-1;n>0;--n) { // F72E-F769
        h>>=1;w>>=1;                                           // F740/F74D SAR
        total+=static_cast<Word>(blocks4(h))*static_cast<Word>(blocks4(w))*block;
    }
    if(faces==6)return total*6u;                               // F76C-F778
    return total;                                              // F789
}

// 0070FA20 (complete, ECX helper, RET0): address of the BBF owner tag
// (data + encoded size + TXI + 4), or null when not demanded.
extern "C" __declspec(noinline) void* __fastcall aurora_processed_info_0070fa20(void* raw_self,void*) {
    ProcessedTexture* const self=static_cast<ProcessedTexture*>(raw_self);
    void* resource=self->resource;                             // FA23
    if(!resource||aurora_get_demands_004064b0(resource,nullptr)<1)return nullptr; // FA26-FA32, FA94
    std::uintptr_t data=0;                                     // FA53 EBX=0
    resource=self->resource;                                   // FA34
    if(resource&&aurora_get_demands_004064b0(resource,nullptr)>=1) // FA37-FA45
        data=reinterpret_cast<std::uintptr_t>(aurora_processed_pointer_00712f90(self->resource,nullptr)); // FA47-FA4F
    Word txi=0;                                                // FA83 EDI=0
    resource=self->resource;                                   // FA55
    if(resource&&aurora_get_demands_004064b0(resource,nullptr)>=1) // FA58-FA64
        txi=aurora_txi_size_00712fc0(self->resource,nullptr);  // FA66-FA69
    const Word size=aurora_processed_size_0070f690(self,nullptr); // FA72 / FA85
    // FA77-FA7A / FA8A-FA8D: LEA EAX,[size+txi+data+4] (wrapping, even if data=0).
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(static_cast<Word>(data)+size+txi+4u));
}
