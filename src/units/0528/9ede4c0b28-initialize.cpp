#include "aaf9b2970f-texture_join.h"
#include "d2c5c47a35-allocator_live.h" // E1 edit TC1: stage header_root (was absolute R/task-23 path)
namespace material_owner {
static bool selected(const char* name) {
    if(!name||!name[0])return false;
    const char sentinel[5]={'N','U','L','L',0};
    for(unsigned i=0;i<5;++i)if(name[i]!=sentinel[i])return true;
    return false;
}
static font::Texture* acquire(const char* name) {
    return selected(name) ? font::reference_00423a60(name,nullptr) : texture_join::get_null_004230c0();
}
void __thiscall Material::initialize_textures_0047b560(const char* first,const char* second,const char* third,const char* fourth) {
    font::assign_00447fa0(&textures[0],nullptr,acquire(first));
    if(selected(first))init_0047b110(first); // Recompare live name after assignment.
    font::assign_00447fa0(&textures[1],nullptr,acquire(second));
    font::assign_00447fa0(&textures[2],nullptr,acquire(third));
    font::assign_00447fa0(&textures[3],nullptr,acquire(fourth));
    const char* name=names[0];
    if(name) {
        if(selected(name))font::assign_00447fa0(&textures[4],nullptr,font::reference_00423a60(name,nullptr));
        live_free(names[0]); // Fresh volatile reload after assignment.
        names[0]=nullptr;
    }
    name=names[1];
    if(name) {
        if(selected(name)) {
            font::assign_00447fa0(&textures[5],nullptr,font::reference_00423a60(name,nullptr));
            font::Texture* const result=texture_join::call_slot08(textures[5].target,nullptr);
            result->environment=1; // Full pointer, exact DWORD+14; no null guard.
        }
        live_free(names[1]);names[1]=nullptr;
    }
    name=names[2];
    if(name) {
        if(selected(name)) {
            font::assign_00447fa0(&textures[5],nullptr,font::reference_00423a60(name,nullptr));
            font::Texture* const result=texture_join::call_slot08(textures[5].target,nullptr);
            result->environment=1;
        }
        texture_join::set_environment_00421480(textures[0].target,nullptr,1);
        live_free(names[2]);names[2]=nullptr;
    }
}
}
