#include "97677fa6bb-extension_loader.h"
#include "39c670fc66-allocator_live.h"
#include "1460ea702e-render_system_gl.h"
#include <cstring>
namespace kotor_renderer_init {
template<class T>static T ReadCell(const void* p){T value;std::memcpy(&value,p,4);return value;}
template<class T>static void WriteCell(void* p,T value){static_assert(sizeof(T)==4,"Raw canonical word");std::memcpy(p,&value,4);}
static U32 Length(const char* p){U32 n=0;while(p[n]!=0)++n;return n;}
static void Append(char* target,const char* base,const char* suffix){
    char* out=target;do{*out++=*base;}while(*base++!=0);
    out=target;while(*out!=0)++out;
    do{*out++=*suffix;}while(*suffix++!=0);
}
void __stdcall LoadExtensions_00436490(){
    auto& b=extension_loader;
    const LoadProcedure load=*b.load_iat_0073d310;
    WriteCell(b.extension_cell_007bb7ec,load(b.extension_name_0073f45c));
    const StringProcedure strings=*b.string_iat_0073d290;
    const char* gl=strings(0x1f03);const char* wgl=nullptr;
    if(ReadCell<ExtensionString>(b.extension_cell_007bb7ec)!=nullptr){
        void* dc=reinterpret_cast<void*>(static_cast<std::uintptr_t>(aurora_context_adoption::current_dc_0073d2dc()));
        wgl=ReadCell<ExtensionString>(b.extension_cell_007bb7ec)(dc);
    }
    for(unsigned i=0;i<28;++i){
        const auto& row=b.capability_rows_28[i];
        if((!row.wgl||wgl!=nullptr)&&CheckExtension_0045f980(row.name,row.wgl?wgl:gl)!=0)
            *b.capabilities_007bb85c |= *row.mask;
    }
    for(unsigned i=0;i<201;++i){
        const auto& row=b.procedure_rows_201[i];WriteCell(row.cell,load(row.name));
    }
    if((*b.capabilities_007bb85c & *b.renderer_mask_0078e49c)!=0){
        const char* renderer=strings(0x1f01);
        if(StringSearch_006fbd00(renderer,b.special_renderer_0073f52c)!=nullptr)*b.renderer_flag_0078e398=0;
    }
    if(AnisotropicAvailable_0045f8e0()!=0)glGetFloatv_0073d218(0x84ff,b.anisotropy_007a6858);
    InitAttenuationTexture_004263c0();
    const U32 required=*b.ati_mask_0078e4d0;
    const bool ati=(*b.capabilities_007bb85c & required)==required;
    *b.enable_007a47e0=ati?b.ati_enable:b.nv_enable;
    *b.constant0_007a6820=ati?b.ati_constant0:b.nv_constant0;
    *b.constant1_007a47dc=ati?b.ati_constant1:b.nv_constant1;
    const char* suffix=ati?b.ati_suffix_0078fe18:b.nv_suffix_0078fd78;
    *b.first_program_0083028c=static_cast<char*>(live_operator_new(
        Length(b.first_base_0078f600)+Length(suffix)+1u));
    char* second=static_cast<char*>(live_operator_new(
        Length(b.second_base_00791240)+Length(suffix)+1u));
    char* first=*b.first_program_0083028c;
    *b.second_program_00830290=second;
    Append(first,b.first_base_0078f600,suffix);
    // Original reloads each published output word before appending/copying.
    Append(*b.second_program_00830290,b.second_base_00791240,suffix);
    if(ati)InitializeATIFragmentShaders_004794e0();
    if(*b.programs_0078e5ec!=0)*b.vertex_enum_0078d46c=0x8620;
    if(VertexBufferAvailable_0045fc90()!=0)InitVertexBufferPool_00436400();
    else InitSystemPool_00436390();
    PoolReceiver* pool=*b.pool_007a68a4;
    pool->RestorePoolManager();
}
}
